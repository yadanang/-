// 忽略与安全性相关的警告
#define _CRT_SECURE_NO_WARNINGS 1

// 包含Windows API头文件，该头文件包含了在Windows上使用的函数和常量
#include <windows.h>

// 包含标准输入输出的头文件，用于处理输入输出操作
#include <stdio.h>

// 定义函数clickMouse，用于模拟鼠标点击
void clickMouse(int clicks) {
    int i;
    // 循环执行鼠标点击操作指定次数
    for (i = 0; i < clicks; i++) {
        // 使用Windows API的mouse_event函数模拟鼠标左键按下事件
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        // 使用Windows API的mouse_event函数模拟鼠标左键释放事件
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        // 控制点击速度，每次点击之间间隔一定时间
        Sleep(1 / clicks);
    }
}

// 主函数
int main() {
    // 声明一个整型变量clicksPerMinute，用于存储用户输入的每分钟点击次数
    int clicksPerMinute;

    // 打印提示信息，提示用户输入每分钟点击次数
    printf("程序10s会休息一次，点击次数取决于电脑配置 一般10s＞750下 ，请输入每10s点击次数：");
    // 从标准输入中读取用户输入的每分钟点击次数
    scanf("%d", &clicksPerMinute);

    // 进入一个无限循环，程序将在此循环中不断执行鼠标点击操作
    while (1) {
        // 调用clickMouse函数，传递用户输入的每分钟点击次数作为参数，模拟鼠标点击
        clickMouse(clicksPerMinute);
        // 使程序休眠60秒，以满足每分钟执行一次鼠标点击的要求
        Sleep(10000);
    }

    // 主函数返回0，表示程序成功执行结束
    return 0;
}
