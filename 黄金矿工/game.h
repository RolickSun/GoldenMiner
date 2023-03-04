#pragma once

#include <graphics.h>
#include <conio.h>
#include <stdio.h>

#pragma comment(lib,"msimg32.lib")


#define WINDOWS_WIDTH 960
#define WINDOWS_HEIGHT 540

enum GameState
{
	Running,
	Pause,
	GameOver,
};
