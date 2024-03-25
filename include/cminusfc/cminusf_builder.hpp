#pragma once

#include "BasicBlock.hpp"
#include "Constant.hpp"
#include "Function.hpp"
#include "IRBuilder.hpp"
#include "Module.hpp"
#include "Type.hpp"
#include "ast.hpp"

#include <map>
#include <memory>

class Scope {
  public:
    // enter a new scope
    void enter() {
      inner.emplace_back();
      array_exps_inner.emplace_back();
    }

    // exit a scope
    void exit() {
      inner.pop_back();
      array_exps_inner.pop_back();
    }

    bool in_global() { return inner.size() == 1; }

    enum VarType {
        GlobalVar,
        GlobalArray,
        LocalVar,
        LocalArray,
        ParamVar,
        ParamArray,
        Function
    };

    struct ValueWithType {
        Value *val;
        VarType type;
    };

    // push a name to scope
    // return true if successful
    // return false if this name already exits
    bool push(const std::string& name, Value *val, VarType type) {
        auto result = inner[inner.size() - 1].insert({name, {val, type}});
        return result.second;
    }

    bool push_array_exps(const std::string& name, const std::vector<int>& array_exps) {
      auto result = array_exps_inner[array_exps_inner.size() - 1].insert({name, array_exps});
      return result.second;
    }

    ValueWithType find(const std::string& name) {
        for (auto s = inner.rbegin(); s != inner.rend(); s++) {
            auto iter = s->find(name);
            if (iter != s->end()) {
                return iter->second;
            }
        }

        // Name not found: handled here?
        assert(false && "Name not found in scope");

        return {};
    }

    std::vector<int> find_array_exps(const std::string& name) {
      for (auto s = array_exps_inner.rbegin(); s != array_exps_inner.rend(); s++) {
        auto iter = s->find(name);
        if (iter != s->end()) {
          return iter->second;
        }
      }

      // Name not found: handled here?
      assert(false && "array_exps not found in scope");

      return {};
    }

  private:
    std::vector<std::map<std::string, ValueWithType>> inner;
    std::vector<std::map<std::string, std::vector<int>>> array_exps_inner;
};

class CminusfBuilder : public syntax_tree_visitor {
  public:
    CminusfBuilder() {
        module = std::make_unique<Module>();
        builder = std::make_unique<IRBuilder>(nullptr, module.get());
        auto *TyVoid = module->get_void_type();
        auto *TyInt32 = module->get_int32_type();
        auto *TyFloat = module->get_float_type();
        auto *TyInt32Ptr = module->get_int32_ptr_type();
        auto *TyFloatPtr = module->get_float_ptr_type();

        auto *input_type = FunctionType::get(TyInt32, {});
        auto *input_fun = Function::create(input_type, "input", module.get());

        std::vector<Type *> output_params;
        output_params.push_back(TyInt32);
        auto *output_type = FunctionType::get(TyVoid, output_params);
        auto *output_fun = Function::create(output_type, "output", module.get());

        std::vector<Type *> output_float_params;
        output_float_params.push_back(TyFloat);
        auto *output_float_type = FunctionType::get(TyVoid, output_float_params);
        auto *output_float_fun =
            Function::create(output_float_type, "outputFloat", module.get());

        auto *neg_idx_except_type = FunctionType::get(TyVoid, {});
        auto *neg_idx_except_fun = Function::create(
            neg_idx_except_type, "neg_idx_except", module.get());

        auto *getint_type = FunctionType::get(TyInt32, {});
        auto *getint_fun = Function::create(
                getint_type, "getint", module.get());

        auto *getch_type = FunctionType::get(TyInt32, {});
        auto *getch_fun = Function::create(
                getch_type, "getch", module.get());

        auto *getfloat_type = FunctionType::get(TyFloat, {});
        auto *getfloat_fun = Function::create(
                getfloat_type, "getfloat", module.get());

        auto *getarray_type = FunctionType::get(TyInt32, {TyInt32Ptr});
        auto *getarray_fun = Function::create(getarray_type, "getarray", module.get());

        auto *getfarray_type = FunctionType::get(TyInt32, {TyFloatPtr});
        auto *getfarray_fun = Function::create(getfarray_type, "getfarray", module.get());

        auto *putint_type = FunctionType::get(TyVoid, {TyInt32});
        auto *putint_fun = Function::create(putint_type, "putint", module.get());

        auto *putch_type = FunctionType::get(TyVoid, {TyInt32});
        auto *putch_fun = Function::create(putch_type, "putch", module.get());

        auto *putarray_type = FunctionType::get(TyVoid, {TyInt32, TyInt32Ptr});
        auto *putarray_fun = Function::create(putarray_type, "putarray", module.get());

        auto *putfloat_type = FunctionType::get(TyVoid, {TyFloat});
        auto *putfloat_fun = Function::create(putfloat_type, "putfloat", module.get());

        auto *putfarray_type = FunctionType::get(TyVoid, {TyInt32, TyFloatPtr});
        auto *putfarray_fun = Function::create(putfarray_type, "putfarray", module.get());

        scope.enter();
        scope.push("input", input_fun, Scope::VarType::Function);
        scope.push("output", output_fun, Scope::VarType::Function);
        scope.push("outputFloat", output_float_fun, Scope::VarType::Function);
        scope.push("neg_idx_except", neg_idx_except_fun, Scope::VarType::Function);
        scope.push("getint", getint_fun, Scope::VarType::Function);
        scope.push("getch", getch_fun, Scope::VarType::Function);
        scope.push("getfloat", getfloat_fun, Scope::VarType::Function);
        scope.push("getarray", getarray_fun, Scope::VarType::Function);
        scope.push("getfarray", getfarray_fun, Scope::VarType::Function);
        scope.push("putint", putint_fun, Scope::VarType::Function);
        scope.push("putch", putch_fun, Scope::VarType::Function);
        scope.push("putarray", putarray_fun, Scope::VarType::Function);
        scope.push("putfloat", putfloat_fun, Scope::VarType::Function);
        scope.push("putfarray", putfarray_fun, Scope::VarType::Function);
    }

