// console_menu.c
#include <stdio.h>
#include <stdlib.h> // 用于 system("cls") 或 system("clear")

// 函数声明
void display_menu();
void option_1();
void option_2();
void option_3();
void clear_input_buffer();

int main() {
    int choice;

    do {
        display_menu();
        printf("请输入您的选择: ");

        // 读取用户输入
        if (scanf("%d", &choice) != 1) {
            printf("无效输入，请输入一个数字。\n");
            clear_input_buffer(); // 清理无效输入
            printf("按回车键继续...");
            getchar(); // 等待用户按键
            continue; // 跳过本次循环的剩余部分
        }
        
        clear_input_buffer(); // 清理 scanf 留下的换行符

        switch (choice) {
            case 1:
                option_1();
                break;
            case 2:
                option_2();
                break;
            case 3:
                option_3();
                break;
            case 0:
                printf("感谢使用，程序退出。\n");
                break;
            default:
                printf("无效选择，请输入 0-3 之间的数字。\n");
                break;
        }

        // 如果用户没有选择退出，则暂停等待用户按键
        if (choice != 0) {
            printf("\n按回车键返回主菜单...");
            getchar(); 
        }

    } while (choice != 0);

    return 0;
}

// 清理输入缓冲区，直到遇到换行符或文件结尾
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 显示主菜单
void display_menu() {
    // 根据操作系统选择清屏命令
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    printf("=========================\n");
    printf("       控制台菜单\n");
    printf("=========================\n");
    printf("    1. 执行功能一\n");
    printf("    2. 执行功能二\n");
    printf("    3. 执行功能三\n");
    printf("    0. 退出程序\n");
    printf("=========================\n");
}

// 菜单选项1对应的功能
void option_1() {
    printf("\n--- 您选择了功能一 ---\n");
    // 在这里添加功能一的具体代码
    printf("功能一执行完毕。\n");
}

// 菜单选项2对应的功能
void option_2() {
    printf("\n--- 您选择了功能二 ---\n");
    // 在这里添加功能二的具体代码
    printf("功能二执行完毕。\n");
}

// 菜单选项3对应的功能
void option_3() {
    printf("\n--- 您选择了功能三 ---\n");
    // 在这里添加功能三的具体代码
    printf("功能三执行完毕。\n");
}
