#pragma once

#include "Module.hpp"
#include "PassManager.hpp"
#include "CFGAnalyse.hpp"
#include <vector>
#include <map>
#include <stack>
#include <set>
#include <memory>

class LoopInvariant : public Pass {

public:
    LoopInvariant(Module *m, bool print_ir = false) : Pass(m) {}

    void run() override;

    void find_invariants(std::vector<BasicBlock *> *loop);

    void move_invariants_out(std::vector<BasicBlock *> *loop);

    const std::string get_name() const { return name; }

private:
    std::vector<std::pair<BasicBlock *, std::vector<Instruction *>>> invariants;
    std::unique_ptr<CFGAnalyse> CFG_analyser;
    std::string name = "LoopInvariant";
};







