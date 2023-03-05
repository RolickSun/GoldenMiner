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
	int x;	//���x
	int y;	//���y
	int endx;	//�յ�x
	int endy;	//�յ�y
	int angle;	//�Ƕ� (�Ƕ��� -90~90 0��ʱ������ֵ����)
	int dir;	//���� 0-��1-��
	int length;	//����
	int speed;	//�����ٶ�
} Hook;

typedef struct
{
	int x;
	int y;
	int score;
} Player;
