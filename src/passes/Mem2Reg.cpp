#include "Mem2Reg.hpp"
#include "IRBuilder.hpp"
#include "Value.hpp"
#include "logging.hpp"
#include "myassert.h"
#include "Constant.hpp"

#include <memory>
#include <unordered_set>
#include <queue>

void Mem2Reg::run() {
    // 创建支配树分析 Pass 的实例
    dominators_ = std::make_unique<Dominators>(m_);
    // 建立支配树
    dominators_->run();
    // 以函数为单元遍历实现 Mem2Reg 算法
    for (auto &f : m_->get_functions()) {
        if (f->is_declaration())
            continue;
        func_ = f;
        if (func_->get_basic_blocks().size() >= 1) {
            bb_phi2value_.clear();
            bb_value2phi_.clear();
            stack_.clear();

            // 对应伪代码中 phi 指令插入的阶段
            generate_phi();
            // 对应伪代码中重命名阶段
            // 可以先把rename注释掉来测试generate_phi
            rename(func_->get_entry_block());
        }
        // 后续 DeadCode 将移除冗余的局部变量的分配空间
    }
}

/*
1.LLVM assumes that all locals are introduced in the entry basic block of a function with an alloca instruction.
LLVM also assumes that all allocas appear at the start of the entry block continuously.
This assumption could be easily violated by the front-end, but it is a reasonable assumption to make.

2.For each alloca seen in step 1, LLVM checks if it is promotable based on the use of this local.
It is deemed promotable iff:
 (a) It is not used in a volatile instruction.
 (b) It is loaded or stored directly, i.e, its address is not taken.

3.For each local selected for promotion in step 2, a list of blocks which use it,
and a list of block which define it are maintained by making a linear sweep over each instruction of each block.
4.Some basic optimizations are performed:
 (a) Unused allocas are removed.
 (b) If there is only one defining block for an alloca, all loads which are dominated by the definition are replaced with the value.
 (c) allocas which are read and written only in a block can avoid traversing CFG, and PHI-node insertion by simply inserting each load with the value from nearest store.
5.A dominator tree is constructed.
6.For each candidate for promotion, points to insert PHI nodes is computed as follows:
 (a) A list of blocks which use it without defining it (live-in blocks or upward exposed blocks) are determined with the help of using and defining blocks created in Step 3.
 (b) A priority queue keyed on dominator tree level is maintained so that inserted nodes corresponding to defining blocks are handled from the bottom of the dominator tree upwards. This is done by giving each block a level based on its position in the dominator tree.
 (c) For each node — root, in the priority queue:
  i. Iterated dominance frontier of a definition is computed by walking all dominator tree children of root, inspecting their CFG edges with targets elsewhere on the dominator tree. Only targets whose level is at most root level are added to the iterated dominance frontier.
  ii. PHI-nodes are inserted at the beginning in each block in the iterated dominance frontier computed in the previous step. There will be predecessor number of dummy argument to the PHI function at this point.
7.Once all PHI-nodes are prepared, a rename phase start with a worklist containing just entry block as follows:
 (a) A hash table of IncomingVals which is a map from a alloca to its most recent name is created. Most recent name of each alloca is an undef value to start with.
 (b) While (worklist != NULL)
   i. Remove block B from worklist and mark B as visited.
   ii. For each instruction in B:
    A. If instruction is a load instruction from location L (where L is a promotable candidate) to value V, delete load instruction, replace all uses of V with most recent value of L i.e, IncomingVals[L].
    B. If instruction is a store instruction to location L (where L is a promotable candidate) with value V, delete store instruction, set most recent name of L i.e, IncomingVals[L] = V.
    C. For each PHI-node corresponding to a alloca — L , in each successor of B, fill the corresponding PHI-node argument with most recent name for that location i.e, IncomingVals[L].
   iii. Add each unvisited successor of B to worklist.
 */
