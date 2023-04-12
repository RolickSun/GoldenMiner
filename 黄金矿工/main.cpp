#include "game.h"

#pragma region Images

IMAGE i_back;
IMAGE i_brick;
IMAGE i_player[2];
IMAGE i_mplayer[2];
IMAGE i_hook[2];
IMAGE i_mhook[2];
IMAGE i_clear;
IMAGE i_end;
IMAGE i_biggold;
IMAGE i_mbiggold;
IMAGE i_smallgold;
IMAGE i_msmallgold;
IMAGE i_diamond;
IMAGE i_mdiamond;
IMAGE i_stone;
IMAGE i_mstone;
IMAGE i_shushu_left[2];
IMAGE i_mshushu_left[2];
IMAGE i_shushu_right[2];
IMAGE i_mshushu_right[2];
IMAGE pause_background;
IMAGE img;
IMAGE i_start;
IMAGE i_logo;
IMAGE i_startbottom;

#pragma endregion
IMAGE img_tmp[BIG_GOLD_AMOUNT + SMALL_GOLD_AMOUNT + STONE_AMOUNT][2];
GameState gameState;
MOUSEMSG m; //���������Ϣ
Hook hook;
Player player;
List list;	//��������������Ϸ����
int Time = 30;	//��Ϸ����ʱ
int timer = 0;	//��ʱ��

//TCHAR debugText[30];
//clock_t start, finish;	//debug
//clock_t await=clock_t(0);	//debug
//clock_t start2, finish2;	//debug
//int count = 0;	//debug

//�������
void Add(List* pList, Object obj) {
	Node* p = (Node*)malloc(sizeof(Node));
	if (p) {
		p->object = obj;
		p->next = NULL;
	}

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

//����ɾ��
void Delete(List* pList, Object* obj) {
	Node* p, * q;
	for (p = pList->head, q = NULL; p; q = p, p = p->next) {
		if (&p->object==obj) {
			if (q) {
				q->next = p->next;	//q�ĺ�λ����p�ĺ�λ�����ǰ�p�������Ϻ��Ե�
			}
			else {	//�����λ����Ӧɾȥ��ֵ
				pList->head = p->next;	//����ͷΪp�ĺ�λ
			}
			free(p);
			break;
		}
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
	loadimage(&i_hook[0], _T(".\\Resources\\pictures\\hook.bmp"));
	loadimage(&i_mhook[0], _T(".\\Resources\\pictures\\hook_mask.bmp"));
	loadimage(&i_biggold, _T(".\\Resources\\pictures\\big_gold.png"));
	loadimage(&i_mbiggold, _T(".\\Resources\\pictures\\big_gold_mask.png"));
	loadimage(&i_smallgold, _T(".\\Resources\\pictures\\small_gold.bmp"));
	loadimage(&i_msmallgold, _T(".\\Resources\\pictures\\small_gold_mask.bmp"));
	loadimage(&i_diamond, _T(".\\Resources\\pictures\\diamond.png"));
	loadimage(&i_mdiamond, _T(".\\Resources\\pictures\\diamond_mask.png"));
	loadimage(&i_stone, _T(".\\Resources\\pictures\\stone.png"));
	loadimage(&i_mstone, _T(".\\Resources\\pictures\\stone_mask.png"));
	loadimage(&i_shushu_left[0], _T(".\\Resources\\pictures\\shushu.png"));
	loadimage(&i_shushu_left[1], _T(".\\Resources\\pictures\\shushu_diamond.png"));
	loadimage(&i_mshushu_left[0], _T(".\\Resources\\pictures\\shushu_mask.png"));
	loadimage(&i_mshushu_left[1], _T(".\\Resources\\pictures\\shushu_diamond_mask.png"));
	loadimage(&i_clear, _T(".\\Resources\\pictures\\clear.png"));
	loadimage(&i_end, _T(".\\Resources\\pictures\\end.jpg"));
	loadimage(&pause_background, _T(".\\Resources\\pictures\\pause_background.png"));
	loadimage(&i_start, _T(".\\Resources\\pictures\\start.png"));
	loadimage(&i_logo, _T(".\\Resources\\pictures\\logo.png"));
	loadimage(&i_startbottom, _T(".\\Resources\\pictures\\startbottom2.png"));
	i_hook[1] = i_hook[0];
	i_mhook[1] = i_mhook[0];

	for (int i = 0; i < 2; i++) {
		i_shushu_right[i] = i_shushu_left[i];
		FlipImage(&i_shushu_right[i], &i_shushu_left[i]);
		i_mshushu_right[i] = i_mshushu_left[i];
		FlipImage(&i_mshushu_right[i], &i_mshushu_left[i]);
	}
}

//���ҷ�תͼƬ
void FlipImage(IMAGE* pDst, IMAGE* pSrc) {
	DWORD* pdSrc = GetImageBuffer(pSrc);
	DWORD* pdDst = GetImageBuffer(pDst);
	int width = pSrc->getwidth();
	int height = pSrc->getheight();
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int r = GetRValue(pdSrc[x + y * width]);
			int g = GetGValue(pdSrc[x + y * width]);
			int b = GetBValue(pdSrc[x + y * width]);
			pdDst[width - x + y * width] = RGB(r, g, b);
		}
	}
}

