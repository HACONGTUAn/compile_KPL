/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */
#include <stdio.h>
#include <stdlib.h>

#include "reader.h"
#include "scanner.h"
#include "parser.h"
#include "semantics.h"
#include "error.h"
#include "debug.h"

Token *currentToken;
Token *lookAhead;

extern Type *intType;
extern Type *charType;
extern SymTab *symtab;

void scan(void)
{
  Token *tmp = currentToken;
  currentToken = lookAhead;
  lookAhead = getValidToken();
  free(tmp);
}

void eat(TokenType tokenType)
{
  if (lookAhead->tokenType == tokenType)
  {
    scan();
  }
  else
    missingToken(tokenType, lookAhead->lineNo, lookAhead->colNo);
}

void compileProgram(void)
{
  Object *program;

  eat(KW_PROGRAM);// từ khóa
  eat(TK_IDENT);

  program = createProgramObject(currentToken->string);// tạo đối tượng với loại prorgram
  enterBlock(program->progAttrs->scope);// và một khối 

  eat(SB_SEMICOLON);

  compileBlock();
  eat(SB_PERIOD);

  exitBlock();// thoát khỏi khối
}

void compileBlock(void)
{
  Object *constObj;
  ConstantValue *constValue;

  if (lookAhead->tokenType == KW_CONST)
  {
    eat(KW_CONST);

    do
    {
      eat(TK_IDENT);

      checkFreshIdent(currentToken->string);
      constObj = createConstantObject(currentToken->string);// đối tượng hằng số

      eat(SB_EQ);
      constValue = compileConstant();

      constObj->constAttrs->value = constValue;
      declareObject(constObj);

      eat(SB_SEMICOLON);
    } while (lookAhead->tokenType == TK_IDENT);

    compileBlock2();
  }
  else
    compileBlock2();
}
// khai báo kiểu người dùng tự đinh nghĩa
// được tạo ra va khai báo ở block 2
// kiểu hiện tại lấy từ quá trình duyệt kiểu compiletype
void compileBlock2(void)
{
  Object *typeObj;
  Type *actualType;

  if (lookAhead->tokenType == KW_TYPE)
  {
    eat(KW_TYPE);

    do
    {
      eat(TK_IDENT);

      checkFreshIdent(currentToken->string);
      typeObj = createTypeObject(currentToken->string);

      eat(SB_EQ); // = 
      actualType = compileType();// kiểu hiện tại

      typeObj->typeAttrs->actualType = actualType;
      declareObject(typeObj);

      eat(SB_SEMICOLON);// " ; "
    } while (lookAhead->tokenType == TK_IDENT);

    compileBlock3();
  }
  else
    compileBlock3();
}
// các đối tượng biến được khai báo ở block 3
// kiểu của biến được lấy từ quá trình duyệt bằng hàm compiletype
void compileBlock3(void)
{
  Object *varObj;
  Type *varType;

  if (lookAhead->tokenType == KW_VAR)
  {
    eat(KW_VAR);

    do
    {
      eat(TK_IDENT);
      checkFreshIdent(currentToken->string);
      varObj = createVariableObject(currentToken->string);
      eat(SB_COLON);
      varType = compileType();
      varObj->varAttrs->type = varType;
      declareObject(varObj);
      eat(SB_SEMICOLON);
    } while (lookAhead->tokenType == TK_IDENT);

    compileBlock4();
  }
  else
    compileBlock4();
}

void compileBlock4(void)
{
  compileSubDecls();
  compileBlock5();
}

void compileBlock5(void)
{
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
}
// bộ phụ
void compileSubDecls(void)
{
  while ((lookAhead->tokenType == KW_FUNCTION) || (lookAhead->tokenType == KW_PROCEDURE))
  {
    if (lookAhead->tokenType == KW_FUNCTION)
      compileFuncDecl();
    else
      compileProcDecl();
  }
}

void compileFuncDecl(void)
{
  Object *funcObj;
  Type *returnType;

  eat(KW_FUNCTION);
  eat(TK_IDENT);

  checkFreshIdent(currentToken->string);
  funcObj = createFunctionObject(currentToken->string);
  declareObject(funcObj);

  enterBlock(funcObj->funcAttrs->scope);

  compileParams();

  eat(SB_COLON);
  returnType = compileBasicType();// kiểu trả về
  funcObj->funcAttrs->returnType = returnType;

  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);

  exitBlock();
}

