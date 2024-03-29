
#ifndef __CHARCODE_H__
#define __CHARCODE_H__

typedef enum {
  CHAR_SPACE,// khoảng trống
  CHAR_LETTER,//chữ cái
  CHAR_DIGIT,// chữ số
  CHAR_PLUS,// '+'
  CHAR_MINUS,// '-'
  CHAR_TIMES,// '*'
  CHAR_SLASH,// '/'
  CHAR_LT,// '<'
  CHAR_GT,// '>'
  CHAR_EXCLAIMATION,// '!'
  CHAR_EQ,// '='
  CHAR_COMMA,// ','
  CHAR_PERIOD,// '.'
  CHAR_COLON,//  ':'
  CHAR_SEMICOLON,// ';'
  CHAR_SINGLEQUOTE,// '\'
  CHAR_LPAR,// '('
  CHAR_RPAR,// ')'
  CHAR_UNKNOWN, // 'ký tự ngoài bảng chữ cái '
  CHAR_GD
} CharCode;

#endif