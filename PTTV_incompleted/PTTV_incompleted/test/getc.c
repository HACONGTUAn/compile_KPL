#include<stdio.h>
FILE *inputStream;
int currentChar;
int lineNo, colNo;
int readChar(void) {
  currentChar = getc(inputStream);// đọc 1 ký tự từ file
  colNo ++;
  if (currentChar == '\n') {
    lineNo ++;
    colNo = 0;
  }
  return currentChar;// trả về giá trị nguyên của currentchar
}
openInputStream() {
  inputStream = fopen("example1.kpl", "rt");
    if (inputStream == NULL)
    {printf("Khong thay");}
    else printf("Da mo xong");

  lineNo = 1;
  colNo = 0;
  readChar();
 
}
int main()
{
 openInputStream();
   printf("%d",currentChar);
   printf("%d va %d",colNo,lineNo);
   return(0);
}