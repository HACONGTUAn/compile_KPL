/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdlib.h>

#include "reader.h"
#include "scanner.h"
#include "parser.h"
#include "error.h"
Token *currentToken;
Token *lookAhead;

void scan(void) {
  Token* tmp = currentToken;
  currentToken = lookAhead;
  lookAhead = getValidToken();
  free(tmp);
}

void eat(TokenType tokenType) {
  if (lookAhead->tokenType == tokenType) {
    printToken(lookAhead);
    scan();
  } else missingToken(tokenType, lookAhead->lineNo, lookAhead->colNo);
}

void compileProgram(void) {
  assert("Parsing a Program ....");
  eat(KW_PROGRAM);
  eat(TK_IDENT);
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_PERIOD);
  assert("Program parsed!");
}
//Block
void compileBlock(void) {
  assert("Parsing a Block ....");
  if(lookAhead->tokenType==SB_SEMICOLON){
    compileDeclarations();
    compileBlock2();
  }
  assert("Block parsed!");
}
// Declaration
void compileDeclarations(void){
  switch (lookAhead->tokenType)
  {
  case KW_CONST:
      compileConstDecls();
      compileDeclarations();
    break;
  case TK_IDENT:
      compileTypeDecls();
      compileDeclarations();
      break;
  case KW_VAR:
      compileVarDecls();
      compileDeclarations();
  break;
  case KW_FUNCTION:
  compileSubDecls();
  compileDeclarations();
  break;
  case KW_BEGIN:
  break;
  }
}
//Block2
void compileBlock2(void) {
  if (lookAhead->tokenType == KW_BEGIN) {
    eat(KW_BEGIN);
   compileStatement();
    eat(KW_END);
  } 
}


// Constdecls
void compileConstDecls(void) {
  assert("Parsing subconstants...");
  while (lookAhead->tokenType == KW_CONST)
    {  compileConstDecl();
      compileConstDecls2();}
  assert("Subconstants parsed!");
}
// constdecls2
void compileConstDecls2(void){
  switch(lookAhead->tokenType){
    case TK_IDENT:
    compileConstDecl();
    compileConstDecls2();
    break;
    case KW_BEGIN: 
    break;
  }
 
}
// constDecl
void compileConstDecl(void) {
  assert("Parsing constant...");
  eat(TK_IDENT);
  eat(SB_EQ);
  compileConstant();
  eat(SB_SEMICOLON);
  assert("Constant parsed!");
}
//typedcls
void compileTypeDecls(void) {
  assert("Parsing subtypes...");
  while (lookAhead->tokenType == TK_IDENT)
      switch(lookAhead->tokenType){
        case KW_TYPE:
          compileTypeDecl();
          break;
        case KW_BEGIN:
          break;
          default: compileTypeDecls2();
      }
      
      

  assert("Subtypes parsed!");
}
//typedecl
void compileTypeDecl(void) {
  assert("Parsing a type...");
  if(lookAhead->tokenType == KW_TYPE)
  {
  eat(KW_TYPE);
  compileTypeDecl();
compoleVarDecls2();
  }
  assert("Type parsed!");
}
// typedecls2
void compileTypeDecls2(void){
 switch(lookAhead->tokenType){
   case TK_IDENT:
   compileTypeDecl();
   break;
   default :  
   compileTypeDecls2();
   break;
 }
}
//vardecls
void compileVarDecls(void) {

  while(lookAhead->tokenType == TK_IDENT)
  if(lookAhead->tokenType == KW_VAR)
   {   compileVarDecl();}
      else compoleVarDecls2();

}
//vardecl
void compileVarDecl(void) {
  switch(lookAhead->tokenType){
  case KW_VAR:
  eat(KW_VAR);
  compileVarDecl();
  compoleVarDecls2();
  //fl(vardecls2)
  case KW_BEGIN:
  break;
  }
}
//vardecls2
void compoleVarDecls2(void){
  compileVarDecl();
  compileVarDecls();
  
}
//subdecls
void compileSubDecls(void) {
  assert("Parsing subtoutines ....");
  switch(lookAhead->tokenType){
  case KW_FUNCTION:
     compileFuncDecl();
      compileSubDecls();
      break;

case KW_PROCEDURE:
  compileProcDecl();
      compileSubDecls();
      break;
      // fl(subdecls)
  case KW_BEGIN:
  break;
  }
  assert("Subtoutines parsed ....");
}
//funcdecl
void compileFuncDecl(void) {
  assert("Parsing a function ....");
  eat(KW_FUNCTION);
  eat(TK_IDENT);
  compileParams();
  eat(SB_COLON);
  compileBasicType();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Funterm2tion parsed ....");
}
//procompileterm2decl
void compileProcDecl(void) {
  assert("Parsing a procedure ....");
  eat(KW_PROCEDURE);
  eat(TK_IDENT);
  compileParams();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Procedure parsed ....");
}

