#include <queue>
#include "cminusf_builder.hpp"
#include "myassert.h"
#include "logging.hpp"

#define CONST_FP(num) ConstantFP::get((float)num, module.get())
#define CONST_INT(num) ConstantInt::get(num, module.get())
#define CONST_BOOL(num) ConstantInt::get(num, module.get())

// types
Type *VOID_T;
Type *INT1_T;
Type *INT32_T;
Type *INT32PTR_T;
Type *FLOAT_T;
Type *FLOATPTR_T;

int num_semantic_error;

std::ostream& semantic_stream = std::cerr;

std::ostream& semantic_error() {
  num_semantic_error++;
  semantic_stream << "semantic error: ";
  return semantic_stream;
}

// 先将所有函数都ASSERT表示还未实现

inline int to_int(Value* value) {
  if (auto* constInt = dynamic_cast<ConstantInt*>(value); constInt) {
    return constInt->get_value();
  } else {
    MY_ASSERT(false);
  }
}

ConstantInt * CminusfBuilder::to_const_index(Value * value) {
  if (auto* constInt = dynamic_cast<ConstantInt*>(to_int32_type(value)); constInt && constInt->get_value() >= 0) {
    return constInt;
  } else {
    semantic_error() << "array bounds or index must be non-negative constants when declare.";
  }
  return nullptr;
}

std::vector<int> CminusfBuilder::to_indices(std::vector<Value*>& values) {
  std::vector<int> result;
  result.reserve(values.size());
  // TODO 需要确定一下是顺序还是逆序
  for (auto &item: values) {
    // TODO 如果 pInt->get_value()=0 ?
    result.push_back(to_const_index(item)->get_value());
  }
  return result;
}

template<typename T>
T* to_const(Value* value) {
  if (auto constant = dynamic_cast<T*>(value); constant) {
    return constant;
  }
  return nullptr;
}

/*
 * use CMinusfBuilder::Scope to construct scopes
 * scope.enter: enter a new scope
 * scope.exit: exit current scope
 * scope.push: add a new binding to current scope
 * scope.find: find and return the value bound to the name
 */

Value* CminusfBuilder::visit(AstCompUnit &node) {
  VOID_T = module->get_void_type();
  INT1_T = module->get_int1_type();
  INT32_T = module->get_int32_type();
  INT32PTR_T = module->get_int32_ptr_type();
  FLOAT_T = module->get_float_type();
  FLOATPTR_T = module->get_float_ptr_type();

  for (std::shared_ptr<AstDeclDef> &decl: node.DeclDefList) {
    context.global = true;
    decl->accept(*this);
  }
  return nullptr;
}

// ConstDecl → 'const' BType ConstDef { ',' ConstDef } ';'
Value* CminusfBuilder::visit(AstConstDecl &node) {
  BType bType = node.type;
  Type* type;
  if (bType == TYPE_INT) {
    type = INT32_T;
  } else if (bType == TYPE_FLOAT) {
    type = FLOAT_T;
  } else {
    MY_ASSERT(false);
  }

  context.decl_type = type;

  for (const std::shared_ptr<AstConstDef>& varDef : node.ConstDefList) {
    varDef->accept(*this);
  }
  return nullptr;
}

// TODO 语义里const必须初始化: C中可以不初始化,按照C的
// const int x = 3;
// int* px = &x; Cannot initialize a variable of type 'int *' with an rvalue of type 'const int *'
// ConstDef → Ident { '[' ConstExp ']' } '=' ConstInitVal
Value* CminusfBuilder::visit(AstConstDef &node) {
  std::vector<int> array_exps_int = defInit(node.id, node.ArrayConstExpList);
  context.array_exps_int = array_exps_int;

  if (array_exps_int.empty()) {
    set_context_const_init(node);
    MY_ASSERT(context.const_init);
    Value *init_value = context.const_init->accept(*this);
    MY_ASSERT(init_value);

    if (context.global) {
      Constant* init = get_global_constant_init(init_value);
      MY_ASSERT(init);
      GlobalVariable *pGlobal = GlobalVariable::create(node.id, module.get(), context.decl_type, true, init);
      scope.push(node.id, init, Scope::VarType::ConstGlobalVar);
    } else { // context.global
      BasicBlock *entry_block = context.func->get_entry_block();
      Instruction *terminator = entry_block->is_terminated() ? entry_block->get_terminator() : nullptr;
      if(terminator != nullptr)
        entry_block->get_instructions().pop_back();

      BasicBlock *curr_block = builder->get_insert_block();
      builder->set_insert_point(entry_block);
      AllocaInst *pInst = builder->create_alloca(context.decl_type);

      if(terminator != nullptr)
        entry_block->add_instruction(terminator);

      builder->set_insert_point(curr_block);

      builder->create_store(init_value, pInst);
      scope.push(node.id, init_value, Scope::VarType::ConstLocalVar);
    }
  } else { // array_exps.empty()
    Type* array_type = set_context_array_type(array_exps_int);
    set_context_const_array_init_value(node, array_exps_int);

    if (context.global) {
      Constant* init = get_global_array_constant_init(array_type, array_exps_int, true);
      MY_ASSERT(init);
      GlobalVariable *pVariable = GlobalVariable::create(node.id, module.get(), array_type, true, init);
      scope.push(node.id, pVariable, Scope::VarType::ConstGlobalArray);
    } else { // context.global
      BasicBlock *entry_block = context.func->get_entry_block();
      Instruction *terminator = entry_block->is_terminated() ? entry_block->get_terminator() : nullptr;
      if(terminator != nullptr)
        entry_block->get_instructions().pop_back();

      BasicBlock *curr_block = builder->get_insert_block();
      builder->set_insert_point(entry_block);
      AllocaInst *pInst = builder->create_alloca(array_type);

      if(terminator != nullptr)
        entry_block->add_instruction(terminator);

      builder->set_insert_point(curr_block);

      scope.push(node.id, pInst, Scope::VarType::ConstLocalArray);
      MY_ASSERT(not context.const_array_init.empty());
      std::vector<Value*> pos{pInst};
      int curr = 0;
      initializeArray(0, curr, pos, array_exps_int, true);
    }
  }
  return nullptr;
}