void Mem2Reg::generate_phi() {
    // TODO
    // 步骤一：找到活跃在多个 block 的全局名字集合，以及它们所属的 bb 块
    // 只在单个基本块中活动的变量，绝不会出现与之相应的活动Φ函数
    // globals存的就是不止在单个块中被使用的变量集合
    // Blocks(x)存储的是定义了x的基本块集合。
    std::unordered_set<Value*> globals_;
    std::unordered_map<Value*, std::unordered_set<BasicBlock*>> blocks_;

    globals_.clear();
    blocks_.clear();

    for (BasicBlock *b : func_->get_basic_blocks()) {
        std::unordered_set<Value*> var_kill;
        for (Instruction *i : b->get_instructions()) {
            if (i->is_store()) { // define
                Value *x = i->get_operand(1);
                LOG_DEBUG << x->get_name();
                if (is_valid_ptr(x)) {
                    var_kill.insert(x);
                    blocks_[x].insert(b);
                }
            } else if (i->is_load()) { // use
                for (Value *op : i->get_operands()) {
                    LOG_DEBUG << op->get_name();
                    if (is_valid_ptr(op) && var_kill.find(op) == var_kill.end()) {
                        globals_.insert(op);
                    }
                }
            }
        }
    }

    // 步骤二：从支配树获取支配边界信息，并在对应位置插入 phi 指令
    std::queue<BasicBlock*> workList;
    for (Value *x : globals_) {
        std::unordered_map<BasicBlock*, bool> has_x_phi;
        std::unordered_map<BasicBlock*, bool> in_work_list;
        for (BasicBlock *b: blocks_[x]) {
            workList.push(b);
            in_work_list[b] = true;
        }

        while (not workList.empty()) {
            BasicBlock *b = workList.front();
            workList.pop();
            const Dominators::BBSet &df_b = dominators_->get_dominance_frontier(b);
            for (BasicBlock *d : df_b) {
                if (not has_x_phi[d]) {
                    // insert a Φ-function for x in d
                    // x->get_type() -> x->get_type()->get_pointer_element_type()
                    PhiInst *phi_x = PhiInst::create_phi(x->get_type()->get_pointer_element_type(), d);
                    d->add_instr_begin(phi_x);
                    bb_value2phi_[d][x] = phi_x;
                    bb_phi2value_[d][phi_x] = x;
                    has_x_phi[d] = true;
                    //
                    if (not in_work_list[d]) {
                        workList.push(d);
                        in_work_list[d] = true;
                    }
                }
            }
        }
    }
}

void Mem2Reg::rename(BasicBlock *bb) {
    // TODO
    // 步骤三：将 phi 指令作为 lval 的最新定值，lval 即是为局部变量 alloca 出的地址空间
    // 步骤四：用 lval 最新的定值替代对应的load指令
    // 步骤五：将 store 指令的 rval，也即被存入内存的值，作为 lval 的最新定值
    // 步骤六：为 lval 对应的 phi 指令参数补充完整
    // 步骤七：对 bb 在支配树上的所有后继节点，递归执行 re_name 操作
    // 步骤八：pop出 lval 的最新定值
    // 步骤九：清除冗余的指令
    LOG_DEBUG << bb->get_name();
    // std::unordered_map<Value*, std::stack<Value*>> stack_;
    for (Instruction *i : bb->get_instructions()) {
        LOG_DEBUG << i->get_name() << ":" << i->get_type();
        if (i->is_phi()) {
            // find the phi function to which Memory Variables i belongs
            Value *x = bb_phi2value_[bb][i];
            MY_ASSERT(is_valid_ptr(x));
            stack_[x].push(i);
        } else if (i->is_store()) {
            Value *x = i->get_operand(1);
            Value *val = i->get_operand(0);
            if (is_valid_ptr(x)) {
                stack_[x].push(val);
            } else {
                MY_ASSERT(stack_[x].empty());
            }
        } else if (i->is_load()) {
            Value *x = i->get_operand(0);
            LOG_DEBUG << x->get_name();
            if (is_valid_ptr(x)) {
                MY_ASSERT(not stack_[x].empty());
                // 可能会是空
                i->replace_all_use_with(stack_[x].top());
            }
        }
    }
    // foreach S in successor of B in the CFG do:
    //     fill in memory variables i’s phi-function parameters with top(stack[i])
    for (BasicBlock * cfg_succ : bb->get_succ_basic_blocks()) {
        for (std::pair<Value *, PhiInst*> p: bb_value2phi_[cfg_succ]) {
            Value *x = p.first;
            PhiInst *phi = p.second;
            if (not stack_[x].empty()) {
                phi->add_phi_pair_operand(stack_[x].top(), bb);
            } // else 由IRPrint处理
        }
    }
    for (BasicBlock * dom_tree_succ : dominators_->get_dom_tree_succ_blocks(bb)) {
        rename(dom_tree_succ);
    }
    for (Instruction *i : bb->get_instructions()) {
        if (i->is_phi()) {
            Value *x = bb_phi2value_[bb][i];
            LOG_DEBUG << "pop";
            stack_[x].pop();
        } else if (i->is_store()) {
            Value *x = i->get_operand(1);
            if (is_valid_ptr(x)) {
                LOG_DEBUG << "pop";
                stack_[x].pop();
            }
        }
    }
    std::unordered_set<Instruction *> wait_del{};
    for (Instruction *i : bb->get_instructions()) {
        if (i->is_alloca()) {
            auto *alloc = dynamic_cast<AllocaInst *>(i);
            Type *alloca_type = alloc->get_alloca_type();
            // ?
            if (alloca_type->is_array_type()) {
                continue;
            }
            MY_ASSERT(alloca_type->is_pointer_type() || alloca_type->is_float_type() || alloca_type->is_integer_type());
            wait_del.insert(i);
            for (Use use : i->get_use_list()) {
                auto *ins = dynamic_cast<Instruction *>(use.val_);
                MY_ASSERT(ins && (ins->is_store() || ins->is_load()));
                wait_del.insert(ins);
            }
        }
    }
    for (auto inst : wait_del)
        inst->remove_all_operands();
    for (auto inst : wait_del)
        inst->get_parent()->erase_instr(inst);
}
