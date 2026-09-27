#include <stdio.h>
#include <string.h>

int main(void) {
    //1.利用逗号运算符简化代码 单行完成多步操作（注意返回值是最后一个表达式）
    int a = (printf("初始化中...\n"), 10);

    //2.结构体初始化归零
    struct Data {
        int id;
        char name[20];
    };
    struct Data data = {0}; // 所有成员初始化为0

    //4.宏定义中的#和##
#define PRINT_VAR(x) printf(#x " = %d\n", x)
#define CONCAT(a, b) a##b

    int var = 42;
    PRINT_VAR(var); // 输出 "var = 42"
    int CONCAT(num, 1) = 10; // 定义变量 num1
    printf("num1= %d\n", num1);

    //5.利用do { ... } while(0)封装宏，避免宏在 if-else 中因分号导致的逻辑错误。
#define SAFE_FREE(ptr) do { free(ptr); ptr = NULL; } while(0)

    //7.利用offsetof获取结构体成员偏移量
#include <stddef.h>
    size_t offset = offsetof(struct Data, name); // 获取name的偏移量
    printf("offset: %zu\n", offset);

#define JOIN(a, b) (a""b)
    const char *str = JOIN("hi", "gji");
    printf(str);

    printf("\n""df""du");

    //9.通过volatile防止编译器优化
    volatile int flag = 0;
    while (!flag) { /* 等待外部修改flag */ }
    //场景：嵌入式开发中访问硬件寄存器或多线程共享变量。

    //15.利用errno捕获错误信息
#include <errno.h>

    FILE *fp = fopen("nonexist.txt", "r");
    if (!fp) {
        printf("错误: %s\n", strerror(errno)); // 输出具体错误
    }


    return 0;
}
