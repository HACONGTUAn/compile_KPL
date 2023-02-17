/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __DEBUG_H__
#define __DEBUG_H_

#include "symtab.h"
// in kiểu
void printType(Type* type);
// in hằng số
void printConstantValue(ConstantValue* value);
// in đối tượng
void printObject(Object* obj, int indent);
// in danh sách đối tượng
void printObjectList(ObjectNode* objList, int indent);
// in phạm vi
void printScope(Scope* scope, int indent);

#endif
