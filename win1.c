//头文件，很多数据类型、结构、API接口函数都在这个头文件或者所包含的其他头文件中进行了声明。如果不声明则不识别MessageBox
#include <windows.h> 

//将源文件编译生成目标文件（.obj）后，将目标文件链接生成可执行文件时，需要用到lib，因为lib中包含了MessageBox的链接信息。
//#pragma comment(lib, "User32.Lib")

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // 显示一个消息框，包含标题和消息文本
    // 调用API函数 MessageBox
    MessageBox(NULL, TEXT("开始学习Windows编程"), TEXT("消息对话框"), MB_OK);

    return 0;
}