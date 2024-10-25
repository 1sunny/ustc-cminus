#include"LoopInvariant.hpp"

void LoopInvariant::run() {
  CFG_analyser = std::make_unique<CFGAnalyse>(m_);
  CFG_analyser->run();
  auto loops = CFG_analyser->get_loops();
  for (auto loop: *loops) {
    bool inner_first = false;
    for (auto bb: *loop) {
      // 如果一个基本块所在的loop不是当前这个loop,证明这个基本块在内层循环被重新设置了bb_loop
      if (CFG_analyser->find_bb_loop(bb) != loop) {
        inner_first = true;
        break;
      }
    }
    if (inner_first) continue;

    auto cur_loop = loop;
    while (cur_loop != nullptr) {
      invariants.clear();
      find_invariants(cur_loop);
      if (!invariants.empty()) {
        move_invariants_out(cur_loop);
      }
      // 从内向外,迭代地将不变量往外层提取,for(auto loop : *loops)不会重复处理,因为外层循环间是独立的
      cur_loop = CFG_analyser->find_outer_loop(cur_loop);
    }
  }
  std::cout << "Finished" << std::endl;
}

void LoopInvariant::find_invariants(std::vector<BasicBlock *> *loop) {
  std::set<Value *> defined_in_loop;
  std::map<Value *, std::set<Instruction *>> addr2st;
  std::map<Instruction *, Value *> st2addr; // store指令store的基础地址
  std::map<Instruction *, Value *> ld2addr; // load指令load的基础地址
  std::vector<Instruction *> invariant;
  bool have_call = false;
  for (auto bb: *loop) {
    for (auto instr: bb->get_instructions()) {
      defined_in_loop.insert(instr);
      if (instr->is_store()) {
        // store is_void
        auto ptr = instr->get_operand(1);
        addr2st[ptr].insert(instr);
        // 计算store的基础地址(数组起始地址)
        while (dynamic_cast<Instruction *>(ptr) &&
               dynamic_cast<Instruction *>(ptr)->is_gep()) {
          ptr = dynamic_cast<Instruction *>(ptr)->get_operand(0);
        }
        st2addr[instr] = ptr;
      }
      if (instr->is_load()) {
        auto ptr = instr->get_operand(0);
        // 计算load的基础地址
        while (dynamic_cast<Instruction *>(ptr) &&
               dynamic_cast<Instruction *>(ptr)->is_gep()) {
          ptr = dynamic_cast<Instruction *>(ptr)->get_operand(0);
        }
        ld2addr[instr] = ptr;
      }
      if (instr->is_call())
        have_call = true;
    }
  }
  bool changed = false;
  do {
    bool st_changed = false;
    changed = false;
    for (auto bb: *loop) {
      invariant.clear();
      for (auto instr: bb->get_instructions()) {
        bool is_invariant = true;
        if (instr->is_call() || instr->is_alloca() || instr->is_ret() || instr->is_br() ||
            instr->is_cmp() || instr->is_phi())
          continue;
        if (defined_in_loop.find(instr) == defined_in_loop.end())
          continue;
        // 判断操作数是否在loop中被定义,如果定义过就不是循环不变量
        // 因为已经常量传播过了,所以可以直接用这个判断是不是循环不变量,即不考虑某条指令的执行结果为固定值这类循环不变量
        for (auto operand: instr->get_operands()) {
          if (defined_in_loop.find(operand) != defined_in_loop.end()) {
            is_invariant = false;
          }
        }
        if (instr->is_store()) continue;
        // 如果有函数调用,load直接被判定为不是invariant
        if ((instr->is_load() || instr->is_store()) && have_call)
          continue;
        // if(instr->is_store() ){
        //     auto ptr = st2addr[instr];
        //     if(addr2st[ptr].size()>1)
        //         continue;
        // }
        // 判断loop中的store有没有写入过要load的基础地址,如果写入过,则load不是invariant
        // int a[2]; int *b = a; 因为不会有这样的操作,所以可以用数组名字(因为所有地方只会用a来访问数组)来看有没有向数组写入
        if (is_invariant && instr->is_load()) {
          auto ptr = ld2addr[instr];
          for (auto pair: st2addr) {
            if (pair.second == ptr) {
              is_invariant = false;
              break;
            }
          }
        }
        if (is_invariant) {
          // 在loop定义中删除不变量
          defined_in_loop.erase(instr);
          invariant.push_back(instr);
          changed = true;
        }
      }
      if (!invariant.empty()) {
        invariants.push_back({bb, invariant});
      }
    }
  } while (changed);
}

