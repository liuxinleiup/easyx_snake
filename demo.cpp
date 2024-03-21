#include <graphics.h>
#include <conio.h>

#define WIDTH 40				//�����40������
#define HEIGHT 30				//������30������
#define SIZE 20					//ÿ�����ӵĴ�СΪ20

// ȫ�ֱ�������
int Blocks[WIDTH][HEIGHT] = { 0 };		//��ʾ���еķ������ڼ�¼��ͼ�����е���Ϸ����
char moveDirection,oldMoveDirection;	// moveDirection��ʼ���ƶ�����
int foodX, foodY;						//ʳ�������
int isFailure = 0;						//��Ϸ״̬

//��������
void initGame();				//��ʼ����Ϸ
void show();					//ÿһ������ʱ���»����ߺ�ʳ���ͼ��
void updateWithInput();			//�������йصĸ���
void updateWithoutInput();		//�������޹صĸ���
void moveSnake();				//���˶�

//---------------------------------------------------------------------------------//
int main() {
	//��ʼ����Ϸ
	initGame();
	while (1) {
		show();						//ÿһ������ʱ���»����ߺ�ʳ���ͼ��
		updateWithoutInput();		//�������޹صĸ���
		updateWithInput();			//�������޹صĸ���
	}
	return 0;
}
//---------------------------------------------------------------------------------//

//��ʼ����Ϸ
void initGame() {
	//��ʼ������
	initgraph(WIDTH * SIZE, HEIGHT * SIZE);
	//���ñ�����ɫ
	setbkcolor(LIGHTGRAY);
	// ��ʼ��������
	BeginBatchDraw();
	cleardevice();

	//��ʼ����
		setlinecolor(WHITE);
		int i;
		//����
		for (i = SIZE; i < WIDTH * SIZE; i += SIZE)
			line(i, 0, i, HEIGHT * SIZE);
		//����
		for (i = SIZE; i < HEIGHT * SIZE; i += SIZE)
			line(0, i, WIDTH * SIZE, i);

	//��ʼ��������
	moveDirection = oldMoveDirection = 'd';

	//��ʼ����
		//����ͷ:�����м仭��ͷ������Ϊ1
		Blocks[WIDTH / 2][HEIGHT / 2] = 1;
		//����β3��λ��:��������3��������ֵ����Ϊ2,3,4,
		for (i = 1; i < 4; i++)
			Blocks[WIDTH / 2 - i][HEIGHT / 2] = i + 1;

	//��ʼ��ʳ���������
		foodX = rand() % (WIDTH - 2) + 1;//1��38
		foodY = rand() % (HEIGHT- 2) + 1;
}

//��ʼ��ͼ��ÿһ������ʱ���»����ߺ�ʳ���ͼ��
void show() {
	//���ߣ����ҵ��ߵ�λ��
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			if (Blocks[i][j] != 0)
				// Ԫ�ز�����0��ʾ���ߣ��������ߵ�������ɫɫ������
				setfillcolor(HSVtoRGB(Blocks[i][j] * 10, 0.9, 1));
			else
				//����������ɫΪǳ��ɫ
				setfillcolor(LIGHTGRAY);
			// �ڶ�Ӧλ�ô����Զ�Ӧ��ɫ����С����
			fillrectangle(i * SIZE, j * SIZE, (i + 1) * SIZE, (j + 1) * SIZE);
		}
	}

	//��ʳ��
		//ʳ����ɫ
		setfillcolor(LIGHTGREEN);
		//  ����ʳ��С����
		fillrectangle(foodX * SIZE, foodY * SIZE, (foodX + 1) * SIZE, (foodY + 1) * SIZE);

	//��Ϸ״̬
	if (isFailure) {
		//����������ɫ
		settextcolor(LIGHTRED);
		settextstyle(80, 0, _T("����"));
		//��������ı���
		setbkmode(TRANSPARENT);
		outtextxy(240, 220, _T("��Ϸʧ��"));
	}

	// �������ƽ���
	FlushBatchDraw();
}

