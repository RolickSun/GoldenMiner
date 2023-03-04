#include "game.h"

GameState gameState;
IMAGE back;
IMAGE player[2];
IMAGE m_player[2];
MOUSEMSG m; //设置鼠标信息

void LoadImages() {
	loadimage(&back, _T("E:\\PG files\\pictures\\level-background-0.jpg"));
	loadimage(&player[0], _T("E:\\PG files\\pictures\\char1.jpg"));
	loadimage(&player[1], _T("E:\\PG files\\pictures\\char2.jpg"));
	loadimage(&m_player[0], _T("E:\\PG files\\pictures\\char1_mask.jpg"));
	loadimage(&m_player[1], _T("E:\\PG files\\pictures\\char2_mask.jpg"));
}

void PutImageWithMask(int PosX, int PosY, IMAGE* pImg, IMAGE* pImgMask) {
	putimage(PosX, PosY, pImgMask, NOTSRCERASE);
	putimage(PosX, PosY, pImg, SRCINVERT);
}

void GetKeyboard() {
	if (GetAsyncKeyState(27) & 0x8000) {	//ESC
		gameState = GameOver;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {

	}
}

void MouseEvent() {
	char ch[20];//字符临时变量
	if (MouseHit()) {
		m = GetMouseMsg();
	}
	switch (m.uMsg)//对事件进行分类
	{
	case WM_MOUSEMOVE://如果是鼠标移动事件
		sprintf_s(ch, "x:%d y:%d   ", m.x, m.y);//将坐标输入到ch
		//outtextxy(100, 100, ch);//将字符串ch输出到(100,100)
		break;
	case WM_LBUTTONDOWN://如果是点击鼠标左键，在点击位置出现数组ch中的坐标
		//outtextxy(m.x, m.y, ch);
		break;
	case WM_RBUTTONDOWN://如果是点击鼠标右键
		return;//退出
	}

}

void Start() {
	initgraph(WINDOWS_WIDTH, WINDOWS_HEIGHT);
	gameState = Running;
	LoadImages();
	m = GetMouseMsg();
}

void Update() {
	putimage(0, 0, &back);
	PutImageWithMask(WINDOWS_WIDTH / 2 - 50, WINDOWS_HEIGHT / 2 - 200, &player[0], &m_player[0]);
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