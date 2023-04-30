#include "game.h"

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
IMAGE i_start;
IMAGE i_logo;
IMAGE i_startbottom;
IMAGE img;
IMAGE img_tmp[BIG_GOLD_AMOUNT + SMALL_GOLD_AMOUNT + STONE_AMOUNT][2];

GameState gameState;
MOUSEMSG m; //设置鼠标信息
Hook hook;
Player player;
List list;	//链表用来储存游戏对象
int Time = 30;	//游戏倒计时
int timer = 0;	//计时器

//链表添加
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

//链表删除
void Delete(List* pList, int index) {
	Node* p, * q;
	int i = 0;
	for (p = pList->head, q = NULL; p; q = p, p = p->next) {
		if (i==index) {
			if (q) {
				q->next = p->next;	//q的后位等于p的后位，就是把p从链表上忽略掉
			}
			else {	//如果首位就是应删去的值
				pList->head = p->next;	//链表头为p的后位
			}
			free(p);
			break;
		}
		i++;
	}

}

//链表删除
void Delete(List* pList, Object* obj) {
	Node* p, * q;
	for (p = pList->head, q = NULL; p; q = p, p = p->next) {
		if (&p->object==obj) {
			if (q) {
				q->next = p->next;	//q的后位等于p的后位，就是把p从链表上忽略掉
			}
			else {	//如果首位就是应删去的值
				pList->head = p->next;	//链表头为p的后位
			}
			free(p);
			break;
		}
	}

}

//通过索引找对象
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

//获取键盘事件
void KeyboardEvent() {
	if (GetAsyncKeyState(27) & 0x8000) {	//ESC
		getimage(&img, 0, 0, WINDOWS_WIDTH, WINDOWS_HEIGHT);
		gameState = Pause;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		if (hook.isThrow)	//如果钩子已经投出，则不再监视
			return;
		ThrowHook();
	}
}

//获取鼠标事件
void MouseEvent() {
	switch (m.uMsg)//对事件进行分类
	{
	case WM_MOUSEMOVE://如果是鼠标移动事件
		//_stprintf_s(debugText, _T("x:%d y:%d"), m.x, m.y);
		break;
	case WM_LBUTTONDOWN://如果是点击鼠标左键
		if (gameState == Running) {
			if (hook.isThrow)	//如果钩子已经投出，则不再监视
				return;
			ThrowHook();
		}
		if (gameState == Pause) {
			if (m.x > 85 && m.x < 177 && m.y>178 && m.y < 214) {	//继续
				gameState = Running;
			}
			if (m.x > 238 && m.x < 328 && m.y>178 && m.y < 214) {	//退出
				closegraph();
				exit(0);
			}
		}
		if (gameState == Begin) {
			if (m.x > 90 && m.x < 236 && m.y>120 && m.y < 207) {	//开始游戏
				InputBox(player.name, 10, L"请输入玩家名称", L"输入玩家名", L"player");
				Resize(NULL, WINDOWS_WIDTH, WINDOWS_HEIGHT);
				gameState = Running;
			}
		}
		break;
	case WM_RBUTTONDOWN://如果是点击鼠标右键
		return;
	}

}

//加载图片
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

