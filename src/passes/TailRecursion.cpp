#include "ConstantPropagation.hpp"
#include "DeadCode.hpp"
#include "TailRecursion.hpp"

void TailRecursion::run() {
  for (auto f: m_->get_functions()) {
    // 如果函数返回类型是void类型,直接跳过
    if (f->get_return_type()->is_void_type())
      continue;
    f_ = f;
    preheader = latch = header = return_bb = nullptr;
    phi_args.clear();
    args_latch.clear();
    ret_phi = nullptr;
    acc_phi = acc_latch = nullptr;
    if (f_->get_basic_blocks().size() == 0)
      continue;
    for (auto bb: f->get_basic_blocks()) {
      cur_bb = bb;
      if (auto call = get_candidate(cur_bb)) {
        auto b = eliminate_call(call);
      }
    }
  }

  // auto cp = ConstProp(m_);
  // cp.execute();

  // auto dce = DeadCode(m_);
  // dce.execute();
}

bool TailRecursion::eliminate_call(CallInst *call) {
  auto parent = call->get_parent();
  // const auto call_it = call->get_iterator();
  auto it = cur_bb->get_instructions().begin();
  for (; it != cur_bb->get_instructions().end(); it++) {
    auto call_instr = dynamic_cast<CallInst *>(*it);
    if (call_instr && call_instr == call)
      break;
  }
  // 没找到这条CallInst call
  if (it == cur_bb->get_instructions().end()) {
    return false;
  }
  it++;
  // auto it = ++call->get_iterator();
  auto term_it = call->get_parent()->get_terminator_itr();
  Instruction *accumulator_or_call = dynamic_cast<Instruction *>(call);
  // 若通过中间的空跳转块，无法消除
  if (!(*term_it)->is_br()) {
    return false;
  }
  // 不能是条件分支指令,必须直接跳转到ret基本块
  if ((*term_it)->get_num_operand() == 3) {
    return false;
  }
  BasicBlock *bb = dynamic_cast<BasicBlock *>((*term_it)->get_operand(0));
  // 当前基本块的终止指令需要是返回指令
  if (!bb->get_terminator()->is_ret()) {
    return false;
  } else {
    ret_phi = dynamic_cast<Instruction *>(bb->get_terminator()->get_operand(0));
    // return的那个操作数一定是PhiInst,该函数一定有2个return语句
    assert(dynamic_cast<PhiInst *>(ret_phi));
    return_bb = bb;
  }

  if (it != term_it)
    return false;
  int id = -1;
  for (auto use: accumulator_or_call->get_use_list())
    if (use.val_ == ret_phi) {
      id = use.arg_no_;
      break;
    }
  if (id == -1) {
    LOG(ERROR) << "did not find use of call";
    return false;
  }
  if (not preheader)
    create_header();

  for (size_t i = 1; i < call->get_num_operand(); ++i) {
    auto op = call->get_operand(i);
    // 可能会有多个尾递归调用指令
    args_latch[i - 1]->add_phi_pair_operand(op, call->get_parent());
  }
  // 把原来ret_phi上使用call返回值对应的操作数删掉(值和基本块)
  ret_phi->remove_operands(id, id + 1);
  // 删除后需要调整use的arg_no
  for (int i = 0; i < (int) ret_phi->get_num_operand() / 2; i++) {
    auto value = ret_phi->get_operand(i * 2);
    auto bb = ret_phi->get_operand(i * 2 + 1);
    value->remove_use(ret_phi);
    value->add_use(ret_phi, i * 2);
    bb->remove_use(ret_phi);
    bb->add_use(ret_phi, i * 2 + 1);
  }
  // 去掉基本块的调用指令和之前的跳转指令,改为跳转到tail_latch
  parent->erase_instr(call);
  parent->erase_instr(*(parent->get_terminator_itr()));
  for (auto succ: parent->get_succ_basic_blocks())
    succ->remove_pre_basic_block(parent);
  parent->get_succ_basic_blocks().clear();
  // parent -> latch
  BranchInst::create_br(latch, parent);

  return true;
}

CallInst *TailRecursion::get_candidate(BasicBlock *bb) {
  auto &list = bb->get_instructions();
  for (auto it = list.rbegin(); it != list.rend(); ++it) {
    auto call = dynamic_cast<CallInst *>(*it);
    if (call && dynamic_cast<Function *>(call->get_operand(0)) == f_) {
      return call;
    }
  }
  return {};
}

void TailRecursion::create_header() {
  auto original_entry = header = f_->get_entry_block();
  preheader = BasicBlock::create(m_, "", f_);
  f_->get_basic_blocks().pop_back();
  f_->get_basic_blocks().push_front(preheader);
  // preheader用original_entry的name
  preheader->take_name(original_entry);
  // 从preheader跳转到original_entry(tail_header)
  BranchInst::create_br(original_entry, preheader);
  latch = BasicBlock::create(m_, "", f_); // latch:循环的结尾块,用于在递归结束后跳转回循环开头(tail_header)
  latch->set_name("tail_latch");
  // 根据循环条件判断是跳转到return_bb还是循环开头original_entry
  BranchInst::create_cond_br(ConstantInt::get(true, m_), original_entry, return_bb, latch);
  original_entry->set_name("tail_header");

  // 将alloca指令移动到preheader
  std::vector<Instruction *> delete_list;
  for (auto instr: original_entry->get_instructions()) {
    if (instr->is_alloca()) {
      delete_list.push_back(instr);
    }
  }
  for (auto instr: delete_list) {
    original_entry->erase_instr(instr);
    preheader->add_instr_begin(instr);
  }


  for (auto arg: f_->get_args()) {
    // 第一次进入循环(来自label_entry)或来自循环末尾(latch)
    PhiInst *phi_arg = PhiInst::create_phi(arg->get_type(), original_entry);
    original_entry->add_instr_begin(dynamic_cast<Instruction *>(phi_arg));
    // 来自所有尾递归call指令,在这个基本块(latch)汇总之前的call指令要传递的函数参数值,然后跳转到tail_header(充当之前的递归操作)
    PhiInst *phi_latch = PhiInst::create_phi(arg->get_type(), latch);
    latch->add_instr_begin(dynamic_cast<Instruction *>(phi_latch));
    arg->replace_all_use_with(phi_arg);
    phi_arg->add_phi_pair_operand(dynamic_cast<Value *>(arg), preheader);
    phi_arg->add_phi_pair_operand(dynamic_cast<Value *>(phi_latch), latch);
    phi_args.push_back(phi_arg);
    args_latch.push_back(phi_latch);
  }
}
