#pragma once

#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <math.h>

#pragma comment(lib,"msimg32.lib")

#define WINDOWS_WIDTH 960
#define WINDOWS_HEIGHT 540

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
} Hook;

typedef struct
{
	int x;
	int y;
	int score;
} Player;
