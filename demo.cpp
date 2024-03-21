#include <graphics.h>
#include <conio.h>

#define WIDTH 40				//横的有40个格子
#define HEIGHT 30				//竖的有30个格子
#define SIZE 20					//每个格子的大小为20

// 全局变量定义
int Blocks[WIDTH][HEIGHT] = { 0 };		//表示所有的方格，用于记录地图中所有的游戏数据
char moveDirection,oldMoveDirection;	// moveDirection初始化移动方向
int foodX, foodY;						//食物的坐标
int isFailure = 0;						//游戏状态

//声明函数
void initGame();				//初始化游戏
void show();					//每一次运行时重新绘制蛇和食物的图像
void updateWithInput();			//与输入有关的更新
void updateWithoutInput();		//与输入无关的更新
void moveSnake();				//蛇运动

//---------------------------------------------------------------------------------//
int main() {
	//初始化游戏
	initGame();
	while (1) {
		show();						//每一次运行时重新绘制蛇和食物的图像
		updateWithoutInput();		//与输入无关的更新
		updateWithInput();			//与输入无关的更新
	}
	return 0;
}
//---------------------------------------------------------------------------------//

//初始化游戏
void initGame() {
	//初始化窗口
	initgraph(WIDTH * SIZE, HEIGHT * SIZE);
	//设置背景颜色
	setbkcolor(LIGHTGRAY);
	// 开始批量绘制
	BeginBatchDraw();
	cleardevice();

	//开始画线
		setlinecolor(WHITE);
		int i;
		//竖线
		for (i = SIZE; i < WIDTH * SIZE; i += SIZE)
			line(i, 0, i, HEIGHT * SIZE);
		//横线
		for (i = SIZE; i < HEIGHT * SIZE; i += SIZE)
			line(0, i, WIDTH * SIZE, i);

	//初始化向右走
	moveDirection = oldMoveDirection = 'd';

	//初始化蛇
		//画蛇头:画面中间画蛇头，数字为1
		Blocks[WIDTH / 2][HEIGHT / 2] = 1;
		//画蛇尾3个位置:向左依次3个蛇身，数值依次为2,3,4,
		for (i = 1; i < 4; i++)
			Blocks[WIDTH / 2 - i][HEIGHT / 2] = i + 1;

	//初始化食物随机放置
		foodX = rand() % (WIDTH - 2) + 1;//1到38
		foodY = rand() % (HEIGHT- 2) + 1;
}

//开始绘图：每一次运行时重新绘制蛇和食物的图像
void show() {
	//画蛇：先找到蛇的位置
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			if (Blocks[i][j] != 0)
				// 元素不等于0表示是蛇：这里让蛇的身体颜色色调渐变
				setfillcolor(HSVtoRGB(Blocks[i][j] * 10, 0.9, 1));
			else
				//设置填充的颜色为浅灰色
				setfillcolor(LIGHTGRAY);
			// 在对应位置处，以对应颜色绘制小方格
			fillrectangle(i * SIZE, j * SIZE, (i + 1) * SIZE, (j + 1) * SIZE);
		}
	}

	//画食物
		//食物颜色
		setfillcolor(LIGHTGREEN);
		//  绘制食物小方块
		fillrectangle(foodX * SIZE, foodY * SIZE, (foodX + 1) * SIZE, (foodY + 1) * SIZE);

	//游戏状态
	if (isFailure) {
		//设置文字颜色
		settextcolor(LIGHTRED);
		settextstyle(80, 0, _T("宋体"));
		//设置字体的背景
		setbkmode(TRANSPARENT);
		outtextxy(240, 220, _T("游戏失败"));
	}

	// 批量绘制结束
	FlushBatchDraw();
}

//与输入有关的更新：检测移动方向
void updateWithInput() {
	//  如果有按键输入，并且不失败
	if (_kbhit() && isFailure == 0) {
		 //  获得按键输入
		char input = _getch();
		if (input == 'a' || input == 's' || input == 'd' || input == 'w') {
			 // 设定移动方向
			moveDirection = input;

			//放置误按：调用小蛇移动函数
			if (moveDirection == 'a' && oldMoveDirection == 'd')
				moveDirection = 'd';
			else if (moveDirection == 's' && oldMoveDirection == 'w')
				moveDirection = 'w';
			else if (moveDirection == 'd' && oldMoveDirection == 'a')
				moveDirection = 'a';
			else if (moveDirection == 'w' && oldMoveDirection == 's')
				moveDirection = 's';
			else oldMoveDirection = input;

			//移动蛇
			moveSnake();
		}
	}
}

//与输入无关的更新
void updateWithoutInput() {
	//  如果游戏失败，函数返回
	if (isFailure)
		return;
	
	// 静态局部变量，初始化时为1
	static int waitIndex = 1;
	waitIndex++;					//每一帧+1
	if (waitIndex == 15) {			//小蛇每隔10帧移动一次
		//蛇运动
		moveSnake();
		// 再变成1
		waitIndex = 1;
	}
}

//蛇运动
void moveSnake() {
	int i, j;
	//日常对蛇身更新：蛇移动
		for (i = 0; i < WIDTH; i++)
			for (j = 0; j < HEIGHT; j++)
				// 不等于0的为小蛇元素 
				if (Blocks[i][j] != 0)
					Blocks[i][j]++;

	//旧蛇头蛇尾：寻找旧蛇头和旧蛇尾的xy坐标
		int oldHeadX, oldHeadY, oldTailX, oldTailY;
		//找最大的：蛇尾
		int tailBlocks = 0;	

		for (i = 0; i < WIDTH; i++) {
			for (j = 0; j < HEIGHT; j++) {
				//旧蛇尾:4最大的
				if (tailBlocks < Blocks[i][j]) {
					//更新最大的值
					tailBlocks = Blocks[i][j];
					//记录最大值的坐标，就是旧蛇尾的位置
					oldTailX = i;
					oldTailY = j;
				}
				//旧蛇头:1 2 3 4
				if (Blocks[i][j] == 2) {
					//数值为2恰好是旧蛇头的位置
					oldHeadX = i;
					oldHeadY = j;
				}
			}
		}

	//新蛇头：根据蛇的移动方向来寻找赋值新蛇头的xy坐标
		//设定变量存储新蛇头的位置
		int newHeadX = oldHeadX, newHeadY = oldHeadY;

		switch (moveDirection) {
		case'a'://左边移动	x
			newHeadX -= 1;
			break;
		case 's'://下边移动	y
			newHeadY += 1;
			break;
		case 'd'://右边移动	x
			newHeadX += 1;
			break;
		case 'w'://上边移动	y
			newHeadY -= 1;
			break;
		}

	//判断游戏是否失败：如果蛇头超出边界，或者蛇头碰到蛇身，游戏失败
	if (newHeadX >= WIDTH || newHeadX < 0 || newHeadY >= HEIGHT || newHeadY < 0 || Blocks[newHeadX][newHeadY] != 0) {
		isFailure = 1;
		return;
	}

	// 新蛇头位置数值为1
	Blocks[newHeadX][newHeadY] = 1;

	//对吃到食物以及蛇尾的处理
		//吃到食物
		if (newHeadX == foodX && newHeadY == foodY) {
			 //食物重新随机位置
			foodX = rand() % (WIDTH - 2) + 1;
			foodY = rand() % (HEIGHT - 2) + 1;
			//不对旧蛇尾处理，相当于蛇的长度+1
		}
		//没有吃到食物
		else
			 // 旧蛇尾变成空白，不吃食物时保持蛇的长度不变
			Blocks[oldTailX][oldTailY] = 0;	//灰格
}