Value* CminusfBuilder::visit(AstConstInitVal &node) {
  if (context.curr_array_type == nullptr) {
    MY_ASSERT(node.ConstInitValList.empty());
    context.const_init = node.ConstExp;
    return nullptr;
  }
  if (context.curr_array_type &&
      (context.curr_array_type->get_array_element_type() == INT32_T ||
       context.curr_array_type->get_array_element_type() == FLOAT_T)) {
    MY_ASSERT(node.ConstInitValList.size() <= context.array_exps_int.back());
    std::vector<std::shared_ptr<AstConstExp>> tmp_init;
    for (const auto& initVal : node.ConstInitValList) {
      MY_ASSERT(initVal->ConstExp != nullptr);
      tmp_init.push_back(initVal->ConstExp);
    }
    while (tmp_init.size() < context.array_exps_int.back()) {
      tmp_init.push_back(nullptr);
    }
    context.const_array_init.insert(context.const_array_init.end(), tmp_init.begin(), tmp_init.end());
    return nullptr;
  }
  Type *saved_array_type = context.curr_array_type;
  int column = 0;

  for (int i = 0; i < node.ConstInitValList.size(); i++) {
    std::shared_ptr<AstConstInitVal> initVal = node.ConstInitValList[i];
    if (initVal->ConstExp == nullptr) { // {...}
      MY_ASSERT(column % context.array_exps_int.back() == 0); // TODO report error
      column = 0; // ?
      context.curr_array_type = context.curr_array_type->get_array_element_type();
      initVal->accept(*this);
      context.curr_array_type = saved_array_type;
    } else {
      context.const_array_init.push_back(initVal->ConstExp);
      column++;
    }
  }
  return nullptr;
}

Value* CminusfBuilder::visit(AstVarDecl &node) {
  // TODO 判断是否已经定义过
  BType bType = node.type;
  Type* type;
  if (bType == TYPE_INT) {
    type = INT32_T;
  } else if (bType == TYPE_FLOAT) {
    type = FLOAT_T;
  } else {
    MY_ASSERT(false);
  }

  context.decl_type = type;
  // TODO int a=1,b=a; 能满足吗
  // 是在这里create_alloca还是在varDef
  for (std::shared_ptr<AstVarDef>& varDef : node.VarDefList) {
    varDef->accept(*this);
  }
  return nullptr;
}

Value * CminusfBuilder::convertType(Value *value, Type* type) {
  if (type->is_float_type()) {
    return to_float_type(value);
  } else if (type->is_integer_type()) {
    return to_int32_type(value);
  } else {
    MY_ASSERT(false);
  }
}

void CminusfBuilder::initializeArray(int u, int& curr,
                                     std::vector<Value*>& pos, std::vector<int> array_exps_int, bool const_array) {
    if (u == array_exps_int.size()) {
      // TODO context.array_init[curr++] = nullptr
      Value* pValue;
      if (const_array) {
        if (context.const_array_init[curr] == nullptr) {
          pValue = context.decl_type == INT32_T ? static_cast<Value*>(CONST_INT(0))
                                                : static_cast<Value*>(CONST_FP(0));
          curr++;
        } else {
          pValue = context.const_array_init[curr++]->accept(*this);
        }
        // TODO 判断是否是常量
        if (auto constant = dynamic_cast<Constant*>(pValue);
            not (constant && (constant->get_type()->is_float_type() || constant->get_type()->is_integer_type()))){
              semantic_error() << "global init must be constant.\n";
        }
      } else {
        if (context.array_init[curr] == nullptr) {
          pValue = context.decl_type == INT32_T ? static_cast<Value*>(CONST_INT(0))
                                                : static_cast<Value*>(CONST_FP(0));
          curr++;
        } else {
          pValue = context.array_init[curr++]->accept(*this);
        }
      }
      // TODO 判断类型是否匹配,clang是运行数组初始化时进行float与int间的隐式转换
      MY_ASSERT(context.decl_type == pos.back()->get_type()->get_pointer_element_type());
      // TODO 很多地方有这种代码,考虑用函数
      pValue = convertType(pValue, context.decl_type);
      builder->create_store(pValue, pos.back());
      return;
    }
    pos.push_back(builder->create_gep(pos.back(), {CONST_INT(0), CONST_INT(0)}));
    for (int i = 0; i < array_exps_int[u]; ++i) {
      if (i != 0) {
        pos.back() = builder->create_gep(pos.back(), {CONST_INT(1)});
      }
      initializeArray(u + 1, curr, pos, array_exps_int, const_array);
    }
    pos.pop_back();
}

std::vector<int> CminusfBuilder::defInit(const std::string& id, std::vector<std::shared_ptr<AstConstExp>>& ArrayConstExpList) {
  LOG_DEBUG << id;
  context.var_init = nullptr;
  context.const_init = nullptr;
  context.array_init.clear();
  context.const_array_init.clear();
  context.curr_array_type = nullptr;
  context.curr_id = id;
  std::vector<Value *> array_exps;
  array_exps.reserve(ArrayConstExpList.size());

  for (std::shared_ptr<AstConstExp>& array_exp : ArrayConstExpList) {
    Value *exp = array_exp->accept(*this);
    MY_ASSERT(exp);
    // TODO exp必须为非负整数,在下面处理了
    array_exps.push_back(exp);
  }
  return to_indices(array_exps);
}

void CminusfBuilder::set_context_var_init(AstVarDef &node) {
  if (node.InitVal) {
    node.InitVal->accept(*this);
    MY_ASSERT(context.var_init);
  }
}

// Cpp中const必须要显式初始化, C中不需要
void CminusfBuilder::set_context_const_init(AstConstDef &node) {
  if (node.ConstInitVal) {
    node.ConstInitVal->accept(*this);
    MY_ASSERT(context.const_init);
  } else {
    semantic_error() << "const object should be initialized\n";
  }
}

Constant* CminusfBuilder::get_global_constant_init(Value *init_value) {
  Constant* init = nullptr;
  if (init_value) {
    if (auto* constant = dynamic_cast<Constant*>(init_value);
        constant && (constant->get_type()->is_float_type() || constant->get_type()->is_integer_type())) {
      init = constant;
    } else {
      // TODO error
      semantic_error() << "global init must be constant or wrong initialization expression.\n";
    }
  } else { // node.InitVal
    init = ConstantZero::get(context.decl_type, module.get());
  }
  return init;
}

Type* CminusfBuilder::set_context_array_type(std::vector<int> array_exps_int) {
  Type* array_type = context.decl_type;
  for (auto it = array_exps_int.rbegin(); it != array_exps_int.rend(); ++it) {
    array_type = ArrayType::get(array_type, *it);
  }
  context.curr_array_type = array_type;
  return array_type;
}

void CminusfBuilder::set_context_array_init_value(AstVarDef &node, std::vector<int> array_exps_int) {
  if (node.InitVal) {
    node.InitVal->accept(*this);
    int count = 1;
    for (int bound: array_exps_int) {
      count *= bound;
    }
    // 补齐缺省的元素，已添加的肯定是合法的初始化方式
    while (context.array_init.size() < count) {
      context.array_init.push_back(nullptr);
    }
    MY_ASSERT(count == context.array_init.size());
  }
}

