// �����밲ȫ����صľ���
#define _CRT_SECURE_NO_WARNINGS 1

// ����Windows APIͷ�ļ�����ͷ�ļ���������Windows��ʹ�õĺ����ͳ���
#include <windows.h>

// ������׼���������ͷ�ļ������ڴ��������������
#include <stdio.h>

// ���庯��clickMouse������ģ�������
void clickMouse(int clicks) {
    int i;
    // ѭ��ִ�����������ָ������
    for (i = 0; i < clicks; i++) {
        // ʹ��Windows API��mouse_event����ģ�������������¼�
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        // ʹ��Windows API��mouse_event����ģ���������ͷ��¼�
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        // ���Ƶ���ٶȣ�ÿ�ε��֮����һ��ʱ��
        Sleep(1 / clicks);
    }
}

// ������
int main() {
    // ����һ�����ͱ���clicksPerMinute�����ڴ洢�û������ÿ���ӵ������
    int clicksPerMinute;

    // ��ӡ��ʾ��Ϣ����ʾ�û�����ÿ���ӵ������
    printf("����10s����Ϣһ�Σ��������ȡ���ڵ������� һ��10s��750�� ��������ÿ10s���������");
    // �ӱ�׼�����ж�ȡ�û������ÿ���ӵ������
    scanf("%d", &clicksPerMinute);

    // ����һ������ѭ���������ڴ�ѭ���в���ִ�����������
    while (1) {
        // ����clickMouse�����������û������ÿ���ӵ��������Ϊ������ģ�������
        clickMouse(clicksPerMinute);
        // ʹ��������60�룬������ÿ����ִ��һ���������Ҫ��
        Sleep(10000);
    }

    // ����������0����ʾ����ɹ�ִ�н���
    return 0;
}
