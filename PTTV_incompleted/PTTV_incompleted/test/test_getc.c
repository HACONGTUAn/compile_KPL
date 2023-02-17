#include<stdio.h>
int dem;
int main(){
char c;
printf("%d-","/n");
for(dem=0 ; dem<10;dem++){
c=getc(stdin);//đọc 1 ký tự vào và đến EOF thì k đọc đc j nữa 
printf("%d-",dem);// đếm tăng lên tương ứng với cột tăng lên => con trỏ chuột được đưa đến vị trí tiếp theo
// đọc ra ký tự 
}

}