// 全局未初始化时默认初始化为0, local未初始化随机值
void CminusfBuilder::set_context_const_array_init_value(AstConstDef &node, std::vector<int> array_exps_int) {
  if (node.ConstInitVal) {
    node.ConstInitVal->accept(*this);
    int count = 1;
    for (int bound: array_exps_int) {
      count *= bound;
    }
    // 补齐缺省的元素，已添加的肯定是合法的初始化方式
    while (context.const_array_init.size() < count) {
      context.const_array_init.push_back(nullptr);
    }
    MY_ASSERT(count == context.const_array_init.size());
  } else {
    semantic_error() << "const object should be initialized\n";
  }
}

// 语义里要求const需要显式初始化,但C中可以不需要
Constant* CminusfBuilder::get_global_array_constant_init(Type* array_type, std::vector<int> array_exps_int, bool const_array) {
  // cpp全局变量可以用已定义的全局变量初始化, C不行
  // int y[3] = {x}; error: initializer element is not a compile-time constant
  Constant* init = nullptr;

  std::queue<Constant*> queue;
  int total_elem;
  // TODO const全局数组和全局数组, C语言全局初始化必须是常量
  if (not const_array and not context.array_init.empty()) {
    total_elem = context.array_init.size();
    for (const auto &item: context.array_init) {
      Value *pValue;
      if (item == nullptr) {
        pValue = context.decl_type == INT32_T ? static_cast<Value*>(CONST_INT(0))
                                              : static_cast<Value*>(CONST_FP(0));
      } else {
        pValue = item->accept(*this);
      }
      if (auto* constant = dynamic_cast<Constant*>(pValue); constant) {
        queue.push(constant);
      } else {
        semantic_error() << "global init must be constant.\n";
      }
    }
  } else if (const_array and not context.const_array_init.empty()) {
    total_elem = context.const_array_init.size();
    for (const auto &item: context.const_array_init) {
      Value *pValue;
      if (item == nullptr) {
        pValue = context.decl_type == INT32_T ? static_cast<Value*>(CONST_INT(0))
                                              : static_cast<Value*>(CONST_FP(0));
      } else {
        pValue = item->accept(*this);
      }
      if (auto* constant = dynamic_cast<Constant*>(pValue); constant) {
        queue.push(constant);
      } else {
        semantic_error() << "global init must be constant.\n";
      }
    }
  }
  if (not queue.empty()) {
    Type* curr_type = context.decl_type == INT32_T ? INT32_T : FLOAT_T;
    int denominator = 1;
    for (auto it = array_exps_int.rbegin(); it != array_exps_int.rend(); ++it) {
      int bound = *it;
      denominator *= bound;
      curr_type = ArrayType::get(curr_type, bound);
      MY_ASSERT(total_elem % denominator == 0);
      int step = total_elem / denominator;
      for (int i = 0; i < step; ++i) {
        std::vector<Constant*> constants;
        constants.reserve(bound);
        for (int j = 0; j < bound; ++j) {
          constants.push_back(queue.front());
          queue.pop();
        }
        Constant *pArray = ConstantArray::get(static_cast<ArrayType *>(curr_type), constants);
        queue.push(pArray);
      }
    }
    MY_ASSERT(queue.size() == 1);
    init = queue.front();
  } else { // node.InitVal
    init = ConstantZero::get(array_type, module.get());
  }
  return init;
}

// 1. VarDef 用于定义变量。当不含有‘=’和初始值时，其运行时实际初值未定义。
// 2. VarDef 的数组维度和各维长度的定义部分不存在时，表示定义单个变量。存
// 在时，和 ConstDef 类似，表示定义多维数组。（参见 ConstDef 的第 2 点）
// 3. 当 VarDef 含有 ‘=’ 和初始值时， ‘=’ 右边的 InitVal 和 CostInitVal 的结构要
// 求相同，唯一的不同是 ConstInitVal 中的表达式是 ConstExp 常量表达式，而
// InitVal 中的表达式可以是当前上下文合法的任何 Exp。
// 4. VarDef 中表示各维长度的 ConstExp 必须是能求值到 非负整数，但 InitVal 中
// 的初始值为 Exp，其中可以引用变量，例如下图中的变量 e 的初始化表达式 d[2][1]。
Value* CminusfBuilder::visit(AstVarDef &node) {
  std::vector<int> array_exps_int = defInit(node.id, node.ArrayConstExpList);
  context.array_exps_int = array_exps_int;

  if (array_exps_int.empty()) {
    set_context_var_init(node);
    Value *init_value = nullptr;
    if (context.var_init) {
      init_value = context.var_init->accept(*this);
    }
    // 和C不同,Cpp全局变量初始化不一定为常量,可以是在之前定义的全局变量
    if (context.global) {
      Constant* init = get_global_constant_init(init_value); // 可以处理init_value = nullptr的情况
      MY_ASSERT(init);
      // global int x = x; error
      // local int x = x; ok
      GlobalVariable *pGlobal = GlobalVariable::create(node.id, module.get(), context.decl_type, false, init);
      scope.push(node.id, pGlobal, Scope::VarType::GlobalVar);
    } else { // context.global
      BasicBlock *entry_block = context.func->get_entry_block();
      Instruction *terminator = entry_block->is_terminated() ? entry_block->get_terminator() : nullptr;
      if(terminator != nullptr)
        entry_block->get_instructions().pop_back();

      BasicBlock *curr_block = builder->get_insert_block();
      builder->set_insert_point(entry_block);
      AllocaInst *pInst = builder->create_alloca(context.decl_type);

      if(terminator != nullptr)
        entry_block->add_instruction(terminator);

      builder->set_insert_point(curr_block);

      scope.push(node.id, pInst, Scope::VarType::LocalVar);
      // 应该放在scope.push后面, 比如 int a = a;
      // node.InitVal语义检查
      if (init_value) {
        init_value = convertType(init_value, context.decl_type);
        builder->create_store(init_value, pInst);
      }
    }
  } else { // array_exps.empty()
    Type* array_type = set_context_array_type(array_exps_int);
    set_context_array_init_value(node, array_exps_int);

    if (context.global) {
      Constant* init = get_global_array_constant_init(array_type, array_exps_int, false);
      MY_ASSERT(init);
      GlobalVariable *pVariable = GlobalVariable::create(node.id, module.get(), array_type, false, init);
      scope.push(node.id, pVariable, Scope::VarType::GlobalArray);
    } else { // context.global
      // BasicBlock *entry_block = context.func->get_entry_block();
      // Instruction *terminator = entry_block->is_terminated() ? entry_block->get_terminator() : nullptr;
      // if(terminator != nullptr)
      //   entry_block->get_instructions().pop_back();
      //
      // BasicBlock *curr_block = builder->get_insert_block();
      // builder->set_insert_point(entry_block);
      AllocaInst *pInst = builder->create_alloca(array_type);
      //
      // if(terminator != nullptr)
      //   entry_block->add_instruction(terminator);
      //
      // builder->set_insert_point(curr_block);

      scope.push(node.id, pInst, Scope::VarType::LocalArray); // 应该放在node.InitVal前面, 比如 int a = a;
      // TODO array_exps有用到吗, 可以用来检验数组访问是否合法, 没用, 直接用array_exps_int就行了
      if (!context.array_init.empty()) {
        std::vector<Value*> pos{pInst};
        int curr = 0;
        initializeArray(0, curr, pos, array_exps_int, false);
      }
      // TODO 怎么给所有元素初始化为 0?
    }
  }
  return nullptr;
}

