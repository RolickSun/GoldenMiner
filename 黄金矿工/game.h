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
	int score;
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