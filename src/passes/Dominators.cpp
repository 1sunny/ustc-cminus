#include <queue>
#include "Dominators.hpp"
#include "myassert.h"

void Dominators::run() {
    for (auto &f1 : m_->get_functions()) {
        auto f = f1;
        if (f->get_basic_blocks().size() == 0)
            continue;
        for (auto &bb1 : f->get_basic_blocks()) {
            auto bb = bb1;
            idom_.insert({bb, {}});
            dom_frontier_.insert({bb, {}});
            dom_tree_succ_blocks_.insert({bb, {}});
        }

        get_reverse_postorder(f);

        create_idom(f);
        create_dominance_frontier(f);
        create_dom_tree_succ(f);

        print_dom_tree(f);
        print_dom_frontier();
    }
}

void Dominators::search(BasicBlock *block) {
    visited_[block] = true;
    for (BasicBlock * succ : block->get_succ_basic_blocks()) {
        if (not visited_[succ]) {
            search(succ);
        }
    }
    bb_timestamp_[block] = timestamp++;
    reverse_postorder_.push_back(block);
}

void Dominators::get_reverse_postorder(Function *f) {
    BasicBlock *entry_block = f->get_entry_block();
    MY_ASSERT(entry_block);
    reverse_postorder_.reserve(f->get_num_basic_blocks());
    search(entry_block);
    std::reverse(reverse_postorder_.begin(), reverse_postorder_.end());
    std::cout << "reverse postorder:\n";
    for (const auto &item: reverse_postorder_) {
      std::cout << item->get_name() << ", ";
    }
    std::cout << std::endl;
}

BasicBlock *Dominators::intersect(BasicBlock *b1, BasicBlock *b2) {
    BasicBlock *finger1 = b1;
    BasicBlock *finger2 = b2;
    while (finger1 != finger2) {
        while (bb_timestamp_[finger1] < bb_timestamp_[finger2]) {
            finger1 = idom_[finger1];
        }
        while (bb_timestamp_[finger2] < bb_timestamp_[finger1]) {
            finger2 = idom_[finger2];
        }
    }
    return finger1;
}

// TODO 算法复杂度,与其它算法的比较
void Dominators::create_idom(Function *f) {
    // TODO 分析得到 f 中各个基本块的 idom
    for (BasicBlock* b: reverse_postorder_) {
      idom_[b] = nullptr;
    }
    idom_[f->get_entry_block()] = f->get_entry_block();
    processed_[f->get_entry_block()] = true;

    bool changed = true;
    while (changed) {
        changed = false;
        for (BasicBlock* b: reverse_postorder_) {
            std::cout << b->get_name() << std::endl;
            if (b == f->get_entry_block()) {
                continue;
            }
            BasicBlock* new_idom = nullptr;
            BasicBlock* choose_p = nullptr;
            for (BasicBlock* p: b->get_pre_basic_blocks()) {
                if (processed_[p]) {
                    new_idom = p;
                    choose_p = p;
                    break;
                }
            }
            for (BasicBlock* p: b->get_pre_basic_blocks()) {
                std::cout << p->get_name() << std::endl;
                if (p != choose_p && idom_[p] != nullptr) {
                    new_idom = intersect(p, new_idom);
                }
            }
            processed_[b] = true;
            if (idom_[b] != new_idom) {
                std::cout << b->get_name() << " set idm: " << new_idom->get_name() << std::endl;
                idom_[b] = new_idom;
                changed = true;
            }
        }
    }
}

void Dominators::get_dom_tree_postorder(BasicBlock *block) {
  visited_[block] = true;
  for (BasicBlock * succ : dom_tree_succ_blocks_[block]) {
    if (not visited_[succ]) {
      get_dom_tree_postorder(succ);
    }
  }
  dom_tree_postorder_.push_back(block);
}

// 1.n支配m的一个前驱(m是一个汇合点),即q ∈ preds(m)且n ∈ Dom(q)
// 2.n并不严格支配m,即n=m或n不属于Dom(m),使用严格支配性而非支配性,可以使得在单个基本程序块构成的循环起始处插入Φ函数(在这种情况下,n=m)
void Dominators::create_dominance_frontier(Function *f) {
    // TODO 分析得到 f 中各个基本块的支配边界集合

  //  TODO 为什么支配边界还能是自己,比如一个while循环
  // the set of all cfg nodes, y, such that b dominates a predecessor of y
  // but does not strictly dominate y
  for (BasicBlock* b : f->get_basic_blocks()) {
    if (b->get_pre_basic_blocks().size() >= 2) {
      for (BasicBlock* p : b->get_pre_basic_blocks()) {
        BasicBlock* runner = p;
        while (runner != idom_[b]) {
          dom_frontier_[runner].insert(b);
          runner = idom_[runner];
        }
      }
    }
  }

/*
Build_DF() {
  for each node N in the post-order traversal of the dominator tree
    for each Node Y in Succ(N)
      if (idom(Y) != N)
        DF(N) := DF(N) ∪ {Y}
    for each Node Z in Children(N)
      for each Node Y in DF(Z)
        if (idom(Y) != N)
          DF(N) := DF(N) ∪ {Y}
}
 */
}

void Dominators::create_dom_tree_succ(Function *f) {
    // TODO 分析得到 f 中各个基本块的支配树后继
  for (BasicBlock* b : f->get_basic_blocks()) {
    MY_ASSERT(idom_[b]);
    if (b != f->get_entry_block()) {
      dom_tree_succ_blocks_[idom_[b]].insert(b);
    }
  }
}

void Dominators::print_dom_tree(Function *f) {
  std::cout << "\ndom tree:\n";
  std::queue<BasicBlock*> q_cur, q_next;
  q_next.push(f->get_entry_block());

  while (not q_next.empty()) {
    q_cur.swap(q_next);
    std::queue<BasicBlock*>().swap(q_next);
    while (not q_cur.empty()) {
      BasicBlock *b = q_cur.front();
      q_cur.pop();
      for (BasicBlock *succ: dom_tree_succ_blocks_[b]) {
        std::cout << succ->get_name() << ",";
        q_next.push(succ);
      }
      std::cout << "(" << b->get_name() << "), | ";
    }
    std::cout << std::endl;
  }
}

void Dominators::print_dom_frontier() {
  std::cout << "\ndom frontier:\n";
  for (BasicBlock *b: reverse_postorder_) {
    std::cout << b->get_name() << ": ";
    for (BasicBlock *item: dom_frontier_[b]) {
      std::cout << item->get_name() << ", ";
    }
    std::cout << std::endl;
  }
}