//游戏对象初始化
void Initialize() {
	gameState = Begin;
	m = GetMouseMsg();
	srand(time(NULL));

	//初始化player
	player.x = WINDOWS_WIDTH / 2 - 50;
	player.y = WINDOWS_HEIGHT / 2 - 240;
	player.score = 0;
	player.goal = 500;
	player.ani_timer = 0;
	player.ani_index = 0;
	player.isShow = false;

	//初始化hook
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

	list.head = NULL;	//初始化链表
	for (int i = 0; i < BIG_GOLD_AMOUNT; i++) {		// 生成大金块
		Object bigGold;
		bigGold.size = rand() % 3 + 6;	//范围6-8
		bigGold.score = 20 * bigGold.size;	//120-160
		double scale = (bigGold.size - 6) / 10.0 + 1;
		img_tmp[i][0] = ZoomImage(&i_biggold, scale);
		img_tmp[i][1] = ZoomImage(&i_mbiggold, scale);
		bigGold.image = &img_tmp[i][0];
		bigGold.m_image = &img_tmp[i][1];
		SetObjectPosition(&list, &bigGold);
		bigGold.type = Gold;
		Add(&list, bigGold);
	}
	for (int i = 0; i < SMALL_GOLD_AMOUNT; i++) {	//生成小金块
		Object smallGold;
		smallGold.size = rand() % 3 + 1;	//范围1-3
		smallGold.score = 20 * smallGold.size;	//20-60
		double scale = (smallGold.size - 1) / 8.0 + 1;
		img_tmp[BIG_GOLD_AMOUNT + i][0] = ZoomImage(&i_smallgold, scale);
		img_tmp[BIG_GOLD_AMOUNT + i][1] = ZoomImage(&i_msmallgold, scale);
		smallGold.image = &img_tmp[BIG_GOLD_AMOUNT + i][0];
		smallGold.m_image = &img_tmp[BIG_GOLD_AMOUNT + i][1];
		SetObjectPosition(&list, &smallGold);
		smallGold.type = Gold;
		Add(&list, smallGold);
	}
	for (int i = 0; i < STONE_AMOUNT; i++) {		//生成石头
		Object stone;
		img_tmp[BIG_GOLD_AMOUNT + SMALL_GOLD_AMOUNT+i][0] = ZoomImage(&i_stone, 1.6);
		img_tmp[BIG_GOLD_AMOUNT + SMALL_GOLD_AMOUNT+i][1] = ZoomImage(&i_mstone, 1.6);
		stone.image = &img_tmp[BIG_GOLD_AMOUNT + SMALL_GOLD_AMOUNT + i][0];
		stone.m_image = &img_tmp[BIG_GOLD_AMOUNT + SMALL_GOLD_AMOUNT + i][1];
		SetObjectPosition(&list, &stone);
		stone.size = 9;
		stone.score = 10;
		stone.type = Gold;
		Add(&list, stone);
	}
	for (int i = 0; i < DIAMOND_AMOUNT; i++) {	
		Object diamond;			//生成钻石
		diamond.image = &i_diamond;
		diamond.m_image = &i_mdiamond;
		SetObjectPosition(&list, &diamond);
		diamond.size = 1;
		diamond.score = 200;
		diamond.type = Diamond;
		Add(&list, diamond);
			
		Object mouse;			//生成老鼠
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
		mouse.type = Mouse;
		Add(&list, mouse);
	}

}

//绘制图像
void Draw() {
	putimage(0, 0, &i_back);	//绘制背景
	putimage(-10, 100, &i_brick);

	DrawPlayer();
	DrawObject();
	DrawHook();
	DrawUI();
}

//绘制角色
void DrawPlayer() {
	if (hook.isThrow) {	//如果钩子投出，播放动画
		player.ani_timer++;
		if (player.ani_timer > 10) {
			player.ani_timer = 0;
			player.ani_index = (player.ani_index + 1) % 2;
		}
		PutImageWithMask(player.x, player.y, &i_player[player.ani_index], &i_mplayer[player.ani_index]);
	}
	else {
		PutImageWithMask(player.x, player.y, &i_player[0], &i_mplayer[0]);	//绘制主角
	}
}

//绘制UI
void DrawUI() {
	settextstyle(32, 0, L"黑体");
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);

	//绘制分数
	TCHAR scoreText[30];
	_stprintf_s(scoreText, _T("金钱：$%d"), player.score);
	outtextxy(10,10,scoreText);

	//绘制目标分数
	TCHAR goalText[30];
	_stprintf_s(goalText, _T("目标钱数：$%d"), player.goal);
	outtextxy(10, 50, goalText);

	//绘制时间
	TCHAR timeText[30];
	_stprintf_s(timeText, _T("时间：%d"), Time);
	outtextxy(800, 10, timeText);

	//绘制玩家名称
	TCHAR nameText[30];
	_stprintf_s(nameText, _T("玩家名称：%s"), player.name);
	outtextxy(800-(wcslen(player.name)*16), 50, nameText);

	//绘制得分提示
	TCHAR getScoreText[30];
	static int duringTimer = 60;//持续时长
	if (player.isShow)
	{
		_stprintf_s(getScoreText, _T("+%d"), player.getScore);
		settextcolor(LIGHTCYAN);
		outtextxy(player.x+40, player.y-20, getScoreText);
		duringTimer--;
		if (duringTimer == 0) {
			player.isShow = false;
			duringTimer = 60;
		}
	}

}


//绘制游戏对象
void DrawObject() {
	Node* p;
	for (p = list.head; p; p = p->next) {
		PutImageWithMask(p->object.x, p->object.y, p->object.image, p->object.m_image);
	}
}