//�����ɰ�ͼ���ӡ͸������ͼƬ
void PutImageWithMask(int PosX, int PosY, IMAGE* pImg, IMAGE* pImgMask) {
	putimage(PosX, PosY, pImgMask, NOTSRCERASE);
	putimage(PosX, PosY, pImg, SRCINVERT);
}

//��ӡ͸������pngͼ
void TransparentImage(IMAGE* dstimg, int x, int y, IMAGE* srcimg) {
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();
	BLENDFUNCTION bf = { AC_SRC_OVER,0,255,AC_SRC_ALPHA };
	AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}

//��ȡ�����¼�
void GetKeyboard() {
	if (GetAsyncKeyState(27) & 0x8000) {	//ESC
		getimage(&img, 0, 0, WINDOWS_WIDTH, WINDOWS_HEIGHT);
		gameState = Pause;
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
		//_stprintf_s(debugText, _T("x:%d y:%d"), m.x, m.y);
		break;
	case WM_LBUTTONDOWN://����ǵ��������
		if (gameState == Running) {
			if (hook.isThrow)	//��������Ѿ�Ͷ�������ټ���
				return;
			ThrowHook();
		}
		if (gameState == Pause) {
			if (m.x > 85 && m.x < 177 && m.y>178 && m.y < 214) {	//����
				gameState = Running;
			}
			if (m.x > 238 && m.x < 328 && m.y>178 && m.y < 214) {	//�˳�
				closegraph();
				exit(0);
			}
		}
		if (gameState == Begin) {
			if (m.x > 90 && m.x < 236 && m.y>120 && m.y < 207) {	//��ʼ��Ϸ
				InputBox(player.name, 10, L"�������������", L"���������", L"player");
				Resize(NULL, WINDOWS_WIDTH, WINDOWS_HEIGHT);
				gameState = Running;
			}
		}
		break;
	case WM_RBUTTONDOWN://����ǵ������Ҽ�
		return;
	}

}

