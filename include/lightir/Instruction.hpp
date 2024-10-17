#pragma once

#include "Type.hpp"
#include "User.hpp"

#include <cstdint>
#include <llvm/ADT/ilist_node.h>

class BasicBlock;
class Function;

class Instruction : public User/*, public llvm::ilist_node<Instruction>*/ {
  public:
    enum OpID : uint32_t {
        // Terminator Instructions
        ret,
        br,
        // Standard binary operators
        add,
        sub,
        mul,
        mul64,
        sdiv,
        srem, // TODO
        // float binary operators
        fadd,
        fsub,
        fmul,
        fdiv,
        // Memory operators
        alloca,
        load,
        store,
        // Int compare operators
        ge,
        gt,
        le,
        lt,
        eq,
        ne,
        // Float compare operators
        fge,
        fgt,
        fle,
        flt,
        feq,
        fne,
        // Other operators
        phi,
        call,
        getelementptr,
        zext, // zero extend
        fptosi,
        sitofp,
        // float binary operators

        // Logical operators
        And,
        Or,
        Not,
        //& LIR operators
        cmpbr,
        fcmpbr,
        loadoffset,
        storeoffset,
        cmp,
        fcmp
    };
    /* @parent: if parent!=nullptr, auto insert to bb
     * @ty: result type */
    Instruction(Type *ty, OpID id, unsigned num_ops, BasicBlock *parent = nullptr);
    Instruction(const Instruction &) = delete;
    virtual ~Instruction() = default;

    BasicBlock *get_parent() { return parent_; }
    const BasicBlock *get_parent() const { return parent_; }
    void set_parent(BasicBlock *parent) { this->parent_ = parent; }

    // Return the function this instruction belongs to.
    Function *get_function();
    Module *get_module();

    OpID get_instr_type() const { return op_id_; }
    // 获取指令类型的名字
    std::string get_instr_op_name() const;

    bool is_void() {
        return ((op_id_ == ret) || (op_id_ == br) || (op_id_ == store) ||
                (op_id_ == call && this->get_type()->is_void_type()));
    }

    bool is_phi() const { return op_id_ == phi; }
    bool is_store() const { return op_id_ == store; }
    bool is_alloca() const { return op_id_ == alloca; }
    bool is_ret() const { return op_id_ == ret; }
    bool is_load() const { return op_id_ == load; }
    bool is_br() const { return op_id_ == br; }

    bool is_add() const { return op_id_ == add; }
    bool is_sub() const { return op_id_ == sub; }
    bool is_mul() const { return op_id_ == mul; }
    bool is_div() const { return op_id_ == sdiv; }
    bool is_srem() const { return op_id_ == srem; }

    bool is_fadd() const { return op_id_ == fadd; }
    bool is_fsub() const { return op_id_ == fsub; }
    bool is_fmul() const { return op_id_ == fmul; }
    bool is_fdiv() const { return op_id_ == fdiv; }
    bool is_fp2si() const { return op_id_ == fptosi; }
    bool is_si2fp() const { return op_id_ == sitofp; }

    bool is_cmp() const { return op_id_ == cmp; }
    bool is_fcmp() const { return op_id_ == fcmp; }

    bool is_call() const { return op_id_ == call; }
    bool is_gep() const { return op_id_ == getelementptr; }
    bool is_zext() const { return op_id_ == zext; }

    // bool is_and() const { return op_id_ == And; }
    // bool is_or() const { return op_id_ == Or; }
    // bool is_not() const { return op_id_ == Not; }

    bool is_cmpbr() const { return op_id_ == cmpbr; }
    bool is_fcmpbr() const { return op_id_ == fcmpbr; }

    bool is_storeoffset() const { return op_id_ == storeoffset; }

    bool isBinary() const {
        return (is_add() || is_sub() || is_mul() || is_div() || is_fadd() ||
                is_fsub() || is_fmul() || is_fdiv() ||/* is_and() || is_or() ||*/ is_srem())  &&
               (get_num_operand() == 2);
    }

