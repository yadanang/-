// ������Ҫ��ͷ�ļ�
#define _CRT_SECURE_NO_WARNINGS 1
#include <windows.h>
#include <stdio.h>

// ��������¼��궯�����Ͳ������
#define MAX_ACTIONS 10000
#define SAMPLING_INTERVAL 5 // ÿ��5�������һ��

// �����¼�������Ľṹ��
typedef struct {
    double x;       // ��� X ����
    double y;       // ��� Y ����
    int click;      // 0��ʾû�е����1��ʾ��������2��ʾ�Ҽ����
    int scroll;     // ���ֹ���ֵ
} MouseAction;

// ��������
void recordMouseAction(MouseAction* actions, int* actionCount, double x, double y, int click, int scroll);
void saveMouseActions(const MouseAction* actions, int actionCount, const char* filename);
void replayMouseActions(const MouseAction* actions, int actionCount);

int main() {
    MouseAction actions[MAX_ACTIONS];  // �洢��궯��������
    int actionCount = 0;               // ��¼����궯����

    while (1) {
        // �û�ѡ��¼�ơ��طŻ��˳�
        printf("1. ��ʼ¼��\n");
        printf("2. �ط���һ��¼��\n");
        printf("3. �˳�\n");

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

                // ��¼��궯��
                recordMouseAction(actions, &actionCount, mouseX, mouseY, leftClick ? 1 : (rightClick ? 2 : 0), scroll);

                // ����Ƿ��� ESC ��ֹͣ¼��
                if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                    break;
                }

                Sleep(SAMPLING_INTERVAL);
            }

            // ������궯�����ļ�
            saveMouseActions(actions, actionCount, "mouse_actions.txt");
        }
        else if (choice == 2) {
            // �ط���һ��¼�Ƶ���궯��
            replayMouseActions(actions, actionCount);
        }
        else if (choice == 3) {
            // �˳�����
            break;
        }
    }

    return 0;
}









//�����
void recordMouseAction(MouseAction* actions, int* actionCount, double x, double y, int click, int scroll) {
    static int isMouseDown = 0;  // ��¼����Ƿ���

    if (*actionCount < MAX_ACTIONS) {
        MouseAction* currentAction = &actions[*actionCount];
        currentAction->x = x;
        currentAction->y = y;

        // ʹ��GetAsyncKeyState��ȡ��ǰ���״̬
        int currentLeftClick = GetAsyncKeyState(VK_LBUTTON) & 0x8001;

        // �ж��Ƿ����������¼�
        if (click == 1 && !isMouseDown && currentLeftClick) {
            // ��갴��
            isMouseDown = 1;
        }
        else if (isMouseDown && !currentLeftClick) {
            // ����ͷţ�����һ�ε��
            currentAction->click = 1;
            isMouseDown = 0;  // �������״̬
        }
        else {
            currentAction->click = 0;
        }

        currentAction->scroll = scroll;
        (*actionCount)++;
    }
}








// ������궯�����ļ�����
void saveMouseActions(const MouseAction* actions, int actionCount, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file != NULL) {
        for (int i = 0; i < actionCount; ++i) {
            // ������궯�����ݵ��ļ�
            fprintf(file, "%.2f %.2f %d %d\n", actions[i].x, actions[i].y, actions[i].click, actions[i].scroll);
        }
        fclose(file);
        printf("¼����ɣ������ѱ��浽 %s\n", filename);
    }
    else {
        perror("Error saving mouse actions");
    }
}

// �ط���궯������
void replayMouseActions(const MouseAction* actions, int actionCount) {
    printf("��ʼ�ط�...\n");

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

    printf("�ط����\n");
}
