%require "3.2"
%language "c++"
// The traditional interface of yylex is not type safe: since the token kind and the token value are not correlated, you may return a NUMBER with a string as semantic value. To avoid this, we use token constructors
%define api.token.constructor
// To use genuine C++ objects as semantic values—not just PODs—we cannot rely on the union that Bison uses by default to store them, we need variants
%define api.value.type variant
%define parse.assert

// Then come the declarations/inclusions needed by the semantic values. Because the parser uses the parsing driver and reciprocally, both would like to include the header of the other, which is, of course, insane. This mutual dependency will be broken using forward declarations. Because the driver’s header needs detailed knowledge about the parser class (in particular its inner types), it is the parser’s header which will use a forward declaration of the driver. See %code Summary.
// 输出到头文件中
%code requires
{
#include "ast.hpp"
class driver;
}

// The driver is passed by reference to the parser and to the scanner. This provides a simple but effective pure interface, not relying on global variables.
// The parsing context.
%param { driver& drv }

%header
// we request location tracking.
%locations

// Use the following two directives to enable parser tracing and detailed error messages. However, detailed error messages can contain incorrect information if lookahead correction is not enabled (see LAC).
%define parse.trace
%define parse.error detailed
%define parse.lac full

// The code between ‘%code {’ and ‘}’ is output in the *.cc file; it needs detailed knowledge about the driver.
// 输出到源文件中
%code {
# include "driver.hpp"
// 调用driver中的lexer
#define yylex drv.lexer.yylex
}

%token END
%token ERROR 258 ADD 259 SUB 260 MUL 261 DIV 262 LT 263 LTE 264
%token GT 265 GTE 266 EQ 267 NEQ 268 ASSIN 269 SEMICOLON 270
%token COMMA 271 LPARENTHESE 272 RPARENTHESE 273 LBRACKET 274
%token RBRACKET 275 LBRACE 276 RBRACE 277 ELSE 278 IF 279
%token INT 280 FLOAT 281 RETURN 282 VOID 283 WHILE 284
%token <std::string>IDENTIFIER 285
%token <int>INTEGER_CONST 286 // TODO float
%token <float>FLOAT_CONST 287
%token ARRAY 288 LETTER 289 EOL 290 COMMENT 291
%token BLANK 292 CONST 293 BREAK 294 CONTINUE 295 NOT 296
%token AND 297 OR 298 MOD 299

%type <AstCompUnit*>CompUnit
%type <AstDeclDef*>DeclDef
%type <AstConstDecl*>ConstDecl
%type <AstConstDefList*>ConstDefList
%type <AstConstDef*>ConstDef
%type <AstArrayConstExpList*>ArrayConstExpList
%type <AstConstInitVal*>ConstInitVal
%type <AstConstInitValList*>ConstInitValList
%type <AstVarDecl*>VarDecl
%type <AstVarDefList*>VarDefList
%type <AstVarDef*>VarDef
%type <AstInitVal*>InitVal
%type <AstInitValList*>InitValList
%type <AstFuncDef*>FuncDef
%type <BType>BType
%type <AstFuncFParamList*>FuncFParamList
%type <AstFuncFParam*>FuncFParam
%type <AstParamArrayExpList*>ParamArrayExpList
%type <AstBlock*>Block
%type <AstBlockItemList*>BlockItemList
%type <AstBlockItem*>BlockItem
%type <AstStmt*>Stmt
%type <AstAssignStmt*>AssignStmt
%type <AstSelectStmt*>SelectStmt
%type <AstBreakStmt*>BreakStmt
%type <AstContinueStmt*>ContinueStmt
%type <AstIterationStmt*>IterationStmt
%type <AstReturnStmt*>ReturnStmt
%type <AstExp*>Exp
%type <AstCond*>Cond
%type <AstLVal*>LVal
%type <AstArrayExpList*>ArrayExpList
%type <AstPrimaryExp*>PrimaryExp
%type <AstInteger*>Integer
%type <AstFloat*>Float
%type <AstUnaryExp*>UnaryExp
%type <AstCallee*>Callee
%type <UnaryOp>UnaryOp
%type <AstExpList*>ExpList
%type <AstMulExp*>MulExp
%type <AstAddExp*>AddExp
%type <AstRelExp*>RelExp
%type <AstEqExp*>EqExp
%type <AstLAndExp*>LAndExp
%type <AstLOrExp*>LOrExp
%type <AstConstExp*>ConstExp

