#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#define MAX_TASK 50
#define TITLE_LENGTH 38
#define MAX_USER 50

//  现在的时间
int nowYear = 0, nowMonth = 0, nowDay = 0;
//  用户，密码
char userName[30] = "";
char userPassword[30] = "";
//  输入用户，密码
char user[30] = "";
char password[30] = "";

typedef struct Task{
    char title[TITLE_LENGTH];  //  任务名称
    int isFinished;  // 1 完成 0 未完成
    int deadline[3];  //  年 月 日
    int time;  //  所需时长 min
    int degree;  //  重要程度 1--10
}Task;

//  函数声明
void valueTask(Task *task, char title[], int isFinished, int deadline[], int time, int degree);
void saveFile();
void loadFile();
void updateData();
int isRegisterUser();
void loginUser();
void showTask();
void cleanAllFinishedTask();
void cleanAllUnfinishedTask(int index);
void sortDeadline();
void sortDegree();
void sortTask();
void gotoxy(int x, int y);
void getSystemNowTime(int *sy, int *sm, int *sd);
void registerScreen();
void loginScreen();
void mainScreen();
void getTaskScreen();
void setNewWorkScreen();
void changeTaskScreen();
void cleanTaskScreen();

//  赋值Task变量
void valueTask(Task *task, char title[], int isFinished, int deadline[], int time, int degree) {
    strcpy(task->title, title);
    task->isFinished = isFinished;
    task->deadline[0] = deadline[0];
    task->deadline[1] = deadline[1];
    task->deadline[2] = deadline[2];
    task->time = time;
    task->degree = degree;
}

Task AllTaskArr[MAX_TASK];  //  总任务数组
int AllTaskCount = 0;  //  总任务数量
int saveTaskCount = 0;  //  保存任务数量
int deleteCount = 0;


//  保存文件
void saveFile(){
    FILE *fp = fopen("todo.txt","w");  //  文件指针

    if (fp == NULL) {  //  保存检验
        printf("文件保存失败！\n");
        return;
    }
    AllTaskCount -= deleteCount;  //  确认最终总任务数
    fprintf(fp, "%s %s \n%d\n", userName, userPassword, AllTaskCount);  //  保存用户名，密码，任务数量

    int i_max = AllTaskCount + deleteCount;  //  删除操作
    for(int  i = 0; i < i_max; i++) {  //  保存每个任务
        //  用于删除操作
        if (strcmp(AllTaskArr[i].title, "-1") != 0  && AllTaskArr[i].isFinished != -1
            && AllTaskArr[i].deadline[0] != -1 && AllTaskArr[i].deadline[1] != -1 && AllTaskArr[i].deadline[2] != -1
            && AllTaskArr[i].time != -1 && AllTaskArr[i].degree != -1) {
                fprintf(fp, "%s %d %d %d %d %d %d\n",
                    AllTaskArr[i].title, AllTaskArr[i].isFinished, //  名称 完成情况
                    AllTaskArr[i].deadline[0], AllTaskArr[i].deadline[1], AllTaskArr[i].deadline[2],  //  截止日期
                    AllTaskArr[i].time,  //  所需时间
                    AllTaskArr[i].degree);  //  重要程度
        }
    }

    fclose(fp);  //  关闭文件
    deleteCount = 0;  //  重置删除数
}
//  下载文件
void loadFile(){
    FILE *fp = fopen("todo.txt","r");  //  文件指针

    if (fp == NULL) {  //  读入检验
        printf("原文件读取失败！\n");
        AllTaskCount = 0;
        return;
    }

    fscanf(fp,"%s %s %d", userName, userPassword, &AllTaskCount);  //  读入用户名，密码，任务数量

    for (int i = 0; i < AllTaskCount; i++) { //  读取任务
        fscanf(fp,"%s %d %d %d %d %d %d",AllTaskArr[i].title, &AllTaskArr[i].isFinished,  //  名称 完成情况
            &AllTaskArr[i].deadline[0], &AllTaskArr[i].deadline[1], &AllTaskArr[i].deadline[2],  //  截止日期
            &AllTaskArr[i].time,  //  所需时间
            &AllTaskArr[i].degree);  //  重要程度
    }

    fclose(fp);  //  关闭文件
}
//  更新数据
void updateData() {
    sortTask();
    saveFile();
    loadFile();
}


