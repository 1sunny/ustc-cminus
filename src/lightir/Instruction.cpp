#include "Instruction.hpp"
#include "BasicBlock.hpp"
#include "Function.hpp"
#include "IRprinter.hpp"
#include "Module.hpp"
#include "Type.hpp"
#include "logging.hpp"

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <string>
#include <vector>

Instruction::Instruction(Type *ty, OpID id, unsigned num_ops, BasicBlock *parent)
    : User(ty, "", num_ops), op_id_(id), parent_(parent) {
    if (parent)
        parent->add_instruction(this);
}

Function *Instruction::get_function() { return parent_->get_parent(); }
Module *Instruction::get_module() { return parent_->get_module(); }

// std::string Instruction::get_instr_op_name() const {
//     return print_instr_op_name(op_id_);
// }

BinaryInst::BinaryInst(Type *ty, OpID id, Value *v1, Value *v2, BasicBlock *bb)
        : Instruction(ty, id, 2, bb) {
    set_operand(0, v1);
    set_operand(1, v2);
}

BinaryInst *BinaryInst::create_add(Value *v1, Value *v2, BasicBlock *bb, Module *m) {
    return new BinaryInst(v1->get_type()->is_pointer_type() ? v1->get_type() : v2->get_type(), Instruction::add, v1, v2, bb);
}

BinaryInst *BinaryInst::create_sub(Value *v1, Value *v2, BasicBlock *bb, Module *m) {
    return new BinaryInst(Type::get_int32_type(m), Instruction::sub, v1, v2, bb);
}

BinaryInst *BinaryInst::create_mul(Value *v1, Value *v2, BasicBlock *bb, Module *m) {
    return new BinaryInst(Type::get_int32_type(m), Instruction::mul, v1, v2, bb);
}

BinaryInst *BinaryInst::create_mul64(Value *v1, Value *v2, BasicBlock *bb, Module *m) {
    return new BinaryInst(Type::get_int32_type(m), Instruction::mul64, v1, v2, bb);
}

BinaryInst *BinaryInst::create_sdiv(Value *v1, Value *v2, BasicBlock *bb, Module *m) {
    return new BinaryInst(Type::get_int32_type(m), Instruction::sdiv, v1, v2, bb);
}

BinaryInst *BinaryInst::create_srem(Value *v1, Value *v2, BasicBlock *bb, Module *m) {
    return new BinaryInst(Type::get_int32_type(m), Instruction::srem, v1, v2, bb);
}

BinaryInst *BinaryInst::create_fadd(Value *v1, Value *v2, BasicBlock *bb, Module *m) {
    return new BinaryInst(Type::get_float_type(m), Instruction::fadd, v1, v2, bb);
}

BinaryInst *BinaryInst::create_fsub(Value *v1, Value *v2, BasicBlock *bb, Module *m) {
    return new BinaryInst(Type::get_float_type(m), Instruction::fsub, v1, v2, bb);
}

BinaryInst *BinaryInst::create_fmul(Value *v1, Value *v2, BasicBlock *bb, Module *m) {
    return new BinaryInst(Type::get_float_type(m), Instruction::fmul, v1, v2, bb);
}

BinaryInst *BinaryInst::create_fdiv(Value *v1, Value *v2, BasicBlock *bb, Module *m) {
    return new BinaryInst(Type::get_float_type(m), Instruction::fdiv, v1, v2, bb);
}

// BinaryInst *BinaryInst::create_and(Value *v1, Value *v2, BasicBlock *bb, Module *m) {
//     return new BinaryInst(Type::get_int32_type(m), Instruction::land, v1, v2, bb);
// }
//
// BinaryInst *BinaryInst::create_or(Value *v1, Value *v2, BasicBlock *bb, Module *m) {
//     return new BinaryInst(Type::get_int32_type(m), Instruction::lor, v1, v2, bb);
// }
//
// BinaryInst *BinaryInst::create_xor(Value *v1, Value *v2, BasicBlock *bb, Module *m) {
//     return new BinaryInst(Type::get_int32_type(m), Instruction::lxor, v1, v2, bb);
// }
//
// BinaryInst *BinaryInst::create_asr(Value *v1, Value *v2, BasicBlock *bb, Module *m) {
//     return new BinaryInst(Type::get_int32_type(m), Instruction::asr, v1, v2, bb);
// }
//
// BinaryInst *BinaryInst::create_lsl(Value *v1, Value *v2, BasicBlock *bb, Module *m) {
//     return new BinaryInst(Type::get_int32_type(m), Instruction::shl, v1, v2, bb);
// }
//
// BinaryInst *BinaryInst::create_lsr(Value *v1, Value *v2, BasicBlock *bb, Module *m) {
//     return new BinaryInst(Type::get_int32_type(m), Instruction::lsr, v1, v2, bb);
// }
//
// BinaryInst *BinaryInst::create_asr64(Value *v1, Value *v2, BasicBlock *bb, Module *m) {
//     return new BinaryInst(Type::get_int32_type(m), Instruction::asr64, v1, v2, bb);
// }
//
// BinaryInst *BinaryInst::create_lsl64(Value *v1, Value *v2, BasicBlock *bb, Module *m) {
//     return new BinaryInst(Type::get_int32_type(m), Instruction::shl64, v1, v2, bb);
// }
//
// BinaryInst *BinaryInst::create_lsr64(Value *v1, Value *v2, BasicBlock *bb, Module *m) {
//     return new BinaryInst(Type::get_int32_type(m), Instruction::lsr64, v1, v2, bb);
// }



std::string BinaryInst::print() {
    std::string instr_ir;
    instr_ir += "%";
    instr_ir += this->get_name();
    instr_ir += " = ";
    instr_ir += this->get_module()->get_instr_op_name(this->get_instr_type());
    instr_ir += " ";
    instr_ir += this->get_operand(0)->get_type()->print();
    instr_ir += " ";
    instr_ir += print_as_op(this->get_operand(0), false);
    instr_ir += ", ";
    if (Type::is_eq_type(this->get_operand(0)->get_type(), this->get_operand(1)->get_type())) {
        instr_ir += print_as_op(this->get_operand(1), false);
    } else {
        instr_ir += print_as_op(this->get_operand(1), true);
    }
    return instr_ir;
}