//��Ϸ�����ʼ��
void Initialize() {
	gameState = Begin;
	m = GetMouseMsg();
	srand(time(NULL));

	player.x = WINDOWS_WIDTH / 2 - 50;
	player.y = WINDOWS_HEIGHT / 2 - 240;
	player.score = 0;
	player.goal = 500;
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
	for (int i = 0; i < BIG_GOLD_AMOUNT; i++) {		// ���ɴ���
		Object bigGold;
		bigGold.size = rand() % 3 + 6;	//��Χ6-8
		bigGold.score = 20 * bigGold.size;	//120-160
		double scale = (bigGold.size - 6) / 10.0 + 1;
		img_tmp[i][0] = ZoomImage(&i_biggold, scale);
		img_tmp[i][1] = ZoomImage(&i_mbiggold, scale);
		bigGold.image = &img_tmp[i][0];
		bigGold.m_image = &img_tmp[i][1];
		SetObjectPosition(&list, &bigGold);
		bigGold.isMove = 0;
		bigGold.takeAble = 0;
		Add(&list, bigGold);
	}
	for (int i = 0; i < SMALL_GOLD_AMOUNT; i++) {	//����С���
		Object smallGold;
		smallGold.size = rand() % 3 + 1;	//��Χ1-3
		smallGold.score = 20 * smallGold.size;	//20-60
		double scale = (smallGold.size - 1) / 8.0 + 1;
		img_tmp[BIG_GOLD_AMOUNT + i][0] = ZoomImage(&i_smallgold, scale);
		img_tmp[BIG_GOLD_AMOUNT + i][1] = ZoomImage(&i_msmallgold, scale);
		smallGold.image = &img_tmp[BIG_GOLD_AMOUNT + i][0];
		smallGold.m_image = &img_tmp[BIG_GOLD_AMOUNT + i][1];
		SetObjectPosition(&list, &smallGold);
		smallGold.isMove = 0;
		smallGold.takeAble = 0;
		Add(&list, smallGold);
	}
	for (int i = 0; i < STONE_AMOUNT; i++) {		//����ʯͷ
		Object stone;
		img_tmp[BIG_GOLD_AMOUNT + SMALL_GOLD_AMOUNT+i][0] = ZoomImage(&i_stone, 1.6);
		img_tmp[BIG_GOLD_AMOUNT + SMALL_GOLD_AMOUNT+i][1] = ZoomImage(&i_mstone, 1.6);
		stone.image = &img_tmp[BIG_GOLD_AMOUNT + SMALL_GOLD_AMOUNT + i][0];
		stone.m_image = &img_tmp[BIG_GOLD_AMOUNT + SMALL_GOLD_AMOUNT + i][1];
		SetObjectPosition(&list, &stone);
		stone.size = 9;
		stone.score = 10;
		stone.isMove = 0;
		stone.takeAble = 0;
		Add(&list, stone);
	}
	for (int i = 0; i < DIAMOND_AMOUNT; i++) {		//������ʯ������
		Object diamond;
		diamond.image = &i_diamond;
		diamond.m_image = &i_mdiamond;
		SetObjectPosition(&list, &diamond);
		diamond.size = 1;
		diamond.score = 200;
		diamond.isMove = 0;
		diamond.takeAble = 1;
		Add(&list, diamond);

		Object mouse;
		mouse.dir = rand() % 2;
		mouse.y = diamond.y;
		if (mouse.dir == 0) {
			mouse.x = WINDOWS_WIDTH;
			mouse.image = &i_shushu_left[0];
			mouse.m_image = &i_mshushu_left[0];
		}
		else {
			mouse.x = 0;
			mouse.image = &i_shushu_right[0];
			mouse.m_image = &i_mshushu_right[0];
		}
		mouse.size = 1;
		mouse.score = 10;
		mouse.isMove = 1;
		mouse.takeAble = -1;
		Add(&list, mouse);
	}

	//��������ļ�
	mciSendString(_T("open .\\Resources\\sound\\bgm.mp3 alias bgm"), 0, 0, 0);
	mciSendString(_T("open .\\Resources\\sound\\high-value.mp3 alias high-value"), 0, 0, 0);
}

//����ͼ��
void Draw() {
	putimage(0, 0, &i_back);	//���Ʊ���
	putimage(-10, 100, &i_brick);

	DrawPlayer();
	DrawObject();
	DrawHook();
	DrawUI();
}