//绘制钩子
void DrawHook() {
	hook.endx = hook.x + hook.length * sin(hook.angle*3.14/180);	//计算钩子末端坐标
	hook.endy = hook.y + hook.length * cos(hook.angle*3.14/180);
	setlinestyle(PS_SOLID, 2);
	setlinecolor(DARKGRAY);
	line(hook.x, hook.y, hook.endx, hook.endy);	//绘制绳子
	PutImageWithMask(hook.endx-hook.dx, hook.endy-hook.dy, &i_hook[1], &i_mhook[1]);

	//如果钩子被投出
	if (hook.isThrow) {
		int index=0;
		//检测碰撞到墙壁
		if (hook.endx > WINDOWS_WIDTH || hook.endy > WINDOWS_HEIGHT || hook.endx < 0) {
			hook.isBack = true;
		}
		//检测碰撞到物体
		Node* p;
		for (p = list.head; p; p = p->next) {
			if (CollisionDetect(p->object)) {
				hook.isBack = true;
				hook.carry = p->object.size;
				break;
			}
			index++;
		}

		if (hook.isBack)	//钩子回收
			if (hook.carry == 0)	//没有负载，证明没有抓到物品
				HookBack(2 * hook.speed);
			else
				HookBack(2 * hook.speed - hook.carry ,index);	//抓到了物品
		else
			ThrowHook();

	}

}

//钩子旋转
void HookSway() {
	if (hook.isThrow) {	//如果扔出钩子，则返回
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
	rotateimage(&i_hook[1], &i_hook[0], hook.angle * 3.14 / 180, WHITE, true, true);	//旋转钩子图片
	rotateimage(&i_mhook[1], &i_mhook[0], hook.angle * 3.14 / 180, BLACK, true, true);

	//计算偏移量，使钩子视觉上绕轴心旋转
	if (hook.angle > 0) {
		hook.dy = 18 * sin(hook.angle * 3.14 / 180);
		hook.dx = 35 * cos(hook.angle * 3.14 / 180) - 18 * cos(hook.angle * 3.14 / 180);
	}
	else {
		hook.dy = 18 * sin(abs(hook.angle) * 3.14 / 180);
		hook.dx = 20 * sin(abs(hook.angle) * 3.14 / 180) + 18 * cos(abs(hook.angle) * 3.14 / 180);
	}
}

//投掷钩子
void ThrowHook() {
	hook.isThrow = true;
	hook.isBack = false;
	hook.length += hook.speed;
	mciSendString(_T("open .\\Resources\\sound\\dig.mp3 alias dig"), 0, 0, 0);
	mciSendString(_T("play dig"), 0, 0, 0);	//播放音效
}

//收回钩子
void HookBack(int speed) {
	hook.length -= speed;
	if (hook.length <= 0) {
		hook.length = 0;
		hook.isThrow = false;
	}
}

//收回钩子和抓取到的物品
void HookBack(int speed, int index) {
	hook.length -= speed;
	Object* obj = Find(&list, index);	//通过索引找到要收回的对象
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
			player.score += obj->score;	//将分数累加
			player.getScore = obj->score;	//记录本次得分
			player.isShow = true;
			Delete(&list, index);	//删除物品
			hook.isThrow = false;
		}
	}
}

//老鼠移动
void ShushuMove() {
	Node* p;
	for (p = list.head; p; p = p->next) {
		if (p->object.type == Mouse) {	//通过节点p遍历找到老鼠

			if (p->object.dir == 0) {	//老鼠向左走
				p->object.x = p->object.x - 1;
				Node* q;	//通过节点q遍历检测碰撞
					for (q = list.head; q; q = q->next) {
						if (CollisionDetect(p->object, q->object) && q->object.type == Diamond) {	//如果抓到了钻石
							p->object.image = &i_shushu_left[1];
							p->object.m_image = &i_mshushu_left[1];
							p->object.score = 210;
							Delete(&list, &q->object);	//删除钻石
							break;
						}
					}
			}
			else {	//老鼠向右走
				p->object.x = p->object.x + 1;
				Node* q;	//通过节点q遍历检测碰撞
					for (q = list.head; q; q = q->next) {
						if (CollisionDetect(p->object, q->object) && q->object.type == Diamond) {	//如果抓到了钻石
							p->object.image = &i_shushu_right[1];
							p->object.m_image = &i_mshushu_right[1];
							p->object.score = 210;
							Delete(&list, &q->object);
							break;
						}
					}
			}

			if (p->object.score == 210) {	//如果已经抓到了钻石
				if (p->object.x > WINDOWS_WIDTH || p->object.x+28 < 0) {
					Delete(&list, &p->object);	//老鼠移动到屏幕外，删除
					break;
				}
				Node *q;
				for (q = list.head; q; q = q->next) {
					if (CollisionDetect(p->object, q->object) && q->object.type == Gold) {	//如果碰到物体
						if (p->object.dir == 0) {
							p->object.image = &i_shushu_right[1];
							p->object.m_image = &i_mshushu_right[1];
							p->object.dir = 1;
						}
						else {
							p->object.image = &i_shushu_left[1];
							p->object.m_image = &i_mshushu_left[1];
							p->object.dir = 0;
						}
					}
				}

			}

		}
	}

}

