/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __DEBUG_H__
#define __DEBUG_H_

#include "symtab.h"
// in thông tin kiểu
void printType(Type* type);

void printConstantValue(ConstantValue* value);
// in thông tin đối tượng
void printObject(Object* obj, int indent);
// in danh sách danh sách đối tượng
void printObjectList(ObjectNode* objList, int indent);
// in block
void printScope(Scope* scope, int indent);

#endif