//���ƽ�ɫ
void DrawPlayer() {
	if (hook.isThrow) {	//�������Ͷ�������Ŷ���
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

//����UI
void DrawUI() {
	LOGFONT font;
	gettextstyle(&font);
	font.lfHeight = 32;	//ָ������߶�48
	font.lfQuality = ANTIALIASED_QUALITY;	//���忹���
	_tcscpy_s(font.lfFaceName, L"����");
	settextstyle(&font);
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);

	TCHAR scoreText[30];
	_stprintf_s(scoreText, _T("��Ǯ��$%d"), player.score);
	outtextxy(10,10,scoreText);

	TCHAR goalText[30];
	_stprintf_s(goalText, _T("Ŀ��Ǯ����$%d"), player.goal);
	outtextxy(10, 50, goalText);

	TCHAR timeText[30];
	_stprintf_s(timeText, _T("ʱ�䣺%d"), Time);
	outtextxy(800, 10, timeText);

	TCHAR nameText[30];
	_stprintf_s(nameText, _T("������ƣ�%s"), player.name);
	outtextxy(800-(wcslen(player.name)*16), 50, nameText);
}


//������Ϸ����
void DrawObject() {
	Node* p;
	for (p = list.head; p; p = p->next) {
		PutImageWithMask(p->object.x, p->object.y, p->object.image, p->object.m_image);
		//debug
		//setlinecolor(GREEN);
		//rectangle(p->object.x, p->object.y, p->object.x + p->object.image->getwidth(),p->object.y+ p->object.image->getheight());
	}
}

//���ƹ���
void DrawHook() {
	hook.endx = hook.x + hook.length * sin(hook.angle*3.14/180);	//���㹳��ĩ������
	hook.endy = hook.y + hook.length * cos(hook.angle*3.14/180);
	setlinestyle(PS_SOLID, 2);
	setlinecolor(DARKGRAY);
	line(hook.x, hook.y, hook.endx, hook.endy);	//��������
	PutImageWithMask(hook.endx-hook.dx, hook.endy-hook.dy, &i_hook[1], &i_mhook[1]);

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

		if (hook.isBack)	//���ӻ���
			if (hook.carry == 0)	//û�и��أ�֤��û��ץ����Ʒ
				HookBack(2 * hook.speed);
			else
				HookBack(2 * hook.speed - hook.carry ,index);	//ץ������Ʒ
		else
			ThrowHook();

	}
	//debug
	//rectangle(hook.endx - hook.dx, hook.endy-hook.dy, hook.endx -hook.dx+ i_hook[1].getwidth(), hook.endy - hook.dy + i_hook[1].getheight());
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
	rotateimage(&i_hook[1], &i_hook[0], hook.angle * 3.14 / 180, WHITE, true, true);	//��ת����ͼƬ
	rotateimage(&i_mhook[1], &i_mhook[0], hook.angle * 3.14 / 180, BLACK, true, true);

	//����ƫ������ʹ�����Ӿ�����������ת
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
	mciSendString(_T("open .\\Resources\\sound\\dig.mp3 alias dig"), 0, 0, 0);
	mciSendString(_T("play dig"), 0, 0, 0);
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
	Object* obj = Find(&list, index);	//ͨ�������ҵ�Ҫ�ջصĶ���
	if (obj == NULL) {
		return;
	}
	else {
		obj->x = hook.endx - hook.dx;
		obj->y = hook.endy - hook.dy;
		if (hook.length <= 0) {
			if (obj->score <= 10) {
				mciSendString(_T("open .\\Resources\\sound\\low-value.mp3 alias low-value"), 0, 0, 0);
				mciSendString(_T("play low-value"), 0, 0, 0);
			}
			else {
				mciSendString(_T("open .\\Resources\\sound\\normal-value.mp3 alias normal-value"), 0, 0, 0);
				mciSendString(_T("play normal-value"), 0, 0, 0);
			}
			hook.length = 0;
			hook.carry = 0;
			player.score += obj->score;	//�������ۼ�
			Delete(&list, index);	//ɾ����Ʒ
			hook.isThrow = false;
		}
	}
}