// InitVal:
//     Exp {
//         $$ = new AstInitVal();
//         $$->Exp = std::shared_ptr<AstExp>($1);
//     }
//     | LBRACE RBRACE {
//         $$ = new AstInitVal();
//         $$->Exp = nullptr;
//     }
//     | LBRACE InitValList RBRACE {
//         $$ = new AstInitVal();
//         $$->Exp = nullptr;
//         $$->InitValList.swap($2->list);
//     };
// C中:
// int b[2][2]={1, {1, 1}}; wrong
// int b[2][2]={1, {1}}; yes
Value* CminusfBuilder::visit(AstInitVal &node) {
  if (context.curr_array_type == nullptr) {
    MY_ASSERT(node.InitValList.empty());
    context.var_init = node.Exp;
    return nullptr;
  }
  if (context.curr_array_type &&
          (context.curr_array_type->get_array_element_type() == INT32_T ||
           context.curr_array_type->get_array_element_type() == FLOAT_T)) {
    MY_ASSERT(node.InitValList.size() <= context.array_exps_int.back());
    std::vector<std::shared_ptr<AstExp>> tmp_init;
    for (const auto& initVal : node.InitValList) {
      MY_ASSERT(initVal->Exp != nullptr);
      tmp_init.push_back(initVal->Exp);
    }
    while (tmp_init.size() < context.array_exps_int.back()) {
      tmp_init.push_back(nullptr);
    }
    context.array_init.insert(context.array_init.end(), tmp_init.begin(), tmp_init.end());
    return nullptr;
  }
  Type *saved_array_type = context.curr_array_type;
  int column = 0;

  for (int i = 0; i < node.InitValList.size(); i++) {
    std::shared_ptr<AstInitVal> initVal = node.InitValList[i];
    if (initVal->Exp == nullptr) { // {...}
      MY_ASSERT(column % context.array_exps_int.back() == 0); // TODO report error
      column = 0; // ?
      context.curr_array_type = context.curr_array_type->get_array_element_type();
      initVal->accept(*this);
      context.curr_array_type = saved_array_type;
    } else {
      context.array_init.push_back(initVal->Exp);
      column++;
    }
  }
  return nullptr;
}

Value* CminusfBuilder::visit(AstFuncDef &node) {
  LOG_DEBUG << node.id;
  context.global = false;

  FunctionType *fun_type;
  Type *ret_type;
  std::vector<Type *> param_types;
  if (node.type == TYPE_INT) {
    ret_type = INT32_T;
  } else if (node.type == TYPE_FLOAT) {
    ret_type = FLOAT_T;
  } else if (node.type == TYPE_VOID) {
    ret_type = VOID_T;
  } else {
    assert("node.type");
  }
  // TODO void时FuncFParamList有吗? 这里要改,目前还没有void,需要添加
  for (std::shared_ptr<AstFuncFParam> &param: node.FuncFParamList) {
    Type *param_type;
    if (param->type == TYPE_INT) {
      param_type = INT32_T;
    } else if (param->type == TYPE_FLOAT) {
      param_type = FLOAT_T;
    } else if (param->type == TYPE_VOID) {
      // TODO
      assert("param->type == TYPE_VOID");
    }
    if (param->isarray) {
      for (auto it = param->ParamArrayExpList.rbegin(); it != param->ParamArrayExpList.rend(); ++it) {
        Value *exp = (*it)->accept(*this);
        ConstantInt *bound = to_const_index(exp);
        param_type = ArrayType::get(param_type, bound->get_value());
      }
      param_type = PointerType::get(param_type);
    }
    // 但其实clang是这样的:int a[][2][3] -> [2 x [3 x i32]]* %0, 会具体求出类型,而不是直接用ptr
    // push_back(std::move(type)): Std::move of the variable 'type' of the trivially-copyable type 'Type *' has no effect
    // std::move 主要用于将拥有资源的对象的所有权转移给另一个对象，对于指针这种轻量级的类型，使用 std::move 往往是不必要的
    param_types.push_back(param_type);
  }

  fun_type = FunctionType::get(ret_type, param_types);
  auto func = Function::create(fun_type, node.id, module.get());
  scope.push(node.id, func, Scope::VarType::Function);
  context.func = func;
  auto funBB = BasicBlock::create(module.get(), "entry", func);
  builder->set_insert_point(funBB);
  if (node.type == TYPE_INT) {
    context.return_value = builder->create_alloca(INT32_T);
  } else if (node.type == TYPE_FLOAT) {
    context.return_value = builder->create_alloca(FLOAT_T);
  }

  auto retBB = BasicBlock::create(module.get(), "ret", func);
  context.return_bb = retBB;

  scope.enter();

  std::vector<Value *> args;
  for (auto &arg: func->get_args()) {
    args.push_back(arg);
  }
  for (int i = 0; i < node.FuncFParamList.size(); ++i) {
    // 要区分数组!
    AllocaInst *pAlloca = builder->create_alloca(param_types[i]);
    builder->create_store(args[i], pAlloca);
    scope.push(node.FuncFParamList[i]->id, pAlloca, param_types[i]->is_pointer_type()
                                                    ? Scope::VarType::ParamArray : Scope::VarType::ParamVar);

    // // accept返回void,但是需要子节点的返回值Value*,怎么处理比较好?
  }

  node.Block->accept(*this);
  if(not builder->get_insert_block()->is_terminated()) {
    if(context.func->get_return_type() == FLOAT_T) {
      builder->create_store(CONST_FP(0), context.return_value);
    } else if(context.func->get_return_type() == INT32_T) {
      builder->create_store(CONST_INT(0), context.return_value);
    }
    builder->create_br(context.return_bb);
  }
  // ?
  // retBB move to instruction list end?
  builder->set_insert_point(retBB);

  if (context.func->get_return_type()->is_void_type())
    builder->create_void_ret();
  else
    builder->create_ret(builder->create_load(context.return_value));

  // TODO 把return bb放在最后?
  scope.exit();
  return nullptr;
}

Value* CminusfBuilder::visit(AstFuncFParam &node) {
  MY_ASSERT(false);

}

Value* CminusfBuilder::visit(AstBlock &node) {
  // You may need to add some code here
  // to deal with complex statements.
  scope.enter();

  for (std::shared_ptr<AstBlockItem> &item: node.BlockItemList) {
    item->accept(*this);
    if(builder->get_insert_block()->is_terminated()) {
      break;
    }
  }

  scope.exit();
  return nullptr;
}