// Grammar:
%start Begin

%%
Begin:
    CompUnit END {
      drv.compUnitResult = std::shared_ptr<AstCompUnit>($1);
      auto printer = new syntax_tree_printer;
      //printer->visit(*$1);
      return 0;
    }

CompUnit:
    CompUnit DeclDef {
        $1->DeclDefList.push_back(std::shared_ptr<AstDeclDef>($2));
        $$ = $1;
    }
    | DeclDef {
        $$ = new AstCompUnit();
        $$->DeclDefList.push_back(std::shared_ptr<AstDeclDef>($1));
    };

DeclDef:
    ConstDecl {
        $$ = new AstDeclDef();
        $$->ConstDecl = std::shared_ptr<AstConstDecl>($1);
        $$->VarDecl = nullptr;
        $$->FuncDef = nullptr;
    }
    | VarDecl {
        $$ = new AstDeclDef();
        $$->ConstDecl = nullptr;
        $$->VarDecl = std::shared_ptr<AstVarDecl>($1);
        $$->FuncDef = nullptr;
    }
    | FuncDef {
        $$ = new AstDeclDef();
        $$->ConstDecl = nullptr;
        $$->VarDecl = nullptr;
        $$->FuncDef = std::shared_ptr<AstFuncDef>($1);
    };

ConstDecl:
    CONST BType ConstDefList SEMICOLON {
        $$ = new AstConstDecl();
        $$->ConstDefList.swap($3->list);
        $$->type = $2;
    };

ConstDefList:
    ConstDefList COMMA ConstDef {
        $1->list.push_back(std::shared_ptr<AstConstDef>($3));
        $$ = $1;
    }
    | ConstDef {
        $$ = new AstConstDefList();
        $$->list.push_back(std::shared_ptr<AstConstDef>($1));
    };

ConstDef:
    IDENTIFIER ArrayConstExpList ASSIN ConstInitVal {
        $$ = new AstConstDef();
        $$->ArrayConstExpList.swap($2->list);
        $$->id = $1;
        $$->ConstInitVal = std::shared_ptr<AstConstInitVal>($4);
    };

ArrayConstExpList:
    ArrayConstExpList LBRACKET ConstExp RBRACKET {
        $1->list.push_back(std::shared_ptr<AstConstExp>($3));
        $$ = $1;
    }
    | {
        $$ = new AstArrayConstExpList();
    };

ConstInitVal:
    ConstExp {
        $$ = new AstConstInitVal();
        $$->ConstExp = std::shared_ptr<AstConstExp>($1);
    }
    | LBRACE ConstInitValList RBRACE {
        $$ = new AstConstInitVal();
        $$->ConstExp = nullptr;
        $$->ConstInitValList.swap($2->list);
    }
    | LBRACE RBRACE {
        $$ = new AstConstInitVal();
        $$->ConstExp = nullptr;
    };

ConstInitValList:
    ConstInitValList COMMA ConstInitVal {
        $1->list.push_back(std::shared_ptr<AstConstInitVal>($3));
        $$ = $1;
    }
    | ConstInitVal {
        $$ = new AstConstInitValList();
        $$->list.push_back(std::shared_ptr<AstConstInitVal>($1));
    };

VarDecl:
    BType VarDefList SEMICOLON {
        $$ = new AstVarDecl();
        $$->VarDefList.swap($2->list);
        $$->type = $1;
    };

VarDefList:
    VarDefList COMMA VarDef {
        $1->list.push_back(std::shared_ptr<AstVarDef>($3));
        $$ = $1;
    }
    | VarDef {
        $$ = new AstVarDefList();
        $$->list.push_back(std::shared_ptr<AstVarDef>($1));
    };

VarDef:
    IDENTIFIER ArrayConstExpList {
        $$ = new AstVarDef();
        $$->id = $1;
        $$->ArrayConstExpList.swap($2->list);
        $$->InitVal = nullptr;
    }
    | IDENTIFIER ArrayConstExpList ASSIN InitVal {
        $$ = new AstVarDef();
        $$->id = $1;
        $$->ArrayConstExpList.swap($2->list);
        $$->InitVal = std::shared_ptr<AstInitVal>($4);
    };

InitVal:
    Exp {
        $$ = new AstInitVal();
        $$->Exp = std::shared_ptr<AstExp>($1);
    }
    | LBRACE RBRACE {
        $$ = new AstInitVal();
        $$->Exp = nullptr;
    }
    | LBRACE InitValList RBRACE {
        $$ = new AstInitVal();
        $$->Exp = nullptr;
        $$->InitValList.swap($2->list);
    };

