#include "DeadCode.hpp"
#include "logging.hpp"
#include "myassert.h"

// 处理流程：两趟处理，mark 标记有用变量，sweep 删除无用指令
void DeadCode::run() {
    bool changed{};
    func_info->run();
    int cnt=0;
    do {
        changed = false;
        for (auto &F : m_->get_functions()) {
            auto func = F;
            mark(func);
            changed |= sweep(func);
        }
        cnt++;
    } while (changed);
    if (cnt > 2) {
      LOG_ERROR << "dead code pass execute twice !!!";
      exit(123);
    }
    LOG_INFO << "dead code pass erased " << ins_count << " instructions";
}

void DeadCode::mark(Function *func) {
    work_list.clear();
    marked.clear();
    // 将关键指令(影响范围可能在函数外的指令)比较并加入work_list
    for (auto &bb : func->get_basic_blocks()) {
        for (auto &ins : bb->get_instructions()) {
            if (is_critical(ins)) {
                marked[ins] = true;
                work_list.push_back(ins);
            }
        }
    }

    while (work_list.empty() == false) {
        auto now = work_list.front();
        work_list.pop_front();

        mark(now);
    }
}

void DeadCode::mark(Instruction *ins) {
    for (auto op : ins->get_operands()) {
        auto def = dynamic_cast<Instruction *>(op);
        if (def == nullptr)
            continue;
        if (marked[def])
            continue;
        // 如果是Instruction,那应该是在同一个函数吧??
        MY_ASSERT(def->get_function() == ins->get_function());
        if (def->get_function() != ins->get_function())
            continue;
        marked[def] = true;
        work_list.push_back(def);
    }
}

bool DeadCode::sweep(Function *func) {
    std::unordered_set<Instruction *> wait_del{};
    for (auto &bb : func->get_basic_blocks()) {
        for (auto it = bb->get_instructions().begin();
             it != bb->get_instructions().end();) {
            if (marked[*it]) {
                ++it;
                continue;
            } else {
                auto tmp = *it;
                wait_del.insert(tmp);
                it++;
            }
        }
    }
    for (auto inst : wait_del)
        inst->remove_all_operands();//没用 inst->get_parent()->erase_instr里面会删除
    for (auto inst : wait_del) {
        inst->get_parent()->erase_instr(inst);
        // std::cout << "sweep: " << inst->print() << std::endl;
    }

    ins_count += wait_del.size();
    return not wait_del.empty(); // changed
}

bool DeadCode::is_critical(Instruction *ins) {
    // 对纯函数的无用调用也可以在删除之列
    if (ins->is_call()) {
        auto call_inst = dynamic_cast<CallInst *>(ins);
        auto callee = dynamic_cast<Function *>(call_inst->get_operand(0));
        if (func_info->is_pure_function(callee))
            return false;
        return true;
    }
    if (ins->is_br() || ins->is_ret() || ins->is_cmpbr() || ins->is_fcmpbr())
        return true;
    if (ins->is_store() || ins->is_storeoffset()/* || ins->is_memset()*/)
        return true;
    return false;
}
