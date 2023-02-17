/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include "reader.h"

FILE *inputStream;
int lineNo, colNo; // số dòng và số cột hiện tại
int currentChar; // ký tự hiện tại
// đọc ký tự từ kênh vào
int readChar(void) {
  currentChar = getc(inputStream);// đọc 1 ký tự từ file
  colNo ++;       // cot tang len 
  if (currentChar == '\n') {
    lineNo ++;    // dong tang len 
    colNo = 0;    // xet cot ve 0
  }
  return currentChar;// trả về giá trị nguyên của currentchar
}
// mở file vào
int openInputStream() {
  inputStream = fopen("example1.kpl", "rt");
    if (inputStream == NULL)
    {printf("Khong thay");return IO_ERROR;}
    else printf("Da mo xong \n");

  lineNo = 1;// dong =1 
  colNo = 0;// cot = 0
  readChar(); // doc ky tu vao
  return IO_SUCCESS; // tra ve IO_SUCCESS
}
// đóng file 
void closeInputStream() {
  fclose(inputStream);
}