//& CmpInst
CmpInst::CmpInst(Type *ty, CmpOp op, Value *lhs, Value *rhs, BasicBlock *bb)
        : Instruction(ty, Instruction::cmp, 2, bb), cmp_op_(op) {
    set_operand(0, lhs);
    set_operand(1, rhs);
}

CmpInst *CmpInst::create_cmp(CmpOp op, Value *lhs, Value *rhs, BasicBlock *bb, Module *m) {
    return new CmpInst(m->get_int1_type(), op, lhs, rhs, bb);
}

void CmpInst::negation() {
    switch(get_cmp_op()) {
        case CmpOp::EQ:
            cmp_op_ = CmpOp::NE;
        break;
        case CmpOp::NE:
            cmp_op_ = CmpOp::EQ;
        break;
        case CmpOp::GT:
            cmp_op_ = CmpOp::LE;
        break;
        case CmpOp::GE:
            cmp_op_ = CmpOp::LT;
        break;
        case CmpOp::LT:
            cmp_op_ = CmpOp::GE;
        break;
        case CmpOp::LE:
            cmp_op_ = CmpOp::GT;
        break;
        default:
            break;
    }
    return ;
}

std::string CmpInst::print() {
    std::string instr_ir;
    instr_ir += "%";
    instr_ir += this->get_name();
    instr_ir += " = ";
    instr_ir += this->get_module()->get_instr_op_name(this->get_instr_type());
    instr_ir += " ";
    instr_ir += print_cmp_type(this->cmp_op_);
    instr_ir += " ";
    instr_ir += this->get_operand(0)->get_type()->print();
    instr_ir += " ";
    instr_ir += print_as_op(this->get_operand(0), false);
    instr_ir += ", ";
    if (Type::is_eq_type(this->get_operand(0)->get_type(), this->get_operand(1)->get_type())) {
        instr_ir += print_as_op(this->get_operand(1), false);
    } else {
        instr_ir += print_as_op(this->get_operand(1), true);
    }
    return instr_ir;
}


//& FCmpInst

FCmpInst::FCmpInst(Type *ty, CmpOp op, Value *lhs, Value *rhs, BasicBlock *bb)
        : Instruction(ty, Instruction::fcmp, 2, bb), cmp_op_(op) {
    set_operand(0, lhs);
    set_operand(1, rhs);
}

FCmpInst *FCmpInst::create_fcmp(CmpOp op, Value *lhs, Value *rhs, BasicBlock *bb, Module *m) {
    return new FCmpInst(m->get_int1_type(), op, lhs, rhs, bb);
}

void FCmpInst::negation() {
    switch(get_cmp_op()) {
        case CmpOp::EQ:
            cmp_op_ = CmpOp::NE;
        break;
        case CmpOp::NE:
            cmp_op_ = CmpOp::EQ;
        break;
        case CmpOp::GT:
            cmp_op_ = CmpOp::LE;
        break;
        case CmpOp::GE:
            cmp_op_ = CmpOp::LT;
        break;
        case CmpOp::LT:
            cmp_op_ = CmpOp::GE;
        break;
        case CmpOp::LE:
            cmp_op_ = CmpOp::GT;
        break;
        default:
            break;
    }
    return ;
}

std::string FCmpInst::print() {
    std::string instr_ir;
    instr_ir += "%";
    instr_ir += this->get_name();
    instr_ir += " = ";
    instr_ir += this->get_module()->get_instr_op_name(this->get_instr_type());
    instr_ir += " ";
    instr_ir += print_fcmp_type(this->cmp_op_);
    instr_ir += " ";
    instr_ir += this->get_operand(0)->get_type()->print();
    instr_ir += " ";
    instr_ir += print_as_op(this->get_operand(0), false);
    instr_ir += ",";
    if (Type::is_eq_type(this->get_operand(0)->get_type(), this->get_operand(1)->get_type())) {
        instr_ir += print_as_op(this->get_operand(1), false);
    } else {
        instr_ir += print_as_op(this->get_operand(1), true);
    }
    return instr_ir;
}

//& CallInst
CallInst::CallInst(Function *func, std::vector<Value *> args, BasicBlock *bb)
        : Instruction(func->get_return_type(), Instruction::call, args.size()+1, bb) {
    assert(func->get_num_of_args() == args.size());
    int num_ops = args.size() + 1;
    set_operand(0, func);
    for (int i = 1; i < num_ops; i++) {
        set_operand(i, args[i - 1]);
    }
}

CallInst::CallInst(Type *ret_ty, std::vector<Value *> args, BasicBlock *bb)
        : Instruction(ret_ty, Instruction::call, args.size() + 1, bb) {
    int num_ops = args.size() + 1;
    for (int i = 1; i < num_ops; i++) {
        set_operand(i, args[i-1]);
    }
}

CallInst *CallInst::create_call(Function *func, std::vector<Value *> args, BasicBlock *bb) {
    return new CallInst(func, args, bb);
}

std::string CallInst::print() {
    std::string instr_ir;
    if (!this->is_void()) {
        instr_ir += "%";
        instr_ir += this->get_name();
        instr_ir += " = ";
    }
    instr_ir += this->get_module()->get_instr_op_name(this->get_instr_type());
    instr_ir += " ";
    instr_ir += this->get_function_type()->get_return_type()->print();

    instr_ir += " ";
    assert(dynamic_cast<Function *>(this->get_operand(0)) && "Wrong call operand function");
    instr_ir += print_as_op(this->get_operand(0), false);
    instr_ir += "(";
    for (int i = 1; i < this->get_num_operand(); i++) {
        if (i > 1)
            instr_ir += ", ";
        instr_ir += this->get_operand(i)->get_type()->print();
        instr_ir += " ";
        instr_ir += print_as_op(this->get_operand(i), false);
    }
    instr_ir += ")";
    return instr_ir;
}

//& BranchInst
BranchInst::BranchInst(Value *cond, BasicBlock *if_true, BasicBlock *if_false, BasicBlock *bb)
        : Instruction(Type::get_void_type(if_true->get_module()), Instruction::br, 3, bb) {
    set_operand(0, cond);
    set_operand(1, if_true);
    set_operand(2, if_false);
}

