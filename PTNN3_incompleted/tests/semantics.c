/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdlib.h>
#include <string.h>
#include "semantics.h"
#include "error.h"

extern SymTab* symtab;
extern Token* currentToken;
// tra cứu đối tượng
Object* lookupObject(char *name) {
  // TODO
  Scope* scope = symtab->currentScope;// phạm vi hiện tại
Object* obj;
while (scope != NULL) {
obj = findObject(scope->objList, name);
if (obj != NULL) return obj;
scope = scope->outer;
}
obj = findObject(symtab->globalObjectList, name);
if (obj != NULL) return obj;
return NULL;
}
// kiểm tra định danh mới
void checkFreshIdent(char *name) {
  // TODO
 if (findObject(symtab->currentScope->objList, name) != NULL)
    error(ERR_DUPLICATE_IDENT, currentToken->lineNo, currentToken->colNo);
  
}
// kiểm tra định danh đã khai báo
Object* checkDeclaredIdent(char* name) {
  // TODO
  Object * obj = lookupObject(name);
  if(obj == NULL){
    error(ERR_DUPLICATE_IDENT,currentToken->lineNo,currentToken->colNo);
  }
}
// kiểm tra hằng số đã khai báo
Object* checkDeclaredConstant(char* name) {
  // TODO
  Object * obj = lookupObject(name);
  if(obj == NULL){
    error(ERR_UNDECLARED_CONSTANT,currentToken->lineNo,currentToken->colNo);
  }
  if(obj ->kind != OBJ_CONSTANT){
    error(ERR_INVALID_CONSTANT,currentToken->lineNo,currentToken->colNo);
  }
  return obj;
}
// khiểm tra kiểu đã khai báo
Object* checkDeclaredType(char* name) {
  // TODO
  Object * obj = lookupObject(name);
  if(obj == NULL){
    error(ERR_UNDECLARED_TYPE,currentToken->lineNo,currentToken->colNo);
  }
   if(obj -> kind != OBJ_TYPE){
      error(ERR_INVALID_TYPE,currentToken->lineNo,currentToken->colNo);
    }
    return obj;
}
// kiểm tra biễn đã khai báo
Object* checkDeclaredVariable(char* name) {
  // TODO
  Object * obj = lookupObject(name);
  if(obj == NULL){
    error(ERR_UNDECLARED_VARIABLE,currentToken->lineNo,currentToken->colNo);

  }
  if(obj->kind != OBJ_VARIABLE){
    error(ERR_INVALID_VARIABLE,currentToken->lineNo,currentToken->colNo);
  }
  return obj;

}
// kiểm tra func đã khai báo
Object* checkDeclaredFunction(char* name) {
  // TODO
  Object * obj = lookupObject(name);
  if(obj == NULL){
    error(ERR_UNDECLARED_FUNCTION,currentToken->lineNo,currentToken->colNo);// chức năng chưa được khai báo
  }
if(obj->kind != OBJ_FUNCTION){
  error(ERR_INVALID_FUNCTION,currentToken->lineNo,currentToken->colNo);// chức năng không hợp lệ
}
return obj;
}
// kiểm tra prod đã khai báo
Object* checkDeclaredProcedure(char* name) {
  // TODO
  Object* obj = lookupObject(name);
  if(obj == NULL){
error(ERR_UNDECLARED_PROCEDURE,currentToken->lineNo,currentToken->colNo);
  }
  if(obj -> kind != OBJ_PROCEDURE){
    error(ERR_INVALID_PROCEDURE,currentToken->lineNo,currentToken->colNo);
  }
  return obj;
}
// kiểm tra giá trị định danh đã khai báo
Object* checkDeclaredLValueIdent(char* name) {
  // TODO
  Object *obj = lookupObject(name);
  if(obj == NULL){
    error(ERR_UNDECLARED_IDENT,currentToken->lineNo,currentToken->colNo);// định danh chưa khai báo
  }
  switch (obj ->kind)
  {
  case OBJ_VARIABLE:
    break;
  case OBJ_PARAMETER:
  break;
  case OBJ_FUNCTION:
if (obj != symtab->currentScope->owner) 
      error(ERR_INVALID_IDENT,currentToken->lineNo, currentToken->colNo);// định danh không hợp lệ
    break;
  default:
   error(ERR_INVALID_IDENT,currentToken->lineNo, currentToken->colNo);
    break;
  }
  return obj;
}