//  注册
int isRegisterUser() {//  判断是否注册
    if (strcmp(userName, "") == 0 || strcmp(userPassword, "") == 0) {  //  0 不需要 1 需要
        AllTaskCount = 0;  //  初始总任务数为0
        return 1;
    }
    else return 0;
}
//  登录
void loginUser() {
    while (1) {
        loginScreen();
        if (strcmp(user, userName) == 0 && strcmp(password, userPassword) == 0) break;

        printf("                          账号或密码错误请重新输入！\n");
        system("pause");
    }
}


//  查询任务函数
void showTask() {
    for (int i = 0; i < AllTaskCount; i++) {
        printf("| %2d | %-38s|  %2dh%2dm  |%4d/%2d/%-2d|  ",i + 1, AllTaskArr[i].title, AllTaskArr[i].time / 60, AllTaskArr[i].time % 60, AllTaskArr[i].deadline[0], AllTaskArr[i].deadline[1], AllTaskArr[i].deadline[2]);  //  输出任务
        //  任务是否完成
        if (AllTaskArr[i].isFinished == 1) printf("已完成  |\n");
        else printf("未完成  |\n");
    }
}
//  清理所有已完成任务
void cleanAllFinishedTask() {
        //  修改为删除格式-1 -1 -1 -1 -1 -1 -1
    for (int i = 0; i < AllTaskCount; i++) {
        if (AllTaskArr[i].isFinished == 1) {
            int deleteIsFinished[3] = {-1, -1, -1};
            valueTask(&AllTaskArr[i],"-1", -1, deleteIsFinished, -1, -1);
            deleteCount++;  //  记录删除数
        }
    }
    updateData();  //  更新数据
}
//  清理未完成任务
void cleanAllUnfinishedTask(int index){
    //  修改为删除格式-1 -1 -1 -1 -1 -1 -1
    int deleteIsFinished[3] = {-1, -1, -1};
            valueTask(&AllTaskArr[index - 1],"-1", -1, deleteIsFinished, -1, -1);
    deleteCount++;  //  记录删除数
    updateData();  //  更新数据
}
//  排序  按截止日期  从小到大
void sortDeadline() {  //  冒泡排序
    for (int j = 0; j < AllTaskCount - 1; j++) {
        for (int i = 0; i < AllTaskCount - 1 - j; i++) {
            if (AllTaskArr[i].deadline[0] > AllTaskArr[i + 1].deadline[0]) {  //  年
                //  交换
                Task temp;
                valueTask(&temp, AllTaskArr[i + 1].title, AllTaskArr[i + 1].isFinished, AllTaskArr[i + 1].deadline, AllTaskArr[i + 1].time, AllTaskArr[i + 1].degree);
                valueTask(&AllTaskArr[i + 1], AllTaskArr[i].title, AllTaskArr[i].isFinished, AllTaskArr[i].deadline, AllTaskArr[i].time, AllTaskArr[i].degree);
                valueTask(&AllTaskArr[i], temp.title, temp.isFinished, temp.deadline, temp.time, temp.degree);
            }
            else if (AllTaskArr[i].deadline[0] == AllTaskArr[i + 1].deadline[0]) {
                if (AllTaskArr[i].deadline[1] > AllTaskArr[i + 1].deadline[1]) {  //  月
                //  交换
                Task temp;
                valueTask(&temp, AllTaskArr[i + 1].title, AllTaskArr[i + 1].isFinished, AllTaskArr[i + 1].deadline, AllTaskArr[i + 1].time, AllTaskArr[i + 1].degree);
                valueTask(&AllTaskArr[i + 1], AllTaskArr[i].title, AllTaskArr[i].isFinished, AllTaskArr[i].deadline, AllTaskArr[i].time, AllTaskArr[i].degree);
                valueTask(&AllTaskArr[i], temp.title, temp.isFinished, temp.deadline, temp.time, temp.degree);
                }
                else if (AllTaskArr[i].deadline[1] == AllTaskArr[i + 1].deadline[1]) {
                    if (AllTaskArr[i].deadline[2] > AllTaskArr[i + 1].deadline[2]) {  //  日
                        //  交换
                        Task temp;
                        valueTask(&temp, AllTaskArr[i + 1].title, AllTaskArr[i + 1].isFinished, AllTaskArr[i + 1].deadline, AllTaskArr[i + 1].time, AllTaskArr[i + 1].degree);
                        valueTask(&AllTaskArr[i + 1], AllTaskArr[i].title, AllTaskArr[i].isFinished, AllTaskArr[i].deadline, AllTaskArr[i].time, AllTaskArr[i].degree);
                        valueTask(&AllTaskArr[i], temp.title, temp.isFinished, temp.deadline, temp.time, temp.degree);
                    }
                }
            }
        }
    }
}
//  排序  按重要程度  从大到小
void sortDegree() {  //  冒泡排序
    for (int j = 0; j < AllTaskCount - 1; j++) {
        for (int i = 0; i < AllTaskCount - 1 - j; i++) {
            //  同一日期比较
            if (AllTaskArr[i].deadline[0] == AllTaskArr[i + 1].deadline[0]  //  年
                && AllTaskArr[i].deadline[1] == AllTaskArr[i + 1].deadline[1]  //  月
                && AllTaskArr[i].deadline[2] == AllTaskArr[i + 1].deadline[2]  //  日
                && AllTaskArr[i].degree < AllTaskArr[i + 1].degree) {  //  重要程度比较
                //  交换
                Task temp;
                valueTask(&temp, AllTaskArr[i + 1].title, AllTaskArr[i + 1].isFinished, AllTaskArr[i + 1].deadline, AllTaskArr[i + 1].time, AllTaskArr[i + 1].degree);
                valueTask(&AllTaskArr[i + 1], AllTaskArr[i].title, AllTaskArr[i].isFinished, AllTaskArr[i].deadline, AllTaskArr[i].time, AllTaskArr[i].degree);
                valueTask(&AllTaskArr[i], temp.title, temp.isFinished, temp.deadline, temp.time, temp.degree);
            }
        }
    }
}
//  排序
void sortTask() {
    sortDeadline();
    sortDegree();
}
//  光标跳转函数
void gotoxy(int x, int y) { //  行，列  
    COORD pos;
    pos.X = x - 1;
    pos.Y = y - 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
//  获取当前时间
void getSystemNowTime(int *sy, int *sm, int *sd) {
    SYSTEMTIME st;
    GetLocalTime(&st);
    *sy = st.wYear;
    *sm = st.wMonth;
    *sd = st.wDay;
}
//  注册函数
void registerScreen() {  //  首次使用时注册
    system("cls");
    printf("-------------------------------------------------------------------------------\n");
    printf("|                                时间计划助手                                 |\n");
    printf("|                                    注册                                     |\n");
    printf("|                              user:                                          |\n");
    printf("|                              password:                                      |\n");
    printf("|                                                                             |\n");
    printf("|                                                                             |\n");
    printf("-------------------------------------------------------------------------------\n");
    //控制光标
    gotoxy(37,4);
    scanf("%s", userName);
    gotoxy(41,5);
    scanf("%s", userPassword);
    //复位光标
    gotoxy(1,9);
}
//  登录函数
void loginScreen() {
    system("cls");
    printf("-------------------------------------------------------------------------------\n");
    printf("|                                时间计划助手                                 |\n");
    printf("|                                    登录                                     |\n");
    printf("|                              user:                                          |\n");
    printf("|                              password:                                      |\n");
    printf("|                                                                             |\n");
    printf("|                                                                             |\n");
    printf("-------------------------------------------------------------------------------\n");
    //  控制光标
    gotoxy(37,4);
    scanf("%s", user);
    gotoxy(41,5);
    scanf("%s", password);
    //  复位光标
    gotoxy(1,9);
}
//  主界面函数
void mainScreen() { //  maininput用-1表示
    getSystemNowTime(&nowYear, &nowMonth, &nowDay);
    system("cls");
    printf("-------------------------------------------------------------------------------\n");
    printf("|                                时间计划助手                                 |\n");
    printf("|                                                                             |\n");
    printf("|                              1 查询任务                                     |\n");
    printf("|                              2 录入新任务                                   |\n");
    printf("|                              3 修改任务                                     |\n");
    printf("|                              0 退出                                         |\n");
    printf("|                                                                      %2d/50  |\n", AllTaskCount);
    printf("| %4d/%-2d/%-2d                                                       4 清理任务 |\n", nowYear, nowMonth, nowDay);
    printf("-------------------------------------------------------------------------------\n");
    printf("\n\n                               请输入编号:");
    if (AllTaskCount > 40) printf("建议清理任务释放空间！\n");
}
void getTaskScreen() { //maininput用1表示
    system("cls");
    printf("-------------------------------------------------------------------------------\n");
    printf("|                                查询任务                                     |\n");
    printf("|序号| 任务名称                              | 所需时间 | 截止日期 | 完成情况 |\n");
    showTask();  //  查询
    printf("|                                                                             |\n");
    printf("|                                                                             |\n");
    printf("-------------------------------------------------------------------------------\n");
}
void setNewWorkScreen() { //maininput用2表示
    while (1) {
        system("cls");
        printf("-------------------------------------------------------------------------------\n");
        printf("|                                录入新任务                                   |\n");
        printf("|                                                                             |\n");
        printf("|                                新任务数量:                                  |\n");
        printf("|                                                                             |\n");
        printf("|                                                                             |\n");
        printf("|                                                                             |\n");
        printf("-------------------------------------------------------------------------------\n");
        //  控制光标
        gotoxy(37,5);
        scanf("%d", &saveTaskCount);  //  输入保存任务数
        //  复位光标
        gotoxy(1,9);
        if (saveTaskCount + AllTaskCount > MAX_TASK) {
            printf("                          超出总任务数量，请重新输入!\n");
            system("pause");
        }
        else break;
    }

    int count = 0;  //  计数
    while (count < saveTaskCount) {
        while (1) {
            int time_h = 0, time_m = 0;
            system("cls");
            printf("-------------------------------------------------------------------------------\n");
            printf("|                                录入新任务                                   |\n");
            printf("|                                新任务%2d/%-2d                                  |\n", count + 1, saveTaskCount);
            printf("|                           名称:                                             |\n");
            printf("|                           完成情况:       (1已完成, 0 未完成)               |\n");
            printf("|                           截止时间:    /  /                                 |\n");
            printf("|                           所需时间:  h  m                                   |\n");
            printf("|                           重要程度:       (1--10)                           |\n");
            printf("-------------------------------------------------------------------------------\n");
            //控制光标
            gotoxy(34,4);
            scanf("%s", AllTaskArr[count + AllTaskCount].title);
            gotoxy(38,5);
            scanf("%d", &AllTaskArr[count + AllTaskCount].isFinished);
            gotoxy(38,6);
            scanf("%4d", &AllTaskArr[count + AllTaskCount].deadline[0]);
            gotoxy(43,6);
            scanf("%2d", &AllTaskArr[count + AllTaskCount].deadline[1]);
            gotoxy(46,6);
            scanf("%2d", &AllTaskArr[count + AllTaskCount].deadline[2]);
            gotoxy(38,7);
            scanf("%2d", &time_h);
            gotoxy(41,7);
            scanf("%2d", &time_m);
            gotoxy(38,8);
            scanf("%2d", &AllTaskArr[count + AllTaskCount].degree);
            //复位光标
            gotoxy(1,9);
            AllTaskArr[count + AllTaskCount].time = time_h * 60 + time_m;
            //  输入检验
            if ((AllTaskArr[count + AllTaskCount].isFinished != 1 && AllTaskArr[count + AllTaskCount].isFinished != 0)
                || (AllTaskArr[count + AllTaskCount].degree < 1 || AllTaskArr[count + AllTaskCount].degree > 10)) {
                printf("                          非法输入，请重新输入!\n");
                system("pause");
            }
            else {
                count++;
                break;
            }
        }
    }
    AllTaskCount += saveTaskCount;  //  更新总任务数
    updateData();  //  更新数据

    system("cls");
    printf("-------------------------------------------------------------------------------\n");
    printf("|                                录入新任务                                   |\n");
    printf("|                                                                             |\n");
    printf("|                               已录入新任务                                  |\n");
    printf("|                                                                             |\n");
    printf("|                                                                             |\n");
    printf("|                                                                             |\n");
    printf("-------------------------------------------------------------------------------\n");
} 
void changeTaskScreen() { //maininput用3表示
    int index = 0;  //  索引
    while (1) {
        system("cls");
        printf("-------------------------------------------------------------------------------\n");
        printf("|                                修改任务                                     |\n");
        printf("|                                任务编号:                                    |\n");
        printf("|                                                                             |\n");
        printf("|                                                                             |\n");
        printf("|                                                                             |\n");
        printf("|                                                                             |\n");
        printf("-------------------------------------------------------------------------------\n");
        //  边界验证
        if (AllTaskCount == 0) {
            printf("\n\n                              当前无可修改任务!\n");
            break;
        }

        //  控制光标
        gotoxy(37,4);
        scanf("%d", &index);
        //  复位光标
        gotoxy(1,9);
        //  输入检验
        if (index < 1 || index > AllTaskCount){
            printf("                          非法输入，请重新输入!\n");
            system("pause");
        }
        else break;
    }
    
    while (1){
        //  边界验证
        if (AllTaskCount == 0) break;
        int time_h = 0, time_m = 0;
        system("cls");
        printf("-------------------------------------------------------------------------------\n");
        printf("|                                修改任务                                     |\n");
        printf("|                              任务编号:%2d                                    |\n", index);
        printf("|                                                                             |\n");
        printf("|                              名称:%-38s    |\n", AllTaskArr[index - 1].title);
        printf("|                              完成情况:%d       (1已完成, 0 未完成)           |\n", AllTaskArr[index - 1].isFinished);
        printf("|                              截止时间:%4d/%-2d/%-2d                            |\n", AllTaskArr[index -1].deadline[0], AllTaskArr[index - 1].deadline[1], AllTaskArr[index -1].deadline[2]);
        printf("|                              所需时间:%2dh%2dm                                |\n", AllTaskArr[index - 1].time / 60, AllTaskArr[index - 1].time % 60);
        printf("|                              重要程度:%-2d                                    |\n", AllTaskArr[index -1].degree);
        printf("-------------------------------------------------------------------------------\n");
        //  控制光标
        gotoxy(37,5);
        scanf("%s", AllTaskArr[index - 1].title);
        gotoxy(41,6);
        scanf("%d", &AllTaskArr[index - 1].isFinished);
        gotoxy(41,7);
        scanf("%4d", &AllTaskArr[index - 1].deadline[0]);
        gotoxy(46,7);
        scanf("%2d", &AllTaskArr[index - 1].deadline[1]);
        gotoxy(49,7);
        scanf("%2d", &AllTaskArr[index - 1].deadline[2]);
        gotoxy(41,8);
        scanf("%4d", &time_h);
        gotoxy(44,8);
        scanf("%4d", &time_m);
        AllTaskArr[index - 1].time = time_h * 60 + time_m;
        gotoxy(41,9);
        scanf("%2d", &AllTaskArr[index - 1].degree);
        //  复位光标
        gotoxy(1,10);
        //  输入检验
        if (AllTaskArr[index - 1].isFinished != 1 && AllTaskArr[index - 1].isFinished != 0) {
            printf("                          非法输入，请重新输入!\n");
            system("pause");
        }
        else break;
    }
    //  边界验证
        if (AllTaskCount == 0) return;

    //  更新数据
    updateData();

    system("cls");
    printf("-------------------------------------------------------------------------------\n");
    printf("|                                修改任务                                     |\n");
    printf("|                                                                             |\n");
    printf("|                                更改成功                                     |\n");
    printf("|                                                                             |\n");
    printf("|                                                                             |\n");
    printf("|                                                                             |\n");
    printf("-------------------------------------------------------------------------------\n");
}
void cleanTaskScreen() {
    int input_4 = -1;  //  用于内部操作
    while (1) {
        system("cls");
        printf("-------------------------------------------------------------------------------\n");
        printf("|                                清理任务                                     |\n");
        printf("|                                                                             |\n");
        printf("|                              1 清理已完成任务                               |\n");
        printf("|                              2 清理特定任务                                 |\n");
        printf("|                                                                             |\n");
        printf("|                                                                             |\n");
        printf("-------------------------------------------------------------------------------\n");
        printf("                               请输入编号:");
        scanf("%d", &input_4);
        if (input_4 != 1 && input_4 != 2 && input_4 != 0) {
            printf("                          非法输入，请重新输入：");
            system("pause");
        }
        else break;
    }
    if (input_4 == 1) {  //  1 清理已完成任务
        int confirmNum = -1;  //  用于内部确认
        while (1) {
            system("cls");
            printf("-------------------------------------------------------------------------------\n");
            printf("|                                清理任务                                     |\n");
            printf("|                              清理已完成任务                                 |\n");
            printf("|                                                                             |\n");
            printf("|                           确认清理所有已完成任务                            |\n");
            printf("|                                                                             |\n");
            printf("|                             (1 确认 0 取消)                                 |\n");
            printf("-------------------------------------------------------------------------------\n");
            //  控制光标
            gotoxy(38,6);
            scanf("%d", &confirmNum);
            //  复位光标
            gotoxy(1,9);

            if (confirmNum != 1 && confirmNum != 0) {
                printf("                          非法输入，请重新输入：");
            system("pause");
            }
            else break;
        }

        if (confirmNum == 1) {  //  确认
            cleanAllFinishedTask();
            system("cls");
            printf("-------------------------------------------------------------------------------\n");
            printf("|                                清理任务                                     |\n");
            printf("|                              清理已完成任务                                 |\n");
            printf("|                                                                             |\n");
            printf("|                               已清理完成                                    |\n");
            printf("|                                                                             |\n");
            printf("|                                                                             |\n");
            printf("-------------------------------------------------------------------------------\n");
        }
        else {  //  取消
        system("cls");
        printf("-------------------------------------------------------------------------------\n");
        printf("|                                清理任务                                     |\n");
        printf("|                              清理已完成任务                                 |\n");
        printf("|                                                                             |\n");
        printf("|                                 已取消                                      |\n");
        printf("|                                                                             |\n");
        printf("|                                                                             |\n");
        printf("-------------------------------------------------------------------------------\n");
        }
    }
    if (input_4 == 2) {  //  2 清理特定任务
        int index = 0;  //  内部索引
        int confirmNum = -1;  //  用于内部确认
        while (1) {
            system("cls");
            printf("-------------------------------------------------------------------------------\n");
            printf("|                                清理任务                                     |\n");
            printf("|                               清理特定任务                                  |\n");
            printf("|                                任务编号:                                    |\n");
            printf("|                                                                             |\n");
            printf("|                                                                             |\n");
            printf("|                                                                             |\n");
            printf("-------------------------------------------------------------------------------\n");
            //  边界验证
            if (AllTaskCount == 0) {
                //  复位光标
                gotoxy(1,9);
                printf("\n\n                              当前无可清理任务!\n");
                break;
            }
            //  控制光标
            gotoxy(37,5);
            scanf("%d", &index);
            //  复位光标
            gotoxy(1,9);
            //  输入检验
            if (index < 1 || index > AllTaskCount){
                printf("                          非法输入，请重新输入!\n");
                system("pause");
            }
            else break;
    }
    while (1) {
        //  边界验证
        if (AllTaskCount == 0) break;

        system("cls");
        printf("-------------------------------------------------------------------------------\n");
        printf("|                                清理任务                                     |\n");
        printf("|                               清理特定任务                                  |\n");
        printf("|                                任务编号:%2d                                  |\n", index);
        printf("|                              名称:%s                                          |\n", AllTaskArr[index - 1].title);
        printf("|                              确认清理该任务                                 |\n");
        printf("|                                                                             |\n");
        printf("|                             (1 确认 0 取消)                                 |\n");
        printf("-------------------------------------------------------------------------------\n");
        //  控制光标
        gotoxy(38,7);
        scanf("%d", &confirmNum);
        //  复位光标
        gotoxy(1,9);

        if (confirmNum != 1 && confirmNum != 0) {
            printf("                          非法输入，请重新输入：");
        system("pause");
        }
        else break;
    }
    //  边界验证
    if (AllTaskCount == 0) return;

    if (confirmNum == 1) {  //  确认
            cleanAllUnfinishedTask(index);
            system("cls");
            printf("-------------------------------------------------------------------------------\n");
            printf("|                                清理任务                                     |\n");
            printf("|                               清理特定任务                                  |\n");
            printf("|                                                                             |\n");
            printf("|                               已清理完成                                    |\n");
            printf("|                                                                             |\n");
            printf("|                                                                             |\n");
            printf("-------------------------------------------------------------------------------\n");
    }
    else {  //  取消
        system("cls");
        printf("-------------------------------------------------------------------------------\n");
        printf("|                                清理任务                                     |\n");
        printf("|                               清理特定任务                                  |\n");
        printf("|                                                                             |\n");
        printf("|                                 已取消                                      |\n");
        printf("|                                                                             |\n");
        printf("|                                                                             |\n");
        printf("-------------------------------------------------------------------------------\n");
    }   
    }
}

int main() {
    SetConsoleOutputCP(936);
    loadFile();  //  初始化文件

    //  注册
    if (isRegisterUser()) {  //  判断是否注册
        registerScreen();
        //  更新数据
        updateData();}

    //  登录
    loginUser();


    int maininput = -1;  //  用来表示主界面操作
    int input = -1;  //  用来表示次界面操作

    while (1) {
        //主界面操作
        if (maininput == -1) mainScreen();
        if (maininput == 1) getTaskScreen();
        if (maininput == 2) setNewWorkScreen();
        if (maininput == 3) changeTaskScreen();
        if (maininput == 4) cleanTaskScreen();
        if (maininput == 0) {
            system("cls");
            printf("已退出程序!\n");
            break;
        }

        //输入操作
        //主界面输入操作
        if (maininput == -1) {
            scanf("%d", &maininput);
            if (maininput != 1 && maininput != 2 && maininput != 3 && maininput != 4 && maininput !=0) { //  输入检验
                printf("                          非法输入，请重新输入!\n");
                system("pause");
                maininput = -1;
            }
        }
        //次界面返回操作
        else {  //  maininput == 1 || maininput == 2 || maininput == 3 || maininput == 4
            printf("\n\n                            按任意键返回主菜单!\n");
            system("pause");
            maininput = -1;
        }
    }
    return 0;
}