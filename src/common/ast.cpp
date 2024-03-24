#include "ast.hpp"
#include <iostream>
#include <string>

const char* BType_string[] = {"int", "float", "void"};
const char* RelOp_string[] = {"<=", "<", ">", ">="};
const char* EqOp_string[] = {"==", "!="};
const char* AddOp_string[] = {"+", "-"};
const char* MulOp_string[] = {"*", "/", "%"};
const char* UnaryOp_string[] = {"+", "-", "!"};
const char* LogicOp_string[] = {"&&", "||"};

#define _SYNTAX_TREE_NODE_ERROR_                                               \
  std::cerr << "Abort due to node cast error." << std::endl;                   \
  std::abort();

void syntax_tree::run_visitor(syntax_tree_visitor &visitor) {
  root->accept(visitor);
}

Value *AstDeclDef::accept(syntax_tree_visitor &visitor) {
  if (this->ConstDecl) {
    return this->ConstDecl->accept(visitor);
  } else if (this->FuncDef) {
    return this->FuncDef->accept(visitor);
  } else if (this->VarDecl) {
    return this->VarDecl->accept(visitor);
  }
  _SYNTAX_TREE_NODE_ERROR_
}

Value *AstBlockItem::accept(syntax_tree_visitor &visitor) {
  if (this->ConstDecl) {
    return this->ConstDecl->accept(visitor);
  } else if (this->VarDecl) {
    return this->VarDecl->accept(visitor);
  } else if (this->Stmt) {
    return this->Stmt->accept(visitor);
  }
  _SYNTAX_TREE_NODE_ERROR_
}

Value *AstStmt::accept(syntax_tree_visitor &visitor) {
  if (this->AssignStmt) {
    return this->AssignStmt->accept(visitor);
  } else if (this->Block) {
    return this->Block->accept(visitor);
  } else if (this->BreakStmt) {
    return this->BreakStmt->accept(visitor);
  } else if (this->ContinueStmt) {
    return this->ContinueStmt->accept(visitor);
  } else if (this->Exp) {
    return this->Exp->accept(visitor);
  } else if (this->IterationStmt) {
    return this->IterationStmt->accept(visitor);
  } else if (this->ReturnStmt) {
    return this->ReturnStmt->accept(visitor);
  } else if (this->SelectStmt) {
    return this->SelectStmt->accept(visitor);
  }
  _SYNTAX_TREE_NODE_ERROR_
}

Value *AstExp::accept(syntax_tree_visitor &visitor) {
  // return this->AddExp->accept(visitor);
  return visitor.visit(*this);
}

Value *AstCond::accept(syntax_tree_visitor &visitor) {
  // this->LOrExp->accept(visitor);
  return visitor.visit(*this);
}

Value *AstPrimaryExp::accept(syntax_tree_visitor &visitor) {
  // return visitor.visit(*this);
  if (this->Exp) {
    return visitor.visit(*Exp);
  } else if (this->LVal) {
    return visitor.visit(*LVal);
  } else if (this->Integer) {
    return visitor.visit(*Integer);
  } else if (this->Float) {
    return visitor.visit(*Float);
  }
  _SYNTAX_TREE_NODE_ERROR_
}

Value *AstConstExp::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

Value *AstCompUnit::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

Value *AstConstDecl::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

Value *AstConstDef::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

Value *AstConstInitVal::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

Value *AstVarDecl::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

Value *AstVarDef::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

Value *AstInitVal::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

Value *AstFuncDef::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

Value *AstFuncFParam::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

Value *AstBlock::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

Value *AstBreakStmt::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

Value *AstContinueStmt::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

Value *AstAssignStmt::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

Value *AstSelectStmt::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

Value *AstIterationStmt::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

Value *AstReturnStmt::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

Value *AstLVal::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

Value *AstInteger::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

Value *AstFloat::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

Value *AstUnaryExp::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

Value *AstCallee::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

Value *AstMulExp::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

Value *AstAddExp::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

Value *AstRelExp::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

Value *AstEqExp::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

Value *AstLAndExp::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

Value *AstLOrExp::accept(syntax_tree_visitor &visitor) {
  return visitor.visit(*this);
}

/// ******************* syntax_tree_printer *********************** ///

#define _DEBUG_PRINT_N_(N) { std::cout << std::string(N, '-'); }

