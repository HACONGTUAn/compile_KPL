/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __SYMTAB_H__
#define __SYMTAB_H__

#define MAX_IDENT_LEN 15
// phân loại kiểu
enum TypeClass {
  TP_INT,
  TP_CHAR,
  TP_ARRAY
};
// phân loại ký hiệu 
enum ObjectKind {
  OBJ_CONSTANT, // hằng số
  OBJ_VARIABLE, // biến số
  OBJ_TYPE,     //kiểu
  OBJ_FUNCTION, //hàm
  OBJ_PROCEDURE,//thủ tục
  OBJ_PARAMETER,//tham số
  OBJ_PROGRAM   //chương trình
};
// loại tham số
enum ParamKind {
  PARAM_VALUE,    // giá trị tham số
  PARAM_REFERENCE // tham chiếu tham số
};

struct Type_ {
  enum TypeClass typeClass;
  int arraySize;
  struct Type_ *elementType;
};

typedef struct Type_ Type;
typedef struct Type_ BasicType;

//hằng
struct ConstantValue_ {
  enum TypeClass type;
  union {
    int intValue;
    char charValue;
  };
};

typedef struct ConstantValue_ ConstantValue;// giá trị hiện có

struct Scope_;
struct ObjectNode_;
struct Object_;
// thuộc tính không đổi
struct ConstantAttributes_ {
  ConstantValue* value;
};

struct VariableAttributes_ {
  Type *type;
  // pham vi của biên (sử dụng cho pha sinh mã)
  struct Scope_ *scope;
};
// nhập thuộc tính
struct TypeAttributes_ {
  Type *actualType; // loại thực tế
};
// thuộc tính thủ tục
struct ProcedureAttributes_ {
  struct ObjectNode_ *paramList;// danh sách tham số
  struct Scope_* scope;// pham vi
};
// thuộc tính hàm
struct FunctionAttributes_ {
  struct ObjectNode_ *paramList;
  Type* returnType;
  struct Scope_ *scope;
};
// thuộc tính chương trình
struct ProgramAttributes_ {
  struct Scope_ *scope;
};
// thuộc tính tham số
struct ParameterAttributes_ {
  enum ParamKind kind;
  Type* type;
  struct Object_ *function;
};

typedef struct ConstantAttributes_ ConstantAttributes; // thuộc tính hằng số
typedef struct TypeAttributes_ TypeAttributes;         // thuộc tính kiểu
typedef struct VariableAttributes_ VariableAttributes;// thuộc tính biến
typedef struct FunctionAttributes_ FunctionAttributes;// thuộc tính hàm
typedef struct ProcedureAttributes_ ProcedureAttributes;// thuốc tính thủ tục
typedef struct ProgramAttributes_ ProgramAttributes;// thuộc tính chương trình
typedef struct ParameterAttributes_ ParameterAttributes;// thuộc tính tham số
// thuộc tính của đối tượng trên bảng ký hiệu
struct Object_ {
  char name[MAX_IDENT_LEN];
  enum ObjectKind kind;
  union {
    ConstantAttributes* constAttrs; // thuộc tính không đổi
    VariableAttributes* varAttrs;   // thuộc tính biến
    TypeAttributes* typeAttrs;      // thuộc tính type
    FunctionAttributes* funcAttrs;  //
    ProcedureAttributes* procAttrs; //
    ProgramAttributes* progAttrs;   //
    ParameterAttributes* paramAttrs;//
  };
};

typedef struct Object_ Object;

struct ObjectNode_ {
  Object *object;
  struct ObjectNode_ *next;
};

typedef struct ObjectNode_ ObjectNode;
// phạm vi
struct Scope_ {
  ObjectNode *objList;
  Object *owner;
  struct Scope_ *outer;
};

typedef struct Scope_ Scope;
// Bảng ký hiệu 
struct SymTab_ {
  // chương trình chính
  Object* program;
  // phạm vi hiện tại
  Scope* currentScope;
  // các đối tượng toàn cục như
  // ham WRITEI,WRITEC,WRITELN
  //READI,READC
  ObjectNode *globalObjectList;
};

typedef struct SymTab_ SymTab;

Type* makeIntType(void);
Type* makeCharType(void);
Type* makeArrayType(int arraySize, Type* elementType);
Type* duplicateType(Type* type);
int compareType(Type* type1, Type* type2);
void freeType(Type* type);

ConstantValue* makeIntConstant(int i);
ConstantValue* makeCharConstant(char ch);
ConstantValue* duplicateConstantValue(ConstantValue* v);

Scope* createScope(Object* owner, Scope* outer);

Object* createProgramObject(char *programName);
Object* createConstantObject(char *name);
Object* createTypeObject(char *name);
Object* createVariableObject(char *name);
Object* createFunctionObject(char *name);
Object* createProcedureObject(char *name);
Object* createParameterObject(char *name, enum ParamKind kind, Object* owner);

Object* findObject(ObjectNode *objList, char *name);

void initSymTab(void);
void cleanSymTab(void);
void enterBlock(Scope* scope);
void exitBlock(void);
void declareObject(Object* obj);

#endif
