// tabs_mouse_control.c
// 一个在 Windows 控制台下使用 C 语言实现的、支持鼠标和键盘的 Tabs 控件。
// 注意：此代码仅适用于 Windows 平台。
#include <stdio.h>
#include <windows.h>
#include <stdbool.h>

// --- 全局变量和常量 ---
HANDLE h_stdout;
HANDLE h_stdin;
DWORD original_console_mode;

// 定义颜色
#define FOREGROUND_WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define BACKGROUND_BLUE_INTENSE (BACKGROUND_BLUE | BACKGROUND_INTENSITY)
#define NORMAL_COLOR (FOREGROUND_WHITE)
#define HIGHLIGHT_COLOR (BACKGROUND_BLUE_INTENSE | FOREGROUND_WHITE)

// Tab 定义
const char* TABS[] = {"文件", "编辑", "视图", "帮助"};
const int NUM_TABS = sizeof(TABS) / sizeof(TABS[0]);
int active_tab = 0;

// --- 函数声明 ---
void gotoxy(int x, int y);
void setup_console();
void restore_console();
void draw_tabs();
void draw_content_panel();
void handle_input();

int main() {
    setup_console();

    bool running = true;
    bool needs_redraw = true;

    while (running) {
        if (needs_redraw) {
            system("cls"); // 清屏
            draw_tabs();
            draw_content_panel();
            needs_redraw = false;
        }

        DWORD num_events;
        INPUT_RECORD input_record;

        // 等待输入事件
        if (ReadConsoleInput(h_stdin, &input_record, 1, &num_events) && num_events > 0) {
            int old_active_tab = active_tab;

            if (input_record.EventType == KEY_EVENT && input_record.Event.KeyEvent.bKeyDown) {
                // 处理键盘事件
                switch (input_record.Event.KeyEvent.wVirtualKeyCode) {
                    case VK_LEFT:
                        active_tab = (active_tab - 1 + NUM_TABS) % NUM_TABS;
                        break;
                    case VK_RIGHT:
                        active_tab = (active_tab + 1) % NUM_TABS;
                        break;
                    case VK_ESCAPE:
                        running = false;
                        break;
                }
            } else if (input_record.EventType == MOUSE_EVENT) {
                // 处理鼠标事件
                MOUSE_EVENT_RECORD mouse_event = input_record.Event.MouseEvent;
                if (mouse_event.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                    int mouse_x = mouse_event.dwMousePosition.X;
                    int mouse_y = mouse_event.dwMousePosition.Y;

                    // 检查是否点击在 Tab 栏上 (Y=1)
                    if (mouse_y == 1) {
                        int current_x = 2;
                        for (int i = 0; i < NUM_TABS; i++) {
                            int tab_width = strlen(TABS[i]) + 2; // " Tab "
                            if (mouse_x >= current_x && mouse_x < current_x + tab_width) {
                                active_tab = i;
                                break;
                            }
                            current_x += tab_width + 2; // 两个空格作为分隔
                        }
                    }
                }
            }

            if (old_active_tab != active_tab) {
                needs_redraw = true;
            }
        }
    }

    restore_console();
    gotoxy(0, 10); // 将光标移动到下面，避免覆盖退出信息
    printf("程序已退出。\n");
    return 0;
}

// 将光标移动到指定位置
void gotoxy(int x, int y) {
    COORD coord = {x, y};
    SetConsoleCursorPosition(h_stdout, coord);
}

// 初始化控制台设置
void setup_console() {
    h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    h_stdin = GetStdHandle(STD_INPUT_HANDLE);

    // 1. 保存原始控制台模式
    if (!GetConsoleMode(h_stdin, &original_console_mode)) {
        printf("Error getting console mode.\n");
        exit(1);
    }

    // 2. 设置新的控制台模式
    //    - ENABLE_MOUSE_INPUT: 开启鼠标输入
    //    - ENABLE_EXTENDED_FLAGS: 必须和 ENABLE_MOUSE_INPUT 一起使用
    //    - 禁用 QUICK_EDIT_MODE，因为它会捕获鼠标点击，阻止我们的程序接收事件
    DWORD new_mode = ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT;
    if (!SetConsoleMode(h_stdin, new_mode)) {
        printf("Error setting console mode.\n");
        exit(1);
    }
    
    // 隐藏光标
    CONSOLE_CURSOR_INFO cursor_info;
    GetConsoleCursorInfo(h_stdout, &cursor_info);
    cursor_info.bVisible = FALSE;
    SetConsoleCursorInfo(h_stdout, &cursor_info);
}

// 恢复原始控制台设置
void restore_console() {
    // 恢复光标
    CONSOLE_CURSOR_INFO cursor_info;
    GetConsoleCursorInfo(h_stdout, &cursor_info);
    cursor_info.bVisible = TRUE;
    SetConsoleCursorInfo(h_stdout, &cursor_info);

    // 恢复原始控制台模式
    SetConsoleMode(h_stdin, original_console_mode);
}

// 绘制所有 Tab
void draw_tabs() {
    gotoxy(0, 0);
    SetConsoleTextAttribute(h_stdout, NORMAL_COLOR);
    // 绘制一条顶部分隔线
    for(int i = 0; i < 80; i++) printf("=");

    int current_x = 2;
    for (int i = 0; i < NUM_TABS; i++) {
        gotoxy(current_x, 1);
        if (i == active_tab) {
            SetConsoleTextAttribute(h_stdout, HIGHLIGHT_COLOR);
        } else {
            SetConsoleTextAttribute(h_stdout, NORMAL_COLOR);
        }
        printf(" %s ", TABS[i]);

        // 为下一个 Tab 更新 X 坐标
        current_x += strlen(TABS[i]) + 2 + 2; // 标签宽度 + 2个空格分隔
    }
    SetConsoleTextAttribute(h_stdout, NORMAL_COLOR);
    
    gotoxy(0, 2);
    for(int i = 0; i < 80; i++) printf("=");
}

// 绘制当前激活 Tab 的内容面板
void draw_content_panel() {
    gotoxy(2, 4);
    SetConsoleTextAttribute(h_stdout, NORMAL_COLOR);

    switch (active_tab) {
        case 0:
            printf("这里是 [文件] 选项卡的内容。\n");
            gotoxy(2, 5);
            printf("你可以创建、打开或保存文件。");
            break;
        case 1:
            printf("这里是 [编辑] 选项卡的内容。\n");
            gotoxy(2, 5);
            printf("你可以剪切、复制或粘贴文本。");
            break;
        case 2:
            printf("这里是 [视图] 选项卡的内容。\n");
            gotoxy(2, 5);
            printf("你可以放大或缩小视图。");
            break;
        case 3:
            printf("这里是 [帮助] 选项卡的内容。\n");
            gotoxy(2, 5);
            printf("按 ESC 键退出程序。使用鼠标或左右箭头切换。");
            break;
    }
}