    bool isTerminator() const { return is_br() || is_ret() || is_cmpbr() || is_fcmpbr();; }

    void set_id(int id) { id_ = id; }
    int get_id() { return id_; }

    virtual Instruction *copy_inst(BasicBlock *bb) = 0;

  private:
    OpID op_id_;
    BasicBlock *parent_;
    int id_;
};

class BinaryInst : public Instruction {
public:
    static BinaryInst *create_add(Value *v1, Value *v2, BasicBlock *bb, Module *m);
    static BinaryInst *create_sub(Value *v1, Value *v2, BasicBlock *bb, Module *m);
    static BinaryInst *create_mul(Value *v1, Value *v2, BasicBlock *bb, Module *m);
    static BinaryInst *create_mul64(Value *v1, Value *v2, BasicBlock *bb, Module *m);
    static BinaryInst *create_sdiv(Value *v1, Value *v2, BasicBlock *bb, Module *m);
    static BinaryInst *create_srem(Value *v1, Value *v2, BasicBlock *bb, Module *m);
    static BinaryInst *create_fadd(Value *v1, Value *v2, BasicBlock *bb, Module *m);
    static BinaryInst *create_fsub(Value *v1, Value *v2, BasicBlock *bb, Module *m);
    static BinaryInst *create_fmul(Value *v1, Value *v2, BasicBlock *bb, Module *m);
    static BinaryInst *create_fdiv(Value *v1, Value *v2, BasicBlock *bb, Module *m);

    static BinaryInst *create_and(Value *v1, Value *v2, BasicBlock *bb, Module *m);
    static BinaryInst *create_or(Value *v1, Value *v2, BasicBlock *bb, Module *m);
    static BinaryInst *create_xor(Value *v1, Value *v2, BasicBlock *bb, Module *m);

    static BinaryInst *create_asr(Value *v1, Value *v2, BasicBlock *bb, Module *m);
    static BinaryInst *create_lsl(Value *v1, Value *v2, BasicBlock *bb, Module *m);
    static BinaryInst *create_lsr(Value *v1, Value *v2, BasicBlock *bb, Module *m);
    static BinaryInst *create_asr64(Value *v1, Value *v2, BasicBlock *bb, Module *m);
    static BinaryInst *create_lsl64(Value *v1, Value *v2, BasicBlock *bb, Module *m);
    static BinaryInst *create_lsr64(Value *v1, Value *v2, BasicBlock *bb, Module *m);

    virtual std::string print() override;

    Instruction *copy_inst(BasicBlock *bb) override final {
      return new BinaryInst(get_type(), get_instr_type(), get_operand(0), get_operand(1), bb);
    }

private:
    BinaryInst(Type *ty, OpID id, Value *v1, Value *v2, BasicBlock *bb);

    //~ void assert_valid();
};


enum CmpOp {
    EQ, // ==
    NE, // !=
    GT, // >
    GE, // >=
    LT, // <
    LE  // <=
};

class CmpInst : public Instruction {
public:
    static CmpInst *create_cmp(CmpOp op, Value *lhs, Value *rhs, BasicBlock *bb, Module *m);

    CmpOp get_cmp_op() { return cmp_op_; }

    void negation();

    virtual std::string print() override;

    Instruction *copy_inst(BasicBlock *bb) override final {
      return new CmpInst(get_type(), cmp_op_, get_operand(0), get_operand(1), bb);
    }

private:
    CmpInst(Type *ty, CmpOp op, Value *lhs, Value *rhs, BasicBlock *bb);
    //~ void assert_valid();

private:
    CmpOp cmp_op_;

};

class FCmpInst : public Instruction {
public:
    static FCmpInst *create_fcmp(CmpOp op, Value *lhs, Value *rhs, BasicBlock *bb, Module *m);

    CmpOp get_cmp_op() { return cmp_op_; }

    void negation();

