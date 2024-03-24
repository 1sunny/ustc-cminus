#pragma once

#include "User.hpp"
#include "logging.hpp"
#include <memory>
#include <string>
#include <utility>
#include <vector>

enum BType {
    TYPE_INT, TYPE_FLOAT, TYPE_VOID
};

extern const char* BType_string[];

enum RelOp {
    // <=
    OP_LTE,
    // <
    OP_LT,
    // >
    OP_GT,
    // >=
    OP_GTE,
};

extern const char* RelOp_string[];

enum EqOp {
    // ==
    OP_EQ,
    // !=
    OP_NEQ
};

extern const char* EqOp_string[];

enum AddOp {
    // +
    OP_PLUS,
    // -
    OP_MINUS
};

extern const char* AddOp_string[];

enum MulOp {
    // *
    OP_MUL,
    // /
    OP_DIV,
    // %
    OP_MOD
};

extern const char* MulOp_string[];

enum UnaryOp {
    // +
    OP_POS,
    // -
    OP_NEG,
    // !
    OP_NOT
};

extern const char* UnaryOp_string[];

enum LogicOp {
    // &&
    OP_AND,
    // ||
    OP_OR
};

extern const char* LogicOp_string[];

struct AstTreeNode;
struct AstCompUnit;
struct AstDeclDef;
struct AstConstDecl;
struct AstConstDefList;
struct AstConstDef;
struct AstArrayConstExpList;
struct AstConstInitVal;
struct AstConstInitValList;
struct AstVarDecl;
struct AstVarDefList;
struct AstVarDef;
struct AstInitVal;
struct AstInitValList;
struct AstFuncDef;
struct AstFuncFParamList;
struct AstFuncFParam;
struct AstParamArrayExpList;
struct AstBlock;
struct AstBlockItemList;
struct AstBlockItem;
struct AstStmt;
struct AstAssignStmt;
struct AstSelectStmt;
struct AstBreakStmt;
struct AstContinueStmt;
struct AstIterationStmt;
struct AstReturnStmt;
struct AstExp;
struct AstCond;
struct AstLVal;
struct AstArrayExpList;
struct AstPrimaryExp;
struct AstInteger;
struct AstFloat;
struct AstUnaryExp;
struct AstCallee;
struct AstExpList;
struct AstMulExp;
struct AstAddExp;
struct AstRelExp;
struct AstEqExp;
struct AstLAndExp;
struct AstLOrExp;
struct AstConstExp;

class syntax_tree_visitor;

class syntax_tree {
public:
    syntax_tree(std::shared_ptr<AstCompUnit> start_node) {
      // Parameter 'start_node' is passed by value and only copied once; consider moving it to avoid unnecessary copies
      // root = start_node;
      root = std::move(start_node);
    };

    syntax_tree(syntax_tree &&tree) {
      root = tree.root;
      tree.root = nullptr;
    };

    AstCompUnit *get_root() { return root.get(); }

    void run_visitor(syntax_tree_visitor &visitor);

private:
    std::shared_ptr<AstCompUnit> root = nullptr;
};

struct AstTreeNode {
    // 这个应该可以帮子类中有shared_ptr的析构
    virtual Value *accept(syntax_tree_visitor &) = 0;
};

struct AstCompUnit : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    std::vector<std::shared_ptr<AstDeclDef>> DeclDefList;
};

struct AstDeclDef : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    std::shared_ptr<AstConstDecl> ConstDecl;
    std::shared_ptr<AstVarDecl> VarDecl;
    std::shared_ptr<AstFuncDef> FuncDef;
};

struct AstConstDecl : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    std::vector<std::shared_ptr<AstConstDef>> ConstDefList;
    BType type;
};

struct AstConstDefList {
    std::vector<std::shared_ptr<AstConstDef>> list;
};

// struct AstBType;
struct AstConstDef : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    std::vector<std::shared_ptr<AstConstExp>> ArrayConstExpList;
    std::shared_ptr<AstConstInitVal> ConstInitVal;
    std::string id;
};

struct AstArrayConstExpList {
    std::vector<std::shared_ptr<AstConstExp>> list;
};

struct AstConstInitVal : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    std::shared_ptr<AstConstExp> ConstExp;
    std::vector<std::shared_ptr<AstConstInitVal>> ConstInitValList;
    std::vector<int> bounds;
};

struct AstConstInitValList {
    std::vector<std::shared_ptr<AstConstInitVal>> list;
};

