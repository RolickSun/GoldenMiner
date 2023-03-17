#include "game.h"

IMAGE i_back;
IMAGE i_brick;
IMAGE i_player[2];
IMAGE i_mplayer[2];
IMAGE i_hook;
IMAGE i_mhook;
IMAGE i_hook2;
IMAGE i_mhook2;
IMAGE i_clear;

IMAGE i_biggold;
IMAGE i_mbiggold;
IMAGE i_smallgold;
IMAGE i_msmallgold;
IMAGE i_diamond;
IMAGE i_mdiamond;

GameState gameState;
MOUSEMSG m; //���������Ϣ
Hook hook;
Player player;
List list;	//��������������Ϸ����

//�������
void Add(List* pList, Object obj) {
	Node* p = (Node*)malloc(sizeof(Node));
	p->object = obj;
	p->next = NULL;

	Node* last;
	last = pList->head;
	if (last) {
		while (last->next) {
			last = last->next;
		}
		last->next = p;
	}
	else {
		pList->head = p;
	}
}

//����ɾ��
void Delete(List* pList, int index) {
	Node* p, * q;
	int i = 0;
	for (p = pList->head, q = NULL; p; q = p, p = p->next) {
		if (i==index) {
			if (q) {
				q->next = p->next;	//q�ĺ�λ����p�ĺ�λ�����ǰ�p�������Ϻ��Ե�
			}
			else {	//�����λ����Ӧɾȥ��ֵ
				pList->head = p->next;	//����ͷΪp�ĺ�λ
			}
			free(p);
			break;
		}
		i++;
	}

}

//ͨ�������Ҷ���
Object* Find(List* pList, int index) {
	Node* p;
	Object *obj;
	int i = 0;
	for (p = list.head; p; p = p->next) {
		if (i == index) {
			obj = &(p->object);
			return obj;
		}
		i++;
	}
	return NULL;
}

//����ͼƬ
void LoadImages() {
	loadimage(&i_back, _T(".\\Resources\\pictures\\level-background-0.jpg"));
	loadimage(&i_brick, _T(".\\Resources\\pictures\\brick.png"),WINDOWS_WIDTH+30,0,true);
	loadimage(&i_player[0], _T(".\\Resources\\pictures\\char1.jpg"));
	loadimage(&i_player[1], _T(".\\Resources\\pictures\\char2.jpg"));
	loadimage(&i_mplayer[0], _T(".\\Resources\\pictures\\char1_mask.jpg"));
	loadimage(&i_mplayer[1], _T(".\\Resources\\pictures\\char2_mask.jpg"));
	loadimage(&i_hook, _T(".\\Resources\\pictures\\hook.bmp"));
	loadimage(&i_mhook, _T(".\\Resources\\pictures\\hook_mask.bmp"));
	loadimage(&i_biggold, _T(".\\Resources\\pictures\\big_gold.png"));
	loadimage(&i_mbiggold, _T(".\\Resources\\pictures\\big_gold_mask.png"));
	loadimage(&i_smallgold, _T(".\\Resources\\pictures\\small_gold.bmp"));
	loadimage(&i_msmallgold, _T(".\\Resources\\pictures\\small_gold_mask.bmp"));
	loadimage(&i_diamond, _T(".\\Resources\\pictures\\diamond.png"));
	loadimage(&i_mdiamond, _T(".\\Resources\\pictures\\diamond_mask.png"));
	loadimage(&i_clear, _T(".\\Resources\\pictures\\clear.png"));
	i_hook2 = i_hook;
	i_mhook2 = i_mhook;
}

//�����ɰ�ͼ���ӡ͸������ͼƬ
void PutImageWithMask(int PosX, int PosY, IMAGE* pImg, IMAGE* pImgMask) {
	putimage(PosX, PosY, pImgMask, NOTSRCERASE);
	putimage(PosX, PosY, pImg, SRCINVERT);
}

//��ȡ�����¼�
void GetKeyboard() {
	if (GetAsyncKeyState(27) & 0x8000) {	//ESC
		gameState = Clear;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		if (hook.isThrow)	//��������Ѿ�Ͷ�������ټ���
			return;
		ThrowHook();
	}
}

//��ȡ����¼�
void MouseEvent() {
	switch (m.uMsg)//���¼����з���
	{
	case WM_MOUSEMOVE://���������ƶ��¼�
		break;
	case WM_LBUTTONDOWN://����ǵ��������
		if (hook.isThrow)	//��������Ѿ�Ͷ�������ټ���
			return;
		ThrowHook();
		break;
	case WM_RBUTTONDOWN://����ǵ������Ҽ�
		return;//�˳�
	}

}