BranchInst::BranchInst(BasicBlock *if_true, BasicBlock *bb)
        : Instruction(Type::get_void_type(if_true->get_module()), Instruction::br, 1, bb) {
    set_operand(0, if_true);
}

BranchInst::BranchInst(Value *cond, BasicBlock *bb)
        : Instruction(Type::get_void_type(bb->get_module()), Instruction::br, 3, bb) {
    set_operand(0, cond);
}

BranchInst::BranchInst(BasicBlock *bb)
        : Instruction(Type::get_void_type(bb->get_module()), Instruction::br, 1, bb) {
    //nothing to do
}

BranchInst *BranchInst::create_cond_br(Value *cond, BasicBlock *if_true, BasicBlock *if_false, BasicBlock *bb) {
    if_true->add_pre_basic_block(bb);
    if_false->add_pre_basic_block(bb);
    bb->add_succ_basic_block(if_false);
    bb->add_succ_basic_block(if_true);

    return new BranchInst(cond, if_true, if_false, bb);
}

BranchInst *BranchInst::create_br(BasicBlock *if_true, BasicBlock *bb) {
    if_true->add_pre_basic_block(bb);
    bb->add_succ_basic_block(if_true);

    return new BranchInst(if_true, bb);
}

std::string BranchInst::print() {
    std::string instr_ir;
    instr_ir += this->get_module()->get_instr_op_name(this->get_instr_type());
    instr_ir += " ";
    //// instr_ir += this->get_operand(0)->get_type()->print();
    instr_ir += print_as_op(this->get_operand(0), true);
    if (is_cond_br()) {
        instr_ir += ", ";
        instr_ir += print_as_op(this->get_operand(1), true);
        instr_ir += ", ";
        instr_ir += print_as_op(this->get_operand(2), true);
    }
    return instr_ir;
}

//& ReturnInst

ReturnInst::ReturnInst(Value *val, BasicBlock *bb)
        : Instruction(Type::get_void_type(bb->get_module()), Instruction::ret, 1, bb) {
    set_operand(0, val);
}

ReturnInst::ReturnInst(BasicBlock *bb)
        : Instruction(Type::get_void_type(bb->get_module()), Instruction::ret, 0, bb) {

}

ReturnInst *ReturnInst::create_ret(Value *val, BasicBlock *bb) {
    return new ReturnInst(val, bb);
}

ReturnInst *ReturnInst::create_void_ret(BasicBlock *bb) {
    return new ReturnInst(bb);
}

std::string ReturnInst::print() {
    std::string instr_ir;
    instr_ir += this->get_module()->get_instr_op_name(this->get_instr_type());
    instr_ir += " ";
    if (!is_void_ret()) {
        instr_ir += this->get_operand(0)->get_type()->print();
        instr_ir += " ";
        instr_ir += print_as_op(this->get_operand(0), false);
    } else {
        instr_ir += "void";
    }

    return instr_ir;
}

//& GetElementPtrInst
GetElementPtrInst::GetElementPtrInst(Value *ptr, std::vector<Value *> idxs, BasicBlock *bb)
        : Instruction(PointerType::get(get_element_type(ptr, idxs)), Instruction::getelementptr, 1+idxs.size(), bb) {
    set_operand(0, ptr);
    for (int i = 0; i < idxs.size(); i++) {
        set_operand(i + 1, idxs[i]);
    }
    element_ty_ = get_element_type(ptr, idxs);
}

Type *GetElementPtrInst::get_element_type(Value *ptr, std::vector<Value *> idxs) {
    Type *ty = ptr->get_type()->get_pointer_element_type();
    assert("GetElementPtrInst ptr is wrong type" &&
           (ty->is_array_type() || ty->is_integer_type() || ty->is_float_type()));
    if (ty->is_array_type()) {
        ArrayType *arr_ty = static_cast<ArrayType *>(ty);
        for (int i = 1; i < idxs.size(); i++) {
            ty = arr_ty->get_element_type();
            if (i < idxs.size() - 1) {
                assert(ty->is_array_type() && "Index error!");
            }
            if (ty->is_array_type()) {
                arr_ty = static_cast<ArrayType *>(ty);
            }
        }
    }
    return ty;
}

GetElementPtrInst *GetElementPtrInst::create_gep(Value *ptr, std::vector<Value *> idxs, BasicBlock *bb) {
    return new GetElementPtrInst(ptr, idxs, bb);
}

std::string GetElementPtrInst::print() {
    std::string instr_ir;
    instr_ir += "%";
    instr_ir += this->get_name();
    instr_ir += " = ";
    instr_ir += this->get_module()->get_instr_op_name(this->get_instr_type());
    instr_ir += " ";
    assert(this->get_operand(0)->get_type()->is_pointer_type());
    instr_ir += this->get_operand(0)->get_type()->get_pointer_element_type()->print();
    instr_ir += ", ";
    for (int i = 0; i < this->get_num_operand(); i++) {
        if (i > 0)
            instr_ir += ", ";
        instr_ir += this->get_operand(i)->get_type()->print();
        instr_ir += " ";
        instr_ir += print_as_op(this->get_operand(i), false);
    }
    return instr_ir;
}

//& StoreInst
StoreInst::StoreInst(Value *val, Value *ptr, BasicBlock *bb)
        : Instruction(Type::get_void_type(bb->get_module()), Instruction::store, 2, bb) {
    set_operand(0, val);
    set_operand(1, ptr);
}

StoreInst *StoreInst::create_store(Value *val, Value *ptr, BasicBlock *bb) {
    return new StoreInst(val, ptr, bb);
}

std::string StoreInst::print() {
    std::string instr_ir;
    instr_ir += this->get_module()->get_instr_op_name(this->get_instr_type());
    instr_ir += " ";
    instr_ir += this->get_operand(0)->get_type()->print();
    instr_ir += " ";
    instr_ir += print_as_op(this->get_operand(0), false);
    instr_ir += ", ";
    instr_ir += print_as_op(this->get_operand(1), true);
    return instr_ir;
}