// TODO 测试多层 while
Value* CminusfBuilder::visit(AstBreakStmt &node) {
  // TODO 应该在循环中才能使用 break
  MY_ASSERT(!context.while_condBB_stack.empty());
  builder->create_br(context.while_condBB_stack.back().exitBB);
  return nullptr;
}

Value* CminusfBuilder::visit(AstContinueStmt &node) {
  MY_ASSERT(!context.while_condBB_stack.empty());
  builder->create_br(context.while_condBB_stack.back().condBB);
  return nullptr;
}

// TODO const不能assign
Value* CminusfBuilder::visit(AstAssignStmt &node) {
  MY_ASSERT(node.Exp && node.LVal);
  Value *value = node.Exp->accept(*this);

  context.load_lval.push_back(false);
  Value *loc = node.LVal->accept(*this);
  context.load_lval.pop_back();
  // TODO 类型转换
  Type *loc_type = loc->get_type()->get_pointer_element_type();
  MY_ASSERT(loc->get_type()->is_pointer_type()
            && (loc_type->is_float_type() || loc_type->is_integer_type()));
  value = convertType(value, loc_type);
  return builder->create_store(value, loc);
}

// TODO 可以返回右值引用吗?
std::string next_bb_num_string() {
  static int bb_num = 0;
  return "_" + std::to_string(bb_num++);
}

Value* CminusfBuilder::visit(AstSelectStmt &node) {
  // module.get是不是不太好啊, 按理说create应该声明const ?
  std::string no = next_bb_num_string();
  auto trueBB = BasicBlock::create(module.get(), "trueBB" + no, context.func);
  auto falseBB = BasicBlock::create(module.get(), "falseBB" + no, context.func);
  auto exitBB = BasicBlock::create(module.get(), "exitBB" + no, context.func);
  context.logicBB_stack.push_back({trueBB, node.elseStmt == nullptr ? exitBB : falseBB});

  Value *last_value = node.Cond->accept(*this);
  context.logicBB_stack.pop_back();
  Value *cond = gen_cmp(last_value);
  builder->create_cond_br(cond, trueBB, node.elseStmt == nullptr ? exitBB : falseBB);

  builder->set_insert_point(trueBB);
  node.ifStmt->accept(*this);

  if(not builder->get_insert_block()->is_terminated()) {
    builder->create_br(exitBB);
  }
  if(node.elseStmt == nullptr) {
    falseBB->erase_from_parent();
  } else {
    builder->set_insert_point(falseBB);
    node.elseStmt->accept(*this);
    if(not builder->get_insert_block()->is_terminated()) {
      builder->create_br(exitBB); // basic block必须有结束语句
    }
  }
  builder->set_insert_point(exitBB);

  // bug find by 10_if_else.sy: basic block with no pred should be deleted
  if(exitBB->get_pre_basic_blocks().empty()) { // builder->create_br(exitBB);中会维护pre_basic_blocks
    // 这种情况一定是在if和else中*都*return了,但还是需要设置一个插入点,block中会判断结束,就不再继续了
    // if(builder->get_insert_block()->is_terminated()) { break; }
    builder->set_insert_point(trueBB);
    // builder->set_insert_point(nullptr);
    exitBB->erase_from_parent();
  }
  return nullptr;
}

Value* CminusfBuilder::visit(AstIterationStmt &node) {
  std::string no = next_bb_num_string();
  auto condBB = BasicBlock::create(module.get(), "condBB" + no, context.func);
  auto loopBB = BasicBlock::create(module.get(), "loopBB" + no, context.func);
  auto exitBB = BasicBlock::create(module.get(), "successorBB" + no, context.func);

  context.while_condBB_stack.push_back({condBB, exitBB});
  context.logicBB_stack.push_back({loopBB, exitBB});

  if(not builder->get_insert_block()->is_terminated()) {
    builder->create_br(condBB);
  }
  builder->set_insert_point(condBB); // ! 每个set_insert_point都要检查builder->get_insert_block()->get_terminator()
  Value *cond = node.Cond->accept(*this);
  Value *pValue = gen_cmp(cond);
  builder->create_cond_br(pValue, loopBB, exitBB);

  context.logicBB_stack.pop_back();

  builder->set_insert_point(loopBB);
  node.Stmt->accept(*this);
  if(not builder->get_insert_block()->is_terminated()) {
    builder->create_br(condBB);
  }

  builder->set_insert_point(exitBB);

  context.while_condBB_stack.pop_back();
  return nullptr;
}

Value* CminusfBuilder::visit(AstReturnStmt &node) {
  Type *return_type = context.func->get_return_type();

  if (node.Exp == nullptr) {
    if (return_type != VOID_T) {
      semantic_error() << "function '" << context.func->get_name() << "' return type mismatch.\n";
    }
    // builder->create_void_ret();
  } else {
    if (return_type == VOID_T) {
      semantic_error() << "function '" << context.func->get_name() << "' return type mismatch.\n";
    }
    // You need to solve other return cases (e.g. return an integer).
    Value *value = node.Exp->accept(*this);
    MY_ASSERT(return_type->is_float_type() || return_type->is_integer_type());
    value = convertType(value, return_type);
    // builder->create_ret(value);
    builder->create_store(value, context.return_value);
  }
  builder->create_br(context.return_bb);
  return nullptr;
}

Value* CminusfBuilder::visit(AstCallee &node) {
  std::vector<Value*> actual_params;
  actual_params.reserve(node.ExpList.size());
  Scope::ValueWithType valueWithType = scope.find(node.id);
  auto* func = dynamic_cast<Function *>(valueWithType.val);
  MY_ASSERT(func);
  std::list<Argument*> &list = func->get_args();
  MY_ASSERT(list.size() == node.ExpList.size());
  std::list<Argument*>::iterator iterator = list.begin();
  for (int i = 0; i < node.ExpList.size(); i++) {
    std::shared_ptr<AstExp> actual_param = node.ExpList[i];
    Argument *pArgument = *iterator;
    if (pArgument->get_type()->is_pointer_type()) { // 参数是数组形式
      context.load_lval.push_back(false);
    }
    // TODO 传数组应该传数组名(指针), 类型转换int,float
    Value *pValue = actual_param->accept(*this);
    if (pArgument->get_type()->is_pointer_type()) { // 参数是数组形式
      if (not context.from_param_array) {
        pValue = builder->create_gep(pValue, {CONST_INT(0), CONST_INT(0)});
      }
    } else {
      MY_ASSERT(pArgument->get_type()->is_float_type() || pArgument->get_type()->is_integer_type());
      pValue = convertType(pValue, pArgument->get_type());
    }
    if (pArgument->get_type()->is_pointer_type()) { // 参数是数组形式
      context.load_lval.pop_back();
    }
    actual_params.push_back(pValue);
    iterator++;
  }
  CallInst *ret = builder->create_call(func, actual_params);
  return ret;
}