Value *syntax_tree_printer::visit(AstConstExp &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "const_exp" << std::endl;
  add_depth();
  node.AddExp->accept(*this);
  remove_depth();
  return nullptr;
}

Value *syntax_tree_printer::visit(AstCompUnit &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "CompUnit" << std::endl;
  add_depth();
  for (auto decl: node.DeclDefList) {
    decl->accept(*this);
  }
  remove_depth();
  return nullptr;
}

Value *syntax_tree_printer::visit(AstConstDecl &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "ConstDecl: " << BType_string[node.type] << std::endl;
  add_depth();
  for (auto p: node.ConstDefList) {
    p->accept(*this);
  }
  remove_depth();
  return nullptr;
}

Value *syntax_tree_printer::visit(AstConstDef &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "ConstDef: " << node.id << '\n';
  add_depth();
  for (auto p: node.ArrayConstExpList) {
    p->accept(*this);
  }
  if (node.ConstInitVal) {
    node.ConstInitVal->accept(*this);
  }
  remove_depth();
  return nullptr;
}

Value *syntax_tree_printer::visit(AstConstInitVal &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "ConstInitVal" << std::endl;
  add_depth();
  if (node.ConstExp) {
    node.ConstExp->accept(*this);
  } else {
    for (auto p: node.ConstInitValList) {
      p->accept(*this);
    }
  }
  remove_depth();
  return nullptr;
}

Value *syntax_tree_printer::visit(AstVarDecl &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "VarDecl: " << BType_string[node.type] << std::endl;
  add_depth();
  for (auto p: node.VarDefList) {
    p->accept(*this);
  }
  remove_depth();
  return nullptr;
}

Value *syntax_tree_printer::visit(AstVarDef &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "VarDef: " << node.id << std::endl;
  add_depth();
  for (auto p: node.ArrayConstExpList) {
    p->accept(*this);
  }
  if (node.InitVal) {
    node.InitVal->accept(*this);
  }
  remove_depth();
  return nullptr;
}

Value *syntax_tree_printer::visit(AstInitVal &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "InitVal" << std::endl;
  add_depth();
  if (node.Exp) {
    node.Exp->accept(*this);
  } else {
    for (auto p: node.InitValList) {
      p->accept(*this);
    }
  }
  remove_depth();
  return nullptr;
}

Value *syntax_tree_printer::visit(AstFuncDef &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "FuncDef: " << BType_string[node.type] << node.id << std::endl;
  add_depth();
  for (auto p: node.FuncFParamList) {
    p->accept(*this);
  }
  if (node.Block) {
    node.Block->accept(*this);
  }
  remove_depth();
  return nullptr;
}

Value *syntax_tree_printer::visit(AstFuncFParam &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "FuncFParam" << BType_string[node.type] << " " << node.id
            << "\tisarray:" << node.isarray << '\n';
  add_depth();
  for (auto p: node.ParamArrayExpList) {
    p->accept(*this);
  }
  remove_depth();
  return nullptr;
}

Value *syntax_tree_printer::visit(AstBlock &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "Block" << std::endl;
  add_depth();
  for (auto p: node.BlockItemList) {
    p->accept(*this);
  }
  remove_depth();
  return nullptr;
}

Value *syntax_tree_printer::visit(AstBreakStmt &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "break;" << std::endl;
  return nullptr;
}

Value *syntax_tree_printer::visit(AstContinueStmt &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "continue;\n";
  return nullptr;
}

Value *syntax_tree_printer::visit(AstAssignStmt &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "AssignStmt" << std::endl;
  add_depth();
  node.LVal->accept(*this);
  node.Exp->accept(*this);
  remove_depth();
  return nullptr;
}

Value *syntax_tree_printer::visit(AstSelectStmt &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "SelectStmt" << std::endl;
  add_depth();
  node.Cond->accept(*this);
  node.ifStmt->accept(*this);
  if (node.elseStmt) {
    node.elseStmt->accept(*this);
  }
  remove_depth();
  return nullptr;
}

Value *syntax_tree_printer::visit(AstIterationStmt &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "WhileLoop" << std::endl;
  add_depth();
  if (node.Cond) {
    node.Cond->accept(*this);
  }
  node.Stmt->accept(*this);
  remove_depth();
  return nullptr;
}

