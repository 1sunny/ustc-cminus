#pragma once

#include "BasicBlock.hpp"
#include "Function.hpp"
#include "Instruction.hpp"
#include "Module.hpp"
#include "PassManager.hpp"


class TailRecursion : public Pass {
  public:
    TailRecursion(Module *m, bool print_ir=false) : Pass(m) {}
    void run() final;
    CallInst *get_candidate(BasicBlock *);
    void create_header();
    bool eliminate_call(CallInst *);

    const std::string get_name() const {return name;}

  private:
    Function *f_;
    BasicBlock* preheader;
    BasicBlock* latch;
    BasicBlock* header;
    BasicBlock* return_bb;
    BasicBlock* cur_bb;
    std::vector<PhiInst *> phi_args, args_latch;
    Instruction *ret_phi;
    PhiInst* acc_phi;
    PhiInst* acc_latch;
    std::string name;
};