//& MemsetInst
// MemsetInst::MemsetInst(Value *ptr, BasicBlock *bb)
//         : Instruction(Type::get_void_type(bb->get_module()), Instruction::memset, 1, bb) {
//     set_operand(0, ptr);
// }
//
// MemsetInst *MemsetInst::create_memset(Value *ptr, BasicBlock *bb) {
//     return new MemsetInst(ptr, bb);
// }
//
// std::string MemsetInst::print() {
//     std::string instr_ir;
//     instr_ir += this->get_module()->get_instr_op_name(this->get_instr_type());
//     instr_ir += " ";
//     instr_ir += print_as_op(this->get_operand(0), true);
//     return instr_ir;
// }

//& LoadInst
LoadInst::LoadInst(Type *ty, Value *ptr, BasicBlock *bb)
        : Instruction(ty, Instruction::load, 1, bb) {
    assert(ptr->get_type()->is_pointer_type());
    assert(ty == static_cast<PointerType *>(ptr->get_type())->get_element_type());
    set_operand(0, ptr);
}

LoadInst *LoadInst::create_load(Type *ty, Value *ptr, BasicBlock *bb) {
    return new LoadInst(ty, ptr, bb);
}

std::string LoadInst::print() {
    std::string instr_ir;
    instr_ir += "%";
    instr_ir += this->get_name();
    instr_ir += " = ";
    instr_ir += this->get_module()->get_instr_op_name(this->get_instr_type());
    instr_ir += " ";
    assert(this->get_operand(0)->get_type()->is_pointer_type());
    instr_ir += this->get_operand(0)->get_type()->get_pointer_element_type()->print();
    instr_ir += ",";
    instr_ir += " ";
    instr_ir += print_as_op(this->get_operand(0), true);
    return instr_ir;
}

//& AllocInst
AllocaInst::AllocaInst(Type *ty, BasicBlock *bb)
        : Instruction(PointerType::get(ty), Instruction::alloca, 0, bb), alloca_ty_(ty) {

}

AllocaInst *AllocaInst::create_alloca(Type *ty, BasicBlock *bb) {
    return new AllocaInst(ty, bb);
}

std::string AllocaInst::print() {
    std::string instr_ir;
    instr_ir += "%";
    instr_ir += this->get_name();
    instr_ir += " = ";
    instr_ir += this->get_module()->get_instr_op_name(this->get_instr_type());
    instr_ir += " ";
    instr_ir += get_alloca_type()->print();
    return instr_ir;
}

//& ZextInst
ZextInst::ZextInst(OpID op, Value *val, Type *ty, BasicBlock *bb)
        : Instruction(ty, op, 1, bb), dest_ty_(ty) {
    set_operand(0, val);
}

ZextInst *ZextInst::create_zext(Value *val, Type *ty, BasicBlock *bb) {
    return new ZextInst(Instruction::zext, val, ty, bb);
}

std::string ZextInst::print() {
    std::string instr_ir;
    instr_ir += "%";
    instr_ir += this->get_name();
    instr_ir += " = ";
    instr_ir += this->get_module()->get_instr_op_name(this->get_instr_type());
    instr_ir += " ";
    instr_ir += this->get_operand(0)->get_type()->print();
    instr_ir += " ";
    instr_ir += print_as_op(this->get_operand(0), false);
    instr_ir += " to ";
    instr_ir += this->get_dest_type()->print();
    return instr_ir;
}

//& FpToSiInst
FpToSiInst::FpToSiInst(OpID op, Value *val, Type *ty, BasicBlock *bb)
        : Instruction(ty, op, 1, bb), dest_ty_(ty) {
    set_operand(0, val);
}

FpToSiInst *FpToSiInst::create_fptosi(Value *val, Type *ty, BasicBlock *bb) {
    return new FpToSiInst(Instruction::fptosi, val, ty, bb);
}

std::string FpToSiInst::print() {
    std::string instr_ir;
    instr_ir += "%";
    instr_ir += this->get_name();
    instr_ir += " = ";
    instr_ir += this->get_module()->get_instr_op_name(this->get_instr_type());
    instr_ir += " ";
    instr_ir += this->get_operand(0)->get_type()->print();
    instr_ir += " ";
    instr_ir += print_as_op(this->get_operand(0), false);
    instr_ir += " to ";
    instr_ir += this->get_dest_type()->print();
    return instr_ir;
}

//& SiToFpInst
SiToFpInst::SiToFpInst(OpID op, Value *val, Type *ty, BasicBlock *bb)
        : Instruction(ty, op, 1, bb), dest_ty_(ty) {
    set_operand(0, val);
}

SiToFpInst *SiToFpInst::create_sitofp(Value *val, Type *ty, BasicBlock *bb) {
    return new SiToFpInst(Instruction::sitofp, val, ty, bb);
}

std::string SiToFpInst::print() {
    std::string instr_ir;
    instr_ir += "%";
    instr_ir += this->get_name();
    instr_ir += " = ";
    instr_ir += this->get_module()->get_instr_op_name(this->get_instr_type());
    instr_ir += " ";
    instr_ir += this->get_operand(0)->get_type()->print();
    instr_ir += " ";
    instr_ir += print_as_op(this->get_operand(0), false);
    instr_ir += " to ";
    instr_ir += this->get_dest_type()->print();
    return instr_ir;
}

//& PhiInst
PhiInst::PhiInst(OpID op, std::vector<Value *> vals, std::vector<BasicBlock *> val_bbs, Type *ty, BasicBlock *bb)
        : Instruction(ty, op, 2*vals.size()) {
    for (int i = 0; i < vals.size(); i++) {
        set_operand(2 * i, vals[i]);
        set_operand(2 * i + 1, val_bbs[i]);
    }
    this->set_parent(bb);
}

PhiInst *PhiInst::create_phi(Type *ty, BasicBlock *bb) {
    std::vector<Value *> vals;
    std::vector<BasicBlock *> val_bbs;
    return new PhiInst(Instruction::phi, vals, val_bbs, ty, bb);
}