struct AstVarDecl : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    std::vector<std::shared_ptr<AstVarDef>> VarDefList;
    BType type;
};

struct AstVarDefList {
    std::vector<std::shared_ptr<AstVarDef>> list;
};

struct AstVarDef : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    std::string id;
    std::vector<std::shared_ptr<AstConstExp>> ArrayConstExpList;
    std::shared_ptr<AstInitVal> InitVal;
};

struct AstInitVal : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    std::vector<std::shared_ptr<AstInitVal>> InitValList;
    std::shared_ptr<AstExp> Exp;
    std::vector<int> bounds;
};

struct AstInitValList {
    std::vector<std::shared_ptr<AstInitVal>> list;
};

struct AstFuncDef : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    BType type;
    std::string id;
    std::vector<std::shared_ptr<AstFuncFParam>> FuncFParamList;
    std::shared_ptr<AstBlock> Block;
};

// struct AstFuncType;
struct AstFuncFParamList {
    std::vector<std::shared_ptr<AstFuncFParam>> list;
};

struct AstFuncFParam : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    BType type;
    std::string id;
    bool isarray;
    std::vector<std::shared_ptr<AstExp>> ParamArrayExpList;
};

struct AstParamArrayExpList {
    std::vector<std::shared_ptr<AstExp>> list;
};

struct AstBlock : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    std::vector<std::shared_ptr<AstBlockItem>> BlockItemList;
};

struct AstBlockItemList {
    std::vector<std::shared_ptr<AstBlockItem>> list;
};

struct AstBlockItem : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    std::shared_ptr<AstConstDecl> ConstDecl;
    std::shared_ptr<AstVarDecl> VarDecl;
    std::shared_ptr<AstStmt> Stmt;
};

struct AstStmt : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    std::shared_ptr<AstBreakStmt> BreakStmt;
    std::shared_ptr<AstContinueStmt> ContinueStmt;
    std::shared_ptr<AstAssignStmt> AssignStmt;
    std::shared_ptr<AstExp> Exp;
    std::shared_ptr<AstBlock> Block;
    std::shared_ptr<AstSelectStmt> SelectStmt;
    std::shared_ptr<AstIterationStmt> IterationStmt;
    std::shared_ptr<AstReturnStmt> ReturnStmt;
};

struct AstBreakStmt : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;
};

struct AstContinueStmt : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;
};

struct AstAssignStmt : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    std::shared_ptr<AstLVal> LVal;
    std::shared_ptr<AstExp> Exp;
};

struct AstSelectStmt : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    std::shared_ptr<AstCond> Cond;
    std::shared_ptr<AstStmt> ifStmt;
    std::shared_ptr<AstStmt> elseStmt;
};

struct AstIterationStmt : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    std::shared_ptr<AstCond> Cond;
    std::shared_ptr<AstStmt> Stmt;
};

struct AstReturnStmt : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    std::shared_ptr<AstExp> Exp;
};

struct AstExp : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    std::shared_ptr<AstAddExp> AddExp;
};

struct AstCond : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    std::shared_ptr<AstLOrExp> LOrExp;
};

struct AstLVal : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    std::vector<std::shared_ptr<AstExp>> ArrayExpList;
    std::string id;
};

struct AstArrayExpList {
    std::vector<std::shared_ptr<AstExp>> list;
};

struct AstPrimaryExp : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    std::shared_ptr<AstExp> Exp;
    std::shared_ptr<AstLVal> LVal;
    std::shared_ptr<AstInteger> Integer;
    std::shared_ptr<AstFloat> Float;
};

struct AstInteger : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    int32_t _int;
};

struct AstFloat : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    float _float;
};

struct AstUnaryExp : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    UnaryOp op;
    std::shared_ptr<AstPrimaryExp> PrimaryExp;
    std::shared_ptr<AstCallee> Callee;
    std::shared_ptr<AstUnaryExp> UnaryExp;
};

struct AstCallee : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    std::string id;
    std::vector<std::shared_ptr<AstExp>> ExpList;
};

// struct AstUnaryOp:AstTreeNode{};
struct AstExpList {
    std::vector<std::shared_ptr<AstExp>> list;
};

struct AstMulExp : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    MulOp op;
    std::shared_ptr<AstMulExp> MulExp;
    std::shared_ptr<AstUnaryExp> UnaryExp;
};

struct AstAddExp : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    AddOp op;
    std::shared_ptr<AstAddExp> AddExp;
    std::shared_ptr<AstMulExp> MulExp;
};