//��Ϸ�����ʼ��
void Initialize() {
	gameState = Running;
	m = GetMouseMsg();
	srand(time(NULL));

	player.x = WINDOWS_WIDTH / 2 - 50;
	player.y = WINDOWS_HEIGHT / 2 - 240;
	player.score = 0;
	player.goal = 300;
	player.timer = 0;
	player.index = 0;

	hook.x = player.x+20;
	hook.y = player.y+60;
	hook.length = 0;
	hook.angle = 0;
	hook.dir = 0;
	hook.speed = 5;
	hook.isThrow = false;
	hook.dx = 18;
	hook.dy = 0;
	hook.carry = 0;

	list.head = NULL;
	for (int i = 0; i < BIG_GOLD_AMOUNT; i++) {
		Object bigGold;
		bigGold.image = &i_biggold;
		bigGold.m_image = &i_mbiggold;
		SetObjectPosition(&list, &bigGold);
		bigGold.size = rand() % 3 + 6;	//��Χ6-8
		bigGold.score = 20 * bigGold.size;	//120-160
		Add(&list, bigGold);
	}
	for (int i = 0; i < SMALL_GOLD_AMOUNT; i++) {
		Object smallGold;
		smallGold.image = &i_smallgold;
		smallGold.m_image = &i_msmallgold;
		SetObjectPosition(&list, &smallGold);
		smallGold.size = rand() % 3 + 1;	//��Χ1-3
		smallGold.score = 20 * smallGold.size;	//20-60
		Add(&list, smallGold);
	}
	for (int i = 0; i < DIAMOND_AMOUNT; i++) {
		Object diamond;
		diamond.image = &i_diamond;
		diamond.m_image = &i_mdiamond;
		SetObjectPosition(&list, &diamond);
		diamond.size = 1;
		diamond.score = 200;
		Add(&list, diamond);
	}
}

//���ƽ�ɫ
void DrawPlayer() {
	if (hook.isThrow) {
		player.timer++;
		if (player.timer > 10) {
			player.timer = 0;
			player.index = (player.index + 1) % 2;
		}
		PutImageWithMask(player.x, player.y, &i_player[player.index], &i_mplayer[player.index]);
	}
	else {
		PutImageWithMask(player.x, player.y, &i_player[0], &i_mplayer[0]);	//��������
	}
}

//���Ʊ���
void DrawBackground()
{
	putimage(0, 0, &i_back);
	putimage(-10, 100, &i_brick);
}

//����UI
void DrawUI() {
	LOGFONT font;
	gettextstyle(&font);
	font.lfHeight = 32;	//ָ������߶�48
	font.lfQuality = ANTIALIASED_QUALITY;	//���忹���
	_tcscpy_s(font.lfFaceName, L"����");
	settextstyle(&font);
	setbkmode(TRANSPARENT);

	TCHAR scoreText[30];
	_stprintf_s(scoreText, _T("������%d"), player.score);
	outtextxy(0,0,scoreText);

	TCHAR goalText[30];
	_stprintf_s(goalText, _T("Ŀ�꣺%d"), player.goal);
	outtextxy(0, 40, goalText);
}


//������Ϸ����
void DrawObject() {
	Node* p;
	for (p = list.head; p; p = p->next) {
		PutImageWithMask(p->object.x, p->object.y, p->object.image, p->object.m_image);
		setlinecolor(GREEN);
		rectangle(p->object.x, p->object.y, p->object.x + p->object.image->getwidth(),p->object.y+ p->object.image->getheight());
	}
}

//���ƹ���
void DrawHook() {
	hook.endx = hook.x + hook.length * sin(hook.angle*3.14/180);
	hook.endy = hook.y + hook.length * cos(hook.angle*3.14/180);
	setlinestyle(PS_SOLID, 2);
	setlinecolor(DARKGRAY);
	line(hook.x, hook.y, hook.endx, hook.endy);
	PutImageWithMask(hook.endx-hook.dx, hook.endy-hook.dy, &i_hook2, &i_mhook2);

	//������ӱ�Ͷ��
	if (hook.isThrow) {
		int index=0;
		//�����ײ��ǽ��
		if (hook.endx > WINDOWS_WIDTH || hook.endy > WINDOWS_HEIGHT || hook.endx < 0) {
			hook.isBack = true;
		}
		//�����ײ������
		Node* p;
		for (p = list.head; p; p = p->next) {
			if (CollisionDetect(p->object)) {
				hook.isBack = true;
				hook.carry = p->object.size;
				break;
			}
			index++;
		}

		if (hook.isBack)
			if (hook.carry == 0)	//û�и��أ�֤��û��ץ����Ʒ
				HookBack(2 * hook.speed);
			else
				HookBack(2 * hook.speed - hook.carry , index);	//ץ������Ʒ
		else
			ThrowHook();

	}
	//rectangle(hook.endx - hook.dx, hook.endy-hook.dy, hook.endx -hook.dx+ i_hook2.getwidth(), hook.endy-hook.dy + i_hook2.getheight());
}