std::string PhiInst::print() {
    std::string instr_ir;
    instr_ir += "%";
    instr_ir += this->get_name();
    instr_ir += " = ";
    instr_ir += this->get_module()->get_instr_op_name(this->get_instr_type());
    instr_ir += " ";
    instr_ir += this->get_operand(0)->get_type()->print();
    instr_ir += " ";
    for (int i = 0; i < this->get_num_operand() / 2; i++) {
        if (i > 0)
            instr_ir += ", ";
        instr_ir += "[ ";
        instr_ir += print_as_op(this->get_operand(2 * i), false);
        instr_ir += ", ";
        instr_ir += print_as_op(this->get_operand(2 * i + 1), false);
        instr_ir += " ]";
    }
    if (this->get_num_operand() / 2 < this->get_parent()->get_pre_basic_blocks().size()) {
        for (auto pre_bb : this->get_parent()->get_pre_basic_blocks()) {
            if (std::find(this->get_operands().begin(), this->get_operands().end(), static_cast<Value *>(pre_bb)) ==
                this->get_operands().end()) {
                // find a pre_bb is not in phi
                instr_ir += ", [ undef, " + print_as_op(pre_bb, false) + " ]";
            }
        }
    }
    return instr_ir;
}

CmpBrInst::CmpBrInst(CmpOp op, Value *lhs, Value *rhs, BasicBlock *if_true, BasicBlock *if_false, BasicBlock *bb)
        :Instruction(Type::get_void_type(if_true->get_module()), Instruction::cmpbr, 4, bb), cmp_op_(op) {
    set_operand(0, lhs);
    set_operand(1, rhs);
    set_operand(2, if_true);
    set_operand(3, if_false);
}


CmpBrInst::CmpBrInst(CmpOp op, Value *lhs, Value *rhs,
                     BasicBlock *bb)
        : Instruction(Type::get_void_type(bb->get_module()), Instruction::cmpbr, 4, bb), cmp_op_(op) {
    set_operand(0, lhs);
    set_operand(1, rhs);
}

CmpBrInst *CmpBrInst::create_cmpbr(CmpOp op, Value *lhs, Value *rhs, BasicBlock *if_true, BasicBlock *if_false, BasicBlock *bb, Module *m) {
    if_true->add_pre_basic_block(bb);
    if_false->add_pre_basic_block(bb);
    bb->add_succ_basic_block(if_false);
    bb->add_succ_basic_block(if_true);

    return new CmpBrInst(op, lhs, rhs, if_true, if_false, bb);
}

std::string CmpBrInst::print() {
    std::string instr_ir;
    instr_ir += this->get_module()->get_instr_op_name( this->get_instr_type() );
    instr_ir += " ";
    instr_ir += print_cmp_type(this->cmp_op_);
    instr_ir += " ";
    instr_ir += this->get_operand(0)->get_type()->print();
    instr_ir += " ";
    instr_ir += print_as_op(this->get_operand(0), false);
    instr_ir += ", ";
    if (Type::is_eq_type(this->get_operand(0)->get_type(), this->get_operand(1)->get_type())) {
        instr_ir += print_as_op(this->get_operand(1), false);
    } else {
        instr_ir += print_as_op(this->get_operand(1), true);
    }

    instr_ir += ", ";
    instr_ir += print_as_op(this->get_operand(2), true);
    instr_ir += ", ";
    instr_ir += print_as_op(this->get_operand(3), true);

    return instr_ir;
}

FCmpBrInst::FCmpBrInst(CmpOp op, Value *lhs, Value *rhs, BasicBlock *if_true, BasicBlock *if_false, BasicBlock *bb)
        :Instruction(Type::get_void_type(if_true->get_module()), Instruction::fcmpbr, 4, bb), cmp_op_(op) {
    set_operand(0, lhs);
    set_operand(1, rhs);
    set_operand(2, if_true);
    set_operand(3, if_false);
}

FCmpBrInst::FCmpBrInst(CmpOp op, Value *lhs, Value *rhs,
                       BasicBlock *bb)
        : Instruction(Type::get_void_type(bb->get_module()), Instruction::fcmpbr, 4, bb), cmp_op_(op) {
    set_operand(0, lhs);
    set_operand(1, rhs);
}

FCmpBrInst *FCmpBrInst::create_fcmpbr(CmpOp op, Value *lhs, Value *rhs, BasicBlock *if_true, BasicBlock *if_false, BasicBlock *bb, Module *m) {
    if_true->add_pre_basic_block(bb);
    if_false->add_pre_basic_block(bb);
    bb->add_succ_basic_block(if_false);
    bb->add_succ_basic_block(if_true);

    return new FCmpBrInst(op, lhs, rhs, if_true, if_false, bb);
}

std::string FCmpBrInst::print() {
    std::string instr_ir;
    instr_ir += this->get_module()->get_instr_op_name( this->get_instr_type());
    instr_ir += " ";
    instr_ir += print_fcmp_type(this->cmp_op_);
    instr_ir += " ";
    instr_ir += this->get_operand(0)->get_type()->print();
    instr_ir += " ";
    instr_ir += print_as_op(this->get_operand(0), false);
    instr_ir += ", ";
    if (Type::is_eq_type(this->get_operand(0)->get_type(), this->get_operand(1)->get_type())) {
        instr_ir += print_as_op(this->get_operand(1), false);
    } else {
        instr_ir += print_as_op(this->get_operand(1), true);
    }

    instr_ir += ", ";
    instr_ir += print_as_op(this->get_operand(2), true);
    instr_ir += ", ";
    instr_ir += print_as_op(this->get_operand(3), true);

    return instr_ir;
}

LoadOffsetInst::LoadOffsetInst(Type *ty, Value *ptr, Value *offset, BasicBlock *bb)
        : Instruction(ty, Instruction::loadoffset, 2, bb) {
    set_operand(0, ptr);
    set_operand(1, offset);
}

LoadOffsetInst::LoadOffsetInst(Type *ty, Value *ptr, BasicBlock *bb)
        : Instruction(ty, Instruction::loadoffset, 2, bb) {
    set_operand(0, ptr);
}

LoadOffsetInst *LoadOffsetInst::create_loadoffset(Type *ty, Value *ptr, Value *offset, BasicBlock *bb) {
    return new LoadOffsetInst(ty, ptr, offset, bb);
}