    virtual std::string print() override;
    Instruction *copy_inst(BasicBlock *bb) override final {
      return new FCmpInst(get_type(), cmp_op_, get_operand(0), get_operand(1), bb);
    }

private:
    FCmpInst(Type *ty, CmpOp op, Value *lhs, Value *rhs, BasicBlock *bb);

    //~ void assert_valid();

private:
    CmpOp cmp_op_;
};

class CallInst : public Instruction {
public:
    static CallInst *create_call(Function *func, std::vector<Value *>args, BasicBlock *bb);

    FunctionType *get_function_type() const { return static_cast<FunctionType *>(get_operand(0)->get_type()); }

    virtual std::string print() override;

    Instruction *copy_inst(BasicBlock *bb) override final{
      std::vector<Value *> args;
      for (auto i = 1; i < get_num_operand(); i++){
        args.push_back(get_operand(i));
      }
      auto new_inst = new CallInst(get_function_type()->get_return_type(),args,bb);
      new_inst->set_operand(0, get_operand(0));
      return new_inst;
    }

protected:
    CallInst(Function *func, std::vector<Value *>args, BasicBlock *bb);
    CallInst(Type *ret_ty, std::vector<Value *> args, BasicBlock *bb);
};

class BranchInst : public Instruction {
public:
    static BranchInst *create_cond_br(Value *cond, BasicBlock *if_true, BasicBlock *if_false, BasicBlock *bb);
    static BranchInst *create_br(BasicBlock *if_true, BasicBlock *bb);


    bool is_cond_br() const { return get_num_operand() == 3; }
    // bool is_extend_cond_br() const { return get_num_operand() == 3 || get_num_operand() == 4; }

    virtual std::string print() override;

    Instruction *copy_inst(BasicBlock *bb) override final{
      if (get_num_operand() == 1){
        auto new_inst = new BranchInst(bb);
        new_inst->set_operand(0, get_operand(0));
        return new_inst;
      } else {
        auto new_inst = new BranchInst(get_operand(0),bb);
        new_inst->set_operand(1, get_operand(1));
        new_inst->set_operand(2, get_operand(2));
        return new_inst;
      }
    }

private:
    BranchInst(Value *cond, BasicBlock *if_true, BasicBlock *if_false,
               BasicBlock *bb);
    BranchInst(BasicBlock *if_true, BasicBlock *bb);
    BranchInst(BasicBlock *bb);
    BranchInst(Value *cond, BasicBlock *bb);
};

class ReturnInst : public Instruction {
public:
    static ReturnInst *create_ret(Value *val, BasicBlock *bb);
    static ReturnInst *create_void_ret(BasicBlock *bb);

    bool is_void_ret() const { return get_num_operand() == 0; }

    Type * get_ret_type() const { return get_operand(0)->get_type(); }

    virtual std::string print() override;

    Instruction *copy_inst(BasicBlock *bb) override final{
      if (is_void_ret()){
        return new ReturnInst(bb);
      } else {
        return new ReturnInst(get_operand(0),bb);
      }
    }

private:
    ReturnInst(Value *val, BasicBlock *bb);
    ReturnInst(BasicBlock *bb);
};

class GetElementPtrInst : public Instruction {
public:
    static Type *get_element_type(Value *ptr, std::vector<Value *> idxs);
    static GetElementPtrInst *create_gep(Value *ptr, std::vector<Value *> idxs, BasicBlock *bb);
    Type *get_element_type() const { return element_ty_; }

    virtual std::string print() override;

    Instruction *copy_inst(BasicBlock *bb) override final{
      std::vector<Value *> idxs;
      for (auto i = 1; i < get_num_operand(); i++) {
        idxs.push_back(get_operand(i));
      }
      return new GetElementPtrInst(get_operand(0),idxs,bb);
    }

private:
    GetElementPtrInst(Value *ptr, std::vector<Value *> idxs, BasicBlock *bb);

private:
    Type *element_ty_;
};


class StoreInst : public Instruction {
public:
    static StoreInst *create_store(Value *val, Value *ptr, BasicBlock *bb);