// LVal既可以在右边又可以在左边
// 左边不能load, 右边需要看情况决定是否load, 作为数组传参就不能load
Value* CminusfBuilder::visit(AstLVal &node) {
  Scope::ValueWithType valueWithType = scope.find(node.id);
  Value *pValue = valueWithType.val;
  MY_ASSERT(pValue);
  Scope::VarType type = valueWithType.type;
  LOG_DEBUG << "type: " << pValue->get_type()->print() << ", name: " << node.id;
  if (type == Scope::VarType::ConstGlobalVar || type == Scope::VarType::ConstLocalVar) {
    if (auto constInt = dynamic_cast<ConstantInt*>(pValue); constInt) {
      return CONST_INT(constInt->get_value());
    } else if (auto constFp = dynamic_cast<ConstantFP*>(pValue); constFp) {
      return CONST_FP(constFp->get_value());
    } else {
      MY_ASSERT(false);
    }
    return pValue;
  }
  // 普通变量是地址
  // 需要考虑全局变量, 全局变量直接是地址
  // 形参数组一般是把alloc的用来存数组地址的地址存在scope中,后面每次访问也会先load出来数组的地址
  // 直接定义的数组是直接alloc出存数组的地址,和全局变量一样
  // 用一个flag来表示scope里面存的到底是地址的地址还是地址
  Value *loc = pValue;
  if (type == Scope::VarType::ParamArray) {
    loc = builder->create_load(pValue);
  }
  if (node.ArrayExpList.empty()) { // 这个也可能是传的数组名
  } else {
    // param array 和 local定义的array访问区别:
    // define dso_local i32 @test4([6 x i32]* %0) #0 {
    //   %2 = alloca i32, align 4
    //   %3 = alloca [6 x i32]*, align 8
    //   %4 = alloca [3 x [3 x i32]], align 16
    //   store [6 x i32]* %0, [6 x i32]** %3, align 8
    //   %5 = bitcast [3 x [3 x i32]]* %4 to i8*
    //   call void @llvm.memset.p0i8.i64(i8* align 16 %5, i8 0, i64 36, i1 false)
    //   %6 = bitcast i8* %5 to [3 x [3 x i32]]*
    //   %7 = getelementptr inbounds [3 x [3 x i32]], [3 x [3 x i32]]* %6, i32 0, i32 0
    //   %8 = getelementptr inbounds [3 x i32], [3 x i32]* %7, i32 0, i32 0
    //   store i32 3, i32* %8, align 16
    //   %9 = getelementptr inbounds [3 x [3 x i32]], [3 x [3 x i32]]* %4, i64 0, i64 0
    //   %10 = getelementptr inbounds [3 x i32], [3 x i32]* %9, i64 0, i64 1
    //   store i32 2, i32* %10, align 4
    //   %11 = load [6 x i32]*, [6 x i32]** %3, align 8
    //   %12 = getelementptr inbounds [6 x i32], [6 x i32]* %11, i64 3
    //   %13 = getelementptr inbounds [6 x i32], [6 x i32]* %12, i64 0, i64 4
    //   store i32 5, i32* %13, align 4
    //   %14 = load i32, i32* %2, align 4
    //   ret i32 %14
    // }
    for (int i = 0; i < node.ArrayExpList.size(); i++) {
      std::shared_ptr<AstExp> exp = node.ArrayExpList[i];
      context.load_lval.push_back(true);
      Value *offset = exp->accept(*this);
      context.load_lval.pop_back();
      MY_ASSERT(offset->get_type()->is_float_type() || offset->get_type()->is_integer_type());
      // make sure offset is int type
      offset = to_int32_type(offset);

      if (auto constInt = to_const<ConstantInt>(offset); constInt) {
        if (constInt->get_value() < 0) {
          semantic_error() << "negative index exception.\n";
        }
      }
      // fix bug: {offset} -> {CONST_INT(0), offset}
      if (type == Scope::VarType::ParamArray && i == 0) {
        loc = builder->create_gep(loc, {offset});
      } else {
        loc = builder->create_gep(loc, {CONST_INT(0), offset});
      }
      // LOG_DEBUG << pValue->get_type()->print();
    }
  }
  context.from_param_array = (type == Scope::VarType::ParamArray);
  // 什么时候create_load呢? Assign左边的lval不应该,但右边的exp就应该, 函数传参时普通变量应该load,数组传参不应该load
  // const变量应该直接返回第一次创建的变量,不要load,
  MY_ASSERT(loc->get_type()->is_pointer_type());
  return context.load_lval.back() ? builder->create_load(loc) : loc;
}

Value* CminusfBuilder::visit(AstPrimaryExp &node) {
  return node.accept(*this);
}

Value* CminusfBuilder::visit(AstInteger &node) {
  // TODO float
  return CONST_INT(node._int);
}

Value* CminusfBuilder::visit(AstFloat &node) {
  return CONST_FP(node._float);
}

Value* CminusfBuilder::visit(AstUnaryExp &node) {
  if (node.PrimaryExp) {
    return node.PrimaryExp->accept(*this);
  } else if (node.Callee) {
    return node.Callee->accept(*this);
  } else if (node.UnaryExp) {
    Value *pValue = node.UnaryExp->accept(*this);
    MY_ASSERT(pValue->get_type()->is_float_type() || pValue->get_type()->is_integer_type());
    if (auto constInt = to_const<ConstantInt>(pValue); constInt) {
      if (node.op == UnaryOp::OP_NEG) {
        return CONST_INT(-constInt->get_value());
      } else if (node.op == UnaryOp::OP_POS) {
        return constInt;
      } else {
        // TODO '!'仅出现在条件表达式中
        return CONST_BOOL(constInt == CONST_INT(0));
      }
    } else if (auto constFp = to_const<ConstantFP>(pValue); constFp) {
      if (node.op == UnaryOp::OP_NEG) {
        return CONST_FP(-constFp->get_value());
      } else if (node.op == UnaryOp::OP_POS) {
        return constFp;
      } else {
        return CONST_BOOL(constFp == CONST_FP(0));
      }
    } else if (pValue->get_type()->is_integer_type()) {
      if (node.op == UnaryOp::OP_NEG) {
        return builder->create_isub(CONST_INT(0), pValue);
      } else if (node.op == UnaryOp::OP_POS) {
        return pValue;
      } else {
        return builder->create_icmp_eq(pValue, CONST_INT(0));
      }
    } else if (pValue->get_type()->is_float_type()) {
      if (node.op == UnaryOp::OP_NEG) {
        return builder->create_fsub(CONST_FP(0), pValue);
      } else if (node.op == UnaryOp::OP_POS) {
        return pValue;
      } else {
        return builder->create_fcmp_eq(pValue, CONST_FP(0));
      }
    }
  }
  MY_ASSERT(false);
}