void LoopInvariant::move_invariants_out(std::vector<BasicBlock *> *loop) {
  auto first_bb = CFG_analyser->find_loop_entry(loop);
  BasicBlock *new_bb = BasicBlock::create(m_, "", first_bb->get_parent()/*Function **/);

  // 找到所有不在本循环内的loop header的前驱
  std::vector<BasicBlock *> pre_bbs;
  for (auto pre_bb: first_bb->get_pre_basic_blocks()) {
    if (CFG_analyser->find_bb_loop(pre_bb) != loop) {
      pre_bbs.push_back(pre_bb);
    }
  }

  // 逐个处理循环头里的所有phi指令，把所有来自循环外的phi指令操作数移动到new_bb中
  for (auto instr: first_bb->get_instructions()) {
    if (!instr->is_phi())
      break;
    auto inst_phi = dynamic_cast<PhiInst *>(instr);
    std::vector<std::pair<Value *, BasicBlock *>> ops_in_loop; // phi指令中循环内的操作数
    std::vector<std::pair<Value *, BasicBlock *>> ops_out_loop; // phi指令中循环外的操作数
    for (auto i = 0; i < instr->get_num_operand(); i = i + 2) {
      auto val = instr->get_operand(i);
      auto from_bb = dynamic_cast<BasicBlock *>(instr->get_operand(i + 1));
      val->remove_use(instr);
      from_bb->remove_use(instr);
      if (CFG_analyser->find_bb_loop(from_bb) == loop) {
        ops_in_loop.push_back({val, from_bb});
      } else {
        ops_out_loop.push_back({val, from_bb});
      }
    }
    // ops_out_loop:自循环外的phi指令操作数
    if (ops_out_loop.size() > 1) {
      auto new_phi = PhiInst::create_phi((*ops_out_loop.begin()).first->get_type(), new_bb);
      new_bb->add_instruction(new_phi);
      // 把这些循环外的phi指令操作数移动到new_phi中
      for (auto pair: ops_out_loop) {
        new_phi->add_phi_pair_operand(pair.first, pair.second);
      }
      for (int i = 0; i < (int) inst_phi->get_num_operand() / 2; i++) {
        auto value = inst_phi->get_operand(i * 2);
        auto bb = inst_phi->get_operand(i * 2 + 1);
        value->remove_use(inst_phi);
        bb->remove_use(inst_phi);
      }
      inst_phi->remove_operands(0, instr->get_num_operand() - 1);
      // 循环外的phi指令操作数保持不变，还是在循环块头的phi指令中
      for (auto pair: ops_in_loop) {
        inst_phi->add_phi_pair_operand(pair.first, pair.second);
      }
      // 把new_phi添加到new_bb中
      inst_phi->add_phi_pair_operand(new_phi, new_bb);
    } else {
      // 只有一个来自循环外的操作数，不需要创建新的phi指令
      for (int i = 0; i < (int) inst_phi->get_num_operand() / 2; i++) {
        auto value = inst_phi->get_operand(i * 2);
        auto bb = inst_phi->get_operand(i * 2 + 1);
        value->remove_use(inst_phi);
        bb->remove_use(inst_phi);
      }
      inst_phi->remove_operands(0, instr->get_num_operand() - 1);
      for (auto pair: ops_out_loop) {
        inst_phi->add_phi_pair_operand(pair.first, new_bb);
      }
      for (auto pair: ops_in_loop) {
        inst_phi->add_phi_pair_operand(pair.first, pair.second);
      }
    }
  }

  // 将loop中的invariants添加到new_bb中
  for (auto pair: invariants) {
    for (auto instr: pair.second) {
      pair.first->get_instructions().remove(instr);
      new_bb->add_instruction(instr);
      instr->set_parent(new_bb);
    }
  }
  // 从new_bb跳转到first_bb
  BranchInst::create_br(first_bb, new_bb);

  // 在循环头的所有不在循环内的前驱块中添加跳转指令，跳转到new_bb中
  for (auto pre_bb: pre_bbs) {
    auto terminator = pre_bb->get_terminator();
    if (terminator->get_num_operand() == 1) {
      // 直接跳转
      terminator->set_operand(0, new_bb);
    } else {
      // 条件跳转
      if (dynamic_cast<BasicBlock *>(terminator->get_operand(1)) == first_bb) {
        terminator->set_operand(1, new_bb);
      } else {
        terminator->set_operand(2, new_bb);
      }
    }
    first_bb->remove_use(terminator);
    first_bb->remove_pre_basic_block(pre_bb);
    pre_bb->remove_succ_basic_block(first_bb);
    new_bb->add_pre_basic_block(pre_bb);
    pre_bb->add_succ_basic_block(new_bb);
  }
}



