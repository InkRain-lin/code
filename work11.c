#include <stdio.h>
#include <windows.h>
int main(){
    char name[60];
    int age;
    printf("输入名字年龄\n");
    scanf("%s%d", name, &age);
    printf("--------------------\n");
    printf("%s\n%d", name, age);
}