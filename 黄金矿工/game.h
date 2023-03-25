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
	int x;	//起点x
	int y;	//起点y
	int endx;	//终点x
	int endy;	//终点y
	int angle;	//角度 (角度制 -90~90 0°时代表数值向下)
	int dir;	//方向 0-左，1-右
	int length;	//长度
	int speed;	//钩子速度
	bool isThrow;	//判断是否扔出钩子
	bool isBack;	//判断是否收回钩子
	int dx;	//偏移量x
	int dy;	//偏移量y
	int carry;	//负载
} Hook;

typedef struct
{
	int x;
	int y;
	int score;	//分数
	int goal;	//目标
	int timer;	//动画计时器
	int index;	//动画图片索引
} Player;

typedef struct
{
	int x;
	int y;
	int size;
	int score;
	IMAGE* image;
	IMAGE* m_image;
	int dir;	//方向，用于老鼠移动，0-左，1-右
	int isMove;	//是否移动，用来标记是否是老鼠
	int takeAble;	//能否被老鼠拿取
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