Value *syntax_tree_printer::visit(AstReturnStmt &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "Return" << std::endl;
  add_depth();
  if (node.Exp) {
    node.Exp->accept(*this);
  }
  remove_depth();
  return nullptr;
}

Value *syntax_tree_printer::visit(AstLVal &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "Lval: " << node.id << std::endl;
  add_depth();
  for (auto p: node.ArrayExpList) {
    p->accept(*this);
  }
  remove_depth();
  return nullptr;
}

Value *syntax_tree_printer::visit(AstPrimaryExp &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "PrimaryExp:" << std::endl;
  add_depth();
  node.accept(*this);
  // if (node.Exp) {
  //   node.Exp->accept(*this);
  // } else if (node.LVal) {
  //   node.LVal->accept(*this);
  // } else if (node.Number) {
  //   node.Number->accept(*this);
  // }
  remove_depth();
  return nullptr;
}

Value *syntax_tree_printer::visit(AstInteger &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "Integer: " << node._int << '\n';
  return nullptr;
}

Value *syntax_tree_printer::visit(AstFloat &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "Float: " << node._float << '\n';
  return nullptr;
}

Value *syntax_tree_printer::visit(AstUnaryExp &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "UnaryExp: " << UnaryOp_string[node.op] << std::endl;
  add_depth();
  if (node.PrimaryExp) {
    node.PrimaryExp->accept(*this);
  } else if (node.Callee) {
    node.Callee->accept(*this);
  } else {
    // _DEBUG_PRINT_N_(depth);
    // std::cout << node.op<<std::endl;
    node.UnaryExp->accept(*this);
  }
  remove_depth();
  return nullptr;
}

Value *syntax_tree_printer::visit(AstCallee &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "Call: " << node.id << std::endl;
  add_depth();
  for (auto p: node.ExpList) {
    p->accept(*this);
  }
  remove_depth();
  return nullptr;
}

Value *syntax_tree_printer::visit(AstMulExp &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "MulExp: " << MulOp_string[node.op] << std::endl;
  add_depth();
  if (node.MulExp) {
    node.MulExp->accept(*this);
  }
  if (node.UnaryExp) {
    node.UnaryExp->accept(*this);
  }
  remove_depth();
  return nullptr;
}

Value *syntax_tree_printer::visit(AstAddExp &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "AddExp: " << AddOp_string[node.op] << std::endl;
  add_depth();
  if (node.AddExp) {
    node.AddExp->accept(*this);
  }
  if (node.MulExp) {
    node.MulExp->accept(*this);
  }
  remove_depth();
  return nullptr;
}

Value *syntax_tree_printer::visit(AstRelExp &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "RelExp: " << RelOp_string[node.op] << std::endl;
  add_depth();
  if (node.RelExp) {
    node.RelExp->accept(*this);
  }
  if (node.AddExp) {
    node.AddExp->accept(*this);
  }
  remove_depth();
  return nullptr;
}

Value *syntax_tree_printer::visit(AstEqExp &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "EqExp: " << EqOp_string[node.op] << std::endl;
  add_depth();
  if (node.EqExp) {
    node.EqExp->accept(*this);
  }
  if (node.RelExp) {
    node.RelExp->accept(*this);
  }
  remove_depth();
  return nullptr;
}

Value *syntax_tree_printer::visit(AstLAndExp &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "LogicalAndExp: " << LogicOp_string[node.op] << std::endl;
  add_depth();
  if (node.LAndExp) {
    node.LAndExp->accept(*this);
  }
  if (node.EqExp) {
    node.EqExp->accept(*this);
  }
  remove_depth();
  return nullptr;
}

Value *syntax_tree_printer::visit(AstLOrExp &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "LogicalOrExp: " << LogicOp_string[node.op] << std::endl;
  add_depth();
  if (node.LOrExp) {
    node.LOrExp->accept(*this);
  }
  if (node.LAndExp) {
    node.LAndExp->accept(*this);
  }
  remove_depth();
  return nullptr;
}

Value *syntax_tree_printer::visit(AstExp &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "AstExp:" << std::endl;
  add_depth();
  node.AddExp->accept(*this);
  remove_depth();
  return nullptr;
}

Value *syntax_tree_printer::visit(AstCond &node) {
  _DEBUG_PRINT_N_(depth);
  std::cout << "AstCond:" << std::endl;
  add_depth();
  node.LOrExp->accept(*this);
  remove_depth();
  return nullptr;
}