InitValList:
    InitValList COMMA InitVal {
        $1->list.push_back(std::shared_ptr<AstInitVal>($3));
        $$ = $1;
    }
    | InitVal {
        $$ = new AstInitValList();
        $$->list.push_back(std::shared_ptr<AstInitVal>($1));
    };

FuncDef:
    BType IDENTIFIER LPARENTHESE RPARENTHESE Block {
        $$ = new AstFuncDef();
        $$->type = $1;
        $$->id = $2;
        $$->Block = std::shared_ptr<AstBlock>($5);
    }
    | BType IDENTIFIER LPARENTHESE VOID RPARENTHESE Block {
        $$ = new AstFuncDef();
        $$->type = $1;
        $$->id = $2;
        $$->Block = std::shared_ptr<AstBlock>($6);
    }
    | BType IDENTIFIER LPARENTHESE FuncFParamList RPARENTHESE Block {
        $$ = new AstFuncDef();
        $$->type = $1;
        $$->id = $2;
        $$->FuncFParamList.swap($4->list);
        $$->Block = std::shared_ptr<AstBlock>($6);
    };

BType:
    VOID {
        $$ = TYPE_VOID;
    }
    | INT {
        $$ = TYPE_INT;
    }
    | FLOAT {
        $$ = TYPE_FLOAT;
    };

FuncFParamList:
    FuncFParamList COMMA FuncFParam {
        $1->list.push_back(std::shared_ptr<AstFuncFParam>($3));
        $$ = $1;
    }
    | FuncFParam{
        $$ = new AstFuncFParamList();
        $$->list.push_back(std::shared_ptr<AstFuncFParam>($1));
    };

FuncFParam:
    BType IDENTIFIER ParamArrayExpList {
        $$ = new AstFuncFParam();
        $$->type = $1;
        $$->isarray = 1;
        $$->id = $2;
        $$->ParamArrayExpList.swap($3->list);
    }
    | BType IDENTIFIER {
        $$ = new AstFuncFParam();
        $$->type = $1;
        $$->id = $2;
        $$->isarray = 0;
    };

ParamArrayExpList:
    ParamArrayExpList LBRACKET Exp RBRACKET {
        $1->list.push_back(std::shared_ptr<AstExp>($3));
        $$ = $1;
    }
    | ARRAY {
        $$ = new AstParamArrayExpList();
    };

Block:
    LBRACE BlockItemList RBRACE {
        $$ = new AstBlock();
        $$->BlockItemList.swap($2->list);
    };

BlockItemList:
    BlockItemList BlockItem {
        $1->list.push_back(std::shared_ptr<AstBlockItem>($2));
        $$ = $1;
    }
    | {
        $$ = new AstBlockItemList();
    };

BlockItem:
    ConstDecl {
        $$ = new AstBlockItem();
        $$->ConstDecl = std::shared_ptr<AstConstDecl>($1);
        $$->VarDecl = nullptr;
        $$->Stmt = nullptr;
    }
    | VarDecl {
        $$ = new AstBlockItem();
        $$->ConstDecl = nullptr;
        $$->VarDecl = std::shared_ptr<AstVarDecl>($1);
        $$->Stmt = nullptr;
    }
    | Stmt {
        $$ = new AstBlockItem();
        $$->ConstDecl = nullptr;
        $$->VarDecl = nullptr;
        $$->Stmt = std::shared_ptr<AstStmt>($1);
    };

