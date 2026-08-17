#include <stdio.h>
#include <string.h>
#include <windows.h>
#define MAX_TASK 50
#define TITLE_LENGTH 60
#define MAX_USER 50

char userName[30];
char userPassword[30];

typedef struct Task{
    char title[TITLE_LENGTH];  //  任务名称
    int isFinished;  // 1 完成 0 未完成
}Task;

Task AllTaskArr[MAX_TASK];  //  总任务数组
int AllTaskCount = 0;  //  总任务数量
int saveTaskCount = 0;  //  保存任务数量

void saveUser(){  //  注册用户
    scanf("%s", userName);
    scanf("%s", userPassword);
}
void saveNewTask(){  //  保存新任务
    scanf("%d", &saveTaskCount);  //  输入保存任务数

    int preIndex = AllTaskCount;
    AllTaskCount += saveTaskCount;

    for (int i = preIndex; i < AllTaskCount; i++) {  //  保存任务
        scanf("%s", AllTaskArr[i].title);
        scanf("%d", &AllTaskArr[i].isFinished);
    }
}
void saveFile(){
    FILE *fp = fopen("todo.txt","w");  //  文件指针

    if (fp == NULL) {  //  保存检验
        printf("文件保存失败！\n");
        return;
    }

    fprintf(fp, "%s %s \n%d\n", userName, userPassword, AllTaskCount);  //  保存用户名，密码，任务数量

    for(int  i = 0; i < AllTaskCount; i++) {  //  保存每个任务
        fprintf(fp, "%s %d\n", AllTaskArr[i].title, AllTaskArr[i].isFinished);
    }

    fclose(fp);  //  关闭文件
}
void loadFile(){
    FILE *fp = fopen("todo.txt","r");  //  文件指针

    if (fp == NULL) {  //  读入检验
        printf("原文件读取失败！\n");
        AllTaskCount = 0;
        return;
    }

    fscanf(fp,"%s %s %d", userName, userPassword, &AllTaskCount);  //  读入用户名，密码，任务数量

    for (int i = 0; i < AllTaskCount; i++) { //  读取任务
        fscanf(fp,"%s %d",AllTaskArr[i].title, &AllTaskArr[i].isFinished);
    }

    fclose(fp);  //  关闭文件
    printf("原文件成功读取！\n");
}

void showTask() {
    for (int i = 0; i < AllTaskCount; i++) {
        printf("|   %-60s |   ", AllTaskArr[i].title);
        if (AllTaskArr[i].isFinished == 1) printf("已完成   |\n");
        else printf("未完成   |\n");
    }
}
int main() {


    loadFile();  //  初始化文件
    system("pause");
    system("cls");

    printf("-------------------------------------------------------------------------------\n");
    printf("|                                时间计划助手                                 |\n");
    printf("|                                    登录                                     |\n");
    printf("|                              user:                                          |\n");
    printf("|                              password:                                      |\n");
    printf("|                                                                             |\n");
    printf("|                                                                             |\n");
    printf("-------------------------------------------------------------------------------\n");

    if (strcmp(userName, "") == 0 || strcmp(userPassword, "") == 0) {
        printf("请注册！");
        AllTaskCount = 0;
        saveUser();
    }

    int op;
    //while (1) {
        printf("\n1 添加任务\n2 输出任务\n");
        scanf("%d", &op);
        switch (op)
        {
        case 1:
            saveNewTask();
            saveFile();
            break;
        case 2:
            showTask();
            break;
        default:
            break;
    //    }
    }
}