//设置对象位置
void SetObjectPosition(List *pList,Object *obj) {
	if (pList->head == NULL) {
		//第一个对象不需要检测碰撞
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
		//和链表中的每一项检测碰撞
		while (p) {
			if (CollisionDetect(x, y, obj->image->getwidth(), obj->image->getheight(), p->object)) {
				p = pList->head;	//如果碰撞，重新生成位置，重新检测
				break;
			}
			p = p->next;
		}
		//链表中的每一项都检测完毕
		if (p == NULL)
			break;
	}
	obj->x = x;
	obj->y = y;
	return;
}

//碰撞检测
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

//碰撞检测
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

//碰撞检测
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

//游戏开始
void GameBegin() {
	Resize(NULL, i_start.getwidth(), i_start.getheight());
	putimage(0, 0, &i_start);
	TransparentImage(NULL, 20, 20, &i_logo);
	putimage(90, 120, &i_startbottom);

	//打印目标钱数
	settextstyle(32, 0, L"黑体");
	LOGFONT font;
	gettextstyle(&font);
	font.lfQuality = ANTIALIASED_QUALITY;	//设置输出效果为抗锯齿
	settextstyle(&font);
	settextcolor(YELLOW);
	setbkmode(TRANSPARENT);
	TCHAR goalText[30];
	_stprintf_s(goalText, _T("目标钱数：$%d"), player.goal);
	outtextxy(120, 400, goalText);
}

//游戏结束
void GameOver() {
	static int isopen = 1;
	FILE* fp = NULL;
	errno_t err;
	if (isopen == 1) {
		err=fopen_s(&fp, "charts.txt", "a+, ccs=utf-8");
	}
	wchar_t text[30];

	mciSendString(_T("stop bgm"), 0, 0, 0);
	if (player.score >= player.goal) {	//如果分数达到目标，游戏成功
		Resize(NULL, i_clear.getwidth(), i_clear.getheight());
		putimage(0, 0, &i_clear);
		TransparentImage(NULL, 50, 20, &i_logo);
		mciSendString(_T("play high-value"), 0, 0, 0);
		swprintf_s(text, L"玩家：%-10s游戏成功，分数：%d\n", player.name,player.score);
	}
	else {	//没达到目标，游戏失败
		Resize(NULL, i_end.getwidth(), i_end.getheight());
		putimage(0, 0, &i_end);
		swprintf_s(text, L"玩家：%-10s游戏失败，分数：%d\n", player.name, player.score);
	}
	if (isopen == 1&&err==0) {
		fwprintf(fp, L"%s", text);
		fclose(fp);
		isopen = 0;
	}

}

// Start 在程序开始运行时被调用
void Start() {
	initgraph(WINDOWS_WIDTH, WINDOWS_HEIGHT);
	LoadImages();
	Initialize();
	//添加音乐文件
	mciSendString(_T("open .\\Resources\\sound\\bgm.mp3 alias bgm"), 0, 0, 0);
	mciSendString(_T("open .\\Resources\\sound\\high-value.mp3 alias high-value"), 0, 0, 0);
	mciSendString(_T("play bgm repeat"), 0, 0, 0);
}

// Update 在每一帧被调用
void Update() {
	switch (gameState)	//判断游戏状态
	{
	case Begin:
		GameBegin();
		if (MouseHit()) {
			m = GetMouseMsg();
			MouseEvent();
		}
		break;

	case Running:
		timer++;	//计时器+1
		if (timer == 60) {
			timer = 0;
			Time--;	//倒计时
		}

		Draw();	//绘图

		HookSway();	//钩子摆动
		ShushuMove();	//老鼠移动

		KeyboardEvent();	//获取键盘事件
		if (MouseHit()) {
			m = GetMouseMsg();
			MouseEvent();	//获取鼠标事件
		}

		//检测是否清除了全部金块
		if (list.head == NULL) {
			gameState = Finished;
		}
		//检测倒计时是否归零
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
		break;

	case Finished:
		GameOver();
		break;

	}

	//关闭已播放完的音乐文件
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
