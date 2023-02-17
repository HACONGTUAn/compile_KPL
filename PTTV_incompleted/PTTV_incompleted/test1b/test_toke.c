#include <stdlib.h>
#include <ctype.h>
#include "test_toke.h"
#include <stdio.h>
struct {
  char string[MAX_IDENT_LEN + 1];
  TokenType tokenType;
} keywords[KEYWORDS_COUNT] = {
  {"PROGRAM", KW_PROGRAM},
  {"CONST", KW_CONST},
  {"TYPE", KW_TYPE},
  {"VAR", KW_VAR},
  {"INTEGER", KW_INTEGER},
  {"CHAR", KW_CHAR},
  {"ARRAY", KW_ARRAY},
  {"OF", KW_OF},
  {"FUNCTION", KW_FUNCTION},
  {"PROCEDURE", KW_PROCEDURE},
  {"BEGIN", KW_BEGIN},
  {"END", KW_END},
  {"CALL", KW_CALL},
  {"IF", KW_IF},
  {"THEN", KW_THEN},
  {"ELSE", KW_ELSE},
  {"WHILE", KW_WHILE},
  {"DO", KW_DO},
  {"FOR", KW_FOR},
  {"TO", KW_TO}
};
// kiểm tra từ khóa ở đây không phân biệt chữ hoa chữ thường
int keywordEq(char *kw, char *string) {
  while ((*kw != '\0') && (*string != '\0')) {//'\0' ket thuc
    if (*kw != toupper(*string)) break;   // 'toupper(string) chuyển chữ thường thành hoa'
    kw ++; string ++;
  }
  return ((*kw == '\0') && (*string == '\0'));
}
// kiểm tra một xâu có là từ khóa không và trả về từ khóa (token ) đó
TokenType checkKeyword(char *string) {
  int i;
  for (i = 0; i < KEYWORDS_COUNT; i++)
    if (keywordEq(keywords[i].string, string)) 
      return keywords[i].tokenType;// trả về token
  return TK_NONE;
}

// Tạo một token mới với kiểu và vị trí

Token* makeToken(TokenType tokenType, int lineNo, int colNo) {
  Token *token = (Token*)malloc(sizeof(Token));
  token->tokenType = tokenType;
  token->lineNo = lineNo;
  token->colNo = colNo;
  return token;
}

