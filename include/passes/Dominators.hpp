#pragma once

#include "BasicBlock.hpp"
#include "PassManager.hpp"

#include <map>
#include <set>

class Dominators : public Pass {
  public:
    using BBSet = std::set<BasicBlock *>;

    explicit Dominators(Module *m) : Pass(m) {}
    ~Dominators() = default;
    void run() override;

    BasicBlock *get_idom(BasicBlock *bb) { return idom_.at(bb); }
    const BBSet &get_dominance_frontier(BasicBlock *bb) {
        return dom_frontier_.at(bb);
    }
    const BBSet &get_dom_tree_succ_blocks(BasicBlock *bb) {
        return dom_tree_succ_blocks_.at(bb);
    }

  private:
    void create_idom(Function *f);
    void create_dominance_frontier(Function *f);
    void create_dom_tree_succ(Function *f);

    // TODO 补充需要的函数

    std::map<BasicBlock *, BasicBlock *> idom_{};  // 直接支配
    std::map<BasicBlock *, BBSet> dom_frontier_{}; // 支配边界集合
    std::map<BasicBlock *, BBSet> dom_tree_succ_blocks_{}; // 支配树中的后继节点

    void get_reverse_postorder(Function *f);
    void search(BasicBlock *block);
    BasicBlock *intersect(BasicBlock *b1, BasicBlock *b2);
    void print_dom_tree(Function *f);
    void print_dom_frontier();

private:
    std::vector<BasicBlock *> reverse_postorder_{};
    std::vector<BasicBlock *> dom_tree_postorder_{};
    int timestamp = 1;
    std::unordered_map<BasicBlock *, int> bb_timestamp_{};
    std::unordered_map<BasicBlock *, bool> visited_{};
    std::unordered_map<BasicBlock *, bool> processed_{};

    void get_dom_tree_postorder(BasicBlock *block);
};
