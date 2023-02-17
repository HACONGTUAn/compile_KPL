/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __SYMTAB_H__
#define __SYMTAB_H__

#define MAX_IDENT_LEN 15

enum TypeClass {
  TP_INT,
  TP_CHAR,
  TP_ARRAY
};
// loại đối tượng 
enum ObjectKind {
  OBJ_CONSTANT, // đối tượng hằng số
  OBJ_VARIABLE, // đối tượng biến 
  OBJ_TYPE,     // đối tượng kiểu người dùng định nghĩa
  OBJ_FUNCTION, // đối tượng function 
  OBJ_PROCEDURE,// đối tượng procedure
  OBJ_PARAMETER,// đối tượng tham số
  OBJ_PROGRAM // đối tượng program
};
// loại tham số
enum ParamKind {
  PARAM_VALUE, // giá trị tham số
  PARAM_REFERENCE// phạm vi của tham số
};
// kiểu 
struct Type_ {
  enum TypeClass typeClass;//lớp kiểu cơ bản
  int arraySize;            // kích thước mảng
  struct Type_ *elementType;
};

typedef struct Type_ Type;// kiểu
typedef struct Type_ BasicType;// kiểu cơ bản

// giá trị hiện có
struct ConstantValue_ {
  enum TypeClass type;
  union {
    int intValue; // giá trị kiểu int
    char charValue;// giá trị kiểu value
  };
};

typedef struct ConstantValue_ ConstantValue;// giá trị hiện có

struct Scope_;// phạm vi
struct ObjectNode_;// nút đối tượng
struct Object_;// đối tượng
// thuộc tính không đổi
struct ConstantAttributes_ {
  ConstantValue* value;// giá trị
};
// thuộc tính biến 
struct VariableAttributes_ {
  Type *type;
  struct Scope_ *scope;// phạm vi của biến 
};
// thuộc tính kiểu
struct TypeAttributes_ {
  Type *actualType; // loại thực tế
};
// thuộc tính procedure
struct ProcedureAttributes_ {
  struct ObjectNode_ *paramList;// danh sách tham số trong procedure
  struct Scope_* scope;// phạm vi của procedure
};
// thuộc tính function
struct FunctionAttributes_ {
  struct ObjectNode_ *paramList;// danh sách tham trong function
  Type* returnType;
  struct Scope_ *scope;// phạm vi của funtion
};
// thuộc tính program
struct ProgramAttributes_ {
  struct Scope_ *scope;// phạm vi trong program
};
// thuộc tính tham số
struct ParameterAttributes_ {
  enum ParamKind kind;// loại tham số
  Type* type;
  struct Object_ *function;
};

typedef struct ConstantAttributes_ ConstantAttributes;
typedef struct TypeAttributes_ TypeAttributes;
typedef struct VariableAttributes_ VariableAttributes;
typedef struct FunctionAttributes_ FunctionAttributes;
typedef struct ProcedureAttributes_ ProcedureAttributes;
typedef struct ProgramAttributes_ ProgramAttributes;
typedef struct ParameterAttributes_ ParameterAttributes;
// đối tượng :
// tên đối tượng
// loại đối tượng 
struct Object_ {
  char name[MAX_IDENT_LEN];
  enum ObjectKind kind;
  union {
    ConstantAttributes* constAttrs;// thuộc tính không đổi
    VariableAttributes* varAttrs;// thược tính biến
    TypeAttributes* typeAttrs;// thuộc tính kiểu
    FunctionAttributes* funcAttrs;// thuộc tinh func
    ProcedureAttributes* procAttrs;// thuộc tính proc
    ProgramAttributes* progAttrs;// thuộc tính prog
    ParameterAttributes* paramAttrs;// thuộc tính thám số
  };
};

typedef struct Object_ Object;// Đối TƯợng

struct ObjectNode_ {
  Object *object;
  struct ObjectNode_ *next;
};

typedef struct ObjectNode_ ObjectNode;//Nút đối tượng

struct Scope_ {
  ObjectNode *objList;// danh sách đối tượng
  Object *owner;      // hàm ,thủ tục ct,tương ứng BLock
  struct Scope_ *outer;//  pham vi ngoài
};

typedef struct Scope_ Scope;// Phạm vi
// bang ky hieu 
struct SymTab_ {
  // chuong trinh chinh 
  Object* program;
  // pham vi hien tai
  Scope* currentScope;
  // cac doi tuong toan cuc nhu ham writei , writec,..Reacdi
  ObjectNode *globalObjectList;
};

typedef struct SymTab_ SymTab;// Bảng Ký Hiệu

Type* makeIntType(void);// tạo kiểu INT
Type* makeCharType(void);// tạo kiểu CHAR
Type* makeArrayType(int arraySize, Type* elementType);// tạo mảng
Type* duplicateType(Type* type);// tạo kiểu trùng lặp
int compareType(Type* type1, Type* type2); // so sanh kiểu 
void freeType(Type* type);// giải phóng kiểu 

ConstantValue* makeIntConstant(int i);// tạo giá trị hăngf số int
ConstantValue* makeCharConstant(char ch);// tạo giá trị hằng số kiểu char
ConstantValue* duplicateConstantValue(ConstantValue* v);// tạo giá trị hằng số trùng lặp
// tạo phạm vi
Scope* createScope(Object* owner, Scope* outer);

Object* createProgramObject(char *programName);// tạo đối tượng program
Object* createConstantObject(char *name);// tạo đối tượng hằng số
Object* createTypeObject(char *name);// tạo đối tượng kiểu 
Object* createVariableObject(char *name);// tạo đối tượng biến 
Object* createFunctionObject(char *name);// tạo đối tượng function
Object* createProcedureObject(char *name);// tạo đối tượng procedeure
Object* createParameterObject(char *name, enum ParamKind kind, Object* owner);
// tạo đối tượng tham số
Object* findObject(ObjectNode *objList, char *name);// tìm đối tượng
// trong bảng
void initSymTab(void);
// xóa bảng
void cleanSymTab(void);
// vào block
void enterBlock(Scope* scope);
// thoát khỏi block hiện tạo
void exitBlock(void);
// thêm đối tượng vào bảng
void declareObject(Object* obj);

#endif