//�����ƶ�
void ShushuMove() {
	Node* p;
	for (p = list.head; p; p = p->next) {
		if (p->object.isMove == 1) {	//ͨ���ڵ�p�����ҵ�����
			switch (p->object.dir)
			{
			case 0:	//��
				p->object.x = p->object.x - 1;
				{
					Node* q;	//ͨ���ڵ�q���������ײ
					for (q=list.head; q; q = q->next) {
						if (CollisionDetect(p->object, q->object) && q->object.takeAble == 1) {	//���ץ������ʯ
							p->object.image = &i_shushu_left[1];
							p->object.m_image = &i_mshushu_left[1];
							p->object.score = 210;
							Delete(&list, &q->object);
							break;
						}
						if (p->object.score == 210) {	//����Ѿ�ץ������ʯ
							if (CollisionDetect(p->object, q->object)&&q->object.takeAble==0) {	//��������ͷ���
								p->object.image = &i_shushu_right[1];
								p->object.m_image = &i_mshushu_right[1];
								p->object.dir = 1;
							}
						}
					}
				}
				break;
			case 1:	//��
				p->object.x = p->object.x + 1;
				{
					Node* q;
					for (q = list.head; q; q = q->next) {
						if (CollisionDetect(p->object, q->object) && q->object.takeAble == 1) {	//���ץ������ʯ
							p->object.image = &i_shushu_right[1];
							p->object.m_image = &i_mshushu_right[1];
							p->object.score = 210;
							Delete(&list, &q->object);
							break;
						}
						if (p->object.score == 210) {	//����Ѿ�ץ������ʯ
							if (CollisionDetect(p->object, q->object)&&q->object.takeAble==0) {	//��������ͷ���
								p->object.image = &i_shushu_left[1];
								p->object.m_image = &i_mshushu_left[1];
								p->object.dir = 0;
							}
						}
					}
				}
				break;
			}
			if (p->object.score == 210) {	//����Ѿ�ץ������ʯ
				if (p->object.x > WINDOWS_WIDTH || p->object.x+28 < 0) {
					Delete(&list, &p->object);	//�����ƶ�����Ļ�⣬ɾ��
					break;
				}
			}
		}
		continue;
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
	int rect1width = i_hook[1].getwidth();
	int rect1height = i_hook[1].getheight();
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

//��ײ���
bool CollisionDetect(Object obj1, Object obj2) {
	int rect1x = obj1.x;
	int rect1y = obj1.y;
	int rect1width = obj1.image->getwidth();
	int rect1height = obj1.image->getheight();
	int rect2x = obj2.x;
	int rect2y = obj2.y;
	int rect2width = obj2.image->getwidth();
	int rect2height = obj2.image->getheight();

	if ((rect1x + rect1width) >= rect2x &&
		(rect2x + rect2width) >= rect1x &&
		(rect1y + rect1height) >= rect2y &&
		(rect2y + rect2height) >= rect1y)
	{
		return true;
	}
	return false;
}

//��Ϸ����
void GameOver() {
	static int isopen = 1;
	FILE* fp = NULL;
	errno_t err;
	if (isopen == 1) {
		err=fopen_s(&fp, "test.txt", "a+, ccs=utf-8");
	}
	wchar_t text[30];

	mciSendString(_T("stop bgm"), 0, 0, 0);
	if (player.score >= player.goal) {	//��������ﵽĿ�꣬��Ϸ�ɹ�
		Resize(NULL, i_clear.getwidth(), i_clear.getheight());
		putimage(0, 0, &i_clear);
		TransparentImage(NULL, 50, 20, &i_logo);
		mciSendString(_T("play high-value"), 0, 0, 0);
		swprintf_s(text, L"��ң�%-10s��Ϸ�ɹ���������%d\n", player.name,player.score);
	}
	else {	//û�ﵽĿ�꣬��Ϸʧ��
		Resize(NULL, i_end.getwidth(), i_end.getheight());
		putimage(0, 0, &i_end);
		swprintf_s(text, L"��ң�%-10s��Ϸʧ�ܣ�������%d\n", player.name, player.score);
	}
	if (isopen == 1&&err==0) {
		fwprintf(fp, L"%s", text);
		fclose(fp);
		isopen = 0;
	}

}

// Start �ڳ���ʼ����ʱ������
void Start() {
	HWND hWnd = initgraph(WINDOWS_WIDTH, WINDOWS_HEIGHT);
	SetWindowText(hWnd, L"GoldenMiner");
	LoadImages();
	Initialize();
	mciSendString(_T("play bgm repeat"), 0, 0, 0);
	//start2 = clock();
}

// Update ��ÿһ֡������
void Update() {
	switch (gameState)
	{
	case Begin:
		Resize(NULL, i_start.getwidth(), i_start.getheight());
		putimage(0, 0, &i_start);
		TransparentImage(NULL, 20, 20, &i_logo);
		putimage(90, 120, &i_startbottom);
		//��ӡĿ��Ǯ��
		#pragma region	text
		LOGFONT font;
		gettextstyle(&font);
		font.lfHeight = 32;	//ָ������߶�48
		font.lfQuality = ANTIALIASED_QUALITY;	//���忹���
		_tcscpy_s(font.lfFaceName, L"����");
		settextstyle(&font);
		settextcolor(YELLOW);
		setbkmode(TRANSPARENT);
		TCHAR goalText[30];
		_stprintf_s(goalText, _T("Ŀ��Ǯ����$%d"), player.goal);
		outtextxy(120, 400, goalText);
#pragma endregion	
		if (MouseHit()) {
			m = GetMouseMsg();
			MouseEvent();
		}
		break;

	case Running:
		timer++;	//��ʱ��+1
		if (timer == 60) {
			timer = 0;
			Time--;	//����ʱ

			//finish2 = clock();
			//double duration = (double)(finish2 - start2) / CLOCKS_PER_SEC;
			//printf("%10.2f seconds\n", duration);
			//start2 = clock();
		}

		Draw();	//��ͼ

		HookSway();	//���Ӱڶ�
		ShushuMove();	//�����ƶ�

		GetKeyboard();	//��ȡ�����¼�
		if (MouseHit()) {
			m = GetMouseMsg();
			MouseEvent();	//��ȡ����¼�
		}

		//����Ƿ������ȫ�����
		if (list.head == NULL) {
			gameState = Finished;
		}
		//��⵹��ʱ�Ƿ����
		if (Time == 0) {
			gameState = Finished;
		}
		break;

	case Pause:
		putimage(0, 0, &img);
		putimage(50, 50, &pause_background);
		if (MouseHit()) {
			m = GetMouseMsg();
			MouseEvent();
		}
		//outtextxy(300, 300, debugText);
		break;

	case Finished:
		GameOver();
		break;

	}
}


int main() {
	Start();
	BeginBatchDraw();
	while (true)
	{
		//start = clock();
		//count++;
		Update();
		FlushBatchDraw();
		Sleep(1000 / 60);
		cleardevice();
		//if (((double)await/CLOCKS_PER_SEC)>=1.0) {
		//	printf("FPS: %d\n", count);
		//	count = 0;
		//	await = (clock_t)0;
		//}
		//finish = clock();
		//await += (finish - start);

		//�ر��Ѳ�����������ļ�
		WCHAR buffer[48];
		mciSendString(_T("status dig mode"), buffer, _countof(buffer), 0);
		if (wcscmp(buffer, L"stopped") == 0) {
			mciSendString(_T("close dig"), 0, 0, 0);
		}
		mciSendString(_T("status low-value mode"), buffer, _countof(buffer), 0);
		if (wcscmp(buffer, L"stopped") == 0) {
			mciSendString(_T("close low-value"), 0, 0, 0);
		}
		mciSendString(_T("status normal-value mode"), buffer, _countof(buffer), 0);
		if (wcscmp(buffer, L"stopped") == 0) {
			mciSendString(_T("close normal-value"), 0, 0, 0);
		}
	}
	EndBatchDraw();
}
