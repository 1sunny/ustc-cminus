#include"ActiveVar.hpp"

void ActiveVar::run() {
  for (auto curr_func: this->m_->get_functions()) {
    if (curr_func->get_basic_blocks().empty())
      continue;
    else {
      this->func = curr_func;
      ilive_in.clear();
      ilive_out.clear();
      flive_in.clear();
      flive_out.clear();
      map_def.clear();
      map_use.clear();
      get_def_use();
      get_in_out_var();
      for (auto bb: this->func->get_basic_blocks()) {
        bb->set_live_in_int(ilive_in[bb]);
        bb->set_live_out_int(ilive_out[bb]);
        bb->set_live_in_float(flive_in[bb]);
        bb->set_live_out_float(flive_out[bb]);
      }
      std::cout << "**************************************************" << std::endl;
      std::cout << func->print() << std::endl;
      for (auto bb: func->get_basic_blocks()) {
        bb->set_live_in_int(ilive_in[bb]);
        std::cout << "=======================================" << std::endl;
        std::cout << "BasicBlock:" << std::endl;
        std::cout << bb->print() << std::endl;
        std::cout << "live_in:" << std::endl;
        for (auto var: ilive_in[bb]) {
          std::cout << var->print() << "---" << var->get_name() << std::endl;
          if (map_phi[bb].count(var)) {
            // assert(false);
          }
        }
        std::cout << "live_out:" << std::endl;
        for (auto var: ilive_out[bb]) {
          std::cout << var->print() << "---" << var->get_name() << std::endl;
          if (map_phi[bb].count(var)) {
            // assert(false);
          }
        }
        std::cout << "=======================================" << std::endl;
      }
      std::cout << "**************************************************" << std::endl;
    }
  }
}


void ActiveVar::get_def_use() {
  for (auto bb: func->get_basic_blocks()) {
    std::set<Value *> tmp_def;
    std::set<Value *> tmp_use;
    std::set<Value *> tmp_phi;
    map_def.insert(make_pair(bb, tmp_def));
    map_use.insert(make_pair(bb, tmp_use));
    map_phi.insert(make_pair(bb, tmp_phi));
    for (auto instr: bb->get_instructions()) {
      if (!instr->is_void()) {
        map_def[bb].insert(instr);
      }
      if (instr->get_instr_type() == Instruction::OpID::phi) {
        map_phi[bb].insert(instr);
      } else {
        for (auto op: instr->get_operands()) {
          // 跳过全局变量
          if (dynamic_cast<GlobalVariable *>(op)) continue;
          if (dynamic_cast<ConstantInt *>(op) || dynamic_cast<ConstantFP *>(op)) {
            std::cout << "skip constant in ActiveVar" << std::endl;
            continue;
          }
          if (dynamic_cast<BasicBlock *>(op)) {
            std::cout << "skip BasicBlock in ActiveVar" << std::endl;
            continue;
          }
          if (dynamic_cast<Function *>(op)) {
            std::cout << "skip Function in ActiveVar" << std::endl;
            continue;
          }
          if (!map_def[bb].count(op))
            map_use[bb].insert(op);
        }
      }
    }
  }
}

void ActiveVar::get_in_out_var() {
  // 把基本块内使用的变量加入该基本块的live集合
  for (auto bb: this->func->get_basic_blocks()) {
    for (auto val: map_use[bb]) {
      if (val->get_type()->is_float_type()) {
        flive_in[bb].insert(val);
      } else {
        ilive_in[bb].insert(val);
      }
    }
    ilive_out.insert({bb, {}});
    flive_out.insert({bb, {}});
  }
  bool changed = true;
  while (changed) {
    changed = false;
    for (auto bb: this->func->get_basic_blocks()) {
      auto &iout = ilive_out[bb];
      auto &fout = flive_out[bb];
      for (auto succ_bb: bb->get_succ_basic_blocks()) {
        for (auto ilvin: ilive_in[succ_bb]) {
          if (auto constInt = dynamic_cast<ConstantInt *>(ilvin); constInt) {
            std::cout << "noooooooooo!\n";
            std::cout << constInt->print() << '\n';
          }
          // phi不算进live_in里
          if (map_phi[succ_bb].count(ilvin)) {
            continue;
          }
          if (!iout.count(ilvin)) {
            changed = true;
            iout.insert(ilvin);
            // 数据流方程中的减去操作
            if (!map_def[bb].count(ilvin)) {
              ilive_in[bb].insert(ilvin);
            }
          }
        }
        for (auto flvin: flive_in[succ_bb]) {
          if (map_phi[succ_bb].count(flvin)) {
            continue;
          }
          if (!fout.count(flvin)) {
            changed = true;
            fout.insert(flvin);
            if (!map_def[bb].count(flvin)) {
              flive_in[bb].insert(flvin);
            }
          }
        }
        // 后继入口处活跃的变量并非在它所有前驱的出口处都是活跃的
        std::set<Value *> succ_phi = map_phi[succ_bb];
        for (auto phi: succ_phi) {
          auto phi_instr = dynamic_cast<PhiInst *>(phi);
          for (int i = 0; i < phi_instr->get_operands().size(); i += 2) {
            auto vused = phi_instr->get_operand(i);
            auto pre_bb = dynamic_cast<BasicBlock *>(phi_instr->get_operand(i + 1));
            if (vused->get_type()->is_float_type()) {
              if (!fout.count(vused) && pre_bb == bb) {
                changed = true;
                fout.insert(vused);
                if (!map_def[bb].count(vused)) {
                  flive_in[bb].insert(vused);
                }
              }
            } else {
              if (!iout.count(vused) && pre_bb == bb) {
                changed = true;
                iout.insert(vused);
                if (!map_def[bb].count(vused)) {
                  if (auto constInt = dynamic_cast<ConstantInt *>(vused); constInt) {
                    // std::cout << "noooooooooo!\n";
                    // std::cout << bb->print() << " insert " << constInt->print() << '\n';
                  }
                  ilive_in[bb].insert(vused);
                }
              }
            }
          }
        }
      }
    }
  }
  // 把phi里面的操作数从live_in中删除
  //  因为前驱发现后继块里面的phi中使用了自己块中的value,该value就会加入live_out中,
  //  进而流向后继的live_in,但这个value除了计算phi变量的值,后面并不会活跃,phi变量消除的时候就没了
  for (auto bb: this->func->get_basic_blocks()) {
    for (auto phi: map_phi[bb]) {
      auto phi_instr = dynamic_cast<PhiInst *>(phi);
      for (int i = 0; i < phi_instr->get_operands().size(); i += 2) {
        if (phi_instr->get_operand(i)->get_type()->is_float_type()) {
          if (flive_in[bb].count(phi_instr->get_operand(i))) {
            // exit(-1);
          }
          flive_in[bb].erase(phi_instr->get_operand(i));
        } else {
          if (ilive_in[bb].count(phi_instr->get_operand(i))) {
            // exit(-1);
          }
          ilive_in[bb].erase(phi_instr->get_operand(i));
        }
      }
    }
  }
}
