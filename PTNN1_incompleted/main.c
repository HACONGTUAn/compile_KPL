/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>

#include "symtab.h"
#include "debug.h"

extern SymTab* symtab;
/******************************************************************/

int main() {
  Object* obj;

  initSymTab();

  obj = createProgramObject("PRG"); // tao mot doi tuong chuong trinh 
  enterBlock(obj->progAttrs->scope);// vao 1 khoi 

  obj = createConstantObject("c1"); // tao mot doi tuong hang so 
  obj->constAttrs->value = makeIntConstant(10);// co gia tri hang so = 10 
  declareObject(obj);// them doi tuong vao block hien tai 

  obj = createConstantObject("c2"); // tao mot doi tuong hang so c2
  obj->constAttrs->value = makeCharConstant('a');// co gia tri la 'a'
  declareObject(obj);// them doi tuong vao block hien tai

  obj = createTypeObject("t1");// tao mot doi tuong kieu 
  obj->typeAttrs->actualType = makeArrayType(10,makeIntType());// thuoc tinh kieu la 1 mang 10 phan tu 
  declareObject(obj);// them doi tuong vao block

  obj = createVariableObject("v1");
  obj->varAttrs->type = makeIntType();
  declareObject(obj);

  obj = createVariableObject("v2");
  obj->varAttrs->type = makeArrayType(10,makeArrayType(10,makeIntType()));
  declareObject(obj);

  obj = createFunctionObject("f");// tao ham 
  obj->funcAttrs->returnType = makeIntType();// kieu int
  declareObject(obj);// them vao block hien tai
  
    enterBlock(obj->funcAttrs->scope);// vao 1 khoi (khoi cu function)
 
    obj = createParameterObject("p1", PARAM_VALUE, symtab->currentScope->owner);
    obj->paramAttrs->type = makeIntType();
    declareObject(obj);

    obj = createParameterObject("p2", PARAM_REFERENCE, symtab->currentScope->owner);
    obj->paramAttrs->type = makeCharType();
    declareObject(obj);

    exitBlock();// ra khoi khoi (function)

  obj = createProcedureObject("p");
  declareObject(obj);
  
    enterBlock(obj->procAttrs->scope);
 
    obj = createParameterObject("v1", PARAM_VALUE, symtab->currentScope->owner);
    obj->paramAttrs->type = makeIntType();
    declareObject(obj);

    obj = createConstantObject("c1");
    obj->constAttrs->value = makeCharConstant('a');
    declareObject(obj);
    
    obj = createConstantObject("c3");
    obj->constAttrs->value = makeIntConstant(10);
    declareObject(obj);

    obj = createTypeObject("t1");
    obj->typeAttrs->actualType = makeIntType();
    declareObject(obj);

    obj = createTypeObject("t2");
    obj->typeAttrs->actualType = makeArrayType(10,makeIntType());
    declareObject(obj);

    obj = createVariableObject("v2");
    obj->varAttrs->type = makeArrayType(10,makeIntType());
    declareObject(obj);

    obj = createVariableObject("v3");
    obj->varAttrs->type = makeCharType();
    declareObject(obj);

    exitBlock();


  exitBlock();
  printObject(symtab->program,0);
  cleanSymTab();
    
  return 0;
}
