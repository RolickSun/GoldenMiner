#pragma once

#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "image.h"

#pragma comment(lib,"msimg32.lib")
#pragma comment(lib,"Winmm.lib")

#define WINDOWS_WIDTH 960
#define WINDOWS_HEIGHT 540
#define BIG_GOLD_AMOUNT 3
#define SMALL_GOLD_AMOUNT 5
#define DIAMOND_AMOUNT 3
#define STONE_AMOUNT 5

enum GameState
{
	Begin,
	Running,
	Pause,
	Finished,
};

enum Type
{
	Mouse,
	Diamond,
	Gold,
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
	int score;	//����
	int goal;	//Ŀ��
	int ani_timer;	//������ʱ��
	int ani_index;	//����ͼƬ����
	int getScore;	//��ʾ�÷�
	bool isShow;	//�Ƿ���ʾ�÷�
	wchar_t name[10];	//�������
} Player;

typedef struct
{
	int x;
	int y;
	int size;
	int score;
	IMAGE* image;
	IMAGE* m_image;
	int dir;	//�������������ƶ���0-��1-��
	Type type;	//��Ʒ����
} Object;

typedef struct _node {
	Object object;
	struct _node* next;
} Node;

typedef struct _list {
	Node* head;
} List;

void Add(List* pList, Object obj);
void Delete(List* pList, int index);
void Delete(List* pList, Object* obj);
Object* Find(List* pList, int index);
void SetObjectPosition(List* pList, Object* obj);
bool CollisionDetect(Object obj);
bool CollisionDetect(int rect1x, int rect1y, int rect1width, int rect1height, Object obj);
bool CollisionDetect(Object obj1, Object obj2);
void KeyboardEvent();
void MouseEvent();

void Initialize();
void LoadImages();
void Draw();
void DrawPlayer();
void DrawUI();
void DrawObject();
void DrawHook();
void HookSway();
void ThrowHook();
void HookBack(int speed);
void HookBack(int speed, int index);
void ShushuMove();
void GameBegin();
void GameOver();