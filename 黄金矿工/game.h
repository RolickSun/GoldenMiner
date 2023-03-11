#pragma once

#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#pragma comment(lib,"msimg32.lib")

#define WINDOWS_WIDTH 960
#define WINDOWS_HEIGHT 540
#define BIG_GOLD_AMOUNT 3
#define SMALL_GOLD_AMOUNT 5
#define DIAMOND_AMOUNT 3

enum GameState
{
	Running,
	Pause,
	GameOver,
};

typedef struct 
{
	int x;	//���x
	int y;	//���y
	int endx;	//�յ�x
	int endy;	//�յ�y
	int angle;	//�Ƕ� (�Ƕ��� -90~90 0��ʱ������ֵ����)
	int dir;	//���� 0-��1-��
	int length;	//����
	int speed;	//�����ٶ�
	bool isThrow;	//�ж��Ƿ��ӳ�����
	bool isBack;	//�ж��Ƿ��ջع���
	int dx;	//ƫ����x
	int dy;	//ƫ����y
	int carry;	//����
} Hook;

typedef struct
{
	int x;
	int y;
	int score;
	int timer;	//������ʱ��
	int index;	//����ͼƬ����
} Player;

typedef struct
{
	int x;
	int y;
	int size;
	int score;
	IMAGE* image;
	IMAGE* m_image;
} Object;

typedef struct _node {
	Object object;
	struct _node* next;
} Node;

typedef struct _list {
	Node* head;
} List;

void HookSway();
void DrawHook();
void ThrowHook();
void HookBack(int speed);
void HookBack(int speed, Object* obj);
bool CollisionDetect(Object obj);