Stmt:
    AssignStmt {
        $$ = new AstStmt();
        $$->BreakStmt = nullptr;
        $$->ContinueStmt = nullptr;
        $$->AssignStmt = std::shared_ptr<AstAssignStmt>($1);
        $$->Exp = nullptr;
        $$->Block = nullptr;
        $$->SelectStmt = nullptr;
        $$->IterationStmt = nullptr;
        $$->ReturnStmt = nullptr;
    }
    | Exp SEMICOLON {
        $$ = new AstStmt();
        $$->BreakStmt = nullptr;
        $$->ContinueStmt = nullptr;
        $$->AssignStmt = nullptr;
        $$->Exp = std::shared_ptr<AstExp>($1);
        $$->Block = nullptr;
        $$->SelectStmt = nullptr;
        $$->IterationStmt = nullptr;
        $$->ReturnStmt = nullptr;
    }
    | SEMICOLON {
        $$ = new AstStmt();
        $$->BreakStmt = nullptr;
        $$->ContinueStmt = nullptr;
        $$->AssignStmt = nullptr;
        $$->Exp = nullptr;
        $$->Block = nullptr;
        $$->SelectStmt = nullptr;
        $$->IterationStmt = nullptr;
        $$->ReturnStmt = nullptr;
    }
    | Block {
        $$ = new AstStmt();
        $$->BreakStmt = nullptr;
        $$->ContinueStmt = nullptr;
        $$->AssignStmt = nullptr;
        $$->Exp = nullptr;
        $$->Block = std::shared_ptr<AstBlock>($1);
        $$->SelectStmt = nullptr;
        $$->IterationStmt = nullptr;
        $$->ReturnStmt = nullptr;
    }
    | SelectStmt {
        $$ = new AstStmt();
        $$->BreakStmt = nullptr;
        $$->ContinueStmt = nullptr;
        $$->AssignStmt = nullptr;
        $$->Exp = nullptr;
        $$->Block = nullptr;
        $$->SelectStmt = std::shared_ptr<AstSelectStmt>($1);
        $$->IterationStmt = nullptr;
        $$->ReturnStmt = nullptr;
    }
    | IterationStmt {
        $$ = new AstStmt();
        $$->BreakStmt = nullptr;
        $$->ContinueStmt = nullptr;
        $$->AssignStmt = nullptr;
        $$->Exp = nullptr;
        $$->Block = nullptr;
        $$->SelectStmt = nullptr;
        $$->IterationStmt = std::shared_ptr<AstIterationStmt>($1);
        $$->ReturnStmt = nullptr;
    }
    | BreakStmt {
        $$ = new AstStmt();
        $$->BreakStmt = std::shared_ptr<AstBreakStmt>($1);
        $$->ContinueStmt = nullptr;
        $$->AssignStmt = nullptr;
        $$->Exp = nullptr;
        $$->Block = nullptr;
        $$->SelectStmt = nullptr;
        $$->IterationStmt = nullptr;
        $$->ReturnStmt = nullptr;
    }
    | ContinueStmt {
        $$ = new AstStmt();
        $$->BreakStmt = nullptr;
        $$->ContinueStmt = std::shared_ptr<AstContinueStmt>($1);
        $$->AssignStmt = nullptr;
        $$->Exp = nullptr;
        $$->Block = nullptr;
        $$->SelectStmt = nullptr;
        $$->IterationStmt = nullptr;
        $$->ReturnStmt = nullptr;
    }
    | ReturnStmt {
        $$ = new AstStmt();
        $$->BreakStmt = nullptr;
        $$->ContinueStmt = nullptr;
        $$->AssignStmt = nullptr;
        $$->Exp = nullptr;
        $$->Block = nullptr;
        $$->SelectStmt = nullptr;
        $$->IterationStmt = nullptr;
        $$->ReturnStmt = std::shared_ptr<AstReturnStmt>($1);
    };

BreakStmt:
    BREAK SEMICOLON {
        $$ = new AstBreakStmt();
    };

ContinueStmt:
    CONTINUE SEMICOLON {
        $$ = new AstContinueStmt();
    };

AssignStmt:
    LVal ASSIN Exp SEMICOLON {
        $$ = new AstAssignStmt();
        $$->LVal = std::shared_ptr<AstLVal>($1);
        $$->Exp = std::shared_ptr<AstExp>($3);
    };

SelectStmt:
    IF LPARENTHESE Cond RPARENTHESE Stmt {
        $$ = new AstSelectStmt();
        $$->Cond = std::shared_ptr<AstCond>($3);
        $$->ifStmt = std::shared_ptr<AstStmt>($5);
    }
    | IF LPARENTHESE Cond RPARENTHESE Stmt ELSE Stmt {
        $$ = new AstSelectStmt();
        $$->Cond = std::shared_ptr<AstCond>($3);
        $$->ifStmt = std::shared_ptr<AstStmt>($5);
        $$->elseStmt = std::shared_ptr<AstStmt>($7);
    };

IterationStmt:
    WHILE LPARENTHESE Cond RPARENTHESE Stmt {
        $$ = new AstIterationStmt();
        $$->Cond = std::shared_ptr<AstCond>($3);
        $$->Stmt = std::shared_ptr<AstStmt>($5);
    };

