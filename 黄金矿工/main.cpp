#include "game.h"

IMAGE i_back;
IMAGE i_player[2];
IMAGE i_mplayer[2];
IMAGE i_hook;
IMAGE i_mhook;
IMAGE i_hook2;
IMAGE i_mhook2;

GameState gameState;
MOUSEMSG m; //设置鼠标信息
Hook hook;
Player player;

//加载图片
void LoadImages() {
	loadimage(&i_back, _T(".\\Resources\\pictures\\level-background-0.jpg"));
	loadimage(&i_player[0], _T(".\\Resources\\pictures\\char1.jpg"));
	loadimage(&i_player[1], _T(".\\Resources\\pictures\\char2.jpg"));
	loadimage(&i_mplayer[0], _T(".\\Resources\\pictures\\char1_mask.jpg"));
	loadimage(&i_mplayer[1], _T(".\\Resources\\pictures\\char2_mask.jpg"));
	loadimage(&i_hook, _T(".\\Resources\\pictures\\hook.bmp"));
	loadimage(&i_mhook, _T(".\\Resources\\pictures\\hook_mask.bmp"));
	i_hook2 = i_hook;
	i_mhook2 = i_mhook;
}

//利用蒙版图层打印透明背景图片
void PutImageWithMask(int PosX, int PosY, IMAGE* pImg, IMAGE* pImgMask) {
	putimage(PosX, PosY, pImgMask, NOTSRCERASE);
	putimage(PosX, PosY, pImg, SRCINVERT);
}

//获取键盘事件
void GetKeyboard() {
	if (GetAsyncKeyState(27) & 0x8000) {	//ESC
		gameState = GameOver;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {

	}
}

//获取鼠标事件
void MouseEvent() {
	switch (m.uMsg)//对事件进行分类
	{
	case WM_MOUSEMOVE://如果是鼠标移动事件
		break;
	case WM_LBUTTONDOWN://如果是点击鼠标左键，在点击位置出现数组ch中的坐标
		break;
	case WM_RBUTTONDOWN://如果是点击鼠标右键
		return;//退出
	}

}

//游戏对象初始化
void Initialize() {
	gameState = Running;
	m = GetMouseMsg();

	player.x = WINDOWS_WIDTH / 2 - 50;
	player.y = WINDOWS_HEIGHT / 2 - 240;
	player.score = 0;

	hook.x = player.x+20;
	hook.y = player.y+60;
	hook.length = 0;
	hook.angle = 0;
	hook.dir = 0;
	hook.speed = 5;
}

//绘制钩子
void DrawHook() {
	hook.endx = hook.x + hook.length * sin(hook.angle);
	hook.endy = hook.y + hook.length * cos(hook.angle);
	setlinecolor(LIGHTCYAN);
	line(hook.x+18, hook.y, hook.endx+18, hook.endy);
	PutImageWithMask(hook.x, hook.y, &i_hook2, &i_mhook2);
}

//钩子旋转
void HookSway() {
	if (hook.angle >= 90) {
		hook.dir = 0;
	}
	if (hook.angle <= -90) {
		hook.dir = 1;
	}
	switch (hook.dir)
	{
	case 0:
		hook.angle--;
		break;
	case 1:
		hook.angle++;
		break;
	default:
		break;
	}
	rotateimage(&i_hook2, &i_hook, hook.angle * 3.14 / 180, WHITE, true, true);
	rotateimage(&i_mhook2, &i_mhook, hook.angle * 3.14 / 180, BLACK, true, true);
}

void Start() {
	initgraph(WINDOWS_WIDTH, WINDOWS_HEIGHT);
	LoadImages();
	Initialize();
}

void Update() {
	putimage(0, 0, &i_back);	//绘制背景
	PutImageWithMask(player.x, player.y, &i_player[0], &i_mplayer[0]);	//绘制主角
	DrawHook();
	HookSway();
	GetKeyboard();
	if (MouseHit()) {
		m = GetMouseMsg();
		MouseEvent();
	}
}


int main() {
	Start();
	BeginBatchDraw();
	while (true)
	{
		Update();
		FlushBatchDraw();
		Sleep(1000 / 60);
		cleardevice();
	}
	EndBatchDraw();
}


/*
//创建两个HDC数据结构，前者用作获取当前窗口的绘图设备，后者用作获取内存的绘图设备
	HDC current_hdc = NULL, buffer_hdc = NULL;
	//获取当前窗口DC
	current_hdc = GetDC(GetHWnd());
	//获取内存DC
	buffer_hdc = CreateCompatibleDC(NULL);
	//创建与指定的设备环境相关的设备兼容的位图
	HBITMAP bmp = CreateCompatibleBitmap(current_hdc, WINDOWS_WIDTH, WINDOWS_HEIGHT);
	//用bmp替换内存DC中的位图
	SelectObject(buffer_hdc, bmp);

	//这个HDC结构 用作读取磁盘中的图片
	HDC srcDC = NULL;
	//创建两个IMAGE对象，前者存前景（人物），后者存（背景）
	IMAGE fore = NULL, back = NULL;

	loadimage(&back, _T("E:\\PG files\\pictures\\level-background-0.jpg"));
	srcDC = GetImageHDC(&back);

	//将背景绘入内存DC
	TransparentBlt(buffer_hdc, 0, 0, WINDOWS_WIDTH, WINDOWS_HEIGHT, srcDC, 0, 0, WINDOWS_WIDTH, WINDOWS_HEIGHT, 0x000000);

	//将内存DC中的图像按以像素为单位，绘入当前绘图窗口
	BitBlt(current_hdc, 0, 0, WINDOWS_WIDTH, WINDOWS_HEIGHT, buffer_hdc, 0, 0, SRCCOPY);
*/