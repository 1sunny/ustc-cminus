#pragma once

#include "PassManager.hpp"

class FunctionInline : public Pass {
private:
    /* data */
    std::vector<std::pair<Function *, std::pair<Instruction *, Function *>>> calling_pair;
    std::string name = "FuncInline";
    Module *module_;
    int MAX_INSTRUCTION_NUM = 100;
public:
    explicit FunctionInline(Module *module, bool print_ir = false) : Pass(module) { module_ = module; }

    void run() override;

    const std::string get_name() const { return name; }

    void inline_call_find();

    void func_inline();
};