ReturnStmt:
    RETURN Exp SEMICOLON {
        $$ = new AstReturnStmt();
        $$->Exp = std::shared_ptr<AstExp>($2);
    }
    | RETURN SEMICOLON {
        $$ = new AstReturnStmt();
        $$->Exp = nullptr;
    };

Exp:
    AddExp {
        $$ = new AstExp();
        $$->AddExp = std::shared_ptr<AstAddExp>($1);
    };

Cond:
    LOrExp{
        $$ = new AstCond();
        $$->LOrExp = std::shared_ptr<AstLOrExp>($1);
    };

LVal:
    IDENTIFIER ArrayExpList {
        $$ = new AstLVal();
        $$->id = $1;
        $$->ArrayExpList.swap($2->list);
    };

ArrayExpList:
    ArrayExpList LBRACKET Exp RBRACKET {
        $1->list.push_back(std::shared_ptr<AstExp>($3));
        $$ = $1;
    }
    | {
        $$=new AstArrayExpList();
    };

PrimaryExp:
    LPARENTHESE Exp RPARENTHESE {
        $$ = new AstPrimaryExp();
        $$->Exp = std::shared_ptr<AstExp>($2);
        $$->LVal = nullptr;
        $$->Integer = nullptr;
        $$->Float = nullptr;
    }
    | LVal {
        $$ = new AstPrimaryExp();
        $$->Exp = nullptr;
        $$->LVal = std::shared_ptr<AstLVal>($1);
        $$->Integer = nullptr;
        $$->Float = nullptr;
    }
    | Integer {
        $$ = new AstPrimaryExp();
        $$->Exp = nullptr;
        $$->LVal = nullptr;
        $$->Integer = std::shared_ptr<AstInteger>($1);
        $$->Float = nullptr;
    }
    | Float {
        $$ = new AstPrimaryExp();
        $$->Exp = nullptr;
        $$->LVal = nullptr;
        $$->Integer = nullptr;
        $$->Float = std::shared_ptr<AstFloat>($1);
    };

Integer:
    INTEGER_CONST {
        $$ = new AstInteger();
        $$->_int = $1;
    };

Float:
    FLOAT_CONST {
        $$ = new AstFloat();
        $$->_float = $1;
    };

UnaryExp:
    PrimaryExp {
        $$ = new AstUnaryExp();
        $$->op = OP_POS;
        $$->PrimaryExp = std::shared_ptr<AstPrimaryExp>($1);
        $$->Callee = nullptr;
        $$->UnaryExp = nullptr;
    }
    | Callee {
        $$ = new AstUnaryExp();
        $$->op = OP_POS;
        $$->PrimaryExp = nullptr;
        $$->Callee = std::shared_ptr<AstCallee>($1);
        $$->UnaryExp = nullptr;
    }
    | UnaryOp UnaryExp {
        $$ = new AstUnaryExp();
        $$->op = $1;
        $$->PrimaryExp = nullptr;
        $$->Callee = nullptr;
        $$->UnaryExp = std::shared_ptr<AstUnaryExp>($2);
    };

Callee:
    IDENTIFIER LPARENTHESE ExpList RPARENTHESE {
        $$ = new AstCallee();
        $$->id = $1;
        $$->ExpList.swap($3->list);
    }
    | IDENTIFIER LPARENTHESE RPARENTHESE {
        $$ = new AstCallee();
        $$->id = $1;
    };

UnaryOp:
    ADD { $$ = OP_POS; }
    | SUB { $$ = OP_NEG; }
    | NOT { $$ = OP_NOT; };

ExpList:
    ExpList COMMA Exp {
        $1->list.push_back(std::shared_ptr<AstExp>($3));
        $$ = $1;
    }
    | Exp {
        $$ = new AstExpList();
        $$->list.push_back(std::shared_ptr<AstExp>($1));
    };

MulExp:
    UnaryExp {
        $$ = new AstMulExp();
        $$->MulExp = nullptr;
        $$->UnaryExp = std::shared_ptr<AstUnaryExp>($1);
    }
    | MulExp MUL UnaryExp {
        $$ = new AstMulExp();
        $$->MulExp = std::shared_ptr<AstMulExp>($1);
        $$->UnaryExp = std::shared_ptr<AstUnaryExp>($3);
        $$->op = OP_MUL;
    }
    | MulExp DIV UnaryExp {
        $$ = new AstMulExp();
        $$->MulExp = std::shared_ptr<AstMulExp>($1);
        $$->UnaryExp = std::shared_ptr<AstUnaryExp>($3);
        $$->op = OP_DIV;
    }
    | MulExp MOD UnaryExp {
        $$ = new AstMulExp();
        $$->MulExp = std::shared_ptr<AstMulExp>($1);
        $$->UnaryExp = std::shared_ptr<AstUnaryExp>($3);
        $$->op = OP_MOD;
    };

