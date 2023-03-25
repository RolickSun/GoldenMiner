#pragma once

#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

//#pragma comment(lib,"msimg32.lib")

#define WINDOWS_WIDTH 960
#define WINDOWS_HEIGHT 540
#define BIG_GOLD_AMOUNT 3
#define SMALL_GOLD_AMOUNT 5
#define DIAMOND_AMOUNT 3
#define STONE_AMOUNT 5

enum GameState
{
	Running,
	Pause,
	Finished,
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
	int dir;	//�������������ƶ���0-��1-��
	int isMove;	//�Ƿ��ƶ�����������Ƿ�������
	int takeAble;	//�ܷ�������ȡ
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
void LoadImages();
void FlipImage(IMAGE* pDst, IMAGE* pSrc);
void PutImageWithMask(int PosX, int PosY, IMAGE* pImg, IMAGE* pImgMask);
void GetKeyboard();
void MouseEvent();
void Initialize();
void DrawPlayer();
void DrawBackground();
void DrawUI();
void DrawObject();
void DrawHook();
void HookSway();
void ThrowHook();
void HookBack(int speed);
void HookBack(int speed, int index);
void ShushuMove();
void SetObjectPosition(List* pList, Object* obj);
bool CollisionDetect(Object obj);
bool CollisionDetect(int rect1x, int rect1y, int rect1width, int rect1height, Object obj);
bool CollisionDetect(Object obj1, Object obj2);
void GameOver();