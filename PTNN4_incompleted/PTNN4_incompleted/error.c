/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include "error.h"

#define NUM_OF_ERRORS 29

struct ErrorMessage {
  ErrorCode errorCode;
  char *message;
};

struct ErrorMessage errors[29] = {
  {ERR_END_OF_COMMENT, "End of comment expected."},
  {ERR_IDENT_TOO_LONG, "Identifier too long."},
  {ERR_INVALID_CONSTANT_CHAR, "Invalid char constant."},
  {ERR_INVALID_SYMBOL, "Invalid symbol."},
 {ERR_INVALID_IDENT, "An identifier expected."},// một mã định danh không mong đợi
  {ERR_INVALID_CONSTANT, "A constant expected."},
  {ERR_INVALID_TYPE, "A type expected."},// không phải kiểu mong đợi
  {ERR_INVALID_BASICTYPE, "A basic type expected."},
  {ERR_INVALID_VARIABLE, "A variable expected."},
  {ERR_INVALID_FUNCTION, "A function identifier expected."},// một hàm định danh đang mọng đợi
  {ERR_INVALID_PROCEDURE, "A procedure identifier expected."},
  {ERR_INVALID_PARAMETER, "A parameter expected."},// tham số không mong đợi
  {ERR_INVALID_STATEMENT, "Invalid statement."},// tuyên bố không hợp lệ
  {ERR_INVALID_COMPARATOR, "A comparator expected."},// mã so sánh mong đợi
  {ERR_INVALID_EXPRESSION, "Invalid expression."},// biểu thức không hợp lệ
  {ERR_INVALID_TERM, "Invalid term."},//Điều khoản không hợp lệ
  {ERR_INVALID_FACTOR, "Invalid factor."},// yếu tố không hợp lệ
  {ERR_INVALID_LVALUE, "Invalid lvalue in assignment."},
  {ERR_INVALID_ARGUMENTS, "Wrong arguments."},//Lập luận sai
  {ERR_UNDECLARED_IDENT, "Undeclared identifier."},// định danh chưa được khai báo
  {ERR_UNDECLARED_CONSTANT, "Undeclared constant."},// hằng số chưa được khai báo
  {ERR_UNDECLARED_INT_CONSTANT, "Undeclared integer constant."},
  {ERR_UNDECLARED_TYPE, "Undeclared type."},// kiểu chưa được khai báo
  {ERR_UNDECLARED_VARIABLE, "Undeclared variable."},// biến chưa được khai báo
  {ERR_UNDECLARED_FUNCTION, "Undeclared function."},// hàm chưa được khai báo
  {ERR_UNDECLARED_PROCEDURE, "Undeclared procedure."},// thủ tục chưa khai báo
  {ERR_DUPLICATE_IDENT, "Duplicate identifier."},// định danh bị trùng lặp
  {ERR_TYPE_INCONSISTENCY, "Type inconsistency"},// không nhất quán về kiểu
  {ERR_PARAMETERS_ARGUMENTS_INCONSISTENCY, "The number of arguments and the number of parameters are inconsistent."}// đối số và tham số không nhất quán
};

void error(ErrorCode err, int lineNo, int colNo) {
  int i;
  for (i = 0 ; i < NUM_OF_ERRORS; i ++) 
    if (errors[i].errorCode == err) {
      printf("%d-%d:%s\n", lineNo, colNo, errors[i].message);
      exit(0);
    }
}

void missingToken(TokenType tokenType, int lineNo, int colNo) {
  printf("%d-%d:Missing %s\n", lineNo, colNo, tokenToString(tokenType));
  exit(0);
}

void assert(char *msg) {
  printf("%s\n", msg);
}