    std::unique_ptr<Module> getModule() { return std::move(module); }

  private:
    // Clang-Tidy: 'override' is redundant since the function is already declared 'final'
    Value* visit(AstCond &) override final;
    Value* visit(AstExp &) override final;
    Value* visit(AstCompUnit &) override final;
    Value* visit(AstConstDecl &) override final;
    Value* visit(AstConstDef &) override final;
    Value* visit(AstConstInitVal &) override final;
    Value* visit(AstVarDecl &) override final;
    Value* visit(AstVarDef &) override final;
    Value* visit(AstInitVal &) override final;
    Value* visit(AstFuncDef &) override final;
    Value* visit(AstFuncFParam &) override final;
    Value* visit(AstBlock &) override final;
    Value* visit(AstBreakStmt &) override final;
    Value* visit(AstContinueStmt &) override final;
    Value* visit(AstAssignStmt &) override final;
    Value* visit(AstSelectStmt &) override final;
    Value* visit(AstIterationStmt &) override final;
    Value* visit(AstReturnStmt &) override final;
    Value* visit(AstLVal &) override final;
    Value* visit(AstPrimaryExp &) override final;
    Value* visit(AstInteger &) override final;
    Value* visit(AstFloat &) override final;
    Value* visit(AstUnaryExp &) override final;
    Value* visit(AstCallee &) override final;
    Value* visit(AstMulExp &) override final;
    Value* visit(AstAddExp &) override final;
    Value* visit(AstRelExp &) override final;
    Value* visit(AstEqExp &) override final;
    Value* visit(AstLAndExp &) override final;
    Value* visit(AstLOrExp &) override final;
    Value* visit(AstConstExp &) override final;

    std::unique_ptr<IRBuilder> builder;
    Scope scope;
    std::unique_ptr<Module> module;

    struct {
        // function that is being built
        Function *func = nullptr;
        // Module* module = nullptr;
        // TODO: you should add more fields to store state
        std::string curr_id;
        Type* decl_type;
        Type* curr_array_type;
        int max_column;
        std::vector<BasicBlock*> condBB;
        std::vector<BasicBlock*> successorBB;
        bool global;
        std::shared_ptr<AstExp> var_init;
        std::vector<std::shared_ptr<AstExp>> array_init;
        struct IfCondBB {
            BasicBlock* trueBB{};
            BasicBlock* falseBB{};
        };
        struct WhileCondBB {
            BasicBlock* loopBB{};
            BasicBlock* exitBB{};
        };
        std::vector<IfCondBB> CondBBStack;
        std::vector<WhileCondBB> WhileCondBBStack;
        std::vector<bool> load_lval{true};
        bool from_param_array{false};
    } context;

    Value *get_lval_location(const AstLVal &lval);

    Value *float_op(Value *left, Value *right, AddOp op);

    Value *int_op(Value *left, Value *right, AddOp op);

    Value *const_int_op(ConstantInt *left, ConstantInt *right, AddOp op);

    Value *int_op(Value *left, Value *right, MulOp op);

    Value *const_int_op(ConstantInt *left, ConstantInt *right, MulOp op);

    Value *const_float_op(ConstantFP *left, ConstantFP *right, AddOp op);

    Value *float_op(Value *left, Value *right, MulOp op);

    Value *const_float_op(ConstantFP *left, ConstantFP *right, MulOp op);

    template<typename T>
    Value *binary_op(Value *left, Value *right, T op);

    Value *int_op(Value *left, Value *right, RelOp op);

    Value *const_int_op(ConstantInt *left, ConstantInt *right, RelOp op);

    Value *const_float_op(ConstantFP *left, ConstantFP *right, RelOp op);

    Value *float_op(Value *left, Value *right, RelOp op);

    Value *float_op(Value *left, Value *right, EqOp op);

    Value *const_float_op(ConstantFP *left, ConstantFP *right, EqOp op);

    Value *const_int_op(ConstantInt *left, ConstantInt *right, EqOp op);

    Value *int_op(Value *left, Value *right, EqOp op);

    Value *to_int32_type(Value *value);

    Value *to_float_type(Value *value);

    Value *float_op(Value *left, Value *right, LogicOp op);

    Value *const_float_op(ConstantFP *left, ConstantFP *right, LogicOp op);

    Value *gen_cmp(Value *value);

    std::vector<int> to_indices(std::vector<Value *> &values);

    void initializeArray(int u, int& curr, std::vector<Value *> &array_exps, std::vector<Value *> &pos,
                         std::vector<int> array_exps_int);

    ConstantInt *to_const_index(Value *value);
};