    Value *get_rval() { return this->get_operand(0); }
    Value *get_lval() { return this->get_operand(1); }

    virtual std::string print() override;

    Instruction *copy_inst(BasicBlock *bb) override final{
      return new StoreInst(get_operand(0),get_operand(1),bb);
    }

private:
    StoreInst(Value *val, Value *ptr, BasicBlock *bb);
};

//& 加速使用全0初始化数组的代码优化分析和代码生成
class MemsetInst : public Instruction {
public:
    static MemsetInst *create_memset(Value *ptr, BasicBlock *bb);

    Value *get_lval() { return this->get_operand(0); }

    virtual std::string print() override;

    Instruction *copy_inst(BasicBlock *bb) override final{
      return new MemsetInst(get_operand(0),bb);
    }

private:
    MemsetInst(Value *ptr, BasicBlock *bb);
};

class LoadInst : public Instruction {
public:
    static LoadInst *create_load(Type *ty, Value *ptr, BasicBlock *bb);

    Value * get_lval() { return this->get_operand(0); }

    Type *get_load_type() const { return static_cast<PointerType *>(get_operand(0)->get_type())->get_element_type(); }

    virtual std::string print() override;

    Instruction *copy_inst(BasicBlock *bb) override final{
      return new LoadInst(get_type(),get_operand(0),bb);
    }

private:
    LoadInst(Type *ty, Value *ptr, BasicBlock *bb);
};


class AllocaInst : public Instruction {
public:
    static AllocaInst *create_alloca(Type *ty, BasicBlock *bb);

    Type *get_alloca_type() const { return alloca_ty_; }

    virtual std::string print() override;

    Instruction *copy_inst(BasicBlock *bb) override final{
      return new AllocaInst(alloca_ty_,bb);
    }

private:
    AllocaInst(Type *ty, BasicBlock *bb);

private:
    Type *alloca_ty_;
};

class ZextInst : public Instruction {
public:
    static ZextInst *create_zext(Value *val, Type *ty, BasicBlock *bb);

    Type *get_dest_type() const { return dest_ty_; }

    virtual std::string print() override;

    Instruction *copy_inst(BasicBlock *bb) override final{
      return new ZextInst(get_instr_type(),get_operand(0),dest_ty_,bb);
    }

private:
    ZextInst(OpID op, Value *val, Type *ty, BasicBlock *bb);

private:
    Type *dest_ty_;
};

class SiToFpInst : public Instruction {
public:
    static SiToFpInst *create_sitofp(Value *val, Type *ty, BasicBlock *bb);

    Type *get_dest_type() const { return dest_ty_; }

    virtual std::string print() override;

    Instruction *copy_inst(BasicBlock *bb) override final{
      return new SiToFpInst(get_instr_type(), get_operand(0), get_dest_type(), bb);
    }

private:
    SiToFpInst(OpID op, Value *val, Type *ty, BasicBlock *bb);

private:
    Type *dest_ty_;
};

class FpToSiInst : public Instruction {
public:
    static FpToSiInst *create_fptosi(Value *val, Type *ty, BasicBlock *bb);

    Type *get_dest_type() const { return dest_ty_; }

    virtual std::string print() override;

    Instruction *copy_inst(BasicBlock *bb) override final{
      return new FpToSiInst(get_instr_type(), get_operand(0), get_dest_type(), bb);
    }

private:
    FpToSiInst(OpID op, Value *val, Type *ty, BasicBlock *bb);

private:
    Type *dest_ty_;
};

class PhiInst : public Instruction {
public:
    static PhiInst *create_phi(Type *ty, BasicBlock *bb);

    Value *get_lval() { return l_val_; }
    void set_lval(Value *l_val) { l_val_ = l_val; }

    void add_phi_pair_operand(Value *val, Value *pre_bb) {
      this->add_operand(val);
      this->add_operand(pre_bb);
    }

    virtual std::string print() override;

