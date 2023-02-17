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
  {ERR_IDENT_TOO_LONG, "Identifier too long."},// định danh quá dài
  {ERR_INVALID_CONSTANT_CHAR, "Invalid char constant."},// hằng số kiểu char không hợp lệ
  {ERR_INVALID_SYMBOL, "Invalid symbol."},// ký hiệu không hợp lệ
  {ERR_INVALID_IDENT, "An identifier expected."},// định danh không hợp lệ
  {ERR_INVALID_CONSTANT, "A constant expected."},// hằng số không hợp lệ
  {ERR_INVALID_TYPE, "A type expected."},// kiểu không hợp lệ
  {ERR_INVALID_BASICTYPE, "A basic type expected."},// kiểu cơ bản không hợp lệ
  {ERR_INVALID_VARIABLE, "A variable expected."},// biến không hợp lệ
  {ERR_INVALID_FUNCTION, "A function identifier expected."},// định danh fun không hợp lệ
  {ERR_INVALID_PROCEDURE, "A procedure identifier expected."},// định danh proc không hợp lệ
  {ERR_INVALID_PARAMETER, "A parameter expected."},// tham số không hợp lệ
  {ERR_INVALID_STATEMENT, "Invalid statement."},// câu lệnh không hợp lệ
  {ERR_INVALID_COMPARATOR, "A comparator expected."},
  {ERR_INVALID_EXPRESSION, "Invalid expression."},
  {ERR_INVALID_TERM, "Invalid term."},
  {ERR_INVALID_FACTOR, "Invalid factor."},
  {ERR_INVALID_LVALUE, "Invalid lvalue in assignment."},
  {ERR_INVALID_ARGUMENTS, "Wrong arguments."},
  {ERR_UNDECLARED_IDENT, "Undeclared identifier."},
  {ERR_UNDECLARED_CONSTANT, "Undeclared constant."},
  {ERR_UNDECLARED_INT_CONSTANT, "Undeclared integer constant."},
  {ERR_UNDECLARED_TYPE, "Undeclared type."},
  {ERR_UNDECLARED_VARIABLE, "Undeclared variable."},
  {ERR_UNDECLARED_FUNCTION, "Undeclared function."},
  {ERR_UNDECLARED_PROCEDURE, "Undeclared procedure."},
  {ERR_DUPLICATE_IDENT, "Duplicate identifier."},
  {ERR_TYPE_INCONSISTENCY, "Type inconsistency"},
  {ERR_PARAMETERS_ARGUMENTS_INCONSISTENCY, "The number of arguments and the number of parameters are inconsistent."}
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
