#pragma once

#include "Function.hpp"
#include "GlobalVariable.hpp"
#include "Instruction.hpp"
#include "Type.hpp"
#include "Value.hpp"

#include <list>
#include <llvm/ADT/ilist.h>
#include <llvm/ADT/ilist_node.h>
#include <map>
#include <memory>
#include <string>

class GlobalVariable;
class Function;
class Module {
  public:
    Module();
    ~Module() = default;

    Type *get_void_type();
    Type *get_label_type();
    IntegerType *get_int1_type();
    IntegerType *get_int32_type();
    PointerType *get_int32_ptr_type();
    FloatType *get_float_type();
    PointerType *get_float_ptr_type();

    // 获得（创建）自定义的 Pointer 类型
    PointerType *get_pointer_type(Type *contained);
    // 获得（创建）自定义的 Array 类型
    ArrayType *get_array_type(Type *contained, unsigned num_elements);
    FunctionType *get_function_type(Type *retty, std::vector<Type *> &args);

    // 将函数 f 添加到该模块的函数链表上, 在函数被创建的时候会自动调用此方法
    void add_function(Function *f);
    llvm::ilist<Function> &get_functions();
    // 将全局变量 g 添加到该模块的全局变量链表上, 在全局变量被创建的时候会自动调用此方法
    void add_global_variable(GlobalVariable *g);
    // 获取全局变量列表
    llvm::ilist<GlobalVariable> &get_global_variable();

    void set_print_name();
    std::string print();

  private:
    // The global variables in the module
    llvm::ilist<GlobalVariable> global_list_;
    // The functions in the module
    llvm::ilist<Function> function_list_;

    std::unique_ptr<IntegerType> int1_ty_;
    std::unique_ptr<IntegerType> int32_ty_;
    std::unique_ptr<Type> label_ty_;
    std::unique_ptr<Type> void_ty_;
    std::unique_ptr<FloatType> float32_ty_;
    std::map<Type *, std::unique_ptr<PointerType>> pointer_map_;
    std::map<std::pair<Type *, int>, std::unique_ptr<ArrayType>> array_map_;
    std::map<std::pair<Type *, std::vector<Type *>>,
             std::unique_ptr<FunctionType>>
        function_map_;
};
