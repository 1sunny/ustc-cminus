#pragma once

#include <iostream>
#include <vector>

class Module;
class IntegerType;
class FunctionType;
class ArrayType;
class PointerType;
class FloatType;

class Type {
  public:
    enum TypeID {
        VoidTyID,     // Void
        LabelTyID,    // Labels, e.g., BasicBlock
        IntegerTyID,  // Integers, include 32 bits and 1 bit
        FunctionTyID, // Functions
        ArrayTyID,    // Arrays
        PointerTyID,  // Pointer
        FloatTyID     // float
    };

    explicit Type(TypeID tid, Module *m);
    ~Type() = default;

    // 子类的构造函数会调用Type构造函数设置tid_
    TypeID get_type_id() const { return tid_; }

    bool is_void_type() const { return get_type_id() == VoidTyID; }
    bool is_label_type() const { return get_type_id() == LabelTyID; }
    bool is_integer_type() const { return get_type_id() == IntegerTyID; }
    bool is_function_type() const { return get_type_id() == FunctionTyID; }
    bool is_array_type() const { return get_type_id() == ArrayTyID; }
    bool is_pointer_type() const { return get_type_id() == PointerTyID; }
    bool is_float_type() const { return get_type_id() == FloatTyID; }
    bool is_int32_type() const;
    bool is_int1_type() const;

    // Return related data member if is the required type, else throw error
    // 若是 PointerType 则返回指向的类型，若不是则返回 nullptr
    Type *get_pointer_element_type() const;
    // 若是 ArrayType 则返回数组元素的类型，若不是则返回 nullptr
    Type *get_array_element_type() const;

    Module *get_module() const { return m_; }
    unsigned get_size() const;

    std::string print() const;

  private:
    TypeID tid_;
    Module *m_;
};

class IntegerType : public Type {
  public:
    explicit IntegerType(unsigned num_bits, Module *m);

    unsigned get_num_bits() const;

  private:
    unsigned num_bits_;
};

class FunctionType : public Type {
  public:
    FunctionType(Type *result, std::vector<Type *> params);

    // 判断返回值类型是否合法
    static bool is_valid_return_type(Type *ty);
    // 判断参数类型是否合法
    static bool is_valid_argument_type(Type *ty);

    // 根据返回值类型 result，参数类型列表 params 创建函数类型
    static FunctionType *get(Type *result, std::vector<Type *> params);

    // 返回该函数类型的参数个数
    unsigned get_num_of_args() const;

    // 获得该函数类型第 i 个参数的类型
    Type *get_param_type(unsigned i) const;
    // 获得该函数类型的参数类型链表的起始迭代器
    std::vector<Type *>::iterator param_begin() { return args_.begin(); }
    // 获得该函数类型的参数类型链表的结束迭代器
    std::vector<Type *>::iterator param_end() { return args_.end(); }
    // 获得该函数类型的返回值类型
    Type *get_return_type() const;

  private:
    Type *result_;
    std::vector<Type *> args_;
};

class ArrayType : public Type {
  public:
    ArrayType(Type *contained, unsigned num_elements);

    // 判断数组元素类型是否合法
    static bool is_valid_element_type(Type *ty);

    // 通过数组元素类型 contained 以及数组长度 num_elements 创建数组类型
    static ArrayType *get(Type *contained, unsigned num_elements);

    // 得到该数组类型的元素类型
    Type *get_element_type() const { return contained_; }
    // 获得该数组类型的长度
    unsigned get_num_of_elements() const { return num_elements_; }

  private:
    Type *contained_;       // The element type of the array.
    unsigned num_elements_; // Number of elements in the array.
};

class PointerType : public Type {
  public:
    PointerType(Type *contained);
    // 获取该指针类型指向的元素类型
    Type *get_element_type() const { return contained_; }

    // 创建指向类型为 contained 的指针类型
    static PointerType *get(Type *contained);

  private:
    Type *contained_; // The element type of the ptr.
};

class FloatType : public Type {
  public:
    FloatType(Module *m);
    static FloatType *get(Module *m);

  private:
};
