# C语言学习之旅 | Learn C

欢迎来到我的C语言学习项目！该代码库是我学习C语言历程的记录，包含了从基础概念到有趣算法的各种代码示例和实践。

## ✨ 亮点展示

1. `fib_colors.c` 生成的斐波那契数列的控制台彩色文本，展示了C语言在图形和算法结合方面的魅力。

![斐波那契数列颜色可视化](screenshot/fib_colors--20250716.png)

2. `win32_controls_demo.c`
   ![](screenshot/win32_controls_demo.exe.png)

3. `beep_sleep_demo.c` 模拟计算机的蜂鸣声
4.

## 🚀 项目特色

这个仓库中包含了多个有趣的小项目和代码片段：

- **🔢 斐波那契数列**: 多种方式实现斐波那契数列的计算与可视化 (`Fibonacci.c`, `fib_colors.c`, `fib_diagram.c`)。
- **🍩 旋转甜甜圈**: 经典的控制台3D旋转甜甜圈动画 (`donut.c`), 摘自 [a1k0n.net](https://www.a1k0n.net/2021/01/13/optimizing-donut.html) 。
- **🧩 数独游戏**: 一个C语言实现的数独解算器 (`sudoku.c`)。
- **🥧 圆周率计算**: 使用不同算法计算π，包括 Chudnovsky 算法 (`pi.c`, `chudnovsky_pi.c`)。
- **ꡙ‍ maze**: 一个简单的迷宫生成器 (`maze.c`)。
- **🪟 Windows编程**: 使用 WinAPI 进行Windows图形界面编程的尝试 (`win32_controls_demo.c`, `win987.c`)。 To compile with GCC (MinGW in w64devkit):
 ```PowerShell
 gcc win32_controls_demo.c -o win32_controls_demo.exe -lcomctl32 -lgdi32 -luser32
 ```
 
- **🎮 Raylib游戏库**: 使用 Raylib 库进行简单游戏开发的示例 (`raylib123/hiRaylib.c`)。
- **💡 基础概念**: 一些学习C语言核心知识（如数组、指针、文件操作、断言等）的微型程序。

## 🛠️ 如何使用

大部分程序都是独立的 `.c` 文件，可以使用 `tcc`、`gcc` 、`Visual C++` 或其他C编译器轻松编译。

1. **克隆仓库**
   ```
   git clone https://github.com/a-boy/learnc.git
   cd learnc
   ```

2. **编译C文件，运行程序**
   以 `Fibonacci.c` 为例：
   ```PowerShell
      tcc Fibonacci.c
     .\Fibonacci.exe
     :: 使用 ./fibonacci 也会运行.\Fibonacci.exe
   ```

   在 Linux 或 macOS 上:
   ```bash
   gcc Fibonacci.c -o fibonacci
   ./fibonacci
   ```

   > **注意**: 某些程序可能需要链接额外的库。例如，如果用到了数学函数，需要加上 `-lm` 参数。

## 值得学习的C语言开源项目

1. [smallchat](https://github.com/antirez/smallchat) A minimal programming example for a chat server and client.  
<https://github.com/mrchipset/smallchat-win32>
2. [kilo](https://github.com/antirez/kilo) 一个小巧的文本编辑器，可以在终端中运行
3. [Tinyhttpd](https://github.com/EZLippi/Tinyhttpd) 一个基于C语言的HTTP服务器
4.  [WebBench](https://github.com/EZLippi/WebBench)
5. [The C Standard Library](https://github.com/rxi/libs)  一个C语言标准库，提供了一些常用的函数和数据结构。
6. [libuv](https://github.com/libuv/libuv) 一个C语言库，提供了一些常用的异步I/O函数。
7. [libevent](https://github.com/libevent/libevent)
8. [mongoose](https://github.com/cesanta/mongoose) 嵌入式 Web 服务器，带有 TCP/IP 网络堆栈、MQTT 和 Websocket. 易于集成：只需将mongoose.c 和mongoose.h文件复制到您的源代码树即可
9. sqlite
10. Redis
11. nginx
12. Lua
13. [agent-c](https://github.com/bravenewxyz/agent-c)  用C编写的超轻量级AI代理,可与OpenRouter API 通信并执行shell命令  

## 📜 许可证

本项目采用 [MIT License](LICENSE) 开源。
