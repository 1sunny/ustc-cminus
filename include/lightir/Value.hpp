#pragma once

#include <functional>
#include <iostream>
#include <list>
#include <string>

class Type;
class Value;
class User;
struct Use;

class Value {
  public:
    explicit Value(Type *ty, const std::string &name = "")
        : type_(ty), name_(name){};
    virtual ~Value() { replace_all_use_with(nullptr); }

    std::string get_name() const { return name_; };
    // 获取该值的类型
    Type *get_type() const { return type_; }
    // 取得该值的被使用情况
    const std::list<Use> &get_use_list() const { return use_list_; }

    bool set_name(std::string name);

    // 添加该值的被使用情况
    void add_use(User *user, unsigned arg_no);
    // 删除该值的被使用情况
    void remove_use(User *user, unsigned arg_no);

    // 在所有地方将该值用新的值 new_val 替换，并维护好 use_def 和 def_use 链表
    void replace_all_use_with(Value *new_val);
    void replace_use_with_if(Value *new_val, std::function<bool(Use)> pred);

    virtual std::string print() = 0;

  private:
    Type *type_;
    std::list<Use> use_list_; // who use this value
    std::string name_;        // should we put name field here ?
};