AddExp:
    MulExp{
        $$ = new AstAddExp();
        $$->AddExp = nullptr;
        $$->MulExp = std::shared_ptr<AstMulExp>($1);
    }
    | AddExp ADD MulExp{ // 处理表达式优先级:1.修改文法 2.声明结合性
        $$ = new AstAddExp();
        $$->AddExp = std::shared_ptr<AstAddExp>($1);
        $$->MulExp = std::shared_ptr<AstMulExp>($3);
        $$->op = OP_PLUS;
    }
    | AddExp SUB MulExp{
        $$ = new AstAddExp();
        $$->AddExp = std::shared_ptr<AstAddExp>($1);
        $$->MulExp = std::shared_ptr<AstMulExp>($3);
        $$->op = OP_MINUS;
    };

RelExp:
    AddExp {
        $$ = new AstRelExp();
        $$->RelExp = nullptr;
        $$->AddExp = std::shared_ptr<AstAddExp>($1);
    }
    | RelExp LT AddExp {
        $$ = new AstRelExp();
        $$->RelExp = std::shared_ptr<AstRelExp>($1);
        $$->AddExp = std::shared_ptr<AstAddExp>($3);
        $$->op = OP_LT;
    }
    | RelExp GT AddExp {
        $$ = new AstRelExp();
        $$->RelExp = std::shared_ptr<AstRelExp>($1);
        $$->AddExp = std::shared_ptr<AstAddExp>($3);
        $$->op = OP_GT;
    }
    | RelExp LTE AddExp {
        $$ = new AstRelExp();
        $$->RelExp = std::shared_ptr<AstRelExp>($1);
        $$->AddExp = std::shared_ptr<AstAddExp>($3);
        $$->op = OP_LTE;
    }
    | RelExp GTE AddExp {
        $$ = new AstRelExp();
        $$->RelExp = std::shared_ptr<AstRelExp>($1);
        $$->AddExp = std::shared_ptr<AstAddExp>($3);
        $$->op = OP_GTE;
    };

EqExp:
    RelExp {
        $$=new AstEqExp();
        $$->EqExp=nullptr;
        $$->RelExp=std::shared_ptr<AstRelExp>($1);
    }
    | EqExp EQ RelExp {
        $$ = new AstEqExp();
        $$->EqExp = std::shared_ptr<AstEqExp>($1);
        $$->RelExp = std::shared_ptr<AstRelExp>($3);
        $$->op = OP_EQ;
      }
    | EqExp NEQ RelExp {
        $$ = new AstEqExp();
        $$->EqExp = std::shared_ptr<AstEqExp>($1);
        $$->RelExp = std::shared_ptr<AstRelExp>($3);
        $$->op = OP_NEQ;
    };

LAndExp:
    EqExp {
        $$ = new AstLAndExp();
        $$->LAndExp = nullptr;
        $$->EqExp = std::shared_ptr<AstEqExp>($1);
    }
    | LAndExp AND EqExp {
        $$ = new AstLAndExp();
        $$->LAndExp = std::shared_ptr<AstLAndExp>($1);
        $$->EqExp = std::shared_ptr<AstEqExp>($3);
        $$->op = OP_AND;
    };

LOrExp:
    LAndExp {
        $$ = new AstLOrExp();
        $$->LOrExp = nullptr;
        $$->LAndExp = std::shared_ptr<AstLAndExp>($1);
    }
    | LOrExp OR LAndExp {
        $$ = new AstLOrExp();
        $$->LOrExp = std::shared_ptr<AstLOrExp>($1);
        $$->LAndExp = std::shared_ptr<AstLAndExp>($3);
        $$->op = OP_OR;
    };

ConstExp:
    AddExp {
        $$ = new AstConstExp();
        $$->AddExp = std::shared_ptr<AstAddExp>($1);
    };
%%

// Register errors to the driver:
void yy::parser::error(const location_type& l, const std::string& m) {
    drv.error(l, m);
}