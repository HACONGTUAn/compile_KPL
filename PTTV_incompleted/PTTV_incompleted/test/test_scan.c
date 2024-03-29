#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "test_read.h"
#include "test_charcode.h"
#include "test_toke.h"
#include "test_error.h"



extern int lineNo;
extern int colNo;
extern int currentChar;

extern CharCode charCodes[];

/***************************************************************/

void skipBlank() {
  while (currentChar != EOF && charCodes[currentChar] == CHAR_SPACE)
readChar();
}
// bỏ qua comment
void skipComment() {
  readChar();// đọc ký tự vào
  int ln_dau = lineNo;
  int trang_thai = 0;
  while (currentChar != EOF && trang_thai < 2)
  { 
   
if(charCodes[currentChar] == CHAR_TIMES){
  trang_thai = 1;
  }
 else if(charCodes[currentChar] == CHAR_SLASH){
        if(trang_thai == 1) {trang_thai = 2; readChar(); break;}
        else trang_thai = 0;
  }
if(ln_dau != lineNo ){ trang_thai = 2; break;}

   readChar();
  }
 
  if(trang_thai != 2 ) error(ERR_ENDOFCOMMENT,lineNo,colNo);
  
}

Token* readIdentKeyword(void) {
  Token * token= makeToken(TK_NONE, lineNo,colNo); // tạo 1 toke mới với kiểu là NONE  = 'một lỗi ' tại vị trí lineNO va colNo
  int dem = 1;

  token->string[0] = (char)currentChar;
  readChar(); // đọc ký tự

  while((currentChar != EOF )&&((charCodes[currentChar]==CHAR_LETTER)|| (charCodes[currentChar] == CHAR_DIGIT) || (charCodes[currentChar] == CHAR_GD))){
    if (dem <= MAX_IDENT_LEN) token->string[dem++] = (char)currentChar;
    readChar();
  }

  token->string[dem] = '\0';
  token->tokenType = checkKeyword(token->string);
  if (token->tokenType == TK_NONE)
  {
    token->tokenType = TK_IDENT;
  }
  return token;
}


// đọc 1 số
Token* readNumber(void) {
  Token *token = makeToken(TK_NUMBER,lineNo,colNo); // tạo 1 token kiểu số
  int dem =0 ;
  while ((currentChar != EOF) && (charCodes[currentChar] == CHAR_DIGIT) )// kí tự vào khác EOF và == kiểu chữ số thì thực hiện lặp
  {
    if (dem > 9)
    {
      error(ERR_IDENTTOOLONG, token -> lineNo, token->colNo);
    }
    
    token -> string[dem++] = (char)currentChar;
    readChar();
  }
  token -> string[dem] = '\0';
  token -> value = atoi(token->string);
  return token;
}
// đọc hằng ký tự
Token* readConstChar(void) {
  Token *token =makeToken(TK_CHAR,lineNo,colNo);
  readChar();
  if (currentChar == -1)
  {
    error(ERR_INVALIDCHARCONSTANT,token->lineNo,token->colNo);
  }
  
  if(currentChar == EOF){
    token -> tokenType = TK_NONE;
    error(ERR_INVALIDCHARCONSTANT,token->lineNo,token->colNo);
    return token;
  }

  if(charCodes[currentChar]==CHAR_SINGLEQUOTE){
    readChar();
    return token;
  }else{
    token ->tokenType = TK_NONE;
    error(ERR_INVALIDCHARCONSTANT,token->lineNo,token->colNo);
    return token;
  }
}
// kiem tra 1 dinh danh co trung voi tu khoa hay khong 
void kt_dinhdanh(Token * token){
  int n;
  for(n =0 ; n < KEYWORDS_COUNT ; n++){
checkKeyword(token->string);
  }
}

