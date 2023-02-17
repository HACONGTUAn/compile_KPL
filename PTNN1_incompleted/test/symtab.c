/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

void freeObject(Object* obj);
void freeScope(Scope* scope);
void freeObjectList(ObjectNode *objList);
void freeReferenceList(ObjectNode *objList);

SymTab* symtab;
Type* intType;
Type* charType;

/******************* Type utilities ******************************/

Type* makeIntType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_INT;
  return type;
}

Type* makeCharType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_CHAR;
  return type;
}

Type* makeArrayType(int arraySize, Type* elementType) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_ARRAY;
  type->arraySize = arraySize;
  type->elementType = elementType;
  return type;
}

Type* duplicateType(Type* type) {
  // TODO
}

int compareType(Type* type1, Type* type2) {
  // TODO
}

void freeType(Type* type) {
  // TODO
}

/******************* Constant utility ******************************/

ConstantValue* makeIntConstant(int i) {
  // TODO
  ConstantValue * Intconstant = (ConstantValue *)malloc(sizeof(ConstantValue));
  Intconstant->type = TP_INT;
  Intconstant->intValue = i;
  return Intconstant;

}

ConstantValue* makeCharConstant(char ch) {
  // TODO
    ConstantValue * Chconstant = (ConstantValue *)malloc(sizeof(ConstantValue));
  Chconstant->type = TP_INT;
  Chconstant->intValue = ch;
  return Chconstant;
}

ConstantValue* duplicateConstantValue(ConstantValue* v) {
  // TODO
 
}

/******************* Object utilities ******************************/

Scope* createScope(Object* owner, Scope* outer) {
  Scope* scope = (Scope*) malloc(sizeof(Scope));
  scope->objList = NULL;
  scope->owner = owner;
  scope->outer = outer;
  return scope;
}

Object* createProgramObject(char *programName) {
  Object* program = (Object*) malloc(sizeof(Object));
  strcpy(program->name, programName);
  program->kind = OBJ_PROGRAM;
  program->progAttrs = (ProgramAttributes*) malloc(sizeof(ProgramAttributes));
  program->progAttrs->scope = createScope(program,NULL);
  symtab->program = program;

  return program;
}

Object* createConstantObject(char *name) {
  // TODO
  Object* constan = (Object*)malloc(sizeof(Object));
  strcpy(constan->name,name);
  constan->kind = OBJ_CONSTANT;
  constan->constAttrs = (ConstantAttributes*)malloc(sizeof(ConstantAttributes));
  constan->constAttrs->value = NULL;
  return constan;

}

Object* createTypeObject(char *name) {
  // TODO
  Object * type = (Object*)malloc(sizeof(Object));
  strcpy(type->name,name);
  type->kind = OBJ_TYPE;
  type->typeAttrs = (TypeAttributes*)malloc(sizeof(TypeAttributes));
  type->typeAttrs->actualType = NULL;
  return type;
}

Object* createVariableObject(char *name) {
  // TODO
  Object* val = (Object*)malloc (sizeof(Object));
  strcpy(val->name,name);
  val->kind = OBJ_VARIABLE;
  val->varAttrs = (VariableAttributes*)malloc(sizeof(VariableAttributes));
  val->varAttrs->type = NULL;
  val->varAttrs->scope = symtab->currentScope;
  return val;

}

Object* createFunctionObject(char *name) {
  // TODO
  Object* function = (Object*)malloc(sizeof(Object));
  strcpy(function->name,name);
  function->kind = OBJ_FUNCTION;
  function->funcAttrs= (FunctionAttributes*)malloc(sizeof(FunctionAttributes));
  function->funcAttrs->paramList = NULL;
  function->funcAttrs->scope =createScope(function,symtab->currentScope);
  function->funcAttrs->returnType = NULL;
  return function;

}

Object* createProcedureObject(char *name) {
  // TODO
Object * proced = (Object*)malloc(sizeof(Object));
strcpy(proced->name,name);
proced->kind = OBJ_PROCEDURE;
proced->procAttrs = (ProcedureAttributes*)malloc(sizeof(ProcedureAttributes));
proced->procAttrs->paramList = NULL;
proced->procAttrs->scope = createScope(proced,symtab->currentScope);
return proced;
}

Object* createParameterObject(char *name, enum ParamKind kind, Object* owner) {
  // TODO
  Object * praram = (Object*)malloc(sizeof(Object));
strcpy(praram->name,name);
praram->kind = OBJ_PARAMETER;
praram->paramAttrs = (ParameterAttributes*)malloc(sizeof(ParameterAttributes));
praram->paramAttrs->function = NULL;
praram->paramAttrs->type = NULL;
return praram;
}

void freeObject(Object* obj) {
  // TODO
}

void freeScope(Scope* scope) {
  // TODO
}

void freeObjectList(ObjectNode *objList) {
  // TODO
}

void freeReferenceList(ObjectNode *objList) {
  // TODO
}
// them đối tượng
void addObject(ObjectNode **objList, Object* obj) {
  ObjectNode* node = (ObjectNode*) malloc(sizeof(ObjectNode));
  node->object = obj;
  node->next = NULL;
  if ((*objList) == NULL) 
    *objList = node;
  else {
    ObjectNode *n = *objList;
    while (n->next != NULL) 
      n = n->next;
    n->next = node;
  }
}

Object* findObject(ObjectNode *objList, char *name) {
  // TODO
}

/******************* others ******************************/

void initSymTab(void) {
  Object* obj;
  Object* param;

  symtab = (SymTab*) malloc(sizeof(SymTab));
  symtab->globalObjectList = NULL;
  
  obj = createFunctionObject("READC");
  obj->funcAttrs->returnType = makeCharType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createFunctionObject("READI");
  obj->funcAttrs->returnType = makeIntType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEI");
  param = createParameterObject("i", PARAM_VALUE, obj);
  param->paramAttrs->type = makeIntType();
  addObject(&(obj->procAttrs->paramList),param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEC");
  param = createParameterObject("ch", PARAM_VALUE, obj);
  param->paramAttrs->type = makeCharType();
  addObject(&(obj->procAttrs->paramList),param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITELN");
  addObject(&(symtab->globalObjectList), obj);

  intType = makeIntType();
  charType = makeCharType();
}

void cleanSymTab(void) {
  freeObject(symtab->program);
  freeObjectList(symtab->globalObjectList);
  free(symtab);
  freeType(intType);
  freeType(charType);
}

void enterBlock(Scope* scope) {
  symtab->currentScope = scope;
}

void exitBlock(void) {
  symtab->currentScope = symtab->currentScope->outer;
}

void declareObject(Object* obj) {
  if (obj->kind == OBJ_PARAMETER) {
    Object* owner = symtab->currentScope->owner;
    switch (owner->kind) {
    case OBJ_FUNCTION:
      addObject(&(owner->funcAttrs->paramList), obj);
      break;
    case OBJ_PROCEDURE:
      addObject(&(owner->procAttrs->paramList), obj);
      break;
    default:
      break;
    }
  }
 
  addObject(&(symtab->currentScope->objList), obj);
}