//�������йصĸ��£�����ƶ�����
void updateWithInput() {
	//  ����а������룬���Ҳ�ʧ��
	if (_kbhit() && isFailure == 0) {
		 //  ��ð�������
		char input = _getch();
		if (input == 'a' || input == 's' || input == 'd' || input == 'w') {
			 // �趨�ƶ�����
			moveDirection = input;

			//�����󰴣�����С���ƶ�����
			if (moveDirection == 'a' && oldMoveDirection == 'd')
				moveDirection = 'd';
			else if (moveDirection == 's' && oldMoveDirection == 'w')
				moveDirection = 'w';
			else if (moveDirection == 'd' && oldMoveDirection == 'a')
				moveDirection = 'a';
			else if (moveDirection == 'w' && oldMoveDirection == 's')
				moveDirection = 's';
			else oldMoveDirection = input;

			//�ƶ���
			moveSnake();
		}
	}
}

//�������޹صĸ���
void updateWithoutInput() {
	//  �����Ϸʧ�ܣ���������
	if (isFailure)
		return;
	
	// ��̬�ֲ���������ʼ��ʱΪ1
	static int waitIndex = 1;
	waitIndex++;					//ÿһ֡+1
	if (waitIndex == 15) {			//С��ÿ��10֡�ƶ�һ��
		//���˶�
		moveSnake();
		// �ٱ��1
		waitIndex = 1;
	}
}

//���˶�
void moveSnake() {
	int i, j;
	//�ճ���������£����ƶ�
		for (i = 0; i < WIDTH; i++)
			for (j = 0; j < HEIGHT; j++)
				// ������0��ΪС��Ԫ�� 
				if (Blocks[i][j] != 0)
					Blocks[i][j]++;

	//����ͷ��β��Ѱ�Ҿ���ͷ�;���β��xy����
		int oldHeadX, oldHeadY, oldTailX, oldTailY;
		//�����ģ���β
		int tailBlocks = 0;	

		for (i = 0; i < WIDTH; i++) {
			for (j = 0; j < HEIGHT; j++) {
				//����β:4����
				if (tailBlocks < Blocks[i][j]) {
					//��������ֵ
					tailBlocks = Blocks[i][j];
					//��¼���ֵ�����꣬���Ǿ���β��λ��
					oldTailX = i;
					oldTailY = j;
				}
				//����ͷ:1 2 3 4
				if (Blocks[i][j] == 2) {
					//��ֵΪ2ǡ���Ǿ���ͷ��λ��
					oldHeadX = i;
					oldHeadY = j;
				}
			}
		}

	//����ͷ�������ߵ��ƶ�������Ѱ�Ҹ�ֵ����ͷ��xy����
		//�趨�����洢����ͷ��λ��
		int newHeadX = oldHeadX, newHeadY = oldHeadY;

		switch (moveDirection) {
		case'a'://����ƶ�	x
			newHeadX -= 1;
			break;
		case 's'://�±��ƶ�	y
			newHeadY += 1;
			break;
		case 'd'://�ұ��ƶ�	x
			newHeadX += 1;
			break;
		case 'w'://�ϱ��ƶ�	y
			newHeadY -= 1;
			break;
		}

	//�ж���Ϸ�Ƿ�ʧ�ܣ������ͷ�����߽磬������ͷ����������Ϸʧ��
	if (newHeadX >= WIDTH || newHeadX < 0 || newHeadY >= HEIGHT || newHeadY < 0 || Blocks[newHeadX][newHeadY] != 0) {
		isFailure = 1;
		return;
	}

	// ����ͷλ����ֵΪ1
	Blocks[newHeadX][newHeadY] = 1;

	//�ԳԵ�ʳ���Լ���β�Ĵ���
		//�Ե�ʳ��
		if (newHeadX == foodX && newHeadY == foodY) {
			 //ʳ���������λ��
			foodX = rand() % (WIDTH - 2) + 1;
			foodY = rand() % (HEIGHT - 2) + 1;
			//���Ծ���β�����൱���ߵĳ���+1
		}
		//û�гԵ�ʳ��
		else
			 // ����β��ɿհף�����ʳ��ʱ�����ߵĳ��Ȳ���
			Blocks[oldTailX][oldTailY] = 0;	//�Ҹ�
}
