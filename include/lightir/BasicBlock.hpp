#pragma once

#include "Instruction.hpp"
#include "Value.hpp"

#include <list>
#include <llvm/ADT/ilist.h>
#include <llvm/ADT/ilist_node.h>
#include <set>
#include <string>

class Function;
class Instruction;
class Module;

class BasicBlock : public Value/*, public llvm::ilist_node<BasicBlock>*/ {
  public:
    ~BasicBlock() = default;
    // 创建并返回基本块，参数分别是基本块所属的模块，基本块名字（默认为空），基本块所属的函数
    static BasicBlock *create(Module *m, const std::string &name,
                              Function *parent) {
        auto prefix = name.empty() ? "" : "label_";
        return new BasicBlock(m, prefix + name, parent);
    }

    /****************api about cfg****************/
    std::list<BasicBlock *> &get_pre_basic_blocks() { return pre_bbs_; }
    std::list<BasicBlock *> &get_succ_basic_blocks() { return succ_bbs_; }

    void add_pre_basic_block(BasicBlock *bb) { pre_bbs_.push_back(bb); }
    void add_succ_basic_block(BasicBlock *bb) { succ_bbs_.push_back(bb); }
    void remove_pre_basic_block(BasicBlock *bb) { pre_bbs_.remove(bb); }
    void remove_succ_basic_block(BasicBlock *bb) { succ_bbs_.remove(bb); }

    // If the Block is terminated by ret/br
    bool is_terminated() const;
    // Get terminator, only accept valid case use
    // 返回该基本块的终止指令，若基本块的最后一条指令不是终止指令返回则返回 nullptr
    Instruction *get_terminator();

    /****************api about Instruction****************/
    // 将指令 instr 添加到该基本块的指令链表末端，使用 IRBuilder 来创建函数时会自动调用此方法
    void add_instruction(Instruction *instr);
    // 将指令 instr 添加到该基本块的指令链表首部
    void add_instr_begin(Instruction *instr) { instr_list_.push_front(instr); }
    // 将指令 instr 从该基本块的指令链表中移除，该 API 会同时维护好 instr 的操作数的 use 链表。
    void erase_instr(Instruction *instr) {
        instr_list_.remove(instr);
        instr->remove_all_operands();
    }

    // llvm::ilist<Instruction> &get_instructions() { return instr_list_; }
    std::list<Instruction *> &get_instructions() { return instr_list_; }
    bool empty() const { return instr_list_.empty(); }
    int get_num_of_instr() const { return instr_list_.size(); }

    /****************api about accessing parent****************/
    Function *get_parent() { return parent_; }
    Module *get_module();
    void erase_from_parent();

    virtual std::string print() override;

  private:
    BasicBlock(const BasicBlock &) = delete;
    explicit BasicBlock(Module *m, const std::string &name, Function *parent);

    std::list<BasicBlock *> pre_bbs_;
    std::list<BasicBlock *> succ_bbs_;
    std::list<Instruction *> instr_list_;
    // llvm::ilist<Instruction> instr_list_;
    Function *parent_;
};
