#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_POINTS 1000000

int main() {
    int i;
    double x, y;
    int points_inside_circle = 0;

    // 初始化随机数种子
    srand(time(NULL));

    // 进行随机投点
    for (i = 0; i < NUM_POINTS; i++) {
        // 生成随机点 (x, y)，范围在 [0, 1] 之间
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;

        // 判断点是否在单位圆内
        if (x * x + y * y <= 1) {
            points_inside_circle++;
        }
    }

    // 计算圆周率的近似值
    double pi_approx = 4.0 * points_inside_circle / NUM_POINTS;

    // 输出结果
    printf("通过 %d 次随机投点估算的圆周率近似值为: %f\n", NUM_POINTS, pi_approx);

    return 0;
}    