Value* CminusfBuilder::visit(AstMulExp &node) {
  if (node.MulExp) {
    Value *left = node.MulExp->accept(*this);
    Value *right = node.UnaryExp->accept(*this);
    return binary_op(left, right, node.op);
  } else {
    return node.UnaryExp->accept(*this);
  }
}

Value* CminusfBuilder::int_op(Value* left, Value* right, AddOp op) {
  if (op == AddOp::OP_PLUS) {
    return builder->create_iadd(left, right);
  } else if (op == AddOp::OP_MINUS) {
    return builder->create_isub(left, right);
  } else {
    MY_ASSERT(false);
  }
}

Value* CminusfBuilder::const_int_op(ConstantInt* left, ConstantInt* right, AddOp op) {
  if (op == AddOp::OP_PLUS) {
    return CONST_INT(left->get_value() + right->get_value());
  } else if (op == AddOp::OP_MINUS) {
    return CONST_INT(left->get_value() - right->get_value());
  } else {
    MY_ASSERT(false);
  }
}

Value* CminusfBuilder::int_op(Value* left, Value* right, MulOp op) {
  if (op == MulOp::OP_MUL) {
    return builder->create_imul(left, right);
  } else if (op == MulOp::OP_DIV) {
    return builder->create_isdiv(left, right);
  } else if (op == MulOp::OP_MOD) {
    return builder->create_srem(left, right);
  } else {
    MY_ASSERT(false);
  }
}

Value* CminusfBuilder::const_int_op(ConstantInt* left, ConstantInt* right, MulOp op) {
  if (op == MulOp::OP_MUL) {
    return CONST_INT(left->get_value() * right->get_value());
  } else if (op == MulOp::OP_DIV) {
    return CONST_INT(left->get_value() / right->get_value());
  } else if (op == MulOp::OP_MOD) {
    return CONST_INT(left->get_value() % right->get_value());
  } else {
    MY_ASSERT(false);
  }
}

Value* CminusfBuilder::int_op(Value* left, Value* right, RelOp op) {
  if (op == RelOp::OP_LT) {
    return builder->create_icmp_lt(left, right);
  } else if (op == RelOp::OP_LTE) {
    return builder->create_icmp_le(left, right);
  } else if (op == RelOp::OP_GT) {
    return builder->create_icmp_gt(left, right);
  } else if (op == RelOp::OP_GTE) {
    return builder->create_icmp_ge(left, right);
  } else {
    MY_ASSERT(false);
  }
}

Value* CminusfBuilder::const_int_op(ConstantInt* left, ConstantInt* right, RelOp op) {
  if (op == RelOp::OP_LT) {
    return left->get_value() < right->get_value() ? CONST_BOOL(true) : CONST_BOOL(false);
  } else if (op == RelOp::OP_LTE) {
    return left->get_value() <= right->get_value() ? CONST_BOOL(true) : CONST_BOOL(false);
  } else if (op == RelOp::OP_GT) {
    return left->get_value() > right->get_value() ? CONST_BOOL(true) : CONST_BOOL(false);
  } else if (op == RelOp::OP_GTE) {
    return left->get_value() >= right->get_value() ? CONST_BOOL(true) : CONST_BOOL(false);
  } else {
    MY_ASSERT(false);
  }
}

Value* CminusfBuilder::int_op(Value* left, Value* right, EqOp op) {
  if (op == EqOp::OP_EQ) {
    return builder->create_icmp_eq(left, right);
  } else if (op == EqOp::OP_NEQ) {
    return builder->create_icmp_ne(left, right);
  } else {
    MY_ASSERT(false);
  }
}

Value* CminusfBuilder::const_int_op(ConstantInt* left, ConstantInt* right, EqOp op) {
  if (op == EqOp::OP_EQ) {
    return left->get_value() == right->get_value() ? CONST_BOOL(true) : CONST_BOOL(false);
  } else if (op == EqOp::OP_NEQ) {
    return left->get_value() != right->get_value() ? CONST_BOOL(true) : CONST_BOOL(false);
  } else {
    MY_ASSERT(false);
  }
}

Value* CminusfBuilder::float_op(Value* left, Value* right, AddOp op) {
  if (op == AddOp::OP_PLUS) {
    return builder->create_fadd(left, right);
  } else if (op == AddOp::OP_MINUS) {
    return builder->create_fsub(left, right);
  } else {
    MY_ASSERT(false);
  }
}

Value* CminusfBuilder::const_float_op(ConstantFP* left, ConstantFP* right, AddOp op) {
  if (op == AddOp::OP_PLUS) {
    return CONST_FP(left->get_value() + right->get_value());
  } else if (op == AddOp::OP_MINUS) {
    return CONST_FP(left->get_value() - right->get_value());
  } else {
    MY_ASSERT(false);
  }
}

Value* CminusfBuilder::float_op(Value* left, Value* right, MulOp op) {
  if (op == MulOp::OP_MUL) {
    return builder->create_fmul(left, right);
  } else if (op == MulOp::OP_DIV) {
    return builder->create_fdiv(left, right);
  } else if (op == MulOp::OP_MOD) {
    semantic_error() << "operands of binary operator % cannot be a float number";
    return nullptr;
  } else {
    MY_ASSERT(false);
  }
}

Value* CminusfBuilder::const_float_op(ConstantFP* left, ConstantFP* right, MulOp op) {
  if (op == MulOp::OP_MUL) {
    return CONST_FP(left->get_value() * right->get_value());
  } else if (op == MulOp::OP_DIV) {
    return CONST_FP(left->get_value() / right->get_value());
  } else if (op == MulOp::OP_MOD) {
    semantic_error() << "operands of binary operator % cannot be a float number";
    return nullptr;
  } else {
    MY_ASSERT(false);
  }
}

Value* CminusfBuilder::float_op(Value* left, Value* right, RelOp op) {
  if (op == RelOp::OP_LT) {
    return builder->create_fcmp_lt(left, right);
  } else if (op == RelOp::OP_LTE) {
    return builder->create_fcmp_le(left, right);
  } else if (op == RelOp::OP_GT) {
    return builder->create_fcmp_gt(left, right);
  } else if (op == RelOp::OP_GTE) {
    return builder->create_fcmp_ge(left, right);
  } else {
    MY_ASSERT(false);
  }
}

Value* CminusfBuilder::const_float_op(ConstantFP* left, ConstantFP* right, RelOp op) {
  if (op == RelOp::OP_LT) {
    return left->get_value() < right->get_value() ? CONST_BOOL(true) : CONST_BOOL(false);
  } else if (op == RelOp::OP_LTE) {
    return left->get_value() <= right->get_value() ? CONST_BOOL(true) : CONST_BOOL(false);
  } else if (op == RelOp::OP_GT) {
    return left->get_value() > right->get_value() ? CONST_BOOL(true) : CONST_BOOL(false);
  } else if (op == RelOp::OP_GTE) {
    return left->get_value() >= right->get_value() ? CONST_BOOL(true) : CONST_BOOL(false);
  } else {
    MY_ASSERT(false);
  }
}

