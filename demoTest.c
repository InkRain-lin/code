#include <stdio.h>
#include <string.h>
#include <windows.h>
#define MAX_TASK 50
#define TITLE_LENGTH 60
#define MAX_USER 50
typedef struct Task{
    char title[TITLE_LENGTH];  //  任务名称
    int isFinished;  // 1 完成 0 未完成
}Task;

Task AllTaskArr[MAX_TASK];  //  总任务数组
int AllTaskCount = 0;  //  总任务数量
int saveTaskCount = 0;  //  保存任务数量
// 光标跳转函数
void gotoxy(int x, int y)
{
    COORD pos;
    pos.X = x - 1;
    pos.Y = y - 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
int saveTaskCount;
char a[TITLE_LENGTH];
int b;
int index;
void cleanAllFinishedTask(){}
int main() {
    system("cls");
    printf("-------------------------------------------------------------------------------\n");
    printf("|                                查询任务                                     |\n");
    printf("|序号| 任务名称                              | 所需时间 | 截止日期 | 完成情况 |\n");
    showTask();  //  查询
    printf("|                                                                             |\n");
    printf("|                                0 返回                                       |\n");
    printf("-------------------------------------------------------------------------------\n");
}