Type *LoadOffsetInst::get_load_type() const {
    return static_cast<PointerType *>(get_operand(0)->get_type())->get_element_type();
}

std::string LoadOffsetInst::print() {
    std::string instr_ir;
    instr_ir += "%";
    instr_ir += this->get_name();
    instr_ir += " = ";
    instr_ir += this->get_module()->get_instr_op_name( this->get_instr_type() );
    instr_ir += " ";
    instr_ir += this->get_operand(0)->get_type()->get_pointer_element_type()->print();
    instr_ir += ",";
    instr_ir += " ";
    instr_ir += print_as_op(this->get_operand(0), true);
    instr_ir += ", ";
    instr_ir += print_as_op(this->get_operand(1), true);
    return instr_ir;
}

StoreOffsetInst::StoreOffsetInst(Value *val, Value *ptr, Value *offset, BasicBlock *bb)
        : Instruction(Type::get_void_type(bb->get_module()), Instruction::storeoffset, 3, bb) {
    set_operand(0, val);
    set_operand(1, ptr);
    set_operand(2, offset);
}

StoreOffsetInst::StoreOffsetInst(Value *val, Value *ptr, BasicBlock *bb)
        : Instruction(Type::get_void_type(bb->get_module()), Instruction::storeoffset, 3, bb) {
    set_operand(0, val);
    set_operand(1, ptr);
}

Type *StoreOffsetInst::get_store_type() const {
    return static_cast<PointerType *>(get_operand(1)->get_type())->get_element_type();
}

StoreOffsetInst *StoreOffsetInst::create_storeoffset(Value *val, Value *ptr, Value *offset, BasicBlock *bb) {
    return new StoreOffsetInst(val, ptr, offset, bb);
}