Value* CminusfBuilder::float_op(Value* left, Value* right, EqOp op) {
  if (op == EqOp::OP_EQ) {
    return builder->create_fcmp_eq(left, right);
  } else if (op == EqOp::OP_NEQ) {
    return builder->create_fcmp_ne(left, right);
  } else {
    MY_ASSERT(false);
  }
}

Value* CminusfBuilder::const_float_op(ConstantFP* left, ConstantFP* right, EqOp op) {
  if (op == EqOp::OP_EQ) {
    return left->get_value() == right->get_value() ? CONST_BOOL(true) : CONST_BOOL(false);
  } else if (op == EqOp::OP_NEQ) {
    return left->get_value() != right->get_value() ? CONST_BOOL(true) : CONST_BOOL(false);
  } else {
    MY_ASSERT(false);
  }
}

Value* CminusfBuilder::to_int32_type(Value* value) {
  if (value->get_type()->is_float_type()) {
    if (auto constFp = dynamic_cast<ConstantFP*>(value); constFp) {
      return CONST_INT((int)constFp->get_value());
    } else {
      return builder->create_fptosi(value, INT32_T);
    }
  }
  if (value->get_type()->is_int32_type()) {
    return value;
  }
  MY_ASSERT(value->get_type()->is_int1_type());
  return builder->create_zext(value, INT32_T);
}

Value* CminusfBuilder::to_float_type(Value* value) {
  if (value->get_type()->is_float_type()) {
    return value;
  }
  if (value->get_type()->is_int32_type()) {
    if (auto constInt = to_const<ConstantInt>(value); constInt) {
      return CONST_FP(constInt->get_value());
    }
    return builder->create_sitofp(value, FLOAT_T);
  }
  MY_ASSERT(value->get_type()->is_int1_type());
  return builder->create_sitofp(to_int32_type(value), FLOAT_T);
}

template<typename T>
Value* CminusfBuilder::binary_op(Value *left, Value *right, T op) {
  MY_ASSERT(left->get_type()->is_float_type() || left->get_type()->is_integer_type());
  MY_ASSERT(right->get_type()->is_float_type() || right->get_type()->is_integer_type());

  if (left->get_type()->is_float_type()) {
    right = to_float_type(right);
  } else if (right->get_type()->is_float_type()) {
    left = to_float_type(left);
  } else if (left->get_type()->is_int32_type()) {
    right = to_int32_type(right);
  } else if (right->get_type()->is_int32_type()) {
    left = to_int32_type(left);
  } else { // both int1
    MY_ASSERT(left->get_type()->is_int1_type() && right->get_type()->is_int1_type());
  }

  if (left->get_type()->is_float_type()) {
    MY_ASSERT(right->get_type()->is_float_type());
    auto leftConstFp = to_const<ConstantFP>(left);
    auto rightConstFp = to_const<ConstantFP>(right);
    if (leftConstFp && rightConstFp) {
      return const_float_op(leftConstFp, rightConstFp, op);
    } else {
      return float_op(left, right, op);
    }
  } else {
    MY_ASSERT(left->get_type()->is_integer_type() && right->get_type()->is_integer_type());
    auto leftConstInt = to_const<ConstantInt>(left);
    auto rightConstInt = to_const<ConstantInt>(right);
    if (leftConstInt && rightConstInt) {
      return const_int_op(leftConstInt, rightConstInt, op);
    } else {
      return int_op(left, right, op);
    }
  }
}

// 表达式是 int/float 型
// AddExp → MulExp | AddExp ('+' | '−') MulExp
Value* CminusfBuilder::visit(AstAddExp &node) {
  if (node.AddExp) {
    Value *left = node.AddExp->accept(*this);
    Value *right = node.MulExp->accept(*this);
    return binary_op(left, right, node.op);
  } else {
    return node.MulExp->accept(*this);
  }
}

Value* CminusfBuilder::visit(AstRelExp &node) {
  if (node.RelExp) {
    Value *left = node.RelExp->accept(*this);
    Value *right = node.AddExp->accept(*this);
    return binary_op(left, right, node.op);
  } else {
    return node.AddExp->accept(*this);
  }
}

Value* CminusfBuilder::visit(AstEqExp &node) {
  if (node.EqExp) {
    Value *left = node.EqExp->accept(*this);
    Value *right = node.RelExp->accept(*this);
    return binary_op(left, right, node.op);
  } else {
    return node.RelExp->accept(*this);
  }
}

Value* CminusfBuilder::gen_cmp(Value* value) {
  Value *instruction = nullptr;
  if (value->get_type() == INT1_T) {
      instruction = value;
  } else if (value->get_type() == INT32_T) {
      if(auto constInt = to_const<ConstantInt>(value); constInt) {
          instruction = CONST_INT(constInt->get_value() != 0);
      } else {
          instruction = builder->create_icmp_ne(value, CONST_INT(0));
      }
  } else if (value->get_type() == FLOAT_T) {
      if(auto constFp = to_const<ConstantFP>(value); constFp) {
          instruction = CONST_INT(constFp->get_value() != 0);
      } else {
          instruction = builder->create_fcmp_ne(value, CONST_FP(0));
      }
  }
  return instruction;
}

Value* CminusfBuilder::visit(AstLAndExp &node) {
  if(node.LAndExp == nullptr) {
    return node.EqExp->accept(*this);
  } else {
    std::string no = next_bb_num_string();
    auto trueBB = BasicBlock::create(module.get(), "trueBB" + no, context.func);
    Value *left_last_value = node.LAndExp->accept(*this);
    Value *cond = gen_cmp(left_last_value);
    builder->create_cond_br(cond, trueBB, context.logicBB_stack.back().falseBB);
    builder->set_insert_point(trueBB);
    return node.EqExp->accept(*this);
  }
}

Value* CminusfBuilder::visit(AstLOrExp &node) {
  if(node.LOrExp == nullptr) {
    return node.LAndExp->accept(*this);
  } else {
    std::string no = next_bb_num_string();
    auto falseBB = BasicBlock::create(module.get(), "falseBB"  + no, context.func);
    context.logicBB_stack.push_back({context.logicBB_stack.back().trueBB, falseBB});
    Value *left_last_value = node.LOrExp->accept(*this);
    context.logicBB_stack.pop_back();
    Value *cond = gen_cmp(left_last_value);
    builder->create_cond_br(cond, context.logicBB_stack.back().trueBB, falseBB);
    builder->set_insert_point(falseBB);
    return node.LAndExp->accept(*this);
  }
}

Value* CminusfBuilder::visit(AstConstExp &node) {
  return node.AddExp->accept(*this);
}

Value *CminusfBuilder::visit(AstExp &node) {
  return node.AddExp->accept(*this);
}

Value *CminusfBuilder::visit(AstCond &node) {
  return node.LOrExp->accept(*this);
}
