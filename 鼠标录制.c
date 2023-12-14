// 包含必要的头文件
#define _CRT_SECURE_NO_WARNINGS 1
#include <windows.h>
#include <stdio.h>

// 定义最大记录鼠标动作数和采样间隔
#define MAX_ACTIONS 10000
#define SAMPLING_INTERVAL 5 // 每隔5毫秒采样一次

// 定义记录鼠标操作的结构体
typedef struct {
    double x;       // 鼠标 X 坐标
    double y;       // 鼠标 Y 坐标
    int click;      // 0表示没有点击，1表示左键点击，2表示右键点击
    int scroll;     // 滑轮滚动值
} MouseAction;

// 函数声明
void recordMouseAction(MouseAction* actions, int* actionCount, double x, double y, int click, int scroll);
void saveMouseActions(const MouseAction* actions, int actionCount, const char* filename);
void replayMouseActions(const MouseAction* actions, int actionCount);

int main() {
    MouseAction actions[MAX_ACTIONS];  // 存储鼠标动作的数组
    int actionCount = 0;               // 记录的鼠标动作数

    while (1) {
        // 用户选择录制、回放或退出
        printf("1. 开始录制\n");
        printf("2. 回放上一次录制\n");
        printf("3. 退出\n");

        int choice;
        scanf("%d", &choice);

        if (choice == 1) {
            while (1) {
                POINT p;
                GetCursorPos(&p);

                double mouseX = p.x;
                double mouseY = p.y;

                int leftClick = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
                int rightClick = GetAsyncKeyState(VK_RBUTTON) & 0x8000;

                int scroll = 0;
                if (GetAsyncKeyState(VK_MBUTTON) & 0x8000) {
                    scroll = 1;
                }
                else if (GetAsyncKeyState(VK_XBUTTON1) & 0x8000) {
                    scroll = -1;
                }

                // 记录鼠标动作
                recordMouseAction(actions, &actionCount, mouseX, mouseY, leftClick ? 1 : (rightClick ? 2 : 0), scroll);

                // 检测是否按下 ESC 键停止录制
                if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                    break;
                }

                Sleep(SAMPLING_INTERVAL);
            }

            // 保存鼠标动作到文件
            saveMouseActions(actions, actionCount, "mouse_actions.txt");
        }
        else if (choice == 2) {
            // 回放上一次录制的鼠标动作
            replayMouseActions(actions, actionCount);
        }
        else if (choice == 3) {
            // 退出程序
            break;
        }
    }

    return 0;
}









//鼠标点击
void recordMouseAction(MouseAction* actions, int* actionCount, double x, double y, int click, int scroll) {
    static int isMouseDown = 0;  // 记录鼠标是否按下

    if (*actionCount < MAX_ACTIONS) {
        MouseAction* currentAction = &actions[*actionCount];
        currentAction->x = x;
        currentAction->y = y;

        // 使用GetAsyncKeyState获取当前左键状态
        int currentLeftClick = GetAsyncKeyState(VK_LBUTTON) & 0x8001;

        // 判断是否发生左键点击事件
        if (click == 1 && !isMouseDown && currentLeftClick) {
            // 鼠标按下
            isMouseDown = 1;
        }
        else if (isMouseDown && !currentLeftClick) {
            // 鼠标释放，算作一次点击
            currentAction->click = 1;
            isMouseDown = 0;  // 重置鼠标状态
        }
        else {
            currentAction->click = 0;
        }

        currentAction->scroll = scroll;
        (*actionCount)++;
    }
}








// 保存鼠标动作到文件函数
void saveMouseActions(const MouseAction* actions, int actionCount, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file != NULL) {
        for (int i = 0; i < actionCount; ++i) {
            // 保存鼠标动作数据到文件
            fprintf(file, "%.2f %.2f %d %d\n", actions[i].x, actions[i].y, actions[i].click, actions[i].scroll);
        }
        fclose(file);
        printf("录制完成，动作已保存到 %s\n", filename);
    }
    else {
        perror("Error saving mouse actions");
    }
}

// 回放鼠标动作函数
void replayMouseActions(const MouseAction* actions, int actionCount) {
    printf("开始回放...\n");

    for (int i = 0; i < actionCount; ++i) {
        SetCursorPos((int)actions[i].x, (int)actions[i].y);

        if (actions[i].click == 1) {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        }
        else if (actions[i].click == 2) {
            mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
        }

        if (actions[i].scroll != 0) {
            mouse_event(MOUSEEVENTF_WHEEL, 0, 0, actions[i].scroll * WHEEL_DELTA, 0);
        }

        Sleep(SAMPLING_INTERVAL);
    }

    printf("回放完成\n");
}