std::string StoreOffsetInst::print() {
    std::string instr_ir;
    instr_ir += this->get_module()->get_instr_op_name(this->get_instr_type());
    instr_ir += " ";
    instr_ir += this->get_operand(0)->get_type()->print();
    instr_ir += " ";
    instr_ir += print_as_op(this->get_operand(0), false);
    instr_ir += ", ";
    instr_ir += print_as_op(this->get_operand(1), true);
    instr_ir += ", ";
    instr_ir += print_as_op(this->get_operand(2), true);
    return instr_ir;
}
// IBinaryInst::IBinaryInst(OpID id, Value *v1, Value *v2, BasicBlock *bb)
//     : BaseInst<IBinaryInst>(bb->get_module()->get_int32_type(), id, bb) { // BaseInst会构造Instruction,将this加入bb
//     assert(v1->get_type()->is_int32_type() && v2->get_type()->is_int32_type() &&
//            "IBinaryInst operands are not both i32");
//     add_operand(v1);
//     add_operand(v2);
// }
//
// IBinaryInst *IBinaryInst::create_add(Value *v1, Value *v2, BasicBlock *bb) {
//     return create(add, v1, v2, bb);
// }
// IBinaryInst *IBinaryInst::create_sub(Value *v1, Value *v2, BasicBlock *bb) {
//     return create(sub, v1, v2, bb);
// }
// IBinaryInst *IBinaryInst::create_mul(Value *v1, Value *v2, BasicBlock *bb) {
//     return create(mul, v1, v2, bb);
// }
// IBinaryInst *IBinaryInst::create_sdiv(Value *v1, Value *v2, BasicBlock *bb) {
//     return create(sdiv, v1, v2, bb);
// }
// IBinaryInst *IBinaryInst::create_srem(Value *v1, Value *v2, BasicBlock *bb) {
//     return create(srem, v1, v2, bb);
// }
// IBinaryInst *IBinaryInst::create_and(Value *v1, Value *v2, BasicBlock *bb) {
//     return create(And, v1, v2, bb);
// }
// IBinaryInst *IBinaryInst::create_or(Value *v1, Value *v2, BasicBlock *bb) {
//     return create(Or, v1, v2, bb);
// }
//
// FBinaryInst::FBinaryInst(OpID id, Value *v1, Value *v2, BasicBlock *bb)
//     : BaseInst<FBinaryInst>(bb->get_module()->get_float_type(), id, bb) {
//     assert(v1->get_type()->is_float_type() && v2->get_type()->is_float_type() &&
//            "FBinaryInst operands are not both float");
//     add_operand(v1);
//     add_operand(v2);
// }
//
// FBinaryInst *FBinaryInst::create_fadd(Value *v1, Value *v2, BasicBlock *bb) {
//     return create(fadd, v1, v2, bb);
// }
// FBinaryInst *FBinaryInst::create_fsub(Value *v1, Value *v2, BasicBlock *bb) {
//     return create(fsub, v1, v2, bb);
// }
// FBinaryInst *FBinaryInst::create_fmul(Value *v1, Value *v2, BasicBlock *bb) {
//     return create(fmul, v1, v2, bb);
// }
// FBinaryInst *FBinaryInst::create_fdiv(Value *v1, Value *v2, BasicBlock *bb) {
//     return create(fdiv, v1, v2, bb);
// }
//
// ICmpInst::ICmpInst(OpID id, Value *lhs, Value *rhs, BasicBlock *bb)
//     : BaseInst<ICmpInst>(bb->get_module()->get_int1_type(), id, bb) {
//     assert(lhs->get_type()->is_int32_type() &&
//            rhs->get_type()->is_int32_type() &&
//            "CmpInst operands are not both i32");
//     add_operand(lhs);
//     add_operand(rhs);
// }
//
// ICmpInst *ICmpInst::create_ge(Value *v1, Value *v2, BasicBlock *bb) {
//     return create(ge, v1, v2, bb);
// }
// ICmpInst *ICmpInst::create_gt(Value *v1, Value *v2, BasicBlock *bb) {
//     return create(gt, v1, v2, bb);
// }
// ICmpInst *ICmpInst::create_le(Value *v1, Value *v2, BasicBlock *bb) {
//     return create(le, v1, v2, bb);
// }
// ICmpInst *ICmpInst::create_lt(Value *v1, Value *v2, BasicBlock *bb) {
//     return create(lt, v1, v2, bb);
// }
// ICmpInst *ICmpInst::create_eq(Value *v1, Value *v2, BasicBlock *bb) {
//     return create(eq, v1, v2, bb);
// }
// ICmpInst *ICmpInst::create_ne(Value *v1, Value *v2, BasicBlock *bb) {
//     return create(ne, v1, v2, bb);
// }
//
// FCmpInst::FCmpInst(OpID id, Value *lhs, Value *rhs, BasicBlock *bb)
//     : BaseInst<FCmpInst>(bb->get_module()->get_int1_type(), id, bb) {
//     assert(lhs->get_type()->is_float_type() &&
//            rhs->get_type()->is_float_type() &&
//            "FCmpInst operands are not both float");
//     add_operand(lhs);
//     add_operand(rhs);
// }
//
// FCmpInst *FCmpInst::create_fge(Value *v1, Value *v2, BasicBlock *bb) {
//     return create(fge, v1, v2, bb);
// }
// FCmpInst *FCmpInst::create_fgt(Value *v1, Value *v2, BasicBlock *bb) {
//     return create(fgt, v1, v2, bb);
// }
// FCmpInst *FCmpInst::create_fle(Value *v1, Value *v2, BasicBlock *bb) {
//     return create(fle, v1, v2, bb);
// }
// FCmpInst *FCmpInst::create_flt(Value *v1, Value *v2, BasicBlock *bb) {
//     return create(flt, v1, v2, bb);
// }
// FCmpInst *FCmpInst::create_feq(Value *v1, Value *v2, BasicBlock *bb) {
//     return create(feq, v1, v2, bb);
// }
// FCmpInst *FCmpInst::create_fne(Value *v1, Value *v2, BasicBlock *bb) {
//     return create(fne, v1, v2, bb);
// }
//
// CallInst::CallInst(Function *func, std::vector<Value *> args, BasicBlock *bb)
//     : BaseInst<CallInst>(func->get_return_type(), call, bb) {
//     assert(func->get_type()->is_function_type() && "Not a function");
//     assert((func->get_num_of_args() == args.size()) && "Wrong number of args");
//     add_operand(func);
//     auto func_type = static_cast<FunctionType *>(func->get_type());
//     for (unsigned i = 0; i < args.size(); i++) {
//         assert(func_type->get_param_type(i) == args[i]->get_type() &&
//                "CallInst: Wrong arg type");
//         add_operand(args[i]);
//     }
// }
//
// CallInst *CallInst::create_call(Function *func, std::vector<Value *> args,
//                                 BasicBlock *bb) {
//     return create(func, args, bb);
// }
//
// FunctionType *CallInst::get_function_type() const {
//     return static_cast<FunctionType *>(get_operand(0)->get_type());
// }
//
// BranchInst::BranchInst(Value *cond, BasicBlock *if_true, BasicBlock *if_false,
//                        BasicBlock *bb)
//     : BaseInst<BranchInst>(bb->get_module()->get_void_type(), br, bb) {
//     if (cond == nullptr) { // conditionless jump
//         assert(if_false == nullptr && "Given false-bb on conditionless jump");
//         add_operand(if_true);
//         // prev/succ
//         if_true->add_pre_basic_block(bb);
//         bb->add_succ_basic_block(if_true);
//     } else {
//         assert(cond->get_type()->is_int1_type() &&
//                "BranchInst condition is not i1");
//         add_operand(cond);
//         add_operand(if_true);
//         add_operand(if_false);
//         // prev/succ
//         if_true->add_pre_basic_block(bb);
//         if_false->add_pre_basic_block(bb);
//         bb->add_succ_basic_block(if_true);
//         bb->add_succ_basic_block(if_false);
//     }
// }
//
// BranchInst::~BranchInst() {
//     std::list<BasicBlock *> succs;
//     if (is_cond_br()) {
//         succs.push_back(static_cast<BasicBlock *>(get_operand(1)));
//         succs.push_back(static_cast<BasicBlock *>(get_operand(2)));
//     } else {
//         succs.push_back(static_cast<BasicBlock *>(get_operand(0)));
//     }
//     for (auto succ_bb : succs) {
//         if (succ_bb) {
//             succ_bb->remove_pre_basic_block(get_parent());
//             get_parent()->remove_succ_basic_block(succ_bb);
//         }
//     }
// }
//
// BranchInst *BranchInst::create_cond_br(Value *cond, BasicBlock *if_true,
//                                        BasicBlock *if_false, BasicBlock *bb) {
//     return create(cond, if_true, if_false, bb);
// }
//
// BranchInst *BranchInst::create_br(BasicBlock *if_true, BasicBlock *bb) {
//     return create(nullptr, if_true, nullptr, bb);
// }
//
// ReturnInst::ReturnInst(Value *val, BasicBlock *bb)
//     : BaseInst<ReturnInst>(bb->get_module()->get_void_type(), ret, bb) {
//     if (val == nullptr) {
//         assert(bb->get_parent()->get_return_type()->is_void_type());
//     } else {
//         assert(!bb->get_parent()->get_return_type()->is_void_type() &&
//                "Void function returning a value");
//         assert(bb->get_parent()->get_return_type() == val->get_type() &&
//                "ReturnInst type is different from function return type");
//         add_operand(val);
//     }
// }
//
// ReturnInst *ReturnInst::create_ret(Value *val, BasicBlock *bb) {
//     // 调用上面的构造函数创建
//     return create(val, bb);
// }
// ReturnInst *ReturnInst::create_void_ret(BasicBlock *bb) {
//     return create(nullptr, bb);
// }
//
// bool ReturnInst::is_void_ret() const { return get_num_operand() == 0; }
//
// GetElementPtrInst::GetElementPtrInst(Value *ptr, std::vector<Value *> idxs,
//                                      BasicBlock *bb)
//     : BaseInst<GetElementPtrInst>(PointerType::get(get_element_type(ptr, idxs)),
//                                   getelementptr, bb) {
//     add_operand(ptr);
//     for (unsigned i = 0; i < idxs.size(); i++) {
//         Value *idx = idxs[i];
//         assert(idx->get_type()->is_integer_type() && "Index is not integer");
//         add_operand(idx);
//     }
// }
//
// Type *GetElementPtrInst::get_element_type(Value *ptr,
//                                           std::vector<Value *> idxs) {
//     assert(ptr->get_type()->is_pointer_type() &&
//            "GetElementPtrInst ptr is not a pointer");
//
//     Type *ty = ptr->get_type()->get_pointer_element_type();
//     assert(
//         "GetElementPtrInst ptr is wrong type" &&
//         (ty->is_array_type() || ty->is_integer_type() || ty->is_float_type()));
//     if (ty->is_array_type()) {
//         ArrayType *arr_ty = static_cast<ArrayType *>(ty);
//         for (unsigned i = 1; i < idxs.size(); i++) {
//             ty = arr_ty->get_element_type();
//             if (i < idxs.size() - 1) {
//                 assert(ty->is_array_type() && "Index error!");
//             }
//             if (ty->is_array_type()) {
//                 arr_ty = static_cast<ArrayType *>(ty);
//             }
//         }
//     }
//     return ty;
// }
//
// Type *GetElementPtrInst::get_element_type() const {
//     return get_type()->get_pointer_element_type();
// }
//
// GetElementPtrInst *GetElementPtrInst::create_gep(Value *ptr,
//                                                  std::vector<Value *> idxs,
//                                                  BasicBlock *bb) {
//     return create(ptr, idxs, bb);
// }
//
// StoreInst::StoreInst(Value *val, Value *ptr, BasicBlock *bb)
//     : BaseInst<StoreInst>(bb->get_module()->get_void_type(), store, bb) {
//     assert((ptr->get_type()->get_pointer_element_type() == val->get_type()) &&
//            "StoreInst ptr is not a pointer to val type");
//     add_operand(val);
//     add_operand(ptr);
// }
//
// StoreInst *StoreInst::create_store(Value *val, Value *ptr, BasicBlock *bb) {
//     return create(val, ptr, bb);
// }
//
// LoadInst::LoadInst(Value *ptr, BasicBlock *bb)
//     : BaseInst<LoadInst>(ptr->get_type()->get_pointer_element_type(), load,
//                          bb) {
//     assert((get_type()->is_integer_type() or get_type()->is_float_type() or
//             get_type()->is_pointer_type()) &&
//            "Should not load value with type except int/float");
//     add_operand(ptr);
// }
//
// LoadInst *LoadInst::create_load(Value *ptr, BasicBlock *bb) {
//     return create(ptr, bb);
// }
//
// AllocaInst::AllocaInst(Type *ty, BasicBlock *bb)
//     : BaseInst<AllocaInst>(PointerType::get(ty), alloca, bb) {
//     static const std::array allowed_alloc_type = {
//         Type::IntegerTyID, Type::FloatTyID, Type::ArrayTyID, Type::PointerTyID};
//     assert(std::find(allowed_alloc_type.begin(), allowed_alloc_type.end(),
//                      ty->get_type_id()) != allowed_alloc_type.end() &&
//            "Not allowed type for alloca");
// }
//
// AllocaInst *AllocaInst::create_alloca(Type *ty, BasicBlock *bb) {
//     return create(ty, bb);
// }
//
// ZextInst::ZextInst(Value *val, Type *ty, BasicBlock *bb)
//     : BaseInst<ZextInst>(ty, zext, bb) {
//     assert(val->get_type()->is_integer_type() &&
//            "ZextInst operand is not integer");
//     assert(ty->is_integer_type() && "ZextInst destination type is not integer");
//     assert((static_cast<IntegerType *>(val->get_type())->get_num_bits() <
//             static_cast<IntegerType *>(ty)->get_num_bits()) &&
//            "ZextInst operand bit size is not smaller than destination type bit "
//            "size");
//     add_operand(val);
// }
//
// ZextInst *ZextInst::create_zext(Value *val, Type *ty, BasicBlock *bb) {
//     return create(val, ty, bb);
// }
// ZextInst *ZextInst::create_zext_to_i32(Value *val, BasicBlock *bb) {
//     return create(val, bb->get_module()->get_int32_type(), bb);
// }
//
// FpToSiInst::FpToSiInst(Value *val, Type *ty, BasicBlock *bb)
//     : BaseInst<FpToSiInst>(ty, fptosi, bb) {
//     assert(val->get_type()->is_float_type() &&
//            "FpToSiInst operand is not float");
//     assert(ty->is_integer_type() &&
//            "FpToSiInst destination type is not integer");
//     add_operand(val);
// }
//
// FpToSiInst *FpToSiInst::create_fptosi(Value *val, Type *ty, BasicBlock *bb) {
//     return create(val, ty, bb);
// }
// FpToSiInst *FpToSiInst::create_fptosi_to_i32(Value *val, BasicBlock *bb) {
//     return create(val, bb->get_module()->get_int32_type(), bb);
// }
//
// SiToFpInst::SiToFpInst(Value *val, Type *ty, BasicBlock *bb)
//     : BaseInst<SiToFpInst>(ty, sitofp, bb) {
//     assert(val->get_type()->is_integer_type() &&
//            "SiToFpInst operand is not integer");
//     assert(ty->is_float_type() && "SiToFpInst destination type is not float");
//     add_operand(val);
// }
//
// SiToFpInst *SiToFpInst::create_sitofp(Value *val, BasicBlock *bb) {
//     return create(val, bb->get_module()->get_float_type(), bb);
// }
//
// PhiInst::PhiInst(Type *ty, std::vector<Value *> vals,
//                  std::vector<BasicBlock *> val_bbs, BasicBlock *bb)
//     : BaseInst<PhiInst>(ty, phi) {
//     assert(vals.size() == val_bbs.size() && "Unmatched vals and bbs");
//     for (unsigned i = 0; i < vals.size(); i++) {
//         assert(ty == vals[i]->get_type() && "Bad type for phi");
//         add_operand(vals[i]);
//         add_operand(val_bbs[i]);
//     }
//     this->set_parent(bb);
// }
//
// PhiInst *PhiInst::create_phi(Type *ty, BasicBlock *bb,
//                              std::vector<Value *> vals,
//                              std::vector<BasicBlock *> val_bbs) {
//     return create(ty, vals, val_bbs, bb);
// }
