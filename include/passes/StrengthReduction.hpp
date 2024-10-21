#pragma once

#include "Module.hpp"
#include "PassManager.hpp"
#include "Constant.hpp"
#include "ConstantPropagation.hpp"
#include "IRBuilder.hpp"
#include "logging.hpp"
#include <cmath>
#include <string>
#include <vector>
#include <map>
#include <set>

class StrengthReduction : public Pass {
private:
    Module *module_;
    std::string name = "StrengthReduction";

public:
    explicit StrengthReduction(Module *module, bool print_ir = false) : Pass(module) { module_ = module; }

    ~StrengthReduction() {};

    void run() final;

    const std::string get_name() const { return name; }

    bool isNthPower(int x);

    void specInstReduct();

    void specInstReductStrict();

private:
    std::string filename = "";
};

