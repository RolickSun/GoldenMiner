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
	bool isThrow;	//�ж��Ƿ��ӳ�����
	bool isBack;	//�ж��Ƿ��ջع���
	//int dx;	//ƫ����x
	//int dy;	//ƫ����y
} Hook;

typedef struct
{
	int x;
	int y;
	int score;
} Player;


void HookSway();
void DrawHook();
void ThrowHook();
void HookBack(int speed);