//������ת
void HookSway() {
	if (hook.isThrow) {	//����ӳ����ӣ��򷵻�
		return;
	}

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

	if (hook.angle > 0) {
		hook.dy = 18 * sin(hook.angle * 3.14 / 180);
		hook.dx = 35 * cos(hook.angle * 3.14 / 180) - 18 * cos(hook.angle * 3.14 / 180);
	}
	else {
		hook.dy = 18 * sin(abs(hook.angle) * 3.14 / 180);
		hook.dx = 20 * sin(abs(hook.angle) * 3.14 / 180) + 18 * cos(abs(hook.angle) * 3.14 / 180);
	}
}

//Ͷ������
void ThrowHook() {
	hook.isThrow = true;
	hook.isBack = false;
	hook.length += hook.speed;
}

//�ջع���
void HookBack(int speed) {
	hook.length -= speed;
	if (hook.length <= 0) {
		hook.length = 0;
		hook.isThrow = false;
	}
}

//�ջع��������
void HookBack(int speed, int index) {
	hook.length -= speed;
	Object* obj = Find(&list, index);
	if (obj == NULL) {
		return;
	}
	else {
		obj->x = hook.endx - hook.dx;
		obj->y = hook.endy - hook.dy;
		if (hook.length <= 0) {
			hook.length = 0;
			hook.carry = 0;
			player.score += obj->score;
			Delete(&list, index);
			hook.isThrow = false;
		}
	}
}

//���ö���λ��
void SetObjectPosition(List *pList,Object *obj) {
	if (pList->head == NULL) {
		//��һ��������Ҫ�����ײ
		obj->x = rand() % (WINDOWS_WIDTH - 50);
		obj->y = rand() % (WINDOWS_HEIGHT - 160) + 110;
		return;
	}

	int x, y;
	Node* p = pList->head;
	while (1)
	{
		x = rand() % (WINDOWS_WIDTH - 50);
		y = rand() % (WINDOWS_HEIGHT - 160) + 110;
		//�������е�ÿһ������ײ
		while (p) {
			if (CollisionDetect(x, y, obj->image->getwidth(), obj->image->getheight(), p->object)) {
				p = pList->head;	//�����ײ����������λ�ã����¼��
				break;
			}
			p = p->next;
		}
		//�����е�ÿһ�������
		if (p == NULL) {
			break;
		}
	}
	obj->x = x;
	obj->y = y;
	return;
}

//��ײ���
bool CollisionDetect(Object obj) {
	int rect1x = hook.endx-hook.dx;
	int rect1y = hook.endy-hook.dy;
	int rect1width = i_hook2.getwidth();
	int rect1height = i_hook2.getheight();
	int rect2x = obj.x;
	int rect2y = obj.y;
	int rect2width = obj.image->getwidth();
	int rect2height = obj.image->getheight();

	if ((rect1x+rect1width)>=rect2x &&
		(rect2x+rect2width)>=rect1x &&
		(rect1y+rect1height)>=rect2y &&
		(rect2y+rect2height)>=rect1y)
	{
		return true;
	}
	return false;
}

//��ײ���
bool CollisionDetect(int rect1x, int rect1y, int rect1width, int rect1height, Object obj) {
	int rect2x = obj.x;
	int rect2y = obj.y;
	int rect2width = obj.image->getwidth();
	int rect2height = obj.image->getheight();

	if ((rect1x + rect1width) >= rect2x &&
		(rect2x + rect2width) >= rect1x &&
		(rect1y + rect1height) >= rect2y &&
		(rect2y + rect2height) >= rect1y)
	{
		return true;
	}
	return false;
}

void GameOver() {
	putimage(0, 0, &i_clear);
}

void Start() {
	initgraph(WINDOWS_WIDTH, WINDOWS_HEIGHT);
	LoadImages();
	Initialize();
}

void Update() {
	switch (gameState)
	{
	case Running:
		DrawBackground();
		DrawPlayer();
		DrawObject();
		DrawUI();

		DrawHook();	//���ƹ���
		HookSway();	//���Ӱڶ�

		GetKeyboard();
		if (MouseHit()) {
			m = GetMouseMsg();
			MouseEvent();
		}
		//����Ƿ������ȫ�����
		if (list.head == NULL) {
			gameState = Clear;
		}
		break;

	case Clear:
		GameOver();
		break;

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