struct AstRelExp : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    RelOp op;
    std::shared_ptr<AstRelExp> RelExp;
    std::shared_ptr<AstAddExp> AddExp;
};

struct AstEqExp : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    EqOp op;
    std::shared_ptr<AstEqExp> EqExp;
    std::shared_ptr<AstRelExp> RelExp;
};

struct AstLAndExp : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    LogicOp op;
    std::shared_ptr<AstLAndExp> LAndExp;
    std::shared_ptr<AstEqExp> EqExp;
};

struct AstLOrExp : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    LogicOp op;
    std::shared_ptr<AstLOrExp> LOrExp;
    std::shared_ptr<AstLAndExp> LAndExp;
};

struct AstConstExp : AstTreeNode {
    virtual Value *accept(syntax_tree_visitor &) final;

    std::shared_ptr<AstAddExp> AddExp;
};

class syntax_tree_visitor {
public:
    virtual Value *visit(AstCond &) = 0;

    virtual Value *visit(AstExp &) = 0;

    virtual Value *visit(AstConstExp &) = 0;

    virtual Value *visit(AstCompUnit &) = 0;

    virtual Value *visit(AstConstDecl &) = 0;

    virtual Value *visit(AstConstDef &) = 0;

    virtual Value *visit(AstConstInitVal &) = 0;

    virtual Value *visit(AstVarDecl &) = 0;

    virtual Value *visit(AstVarDef &) = 0;

    virtual Value *visit(AstInitVal &) = 0;

    virtual Value *visit(AstFuncDef &) = 0;

    virtual Value *visit(AstFuncFParam &) = 0;

    virtual Value *visit(AstBlock &) = 0;

    virtual Value *visit(AstBreakStmt &) = 0;

    virtual Value *visit(AstContinueStmt &) = 0;

    virtual Value *visit(AstAssignStmt &) = 0;

    virtual Value *visit(AstSelectStmt &) = 0;

    virtual Value *visit(AstIterationStmt &) = 0;

    virtual Value *visit(AstReturnStmt &) = 0;

    virtual Value *visit(AstLVal &) = 0;

    virtual Value *visit(AstPrimaryExp &) = 0;

    virtual Value *visit(AstInteger &) = 0;

    virtual Value *visit(AstFloat &) = 0;

    virtual Value *visit(AstUnaryExp &) = 0;

    virtual Value *visit(AstCallee &) = 0;

    virtual Value *visit(AstMulExp &) = 0;

    virtual Value *visit(AstAddExp &) = 0;

    virtual Value *visit(AstRelExp &) = 0;

    virtual Value *visit(AstEqExp &) = 0;

    virtual Value *visit(AstLAndExp &) = 0;

    virtual Value *visit(AstLOrExp &) = 0;
};

class syntax_tree_printer : public syntax_tree_visitor {
public:
    Value *visit(AstCompUnit &) final;

    Value *visit(AstConstDecl &) final;

    Value *visit(AstConstDef &) final;

    Value *visit(AstConstInitVal &) final;

    Value *visit(AstVarDecl &) final;

    Value *visit(AstVarDef &) final;

    Value *visit(AstInitVal &) final;

    Value *visit(AstFuncDef &) final;

    Value *visit(AstFuncFParam &) final;

    Value *visit(AstBlock &) final;

    Value *visit(AstBreakStmt &) final;

    Value *visit(AstContinueStmt &) final;

    Value *visit(AstAssignStmt &) final;

    Value *visit(AstSelectStmt &) final;

    Value *visit(AstIterationStmt &) final;

    Value *visit(AstReturnStmt &) final;

    Value *visit(AstLVal &) final;

    Value *visit(AstPrimaryExp &) final;

    Value *visit(AstInteger &) final;

    Value *visit(AstFloat &) final;

    Value *visit(AstUnaryExp &) final;

    Value *visit(AstCallee &) final;

    Value *visit(AstMulExp &) final;

    Value *visit(AstAddExp &) final;

    Value *visit(AstRelExp &) final;

    Value *visit(AstEqExp &) final;

    Value *visit(AstLAndExp &) final;

    Value *visit(AstLOrExp &) final;

    Value *visit(AstConstExp &) final;

    Value *visit(AstExp &) final;

    Value *visit(AstCond &) final;

    void add_depth() { depth += 4; }

    void remove_depth() {
      depth -= 4;
    }

private:
    int depth = 0;
};