    Instruction *copy_inst(BasicBlock *bb) override final{
      auto new_inst = create_phi(get_type(), bb);
      for (auto op : get_operands()){
        new_inst->add_operand(op);
      }
      return new_inst;
    }
private:
    PhiInst(OpID op, std::vector<Value *> vals, std::vector<BasicBlock *> val_bbs, Type *ty, BasicBlock *bb);

private:
    Value *l_val_;
};

class CmpBrInst: public Instruction {

public:
    static CmpBrInst *create_cmpbr(CmpOp op, Value *lhs, Value *rhs, BasicBlock *if_true, BasicBlock *if_false, BasicBlock *bb, Module *m);

    CmpOp get_cmp_op() { return cmp_op_; }

    bool is_cmp_br() const;

    virtual std::string print() override;

    Instruction *copy_inst(BasicBlock *bb) override final{
      auto new_inst = new CmpBrInst(cmp_op_,get_operand(0),get_operand(1),bb);
      new_inst->set_operand(2, get_operand(2));
      new_inst->set_operand(3, get_operand(3));
      return new_inst;
    }

private:
    CmpBrInst(CmpOp op, Value *lhs, Value *rhs, BasicBlock *if_true, BasicBlock *if_false, BasicBlock *bb);
    CmpBrInst(CmpOp op, Value *lhs, Value *rhs, BasicBlock *bb);
private:
    CmpOp cmp_op_;

};


class FCmpBrInst : public Instruction {
public:
    static FCmpBrInst *create_fcmpbr(CmpOp op, Value *lhs, Value *rhs, BasicBlock *if_true, BasicBlock *if_false, BasicBlock *bb, Module *m);

    CmpOp get_cmp_op() { return cmp_op_; }

    bool is_fcmp_br() const;

    virtual std::string print() override;

    Instruction *copy_inst(BasicBlock *bb) override final{
      auto new_inst = new FCmpBrInst(cmp_op_,get_operand(0),get_operand(1),bb);
      new_inst->set_operand(2, get_operand(2));
      new_inst->set_operand(3, get_operand(3));
      return new_inst;
    }


private:
    FCmpBrInst(CmpOp op, Value *lhs, Value *rhs, BasicBlock *if_true, BasicBlock *if_false, BasicBlock *bb);
    FCmpBrInst(CmpOp op, Value *lhs, Value *rhs, BasicBlock *bb);
private:
    CmpOp cmp_op_;
};

class LoadOffsetInst: public Instruction {
public:
    static LoadOffsetInst *create_loadoffset(Type *ty, Value *ptr, Value *offset, BasicBlock *bb);

    Value *get_lval() { return this->get_operand(0); }
    Value *get_offset() { return this->get_operand(1); }

    Type *get_load_type() const;

    virtual std::string print() override;

    Instruction *copy_inst(BasicBlock *bb) override final{
      auto new_inst = new LoadOffsetInst(get_type(), get_operand(0), bb);
      new_inst->set_operand(1, get_operand(1));
      return new_inst;
    }

private:
    LoadOffsetInst(Type *ty, Value *ptr, Value *offset, BasicBlock *bb);
    LoadOffsetInst(Type *ty, Value *ptr, BasicBlock *bb);
};


class StoreOffsetInst: public Instruction {

public:
    static StoreOffsetInst *create_storeoffset(Value *val, Value *ptr, Value *offset, BasicBlock *bb);

    Type *get_store_type() const;

    Value *get_rval() { return this->get_operand(0); }
    Value *get_lval() { return this->get_operand(1); }
    Value *get_offset() { return this->get_operand(2); }

    virtual std::string print() override;

    Instruction *copy_inst(BasicBlock *bb) override final{
      auto new_inst = new StoreOffsetInst(get_operand(0), get_operand(1), bb);
      new_inst->set_operand(2, get_operand(2));
      return new_inst;
    }

private:
    StoreOffsetInst(Value *val, Value *ptr, Value *offset, BasicBlock *bb);
    StoreOffsetInst(Value *val, Value *ptr, BasicBlock *bb);
};