void compileProcDecl(void)
{
  Object *procObj;

  eat(KW_PROCEDURE);
  eat(TK_IDENT);

  checkFreshIdent(currentToken->string);
  procObj = createProcedureObject(currentToken->string);
  declareObject(procObj);

  enterBlock(procObj->procAttrs->scope);

  compileParams();

  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);

  exitBlock();
}

ConstantValue *compileUnsignedConstant(void)
{
  ConstantValue *constValue;
  Object *obj;

  switch (lookAhead->tokenType)
  {
  case TK_NUMBER:
    eat(TK_NUMBER);
    constValue = makeIntConstant(currentToken->value);
    break;
  case TK_IDENT:
    eat(TK_IDENT);

    obj = checkDeclaredConstant(currentToken->string);
    constValue = duplicateConstantValue(obj->constAttrs->value);

    break;
  case TK_CHAR:
    eat(TK_CHAR);
    constValue = makeCharConstant(currentToken->string[0]);
    break;
  default:
    error(ERR_INVALID_CONSTANT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
  return constValue;
}

ConstantValue *compileConstant(void)
{
  ConstantValue *constValue;

  switch (lookAhead->tokenType)
  {
  case SB_PLUS:
    eat(SB_PLUS);
    constValue = compileConstant2();
    break;
  case SB_MINUS:
    eat(SB_MINUS);
    constValue = compileConstant2();
    constValue->intValue = -constValue->intValue;
    break;
  case TK_CHAR:
    eat(TK_CHAR);
    constValue = makeCharConstant(currentToken->string[0]);
    break;
  default:
    constValue = compileConstant2();
    break;
  }
  return constValue;
}

ConstantValue *compileConstant2(void)
{
  ConstantValue *constValue;
  Object *obj;

  switch (lookAhead->tokenType)
  {
  case TK_NUMBER:
    eat(TK_NUMBER);
    constValue = makeIntConstant(currentToken->value);
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    obj = checkDeclaredConstant(currentToken->string);
    if (obj->constAttrs->value->type == TP_INT)
      constValue = duplicateConstantValue(obj->constAttrs->value);
    else
      error(ERR_UNDECLARED_INT_CONSTANT, currentToken->lineNo, currentToken->colNo);
    break;
  default:
    error(ERR_INVALID_CONSTANT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
  return constValue;
}
// lấy giá trị của kiểu
Type *compileType(void)
{
  Type *type;
  Type *elementType;
  int arraySize;
  Object *obj;

  switch (lookAhead->tokenType)
  {
  case KW_INTEGER:
    eat(KW_INTEGER);
    type = makeIntType();// tạo giá trị Int
    break;
  case KW_CHAR:
    eat(KW_CHAR);
    type = makeCharType();// tạo giá trị char
    break;
  case KW_ARRAY:
    eat(KW_ARRAY);
    eat(SB_LSEL);
    eat(TK_NUMBER);

    arraySize = currentToken->value;// lấy kích thước mảng

    eat(SB_RSEL);
    eat(KW_OF);
    elementType = compileType();// lấy giá trị kiểu
    type = makeArrayType(arraySize, elementType);
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    obj = checkDeclaredType(currentToken->string);//lỗi nếu xảy ra : kiểu chưa được khai báo và khai bao sai kiểu
    type = duplicateType(obj->typeAttrs->actualType);
    break;
  default:
    error(ERR_INVALID_TYPE, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
  return type;
}

Type *compileBasicType(void)
{
  Type *type;

  switch (lookAhead->tokenType)
  {
  case KW_INTEGER:
    eat(KW_INTEGER);
    type = makeIntType();
    break;
  case KW_CHAR:
    eat(KW_CHAR);
    type = makeCharType();
    break;
  default:
    error(ERR_INVALID_BASICTYPE, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
  return type;
}

void compileParams(void)
{
  if (lookAhead->tokenType == SB_LPAR)
  {
    eat(SB_LPAR);
    compileParam();
    while (lookAhead->tokenType == SB_SEMICOLON)
    {
      eat(SB_SEMICOLON);
      compileParam();
    }
    eat(SB_RPAR);
  }
}

void compileParam(void)
{
  Object *param;
  Type *type;
  enum ParamKind paramKind;

  switch (lookAhead->tokenType)
  {
  case TK_IDENT:
    paramKind = PARAM_VALUE;// tham trị
    break;
  case KW_VAR:
    eat(KW_VAR);
    paramKind = PARAM_REFERENCE;// tham biến
    break;
  default:
    error(ERR_INVALID_PARAMETER, lookAhead->lineNo, lookAhead->colNo);
    break;
  }

  eat(TK_IDENT);
  checkFreshIdent(currentToken->string);
  param = createParameterObject(currentToken->string, paramKind, symtab->currentScope->owner);
  eat(SB_COLON);
  type = compileBasicType();
  param->paramAttrs->type = type;
  declareObject(param);
}
// các câu lênh , tuyên bố
void compileStatements(void)
{
  compileStatement();
  while (lookAhead->tokenType == SB_SEMICOLON)
  {
    eat(SB_SEMICOLON);
    compileStatement();
  }
}

void compileStatement(void)
{
  switch (lookAhead->tokenType)
  {
  case TK_IDENT://*
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
    error(ERR_INVALID_STATEMENT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}
// lấy giá trị bên trái
Type *compileLValue(void)
{
  // TODO: parse a lvalue (a variable, an array element, a parameter, the current function identifier)
  Object *var;
  Type *varType;

  eat(TK_IDENT);
  // check if the identifier is a function identifier, or a variable identifier, or a parameter
  // kiểu tra định danh của 1 funcion là 1 đinh danh hoặc 1 biến , hoặc 1 tham số
  var = checkDeclaredLValueIdent(currentToken->string);// đối tượng bên trái và loại của nó
  if (var->kind == OBJ_VARIABLE){
    if(var->varAttrs->type->typeClass == TP_ARRAY){
        varType = compileIndexes(var->varAttrs->type);
  }else{
    varType = var->varAttrs->type;
  }
  }
else if(var->kind == OBJ_FUNCTION)
varType = var->funcAttrs->returnType;
else if(var->kind == OBJ_PARAMETER)
varType = var->paramAttrs->type;
  return varType;
}
// lệnh gán
void compileAssignSt(void)
{
  // TODO: parse the assignment and check type consistency
  // kiểm tra tính nhất quán của loại
  Type * LType = NULL, *RType;
  LType =  compileLValue();
  eat(SB_ASSIGN);// :=
  RType =compileExpression();
  checkTypeEquality(LType,RType);
}

void compileCallSt(void)
{
  Object *proc;

  eat(KW_CALL);
  eat(TK_IDENT);

  proc = checkDeclaredProcedure(currentToken->string);// đã được khai báo chưa

  compileArguments(proc->procAttrs->paramList);
}

void compileGroupSt(void)
{
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
}

void compileIfSt(void)
{
  eat(KW_IF);
  compileCondition();
  eat(KW_THEN);
  compileStatement();
  if (lookAhead->tokenType == KW_ELSE)
    compileElseSt();
}

void compileElseSt(void)
{
  eat(KW_ELSE);
  compileStatement();
}

void compileWhileSt(void)
{
  eat(KW_WHILE);
  compileCondition();
  eat(KW_DO);
  compileStatement();
}

void compileForSt(void)
{
  // TODO: Check type consistency of FOR's variable
  // kiểm tra tính nhất quán lệnh for
  // <var>,<exp1>,<exp2> phải cùng kiểu
  Object*var = NULL;
  eat(KW_FOR);
  eat(TK_IDENT);

  // check if the identifier is a variable
  // kiểm tra định danh có phải 1 biến hay không

  var = checkDeclaredVariable(currentToken->string);
checkIntType(var->varAttrs->type);
  eat(SB_ASSIGN);//':='
checkIntType(compileExpression());

  eat(KW_TO);
 checkIntType (compileExpression());

  eat(KW_DO);
  compileStatement();
}

void compileArgument(Object *param)
{
  // TODO: parse an argument, and check type consistency(phân tích cú pháp một đối tượng và kiểm tra tính nhất quả về kiểu)
  //       If the corresponding parameter is a reference, the argument must be a lvalue(nếu tham số tướng ứng là tham chiếu, đối số phải là giá trị)
  if(param->paramAttrs->kind == PARAM_REFERENCE){
    if(lookAhead->tokenType == TK_IDENT){
      checkDeclaredLValueIdent(lookAhead->string);
    }else{
      error(ERR_TYPE_INCONSISTENCY,currentToken->lineNo,currentToken->colNo);
    }
  }
 checkTypeEquality( compileExpression(),param->paramAttrs->type);
}
// lấy giá trị các đối số
void compileArguments(ObjectNode *paramList)
{
  // TODO: parse a list of arguments, check the consistency of the arguments and the given parameters
  // phân tích cú pháp danh sách cái đối số , kiểm tra tính nhất quán của các đối số và các tham số đã cho
  if(paramList != NULL && lookAhead->tokenType != SB_LPAR){
    error(ERR_PARAMETERS_ARGUMENTS_INCONSISTENCY,currentToken->lineNo,currentToken->colNo);
  }
  switch (lookAhead->tokenType)
  {
  case SB_LPAR:
    eat(SB_LPAR);
    if(paramList != NULL){
    compileArgument(paramList->object);
    }else{
      error(ERR_PARAMETERS_ARGUMENTS_INCONSISTENCY,currentToken->lineNo,currentToken->colNo);
    }
    while (lookAhead->tokenType == SB_COMMA)//","
    {
      eat(SB_COMMA);
      paramList = paramList->next;// đối tượng tiếp theo của danh sách
      if(paramList!=NULL){
      compileArgument(paramList->object);
         }else{
          error(ERR_PARAMETERS_ARGUMENTS_INCONSISTENCY,currentToken->lineNo,currentToken->colNo);}
            }
paramList = paramList->next;
if(paramList!=NULL){
  if(lookAhead->tokenType == SB_COMMA){
    compileArgument(paramList->object);
  }else{
    error(ERR_PARAMETERS_ARGUMENTS_INCONSISTENCY,currentToken->lineNo,currentToken->colNo);
  }
}
    eat(SB_RPAR);
    break;
    // Check FOLLOW set
  case SB_TIMES:
  case SB_SLASH:
  case SB_PLUS:
  case SB_MINUS:
  case KW_TO:
  case KW_DO:
  case SB_RPAR:
  case SB_COMMA:
  case SB_EQ:
  case SB_NEQ:
  case SB_LE:
  case SB_LT:
  case SB_GE:
  case SB_GT:
  case SB_RSEL:
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
  case KW_THEN:
    break;
  default:
    error(ERR_INVALID_ARGUMENTS, lookAhead->lineNo, lookAhead->colNo);
  }
}
// kiểm tra nhất quán biểu thức trái ,phải
void compileCondition(void)
{
  // TODO: check the type consistency of LHS and RSH, check the basic type
  // kiểm tra kiểu nhất quán của biểu thức bên trái và biểu thức bên phải , kiểm tra kiểu cơ bản
  Type*LExpression =NULL;
   LExpression = compileExpression();

  switch (lookAhead->tokenType)
  {
  case SB_EQ:
    eat(SB_EQ);
    break;
  case SB_NEQ:
    eat(SB_NEQ);
    break;
  case SB_LE:
    eat(SB_LE);
    break;
  case SB_LT:
    eat(SB_LT);
    break;
  case SB_GE:
    eat(SB_GE);
    break;
  case SB_GT:
    eat(SB_GT);
    break;
  default:
    error(ERR_INVALID_COMPARATOR, lookAhead->lineNo, lookAhead->colNo);
  }

  checkTypeEquality(LExpression,compileExpression());
}
// lấy giá trị biểu thức
Type *compileExpression(void)
{
  Type *type;

  switch (lookAhead->tokenType)
  {
  case SB_PLUS:// +
    eat(SB_PLUS);
    type = compileExpression2();
    checkIntType(type);
    break;
  case SB_MINUS:// -
    eat(SB_MINUS);
    type = compileExpression2();
    checkIntType(type);
    break;
  default:
    type = compileExpression2();
  }
  return type;
}

Type *compileExpression2(void)
{
  Type *type;

  type = compileTerm();
  compileExpression3();

  return type;
}

void compileExpression3(void)
{
  Type *type;

  switch (lookAhead->tokenType)
  {
  case SB_PLUS://+
    eat(SB_PLUS);
    type = compileTerm();
    checkIntType(type);
    compileExpression3();
    break;
  case SB_MINUS://-
    eat(SB_MINUS);
    type = compileTerm();
    checkIntType(type);
    compileExpression3();
    break;
    // check the FOLLOW set
  case KW_TO:
  case KW_DO:
  case SB_RPAR:
  case SB_COMMA:
  case SB_EQ:
  case SB_NEQ:
  case SB_LE:
  case SB_LT:
  case SB_GE:
  case SB_GT:
  case SB_RSEL:
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
  case KW_THEN:
    break;
  default:
    error(ERR_INVALID_EXPRESSION, lookAhead->lineNo, lookAhead->colNo);
  }
}

Type *compileTerm(void)
{
  Type *type;

  type = compileFactor();
  compileTerm2();

  return type;
}

void compileTerm2(void)
{
  Type *type;

  switch (lookAhead->tokenType)
  {
  case SB_TIMES://*
    eat(SB_TIMES);
    type = compileFactor();
    checkIntType(type);
    compileTerm2();
    break;
  case SB_SLASH:// '/'
    eat(SB_SLASH);
    type = compileFactor();
    checkIntType(type);
    compileTerm2();
    break;
    // check the FOLLOW set
  case SB_PLUS:
  case SB_MINUS:
  case KW_TO:
  case KW_DO:
  case SB_RPAR:
  case SB_COMMA:
  case SB_EQ:
  case SB_NEQ:
  case SB_LE:
  case SB_LT:
  case SB_GE:
  case SB_GT:
  case SB_RSEL:
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
  case KW_THEN:
    break;
  default:
    error(ERR_INVALID_TERM, lookAhead->lineNo, lookAhead->colNo);
  }
}

Type *compileFactor(void)
{
  // TODO: parse a factor and return the factor's type
// phân tích cú pháp hệ số và trả về kiểu của hệ số đó
  Object *obj;
  Type *type;

  switch (lookAhead->tokenType)
  {
  case TK_NUMBER:
    eat(TK_NUMBER);
    type = makeIntType();
    break;
  case TK_CHAR:
    eat(TK_CHAR);
    type = makeCharType();
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    // check if the identifier is declared
    // kiểm tra xem mã định danh đã được khai báo chưa
    obj = checkDeclaredIdent(currentToken->string);

    switch (obj->kind)
    {
    case OBJ_CONSTANT:
    type = makeIntType();
    type->typeClass = obj ->constAttrs->value->type;
      break;
    case OBJ_VARIABLE:
    if(obj->varAttrs->type->typeClass = TP_ARRAY){
      type = compileIndexes(obj->varAttrs->type);
    }else{
      type = obj->varAttrs->type;
    }
    
      break;
    case OBJ_PARAMETER:
    type = obj->paramAttrs->type;
      break;
    case OBJ_FUNCTION:
      type = obj->funcAttrs->returnType;
      compileArguments(obj->funcAttrs->paramList);
      break;
    default:
      error(ERR_INVALID_FACTOR, currentToken->lineNo, currentToken->colNo);
      break;
    }
    break;
  default:
    error(ERR_INVALID_FACTOR, lookAhead->lineNo, lookAhead->colNo);
  }

  return type;
}
// lấy giá trị kiểu mảng,
Type *compileIndexes(Type *arrayType)
{
  // TODO: parse a sequence of indexes, check the consistency to the arrayType, and return the element type
  // phân tích cú pháp một chuỗi các chỉ mục, kiểm tra tính nhất quán của arrayType và trả về kiểu phần tử
  Type *indexType = NULL, *elementType = NULL;
  while (lookAhead->tokenType == SB_LSEL) //"(."
  {
    eat(SB_LSEL);
    checkArrayType(arrayType);// kiểm tra hiện tại có phải kiểu arr k
    indexType = compileExpression();
  checkIntType(indexType);
    eat(SB_RSEL);
        arrayType = arrayType->elementType;
  }
   elementType = arrayType;

  return elementType;
}

int compile(char *fileName)
{
  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  currentToken = NULL;
  lookAhead = getValidToken();

  initSymTab();

  compileProgram();

  printObject(symtab->program, 0);

  cleanSymTab();

  free(currentToken);
  free(lookAhead);
  closeInputStream();
  return IO_SUCCESS;
}