void compileUnsignedConstant(void) {
  switch (lookAhead->tokenType) {
  case TK_NUMBER:
      eat(TK_NUMBER);
      break;
  case TK_IDENT:
      eat(TK_IDENT);
      break;
  case TK_CHAR:
      eat(TK_CHAR);
      break;
  default:
      error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileConstant(void) {
  assert("Parsing a constant...");
  switch(lookAhead->tokenType) {
  case SB_PLUS:
      eat(SB_PLUS);
      compileConstant2();
      break;
  case SB_MINUS:
      eat(SB_MINUS);
      compileConstant2();
      break;
  case TK_CHAR:
      eat(TK_CHAR);
      break;
  default:
      compileConstant2();
      break;
  }
  assert("Constant parsed!");
}

void compileConstant2(void) {
  switch (lookAhead->tokenType) {
  case TK_IDENT:
      eat(TK_IDENT);
      break;
  case TK_NUMBER:
      eat(TK_NUMBER);
      break;
  default:
      error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileType(void) {
  switch (lookAhead->tokenType) {
  case KW_INTEGER:
      eat(KW_INTEGER);
      break;
  case KW_CHAR:
      eat(KW_CHAR);
      break;
  case TK_IDENT:
      eat(TK_IDENT);
      break;
  case KW_ARRAY:
      eat(KW_ARRAY);
      eat(SB_LSEL);
      eat(TK_NUMBER);
      eat(SB_RSEL);
      eat(KW_OF);
      compileType();
      break;
  default:
      error(ERR_INVALIDTYPE, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileBasicType(void) {
  switch (lookAhead->tokenType) {
  case KW_INTEGER:
      eat(KW_INTEGER);
      break;
  case KW_CHAR:
      eat(KW_CHAR);
      break;
  default:
      error(ERR_INVALIDBASICTYPE, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileParams(void) {
  switch (lookAhead->tokenType) {
  case SB_LPAR:
      eat(SB_LPAR);
      compileParam();
      compileParams2();
      eat(SB_RPAR);
      break;
  case SB_COLON:
  case SB_SEMICOLON:
      break;
  default:
      error(ERR_INVALIDPARAM, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileParams2(void) {
  switch (lookAhead->tokenType) {
  case SB_SEMICOLON:
      eat(SB_SEMICOLON);
      compileParam();
      compileParams2();
      break;
  case SB_RPAR:
      break;
  default:
      error(ERR_INVALIDPARAM, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileParam(void) {
  switch (lookAhead->tokenType) {
  case TK_IDENT:
      eat(TK_IDENT);
      eat(SB_COLON);
      compileBasicType();
      break;
  case KW_VAR:
      eat(KW_VAR);
      eat(TK_IDENT);
      eat(SB_COLON);
      compileBasicType();
      break;
  default:
      error(ERR_INVALIDPARAM, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileStatements(void) {
  compileStatement();
  compileStatements2();
}

void compileStatements2(void) {
  switch (lookAhead->tokenType) {
  case SB_SEMICOLON:
      eat(SB_SEMICOLON);
      compileStatement();
      compileStatements2();
      break;
  // Follow
  case KW_END:
      break;
  // Error
  default:
      error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileStatement(void) {
  switch (lookAhead->tokenType) {
  case TK_IDENT:
    compileAssignSt();
    break;
  case KW_CALL:
    compileCallSt();
    break;
  case KW_BEGIN:
    compileGroupSt();
    break;
  case KW_IF:
    compileIfSt();
    break;
  case KW_WHILE:
    compileWhileSt();
    break;
  case KW_FOR:
    compileForSt();
    break;
    // EmptySt needs to check FOLLOW tokens
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
    break;
    // Error occurs
  default:
    error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileAssignSt(void) {
  assert("Parsing an assign statement ....");
  eat(TK_IDENT);
  if (lookAhead->tokenType == SB_LSEL) {
      compileIndexes();
  }
  eat(SB_ASSIGN);
  compileExpression();
  assert("Assign statement parsed ....");
}

void compileCallSt(void) {
  assert("Parsing a call statement ....");
  eat(KW_CALL);
  eat(TK_IDENT);
  compileArguments();
  assert("Call statement parsed ....");
}

void compileGroupSt(void) {
  assert("Parsing a group statement ....");
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
  assert("Group statement parsed ....");
}

void compileIfSt(void) {
  assert("Parsing an if statement ....");
  eat(KW_IF);
  compileCondition();
  eat(KW_THEN);
  compileStatement();
  if (lookAhead->tokenType == KW_ELSE) 
    compileElseSt();
  assert("If statement parsed ....");
}

void compileElseSt(void) {
  eat(KW_ELSE);
  compileStatement();
}

void compileWhileSt(void) {
  assert("Parsing a while statement ....");
  eat(KW_WHILE);
  compileCondition();
  eat(KW_DO);
  compileStatement();
  assert("While statement pased ....");
}

void compileForSt(void) {
  assert("Parsing a for statement ....");
  eat(KW_FOR);
  eat(TK_IDENT);
  eat(SB_ASSIGN);
  compileExpression();
  eat(KW_TO);
  compileExpression();
  eat(KW_DO);
  compileStatement();
  assert("For statement parsed ....");
}

void compileArguments(void) {
  switch (lookAhead->tokenType) {
  case SB_LPAR:
      eat(SB_LPAR);
      compileExpression();
      compileArguments2();
      eat(SB_RPAR);
      break;
  // Follow - same as call statement as statement:
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
  // Follow - term2
  case SB_TIMES:
  case SB_SLASH:
  // Follow - expression3
  // Follow (For statement)
  case KW_TO:
  case KW_DO:
  // Follow (arguments2)
  case SB_COMMA:
  // Follow (condition2)
  case SB_EQ:
  case SB_NEQ:
  case SB_LE:
  case SB_LT:
  case SB_GE:
  case SB_GT:
  // Follow (factor)
  case SB_RPAR:
  // Follow (indexes)
  case SB_RSEL:
  // Follow (if statement)
  case KW_THEN:
      break;
  // Error
  default:
      error(ERR_INVALIDARGUMENTS, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileArguments2(void) {
  switch (lookAhead->tokenType) {
  case SB_COMMA:
      eat(SB_COMMA);
      compileExpression();
      compileArguments2();
      break;
  // Follow
  case SB_RPAR:
      break;
  // Error:
  default:
      error(ERR_INVALIDARGUMENTS, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileCondition(void) {
  compileExpression();
  compileCondition2();
}

void compileCondition2(void) {
  switch (lookAhead->tokenType) {
  case SB_EQ:
      eat(SB_EQ);
      compileExpression();
      break;
  case SB_NEQ:
      eat(SB_NEQ);
      compileExpression();
      break;
  case SB_LE:
      eat(SB_LE);
      compileExpression();
      break;
  case SB_LT:
      eat(SB_LT);
      compileExpression();
      break;
  case SB_GE:
      eat(SB_GE);
      compileExpression();
      break;
  case SB_GT:
      eat(SB_GT);
      compileExpression();
      break;
  default:
      error(ERR_INVALIDCOMPARATOR, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileExpression(void) {
  assert("Parsing an expression");
  switch (lookAhead->tokenType) {
  case SB_PLUS:
      eat(SB_PLUS);
      compileExpression2();
      break;
  case SB_MINUS:
      eat(SB_MINUS);
      compileExpression2();
      break;
  default:
      compileExpression2();
      break;
  }
  assert("Expression parsed");
}

void compileExpression2(void) {
  compileTerm();
  compileExpression3();
}


void compileExpression3(void) {
  switch(lookAhead->tokenType) {
  case SB_PLUS:
      eat(SB_PLUS);
      compileTerm();
      compileExpression3();
      break;
  case SB_MINUS:
      eat(SB_MINUS);
      compileTerm();
      compileExpression3();
      break;
  // Follow (statement)
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
  // Follow (For statement)
  case KW_TO:
  case KW_DO:
  // Follow (arguments2)
  case SB_COMMA:
  // Follow (condition2)
  case SB_EQ:
  if(lookAhead->tokenType==SB_EQ){
    eat(SB_EQ);
  }
  break;
  case SB_NEQ:
  case SB_LE:
  case SB_LT:
  case SB_GE:
  case SB_GT:
  // Follow (factor)
  case SB_RPAR:
  // Follow (indexes)
  case SB_RSEL:
  // Follow (if statement)
  case KW_THEN:
      break;
  // Error
  default:
      error(ERR_INVALIDEXPRESSION, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileTerm(void) {
  compileFactor();
  compileTerm2();
}

void compileTerm2(void) {
  switch (lookAhead->tokenType) {
  case SB_TIMES:
      eat(SB_TIMES);
      compileFactor();
      compileTerm2();
      break;
  case SB_SLASH:
      eat(SB_SLASH);
      compileFactor();
      compileTerm2();
      break;
  // Follow - same as expression3
  case SB_PLUS:
  case SB_MINUS:
  // Follow (statement)
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
  // Follow (For statement)
  case KW_TO:
  case KW_DO:
  // Follow (arguments2)
  case SB_COMMA:
  // Follow (condition2)
  case SB_EQ:
  case SB_NEQ:
  case SB_LE:
  case SB_LT:
  case SB_GE:
  case SB_GT:
  // Follow (factor)
  case SB_RPAR:
  // Follow (indexes)
  case SB_RSEL:
  // Follow (if statement)
  case KW_THEN:
      break;
  default:
      error(ERR_INVALIDTERM, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileFactor(void) {
  switch (lookAhead->tokenType) {
  case TK_NUMBER:
  case TK_CHAR:
      compileUnsignedConstant();
      break;
  case SB_LPAR:
      eat(SB_LPAR);
      compileExpression();
      eat(SB_RPAR);
      break;
  case TK_IDENT:
      eat(TK_IDENT);
      switch(lookAhead->tokenType) {
      case SB_LSEL:
          compileIndexes();
          break;
      case SB_LPAR:
          compileArguments();
          break;
      default:
          break;
      }
      break;
  default:
      error(ERR_INVALIDFACTOR, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileIndexes(void) {
  if (lookAhead->tokenType == SB_LSEL) {
      eat(SB_LSEL);
      compileExpression();
      eat(SB_RSEL);
      compileIndexes();
  }
}

int compile(char *fileName) {
  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  currentToken = NULL;
  lookAhead = getValidToken();

  compileProgram();

  free(currentToken);
  free(lookAhead);
  closeInputStream();
  return IO_SUCCESS;

}