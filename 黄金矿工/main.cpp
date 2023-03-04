#include "game.h"

GameState gameState;
IMAGE back;
IMAGE player[2];
IMAGE m_player[2];
MOUSEMSG m; //���������Ϣ

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
	char ch[20];//�ַ���ʱ����
	if (MouseHit()) {
		m = GetMouseMsg();
	}
	switch (m.uMsg)//���¼����з���
	{
	case WM_MOUSEMOVE://���������ƶ��¼�
		sprintf_s(ch, "x:%d y:%d   ", m.x, m.y);//���������뵽ch
		//outtextxy(100, 100, ch);//���ַ���ch�����(100,100)
		break;
	case WM_LBUTTONDOWN://����ǵ�����������ڵ��λ�ó�������ch�е�����
		//outtextxy(m.x, m.y, ch);
		break;
	case WM_RBUTTONDOWN://����ǵ������Ҽ�
		return;//�˳�
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
//��������HDC���ݽṹ��ǰ��������ȡ��ǰ���ڵĻ�ͼ�豸������������ȡ�ڴ�Ļ�ͼ�豸
	HDC current_hdc = NULL, buffer_hdc = NULL;
	//��ȡ��ǰ����DC
	current_hdc = GetDC(GetHWnd());
	//��ȡ�ڴ�DC
	buffer_hdc = CreateCompatibleDC(NULL);
	//������ָ�����豸������ص��豸���ݵ�λͼ
	HBITMAP bmp = CreateCompatibleBitmap(current_hdc, WINDOWS_WIDTH, WINDOWS_HEIGHT);
	//��bmp�滻�ڴ�DC�е�λͼ
	SelectObject(buffer_hdc, bmp);

	//���HDC�ṹ ������ȡ�����е�ͼƬ
	HDC srcDC = NULL;
	//��������IMAGE����ǰ�ߴ�ǰ������������ߴ棨������
	IMAGE fore = NULL, back = NULL;

	loadimage(&back, _T("E:\\PG files\\pictures\\level-background-0.jpg"));
	srcDC = GetImageHDC(&back);

	//�����������ڴ�DC
	TransparentBlt(buffer_hdc, 0, 0, WINDOWS_WIDTH, WINDOWS_HEIGHT, srcDC, 0, 0, WINDOWS_WIDTH, WINDOWS_HEIGHT, 0x000000);

	//���ڴ�DC�е�ͼ��������Ϊ��λ�����뵱ǰ��ͼ����
	BitBlt(current_hdc, 0, 0, WINDOWS_WIDTH, WINDOWS_HEIGHT, buffer_hdc, 0, 0, SRCCOPY);
*/