// đọc 1 token tính từ vị trí hiện tại
Token* getToken(void) {
  Token *token;
  int ln, cn;

  if (currentChar == EOF) 
    return makeToken(TK_EOF, lineNo, colNo);

  switch (charCodes[currentChar]) {
  case CHAR_SPACE: skipBlank(); return getToken();
  case CHAR_LETTER: return readIdentKeyword();
  case CHAR_GD: return readIdentKeyword();
  case CHAR_DIGIT: return readNumber();

  case CHAR_SEMICOLON:  
  token = makeToken(SB_SEMICOLON, lineNo, colNo);
    readChar();
  return token;

  case CHAR_PERIOD:
  ln = lineNo;
  cn = colNo;
  readChar();
  if(currentChar != EOF && charCodes[currentChar]==CHAR_RPAR){
    readChar();
    token = makeToken(SB_RSEL,ln,cn);
  }
    else token = makeToken(SB_PERIOD, lineNo, colNo);
  return token;

  case CHAR_PLUS: 
    token = makeToken(SB_PLUS, lineNo, colNo);
    readChar(); 
    return token;

  case CHAR_MINUS :
    token = makeToken(SB_MINUS,lineNo,colNo);
    readChar();
    return token; 

  case CHAR_TIMES:
    token = makeToken(SB_TIMES,lineNo,colNo);
    readChar();
    return token;
// comment bang '//' hoac /* */
  case CHAR_SLASH:
  ln = lineNo;
  cn = colNo;
    readChar();
    if(charCodes[currentChar] == CHAR_TIMES ){
      skipComment();
      return getToken();
    }
    if(charCodes[currentChar] == CHAR_SLASH){
 skipComment();
 return getToken();
    }
    else return makeToken(SB_SLASH,ln,cn);

  case CHAR_LPAR:
 /*   ln = lineNo;
    cn = colNo;
    readChar();
if(currentChar == EOF) return makeToken(SB_LSEL,ln,cn);
if(charCodes[currentChar]==CHAR_PERIOD){
  readChar();
  return makeToken(SB_LSEL,ln,cn);
}
else if (charCodes[currentChar]==CHAR_TIMES)
{ readChar();
skipComment();
return getToken();
}
else return makeToken(SB_LPAR,ln,cn);
*/
case CHAR_RPAR:
token = makeToken(SB_RPAR,lineNo,colNo);
readChar();
return token;
 
case CHAR_COLON:
ln = lineNo;
cn = colNo;
readChar();
if(currentChar != EOF && charCodes[currentChar]==CHAR_EQ){
readChar();
token = makeToken(SB_ASSIGN,ln,cn);
}
else token = makeToken(SB_COLON,ln,cn);
return token;

case CHAR_EQ:
token = makeToken(SB_EQ,lineNo,colNo);
readChar();
return token;

  default:
    token = makeToken(TK_NONE, lineNo, colNo);
    error(ERR_INVALIDSYMBOL, lineNo, colNo);
    readChar(); 
    return token;
  }
}


/******************************************************************/
// in tocken
void printToken(Token *token) {

  printf("%d-%d:", token->lineNo, token->colNo);

  switch (token->tokenType) {
  case TK_NONE: printf("TK_NONE\n"); break;
  case TK_IDENT: printf("TK_IDENT(%s)\n", token->string); break;
  case TK_NUMBER: printf("TK_NUMBER(%s)\n", token->string); break;
  case TK_CHAR: printf("TK_CHAR(\'%s\')\n", token->string); break;
  case TK_EOF: printf("TK_EOF\n"); break;

  case KW_PROGRAM: printf("KW_PROGRAM\n"); break;
  case KW_CONST: printf("KW_CONST\n"); break;
  case KW_TYPE: printf("KW_TYPE\n"); break;
  case KW_VAR: printf("KW_VAR\n"); break;
  case KW_INTEGER: printf("KW_INTEGER\n"); break;
  case KW_CHAR: printf("KW_CHAR\n"); break;
  case KW_ARRAY: printf("KW_ARRAY\n"); break;
  case KW_OF: printf("KW_OF\n"); break;
  case KW_FUNCTION: printf("KW_FUNCTION\n"); break;
  case KW_PROCEDURE: printf("KW_PROCEDURE\n"); break;
  case KW_BEGIN: printf("KW_BEGIN\n"); break;
  case KW_END: printf("KW_END\n"); break;
  case KW_CALL: printf("KW_CALL\n"); break;
  case KW_IF: printf("KW_IF\n"); break;
  case KW_THEN: printf("KW_THEN\n"); break;
  case KW_ELSE: printf("KW_ELSE\n"); break;
  case KW_WHILE: printf("KW_WHILE\n"); break;
  case KW_DO: printf("KW_DO\n"); break;
  case KW_FOR: printf("KW_FOR\n"); break;
  case KW_TO: printf("KW_TO\n"); break;

  case SB_SEMICOLON: printf("SB_SEMICOLON\n"); break;
  case SB_COLON: printf("SB_COLON\n"); break;
  case SB_PERIOD: printf("SB_PERIOD\n"); break;
  case SB_COMMA: printf("SB_COMMA\n"); break;
  case SB_ASSIGN: printf("SB_ASSIGN\n"); break;
  case SB_EQ: printf("SB_EQ\n"); break;
  case SB_NEQ: printf("SB_NEQ\n"); break;
  case SB_LT: printf("SB_LT\n"); break;
  case SB_LE: printf("SB_LE\n"); break;
  case SB_GT: printf("SB_GT\n"); break;
  case SB_GE: printf("SB_GE\n"); break;
  case SB_PLUS: printf("SB_PLUS\n"); break;
  case SB_MINUS: printf("SB_MINUS\n"); break;
  case SB_TIMES: printf("SB_TIMES\n"); break;
  case SB_SLASH: printf("SB_SLASH\n"); break;
  case SB_LPAR: printf("SB_LPAR\n"); break;
  case SB_RPAR: printf("SB_RPAR\n"); break;
  case SB_LSEL: printf("SB_LSEL\n"); break;
  case SB_RSEL: printf("SB_RSEL\n"); break;
  }
}
// quet file va in token
int scan(char *fileName) {
  Token *token;

  if (openInputStream(fileName) == IO_ERROR) 
    return IO_ERROR;

  token = getToken();
  while (token->tokenType != TK_EOF) {
    printToken(token);
    free(token);
    token = getToken();
  }

  free(token);
  closeInputStream();
  return IO_SUCCESS;
}

/******************************************************************/

int main() {

char x = 'example2.kpl';
  if(scan(x) == IO_ERROR){
    return -1;
  }
    //return -1;
    getch();
  }
    