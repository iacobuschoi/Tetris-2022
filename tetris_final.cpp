/****************************************************************************
* @version 1.0
* @date - 2018/4/16
* @brief ���־� C++ ȯ�濡�� �ͺ� C ��Ÿ���� �Լ��� �����Ѵ�.
*****************************************************************************/

/****************************************************************************
*��Ʈ���� ���� ��,
*@version 1.0.0
*@date - 2022/12/06
*@by - 2315 �ֹ���, 2513 ������ 
*****************************************************************************/


#ifndef TURBOC_HEADER //TURBOC_HEADER ��ũ�ΰ� ���ǵǾ� ���� ������ �Ʒ� ���� ������
#define TURBOC_HEADER
#include <stdio.h>
#include <stdlib.h>	
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>
	
   typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } cursor_type;

   void clearScreen();         // ȭ���� ��� �����. //tc->clrscr();
   void gotoXY(int x, int y);   // Ŀ���� x,y��ǥ�� �̵���Ų��.
   int whereX();            // Ŀ���� x ��ǥ�� �����Ѵ�.
   int whereY();            // Ŀ���� y ��ǥ�� �����Ѵ�.
   void setCursorType(cursor_type c);   // Ŀ���� ����ų� �ٽ� ǥ���Ѵ�.

   #define delay(n) Sleep(n)                     // n/1000�ʸ�ŭ �ð� ����
   #define randomize() srand((unsigned)time(NULL))      // ���� �߻��� clearScreen
                        //<stdlib.h> srand�Լ� : ���� �߻��⸦ �ʱ�ȭ �Ѵ�.
   #define random(n) (rand() % (n))               //0~n������ ���� �߻�
                   //<stdlib.h> rand�Լ� : ����ǥ���� ������ �����Ѵ�.

   // �� ��ũ�ΰ� ���ǵǾ� ������ �Լ��� ������ �����ϰ� ���Ǵ� ���� �ʴ´�.
   #ifndef TURBOC_PROTOTYPE_ONLY
      /**
      * @brief clearScreen ȭ���� ����� Ŀ���� (1,1)�� �ű��.
      */
      void clearScreen()
      {
         system("cls");
      }

      /**
      * @brief gotoXY �ܼ�ȭ�鿡�� Ŀ���� (x,y)�� �̵���Ų��.
      * @param int �ܼ�ȭ�鿡���� ���� ��ġ�� ����
      * @param int �ܼ�ȭ�鿡���� ���� ��ġ�� ����
      */
      void gotoXY(int x, int y)
      {
         COORD Cur;
         Cur.X = x;
         Cur.Y = y;
         SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
      }

      /**
      * @brief whereX Ŀ���� X ��ǥ�� �����Ѵ�.
      * @return int ���� X ��ǥ
      */
      int whereX()
      {
         CONSOLE_SCREEN_BUFFER_INFO BufInfo;

         GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufInfo);
         return BufInfo.dwCursorPosition.X;
      }

      /**
      * @brief whereY Ŀ���� Y ��ǥ�� �����Ѵ�.
      * @return int ���� Y ��ǥ
      */
      int whereY()
      {
         CONSOLE_SCREEN_BUFFER_INFO BufInfo;

         GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufInfo);
         return BufInfo.dwCursorPosition.Y;
      }

      /**
      * @brief setCursorType Ŀ���� ����� �����Ѵ�.
      * @param cursor_type NOCURSOR(0), SOLIDCURSOR(1), NORMALCURSOR(2)
      */ 
      void setCursorType(cursor_type c)
      {
         CONSOLE_CURSOR_INFO CurInfo;

         switch (c) {
         case NOCURSOR:
            CurInfo.dwSize = 1;
            CurInfo.bVisible = FALSE;
            break;
         case SOLIDCURSOR:
            CurInfo.dwSize = 100;
            CurInfo.bVisible = TRUE;
            break;
         case NORMALCURSOR:
            CurInfo.dwSize = 20;
            CurInfo.bVisible = TRUE;
            break;
         }
         SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
      }

   #endif // TURBOC_PROTOTYPE_ONLY
#endif // TURBOC_HEADER

#define BX 5 //�������� ��ġ
#define BY 5
#define BW 10 //�������� ũ��
#define BH 20
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80

enum { EMPTY, BRICK, WALL , ALERT_EMPTY, ALERT_BRICK}; 
const char *arTile[5]={". ","��","��", "��", "��"};  
int board[2][BW+2][BH+2];

int nx[2], ny[2];
int brick[2], rot[2], boom[2];
int BoomArange = 3;
int PlayerNum=1;
int cleared[2], sent[2];
//tenet ���� ���� 
int GoRight = 1, GoLeft=-1,ROT=1;
struct Point {
   int x,y;
};

struct Point Shape[][4][4]={
   { {0,0,2,0,1,0,-1,0}, {0,0,0,1,0,-2,0,-1},
   {0,0,-2,0,-1,0,1,0},  {0,0,0,-1,0,2,0,1}}, //0�� ���� - I
   { {0,0,1,0,0,1,1,1}, {0,0,-1,0,-1,1,0,1},
   {0,0,-1,0,0,-1,-1,-1}, {0,0,1,0,0,-1,1,-1} }, //1�� ���� - O
   { {0,0,-1,0,0,-1,1,-1}, {0,0,0,-1,1,0,1,1},
   {0,0,-1,1,0,1,1,0}, {0,0,0,1,-1,0,-1,-1}}, //2�� ���� - S
   
   { {0,0,-1,-1,0,-1,1,0}, {0,0,0,1,1,0,1,-1},
   {0,0,-1,0,1,1,0,1}, {0,0,-1,0,-1,1,0,-1} }, //3�� ���� - Z
   { {0,0,-1,0,1,0,-1,-1}, {0,0,0,-1,0,1,1,-1},
   {0,0,-1,0,1,0,1,1}, {0,0,0,-1,0,1,-1,1}}, //4 ������ - J
   { {0,0,1,0,-1,0,1,-1}, {0,0,0,-1,0,1,1,1},
   {0,0,1,0,-1,0,-1,1}, {0,0,0,1,0,-1,-1,-1}}, //5 ������ -L 
   { {0,0,-1,0,1,0,0,-1}, {0,0,0,-1,0,1,1,0},
   {0,0,-1,0,1,0,0,1}, {0,0,-1,0,0,-1,0,1}}//6 ���� �� -T
};

int bag[10000];

int keyTime = 100;
clock_t time1, time2;

int Mx;

//��ź ���� ���� 
int BOOMNUM=5;
int IsBoom=0;
int LIGHTNING = 5;

int strokes[2];
int StrokeTime[2]={10e6, 10e6}, StrokeLine[2]={7, 9};

void PrintBrick(int t, BOOL Show);//����� ����� �׸���
void tenet(); // ���� Ű ����
void PrintShadow(int t, BOOL Show);//����� �׸��� �׸��� �����
void PrintBoomShadow(int t, BOOL Show);//��ź�� ���� ���� ���� �׸��� �����
void SevenBag();//�� ���� �������� ���ϱ�
int GetAround(int x, int y, int b, int r, int type);//�ش� ĭ�� � ĭ���� ��ȯ
void DrawScreen();//��ũ�� �׸���
void DrawBoard(int t);//���� �׸���
void TestFull(int t);//���� ���������� Ȯ���ϰ�, ��� �ʵ忡 �� �߰�
BOOL MoveDown(int t);//�� ��ĭ�� ������
int ProcessKey();//Ű �Է¹ޱ�
void start();//���� ����
void splash(int x, int y, int b, int r, int type);//��ź ���� ������
void crash(int x, int y, int type);//���� �Ͷ߸���
int Abs();//���� ��ȯ
BOOL SqBoom();//������ ��ź �ݰ� üũ
BOOL PointCheck(int type, int x, int y, int pointType);//�ش� ��ǥ�� ĭ Ȯ��
BOOL SRS(int br, int rotation, int rot_state, int type);//ȸ�� �ý��� ����
void StrikeFlash(int t, BOOL Show); //���� ���� �����ֱ�
void Strike(int t);//���� ����
void PlayerSelect();   //�÷��̾� ����
int SelectSpeed();//�ӵ� ����
void SelectBoom();//��ź �� ����

int main()
{
	clearScreen();
	gotoXY(27,14);
	puts("'��Ʈ����'���� ��");
	gotoXY(60,30); 
	puts("SSHS 33�� �ֹ��� ������ ����");
	int i=0;
	for(i=5;i>0;i--){
		gotoXY(40,16);
		printf("%d �� �� ����...",i);
		delay(1000);
	} 
	clearScreen();//��ũ�� Ŭ����
	PlayerSelect();//�÷��̾� ����
	start();//���� ����
}



void PlayerSelect(){
	gotoXY(27,14);puts("press 1 to play alone, 2 to play 2");
	while(1){//1,2�����Ҷ����� ���
		if(GetAsyncKeyState('1')){
			PlayerNum=1;
			break;
		} 
		delay(20);
		if(GetAsyncKeyState('2')){
			PlayerNum=2;
			break;
		} 
	}
}


int SelectSpeed(){//d
	clearScreen();
	gotoXY(27,15);puts("SELECT SPEED     press < >");
	gotoXY(27,16);puts("������");
	gotoXY(27,18);puts("press SPACE to ");
	int speed = 0,tw=0;
	while(!GetAsyncKeyState(VK_RETURN)){//�¿�Ű�� �ӵ� ����
		tw++;
		tw%=4;
		if(GetAsyncKeyState(VK_RIGHT)){
			if(speed<5)speed++;
		}
		if(GetAsyncKeyState(VK_LEFT)){
			if(speed>0)speed--;
		}
		int i=0;
		for(i=0;i<5;i++){//���õ� �ӵ��� ���̵� �ٷ� ������
			gotoXY(27+2*i,16); puts("��");
		}
		for(i=0;i<speed;i++){
			gotoXY(27+2*i,16); puts("��");
		}
		if(speed>3&&tw%2){//�ӵ��� ���� �̻��̸� ���������� ����
			CONSOLE_SCREEN_BUFFER_INFO buff;
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buff);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  (buff.wAttributes & 0xf0) | (4 & 0xf));
			for(i=0;i<speed;i++){
				gotoXY(27+2*i,16); puts("��");
			}
			if(speed>4){//�ӵ��� �ִ��� �� �ϵ� ��� �ȳ� �޼��� ����
				gotoXY(27+12,16); puts("!!Hard Mode!!");
			} 
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buff);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  (buff.wAttributes & 0xf0) | (15 & 0xf));
		}
		delay(200);
		gotoXY(27+10,16); puts("                  ");
		if(GetAsyncKeyState(VK_SPACE)) break;
	}
	return speed+1;
}
void SelectBoom(){//��ź �󵵸� �¿� Ű�� ����
	clearScreen();
	gotoXY(27,15);puts("SELECT ��ź ��    press < >");
	gotoXY(27,16);puts("������");
	gotoXY(27,18);puts("press SPACE to start");
	int boom=0;
	while(!GetAsyncKeyState(VK_RETURN)){
		if(GetAsyncKeyState(VK_RIGHT)){
			if(boom<5)boom++;
		}
		if(GetAsyncKeyState(VK_LEFT)){
			if(boom>0)boom--;
		}
		int i=0;
		for(i=0;i<5;i++){
			gotoXY(27+2*i,16); puts("��");
		}
		for(i=0;i<boom;i++){
			gotoXY(27+2*i,16); puts("��");//������ ���� ���̵� �ٷ� ������
		}
		delay(200);
		if(GetAsyncKeyState(VK_SPACE)) break;//�����̽��ٷ� ���� �Ϸ�
	}
	BOOMNUM=18;
	BOOMNUM/=boom+1;//������ ���� �ݿ�
} 



void start()
{
	Mx=2*BW+2*BX+4+(PlayerNum-1)*(2*BW+BX+4);
	SevenBag();
	//�� ���� ���� �迭 ����

   	int x,y,winner;
   	int b1=0, b2=0;
   	setCursorType(NOCURSOR); // Ŀ���� �Ⱥ��̰� �ϴ� �Լ�
   	clearScreen(); //ȭ���� ������ ����� �Լ� 
   	int isTenet=0;GoRight = 1;GoLeft=-1;ROT=1;//tenet ���� ���� �ʱ�ȭ 
	
   	for (x=0;x<BW+2;x++) { // board �迭 �ʱ�ȭ 
      	for (y=0;y<BH+2;y++) {//���� �ʱ�ȭ
         	board[0][x][y] = (y==0 || y==BH+1 || x==0 ||x==BW+1) ? WALL : EMPTY;
         	if(PlayerNum-1) board[1][x][y] = (y==0 || y==BH+1 || x==0 ||x==BW+1) ? WALL : EMPTY;
      	}   
   	}
   
   	int nFrame, nStay;//������ �������� �ӵ�
   
   	srand((unsigned)time(NULL));//�����߻��� �ʱ�ȭ
   
   	nFrame=50;//fps
   	int speedCon=SelectSpeed();//�ӵ� ���� 
   	SelectBoom();//��ź �� ���� 
	
   	clearScreen();
   	sent[0] = sent[1] = 0;
   	cleared[0] = cleared[1] = 0;
   	DrawScreen();
	DrawBoard(0);
	//��ũ�� �ʱ�ȭ �� ��ũ��, ���� �׸���

  	if(PlayerNum-1)DrawBoard(1);
  	// ���� �����ϱ�
  	brick[0]=bag[b1++];
	if(PlayerNum-1)brick[1]=bag[b2++];
	
	//boom �ʱ�ȭ 0�̸� �Ϲ� ���, 1�̸� �� ź 
	boom[0]=0;
	if(PlayerNum-1)boom[1]=0; 
	
  	nx[0]=nx[1]=BW/2;  // ������ �ʱ� x��ǥ
  	ny[0]=ny[1]=2; // ������ �ʱ� y��ǥ 
  	rot[0]=rot[1]=0;  //������ ȸ����� ��ȣ
  	PrintBrick(0, TRUE); // ���� �׸��� �Լ� ȣ��
  	PrintShadow(0, TRUE);//�׸��� �׸���
    if(PlayerNum-1)PrintBrick(1, TRUE); 
	if(PlayerNum-1)PrintShadow(1, TRUE); 
    nStay=nFrame/speedCon;//������ �ӵ� �ݿ�

    int TIME=0;//�ð� �ʱ�ȭ
    for (;2;) {
	gotoXY(Mx, BY+8);printf("TIME: %.2f",(float)TIME/(float)1000);//�ð� �ȳ�
    	gotoXY(Mx, BY+10);printf("SPEED: %d",TIME/30000+1);//�ӵ� �ȳ�
    	
    	if((TIME/1000)%70>=32&&(TIME/1000)%70<=50){ 	
			if((TIME/1000)%70<=34){
				CONSOLE_SCREEN_BUFFER_INFO buff;
				GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buff);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  (buff.wAttributes & 0xf0) | (5 & 0xf));
				int i;
				for(i=3;i>0;i--){
					gotoXY(Mx/3, BY+6);printf("%d�� �� �̵� ����!!!",i);
					delay(1000);
				}
				TIME+=3000;
				GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buff);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  (buff.wAttributes & 0xf0) | (15 & 0xf));
			}
			else{
				gotoXY(Mx ,BY+12);printf("%d�� �� ���� ����!!!",50-(TIME/1000)%70);
			}
		}
		else{
			gotoXY(Mx,BY+12);printf("                    ");
		}//Ű ���� �ȳ� �޼���
		
    	if(TIME%70000==35000) tenet();
    	if(TIME%70000==50000) tenet();
    	//Ű ����

    	int t0 = TIME - StrokeTime[0], t1 = TIME - StrokeTime[1];
	    if(PlayerNum==2){ // ���� ���� �������� ���� �ð� ���� ������ ��½�̴� ȿ�� �ֱ�
			if((t0>100 && t0<300) || (t0>500&&t0<700) || (t0>900&&t0<1000)){
	    		StrikeFlash(0, TRUE);
			}
			
			if((t0>300 && t0<500) || (t0>700&&t0<900)|| t0==1000){
	    		StrikeFlash(0, FALSE);
			}
			
			if(t0==960){
				Strike(0);
			}
			
			if((t1>100 && t1<300) || (t1>500&&t1<700) || (t1>900&&t1<1000)){
	    		StrikeFlash(1, TRUE);
			}
			
			if((t1>300 && t1<500) || (t1>700&&t1<900) || t1==1000){
	    		StrikeFlash(1, FALSE);
			}
			if(t1==960){
				Strike(1);
			}
		}

        if (--nStay == 0) {//nStay �����Ӹ��� �� �ٿ� ����
nStay=nFrame/(TIME/30000+1);//30�ʸ��� �ӵ� ����
            nStay/=speedCon; //�ʱ� ���� �ӵ� �ݿ�
             
            if (MoveDown(0)) 
            {
            	boom[0]=1;
            	PrintBrick(0, TRUE);
				brick[0]=bag[b1++];
				boom[0]+=TIME/13;
				boom[0]%=BOOMNUM;// ��ź������ ���θ� �ʱ�ȭ ��� �ð��� ���������� ���� ���� ���� 
            	nx[0]=BW/2;  // ������ �ʱ� x��ǥ
		      	ny[0]=2; // ������ �ʱ� y��ǥ 
		      	rot[0]=0;  //������ ȸ����� ��ȣ
		      	 // ���� �׸��� �Լ� ȣ��
		      	PrintBrick(0, TRUE);
		      	PrintShadow(0, TRUE);
		      	PrintShadow(1, TRUE);
            	if (GetAround(nx[0],ny[0],brick[0],rot[0], 0) != EMPTY){ //���� ���� ��� ������ ����� �׿��ִٸ� �й� ó��.
            		winner=1;
            		break;
				}
			}
			
			if ((PlayerNum-1)&&MoveDown(1)) 
            {
            	boom[1]=1;
            	PrintBrick(1, TRUE);
				brick[1]=bag[b2++];
				boom[1]+=TIME/13;
				boom[1]%=BOOMNUM;// ��ź������ ���θ� �ʱ�ȭ ��� �ð��� ���������� ���� ���� ���� 
            	nx[1]=BW/2;  // ������ �ʱ� x��ǥ
		      	ny[1]=2; // ������ �ʱ� y��ǥ 
		      	rot[1]=0;  //������ ȸ����� ��ȣ
		      	PrintBrick(1, TRUE); // ���� �׸��� �Լ� ȣ��
				PrintShadow(1, TRUE);  
				PrintShadow(0, TRUE);
				if (GetAround(nx[1],ny[1],brick[1],rot[1], 1) != EMPTY) {//���� ���� ��� ������ ����� �׿��ִٸ� �й� ó��.
            		winner=0;
            		break;
				}
			}
			
		}
		if(cleared[0]/LIGHTNING > strokes[0]){//���� �� ���� ī������ ������ ���� ���θ� �����Ѵ�.
      		strokes[0] = cleared[0]/LIGHTNING;
      		StrokeTime[1] = TIME;
      		StrokeLine[1] = rand()%10;
		}
		if(cleared[1]/LIGHTNING > strokes[1]){//���� �� ���� ī������ ������ ���� ���θ� �����Ѵ�.
      		strokes[1] = cleared[1]/LIGHTNING;
      		StrokeTime[0] = TIME;
      		StrokeLine[0] = rand()%10;
		}
		int p = ProcessKey(); //0 if 0, 1 if 1, 2 if 0 and 1
        if (p==0||p==2){
        	if(boom[0]==1&&!IsBoom){
				boom[0]=1;
        		PrintBrick(0, TRUE);
        		IsBoom=0;
			}
         	brick[0]=bag[b1++];
         	boom[0]+=TIME/13;
			boom[0]%=BOOMNUM;// ��ź������ ���θ� �ʱ�ȭ ��� �ð��� ���������� ���� ���� ���� 
         	nx[0]=BW/2;  // ������ �ʱ� x��ǥ
      		ny[0]=2; // ������ �ʱ� y��ǥ 
      		rot[0]=0;  //������ ȸ����� ��ȣ
        	PrintShadow(0, TRUE);//�׸��� �׸��� �Լ� ȣ��
        	PrintBrick(0, TRUE); // ���� �׸��� �Լ� ȣ��
         	if (GetAround(nx[0],ny[0],brick[0],rot[0], 0) != EMPTY){//���� ���� ���� �Ǻ��ϴ� �ڵ�
            		winner=1;
            		break;
				}
     	}
     	if ((PlayerNum-1)&&(p==1||p==2)){
     		if(boom[1]==1&&!IsBoom){
	     		boom[1]=1;
	     		PrintBrick(1, TRUE);
	     		IsBoom=0;
			 }
     		brick[1]=bag[b2++];
     		boom[1]+=TIME/13;
			boom[1]%=BOOMNUM;// ��ź������ ���θ� �ʱ�ȭ ��� �ð��� ���������� ���� ���� ���� 
         	nx[1]=BW/2;  // ������ �ʱ� x��ǥ
      		ny[1]=2; // ������ �ʱ� y��ǥ 
      		rot[1]=0;  //������ ȸ����� ��ȣ
      		PrintShadow(1, TRUE);  //�׸��� �׸��� �Լ� ȣ��
      		PrintBrick(1, TRUE); // ���� �׸��� �Լ�ȣ��
         	if (GetAround(nx[1],ny[1],brick[1],rot[1], 1) != EMPTY){//���� ���� ���θ� �Ǻ��ϴ� �ڵ�
            		winner=0;
            		break;
				}
     	}
     	int boomInd=0;//��ź ���� ���� ���� �÷��̾� �ε��� ����,�ʱ�ȭ
     	for(boomInd=0;boomInd<PlayerNum;boomInd++){//���� ��ź�̸� ��ź �׸��� �׸���
     		if(boom[boomInd]==0) PrintBoomShadow(boomInd,true);
		 }
        delay(1000/nFrame);
        
        for(boomInd=0;boomInd<PlayerNum;boomInd++){//���� ��ź�̸� ��ź �׸��� �����
     		if(boom[boomInd]==0) PrintBoomShadow(boomInd,false);
		 }
        TIME+=1000/nFrame;
      }
   clearScreen();
   gotoXY(30,12);
   if(PlayerNum-1){
	   if(winner==0)puts("player I WINS!");
	   else puts("player II WINS!");
   }
   delay(1000);
   gotoXY(27,14);puts("press space to restart");
   while(1){
	if(GetAsyncKeyState(VK_SPACE)){//������ ������ϴ� �ڵ�
		main();
	}
	delay(20);
   }
   setCursorType(NORMALCURSOR);
}

void tenet() //����� �̵������� ������Ű�� �Լ�
{
	GoRight*=-1;
	GoLeft*=-1;
	ROT*=-1;
}
int max(int a, int b){
	return a>b?a:b;
}
int GetAround(int x, int y, int b, int r, int type)//��� ������ �ٸ� ������ �ִ��� Ȯ���ϴ� �Լ�
{
   int i, k=EMPTY;
   if(x<0||x>BW||y<0||y>BH) return WALL;
   for (i=0;i<4;i++) {//���� ��� ĭ�� ��ȸ�ϸ� �ش� ĭ Ȯ��
      k=max(k, board[type][x+Shape[b][r][i].x][y+Shape[b][r][i].y]%3);   
   }
   return k;
}

void DrawScreen()//��ü���� ��ũ���� �׸��� �Լ�
{
   int x,y;

   for (x=0;x<BW+2;x++) {	
      for (y=0;y<BH+2;y++) {
         gotoXY(BX+x*2,BY+y);
         puts(arTile[board[0][x][y]]);
      }
   }


   if(PlayerNum-1){
	   for (x=0;x<BW+2;x++) {	
	      for (y=0;y<BH+2;y++) {
	         gotoXY(2*BX+BW*2+4+x*2,BY+y);
	         puts(arTile[board[1][x][y]]);
	      }
	   }
   }
   //��Ÿ �ȳ� �޼���
   gotoXY(Mx,BY+3);puts("Tetris Ver 1.0");
   gotoXY(Mx,BY+5);puts("player I: WASD + SPACE"); 
   if(PlayerNum-1){
		gotoXY(Mx,BY+6);puts("player II: ����Ű + ENTER");
   	}
   	gotoXY(Mx, BY+13);printf("player I: %d lines cleared!", cleared[0]);
   	if(PlayerNum-1){
   		gotoXY(Mx, BY+14);printf("player I: %d lines sent!", sent[0]);
   		gotoXY(Mx, BY+16);printf("player II: %d lines cleared!", cleared[1]);
   		gotoXY(Mx, BY+17);printf("player II: %d lines sent!", sent[1]);	
   	}
   gotoXY(BX+2+BW-4,2);puts("player I ");
   if(PlayerNum-1){
   	gotoXY(2*BX+2*BW+6+BW-4,2);puts("player II");
   } 
   
   for (x=0;x<6;x++){
      gotoXY(BX+2+BW-6+2*x,1);puts(arTile[2]);
      gotoXY(BX+2+BW-6+2*x,3);puts(arTile[2]);
      
	  if(PlayerNum-1){
		  gotoXY(2*BX+2*BW+6+BW-6+2*x,1);puts(arTile[2]);
	      gotoXY(2*BX+2*BW+6+BW-6+2*x,3);puts(arTile[2]);
	  }
   }
}

void PrintBrick(int t, BOOL Show)//�÷��̾��� ������ �׸��� �Լ�
{
    int i;
	CONSOLE_SCREEN_BUFFER_INFO buff;
	
	if(boom[t]==0 && Show){//��ź �׸���
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buff);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  (buff.wAttributes & 0xf0) | (5 & 0xf));
	}
	
	if(t==0){//���� ���� �����
		gotoXY(BX+(Shape[brick[0]][rot[0]][0].x+nx[0])*2,BY+Shape[brick[0]][rot[0]][0].y+ny[0]);
		puts(arTile[Show ? BRICK:EMPTY]);
	}
	else{//�Ϲ� ���߿���
		gotoXY(BX*2+BW*2+4+(Shape[brick[1]][rot[1]][0].x+nx[1])*2,BY+Shape[brick[1]][rot[1]][0].y+ny[1]);
		puts(arTile[Show ? BRICK:EMPTY]);
	}
	
	
	if(boom[t]==0 && Show){
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buff);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  (buff.wAttributes & 0xf0) | (4 & 0xf));
	}
	
	if(t==0){//���� �׸���
		for (i=1;i<4;i++) {
			gotoXY(BX+(Shape[brick[0]][rot[0]][i].x+nx[0])*2,BY+Shape[brick[0]][rot[0]][i].y+ny[0]);
			puts(arTile[Show ? BRICK:EMPTY]);
		}
	}
	else{
		for (i=1;i<4;i++) {
			gotoXY(BX*2+BW*2+4+(Shape[brick[1]][rot[1]][i].x+nx[1])*2,BY+Shape[brick[1]][rot[1]][i].y+ny[1]);
			puts(arTile[Show ? BRICK:EMPTY]);
		}
	}
	
	if(boom[t]==0 && Show){//�� ����
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buff);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  (buff.wAttributes & 0xf0) | (15 & 0xf));
	}
}

void DrawBoard(int t)//�÷��̾��� ���带 �׸��� �Լ�
{ 
   	int x,y; 
   	int color;

    for (x=1;x<BW+1;x++) { 
      	for (y=1;y<BH+1;y++) {
			CONSOLE_SCREEN_BUFFER_INFO buff;
        	if(t==0){
        		gotoXY(BX+x*2,BY+y);  
			}
			else{
				gotoXY(2*BX+BW*2+4+x*2,BY+y);
			}
			color = 15;
			if(board[t][x][y]==ALERT_BRICK)color = 4;
			else if (board[t][x][y]==ALERT_EMPTY) color = 12;
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buff);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  (buff.wAttributes & 0xf0) | (color & 0xf));
			puts(arTile[board[t][x][y]]);
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buff);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  (buff.wAttributes & 0xf0) | (15 & 0xf));
		} 
   	} 
}

void TestFull(int t) //�� ���� ���� á������ Ȯ��, ���� ����� �Լ�
{ 
   	int i,x,y,ty; 
   	PrintBrick(t, FALSE);
	for (i=0;i<4;i++) { 
		board[t][nx[t]+Shape[brick[t]][rot[t]][i].x][ny[t]+Shape[brick[t]][rot[t]][i].y]=BRICK; 
	}
   	int lines = 0, tr;
   	for (y=1;y<BH+1;y++) { 
      	for (x=1;x<BW+1;x++) { 
         	if (board[t][x][y] != BRICK) break; 
      	} 
      	if (x == BW+1) { 
         	for (ty=y;ty>1;ty--) { 
            	for (x=1;x<BW+1;x++) { 
               		board[t][x][ty]=board[t][x][ty-1];  
            	} 
         	} 
         	lines += 1;
         	DrawBoard(t); 
         	delay(100); 
      	} 
   	}
   	DrawBoard(t); 
   	cleared[t] += lines;
   	if(lines<=1) tr=0;
   	if(lines==2) tr=1;
   	if(lines==3) tr=2;
   	if(lines==4) tr=4;
   	sent[t] += tr;
	DrawScreen();
	if(lines<=1) return;
   	if(PlayerNum==2){ //2�� ��������, ���� ���� ���� ����� ��뿡�� ���������� ������ �ڵ�
   		
   		
	   	for (ty=1;ty<BH+1-tr;ty++) { 
	    	for (x=1;x<BW+1;x++) { 
	       		board[1-t][x][ty]=board[1-t][x][ty+tr];  
	    	} 
	 	} 
	 	int col = rand()%BW+1;
	 	for(ty=BH+1-tr;ty<BH+1;ty++){
	 		for(x=1;x<BW+1;x++){
	 			board[1-t][x][ty] = (x == col) ? EMPTY : BRICK;
			}
			DrawBoard(1-t);
		}
	}
}

BOOL MoveDown(int t) //����� �� ĭ �Ʒ��� ������ �Լ�
{ 
	PrintBrick(t, FALSE);
   	if (GetAround(nx[t], ny[t]+1, brick[t], rot[t], t)%3 != EMPTY) {  
      	TestFull(t); 
		if(PlayerNum-1)PrintBrick(1-t, TRUE);
      	return TRUE; 
   	} 
   	ny[t]++; 
   	PrintBrick(t, TRUE); 
   	return FALSE; 
}

int ProcessKey() //����� ���� �Է��� �޴� �Լ�
{ 
   	int ch, trot1, trot2; 
   	bool one=false, two=false;
	if(GetAsyncKeyState('A')){
		if (GetAround(nx[0] + GoLeft, ny[0], brick[0], rot[0], 0) == EMPTY) {  
			clock_t now=clock();
			if(now-time1>=keyTime){
				time1 = now;
				PrintShadow(0, FALSE);
				PrintBrick(0, FALSE); //���� �����  
           		nx[0]+=GoLeft ;
           		PrintShadow(0, TRUE);
           		PrintBrick(0, TRUE); //���� �׸���  
			}
        } 
	}
	if(GetAsyncKeyState('D')){
		if (GetAround(nx[0]+ GoRight, ny[0], brick[0], rot[0], 0) == EMPTY) {
			clock_t now=clock();
			if(now-time1>=keyTime){  
				time1 = now;
           		PrintShadow(0, FALSE);		
           		PrintBrick(0, FALSE); //���� �����  
           		nx[0]+=GoRight ;
           		PrintShadow(0, TRUE);
           		PrintBrick(0, TRUE); //���� �׸���  
       		}
        } 
	}
	if(GetAsyncKeyState('W')&0x0001){
		trot1=(rot[0]+4+ROT)%4; 
        if (SRS(brick[0], rot[0], ROT, 0)) { 
           PrintShadow(0, FALSE); 
           PrintBrick(0, FALSE); 
           rot[0]=trot1; 
           PrintShadow(0, TRUE);
           PrintBrick(0, TRUE); 
        } 
	}
	if(GetAsyncKeyState('S')){
		if(MoveDown(0)) { 
			if(PlayerNum-1)PrintShadow(1, TRUE);
	       	one = true; 
	    }
	}
	if(GetAsyncKeyState(VK_SPACE)&0x0001){
		if(boom[0]!=0) 
		{
			while(MoveDown(0)==FALSE) {;}  
			if(PlayerNum-1)PrintShadow(1, TRUE);
		}
	    else{
	    	PrintShadow(0,false);
	    	splash(nx[0],ny[0],0,brick[0],rot[0]);
	    	IsBoom=1;
		}
	   	one = true;
	}
	if(PlayerNum-1){
		if(GetAsyncKeyState(VK_LEFT)){
			if (GetAround(nx[1]+GoLeft, ny[1], brick[1], rot[1], 1) == EMPTY) {  
				clock_t now=clock();
				if(now-time2>=keyTime){  
					time2 = now;
	           		PrintShadow(1, FALSE);
	           		PrintBrick(1, FALSE); //���� �����  
	           		nx[1]+=GoLeft;
	           		PrintShadow(1, TRUE);
	           		PrintBrick(1, TRUE); //���� �׸���  
	           	}
	        } 
		}
		if(GetAsyncKeyState(VK_RIGHT)){
			if (GetAround(nx[1]+GoRight, ny[1], brick[1], rot[1], 1) == EMPTY) {  
	           	clock_t now=clock();
				if(now-time2>=keyTime){  
					time2 = now;
	           		PrintShadow(1, FALSE);
	           		PrintBrick(1, FALSE); //���� �����  
	           		nx[1]+=GoRight;
	           		PrintShadow(1, TRUE);
	           		PrintBrick(1, TRUE); //���� �׸���  
	           	}
	        } 
		}
		if(GetAsyncKeyState(VK_UP)&0x0001){
	        SRS(brick[1], rot[1], ROT, 1); 
		}
		if(GetAsyncKeyState(VK_DOWN)){
			if(MoveDown(1)) { 
				PrintShadow(0, TRUE);
		       two = true; 
		    } 
		}
		if(GetAsyncKeyState(VK_RETURN)&0x0001){
			if(boom[1]!=0) 
			{
				while(MoveDown(1)==FALSE) {;} 
				PrintShadow(0, TRUE);
			}
		    else{
		    	PrintShadow(1,FALSE);
		    	splash(nx[1],ny[1],1,brick[1],rot[1]);
		    	IsBoom=1;
			}
	    	two = true;
		}
	}
   	if(!one && !two){
   		return 3;
	}
	else if(one && two){
		return 2;
	}
	else if(one&&!two){
		return 0;
	}
	else{
		return 1;
	}
}

void PrintShadow(int t, BOOL Show)//����� ������ ��ġ�� �׸��ڷ� �̸� ǥ�����ִ� �ڵ�
{
	int i;
	CONSOLE_SCREEN_BUFFER_INFO buff;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buff);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  (buff.wAttributes & 0xf0) | (8 & 0xf));
   	if(t==0){
   		int y = ny[0];
   		for(i=0;i<4;i++){
   			while(GetAround(nx[0], y, brick[0], rot[0], 0) == EMPTY){
   				y++;	
			}
			y--;
		}
	   	for (i=0;i<4;i++) {
	      	gotoXY(BX+(Shape[brick[0]][rot[0]][i].x+nx[0])*2,BY+Shape[brick[0]][rot[0]][i].y+y);
	      	puts(arTile[Show ? BRICK:EMPTY]);
	   	}
   	}
   	else{
   		int y = ny[1];
   		for(i=0;i<4;i++){
   			while(GetAround(nx[1], y, brick[1], rot[1], 1) == EMPTY){
   				y++;
			}
			y--;
		}
   		for (i=0;i<4;i++) {
	      	gotoXY(BX*2+BW*2+4+(Shape[brick[1]][rot[1]][i].x+nx[1])*2,BY+Shape[brick[1]][rot[1]][i].y+y);
	      	puts(arTile[Show ? BRICK:EMPTY]);
   		}
   	}
   	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buff);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  (buff.wAttributes & 0xf0) | (15 & 0xf));
}

void PrintBoomShadow(int t, BOOL Show)
{//��ź ������ ���������� ǥ�����ִ� �Լ�
	int b=brick[t],r=rot[t],x=nx[t],y=ny[t],type=t;
	struct Point BOOM[4] = {Shape[b][r][0],Shape[b][r][1],Shape[b][r][2],Shape[b][r][3]	};
	int i;
	
	CONSOLE_SCREEN_BUFFER_INFO buff;
	if(Show){
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buff);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  (buff.wAttributes & 0xf0) | (6 & 0xf));
	}
	
	for(i=1;i<4;i++){//������ ���� ������ ���
		int CNT=0;
		while(!PointCheck(type, x+Shape[b][r][i].x+BOOM[i].x, y+Shape[b][r][i].y+BOOM[i].y, WALL)&&CNT<1){
			if(PointCheck(type, x+Shape[b][r][i].x+BOOM[i].x, y+Shape[b][r][i].y+BOOM[i].y, BRICK)) CNT++;
			Shape[b][r][i].x+=BOOM[i].x;
			Shape[b][r][i].y+=BOOM[i].y;
		}
	}
	
	for (i=1;i<4;i++) { 
		gotoXY(BX+type*(BX+BW*2+4)+(nx[type]+Shape[b][r][i].x)*2,BY+ny[type]+Shape[b][r][i].y);
		puts(arTile[Show? BRICK : board[type][nx[type]+Shape[b][r][i].x][ny[type]+Shape[b][r][i].y]]);	
	}
	
	for(i=0;i<4;i++){
		Shape[b][r][i].x=BOOM[i].x;
		Shape[b][r][i].y=BOOM[i].y;
	}
	
	if(Show){
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buff);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  (buff.wAttributes & 0xf0) | (15 & 0xf));
	}
}

void SevenBag()//����� ������ ������ �ƿ� ������ �ƴ϶�, 7���� ���� �� ���ο����� �����ϴ� �ڵ�. 
{
	int i, j, temp, b[7]={0, 1, 2, 3, 4, 5, 6}, idx=0;
	srand((unsigned int) time(NULL));
	for(j=0;j<100;j++){
		int ix=rand()%7, iy=rand()%7;
		temp = b[ix];
		b[ix] = b[iy];
		b[iy] = temp;
	}
	for(i=0;i<1000;i++){
		for(j=0;j<30;j++){
			int ix=rand()%7, iy=rand()%7;
			temp = b[ix];
			b[ix] = b[iy];
			b[iy] = temp;
		}
		for(j=0;j<7;j++){
			bag[idx] = b[j];
			idx++;
		}
	}
}

BOOL PointCheck(int type, int x, int y, int pointType){//��ǥ�� �Է¹��� ������ ĭ���� Ȯ��
	if(x<0||x>BW||y<0||y>BH){
		if(pointType==WALL) return true;
		return false;
	}
	if(board[type][x][y]==pointType) return true;
	return false;
}

void splash(int x, int y, int type, int b, int r)//���߽� ������ �Ѹ���, ������ ���������� ������ �ϴ� �Լ�
{
	struct Point BOOM[4] = {Shape[b][r][0],Shape[b][r][1],Shape[b][r][2],Shape[b][r][3]	};
	int i;
	
	
	for(i=1;i<4;i++){//���� �Ѹ���
		int CNT=0;
		while(!PointCheck(type, x+Shape[b][r][i].x+BOOM[i].x, y+Shape[b][r][i].y+BOOM[i].y, WALL)&&CNT<1){
			if(PointCheck(type, x+Shape[b][r][i].x+BOOM[i].x, y+Shape[b][r][i].y+BOOM[i].y, BRICK)) CNT++;
			PrintBrick(type, false);
			Shape[b][r][i].x+=BOOM[i].x;
			Shape[b][r][i].y+=BOOM[i].y;
			PrintBrick(type, true);
			delay(10);
		}
	}
	gotoXY(BX+type*(BX+BW*2+4)+(nx[type])*2,BY+ny[type]);
	puts(arTile[EMPTY]);
	delay(300);
	for (i=1;i<4;i++) { //���� �Ͷ߸���
	    crash(nx[type]+Shape[b][r][i].x,ny[type]+Shape[b][r][i].y,type);
	}
	for(i=1;i<4;i++){
		Shape[b][r][i].x=BOOM[i].x;
		Shape[b][r][i].y=BOOM[i].y;
	}
}

int Abs(int a){
	return a>0?a:-a;
}

BOOL SqBoom(int x, int y, int Range){//��ǥ�� ���� �ݰ濡 �ش��ϴ��� üũ
	if((Abs(x)==Range&&Abs(y)<=Range)||(Abs(y)==Range&&Abs(x)<=Range)) return true;
	return false;
}

void crash(int x, int y, int type)
{//���� ���� �Լ�
	
	CONSOLE_SCREEN_BUFFER_INFO buff;
	
	int i=-BoomArange,j=BoomArange,Range=0;
	
	for(Range=0;Range<=BoomArange;Range++){//������ ������ ����Ʈ
		
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buff);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  (buff.wAttributes & 0xf0) | (5 & 0xf));
		
		for(i=-BoomArange;i<=BoomArange;i++){//���� ���� �׸���
			for(j=-BoomArange;j<=BoomArange;j++){
				if(!PointCheck(type, x+i, y+j, WALL) && SqBoom(i,j,Range)){
					board[type][x+i][y+j]=EMPTY;
					gotoXY(BX+type*(BX+BW*2+4)+(x+i)*2,BY+y+j);
					puts(arTile[BRICK]);
				}
				
			}
		}
		
		delay(50);
		
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buff);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  (buff.wAttributes & 0xf0) | (15 & 0xf));
		
		for(i=-BoomArange;i<=BoomArange;i++){//���� ����� �ش� ĭ�� ��ĭ���� ����, �׸���
			for(j=-BoomArange;j<=BoomArange;j++){
				if(!PointCheck(type, x+i, y+j, WALL) && SqBoom(i,j,Range)){
					board[type][x+i][y+j]=EMPTY;
					gotoXY(BX+type*(BX+BW*2+4)+(x+i)*2,BY+y+j);
					puts(arTile[EMPTY]);
				}
				
			}
		}
	}
}

BOOL SRS(int br, int rotation, int rot_state, int t){ //Standard Rotation System: �� �ڿ������� ��� ȸ���� ���� ���

	int fin_tests[5][2]={0, 0};
	int i, j;
	if(rot_state==1){
		if(br!=0 && br!=1){
			switch(rotation){
				case 0:
				{
					int tests[5][2] = {{0,0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}};
					for(i=0;i<5;i++){
						fin_tests[i][0]=tests[i][0];
						fin_tests[i][1]=tests[i][1];
					}
					break;
				
				}
				case 1: 
				{
					int tests[5][2] = {{0,0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}};
					for(i=0;i<5;i++){
						fin_tests[i][0]=tests[i][0];
						fin_tests[i][1]=tests[i][1];
					}
					break;
				}
				case 2:
				{
					int tests[5][2] = {{0,0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}};
					for(i=0;i<5;i++){
						fin_tests[i][0]=tests[i][0];
						fin_tests[i][1]=tests[i][1];
					}
					break;
				}
				case 3:
				{
					int tests[5][2] = {{0,0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}};
					for(i=0;i<5;i++){
						fin_tests[i][0]=tests[i][0];
						fin_tests[i][1]=tests[i][1];
					}
					break;
				}
			}
		}
		if(br==0){
			switch(rotation){
				case 0:
				{
					int tests[5][2] = {{0,0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2}};
					for(i=0;i<5;i++){
						fin_tests[i][0]=tests[i][0];
						fin_tests[i][1]=tests[i][1];
					}
					break;
				
				}
				case 1: 
				{
					int tests[5][2] = {{0,0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1}};
					for(i=0;i<5;i++){
						fin_tests[i][0]=tests[i][0];
						fin_tests[i][1]=tests[i][1];
					}
					break;
				}
				case 2:
				{
					int tests[5][2] = {{0,0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2}};
					for(i=0;i<5;i++){
						fin_tests[i][0]=tests[i][0];
						fin_tests[i][1]=tests[i][1];
					}
					break;
				}
				case 3:
				{
					int tests[5][2] = {{0,0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1}};
					for(i=0;i<5;i++){
						fin_tests[i][0]=tests[i][0];
						fin_tests[i][1]=tests[i][1];
					}
					break;
				}
			}
			
		}
		if(br==1){
			PrintShadow(t, FALSE);
           	PrintBrick(t, FALSE); 
			if(rot[t]==0)nx[t]+=1;
			if(rot[t]==2)nx[t]-=1;
			if(rot[t]==1)ny[t]+=1;
			if(rot[t]==3)ny[t]-=1;
			rot[t]=(rot[t]+4+ROT)%4;
           	PrintShadow(t, TRUE);
           	PrintBrick(t, TRUE); 
           	return TRUE;
		}
	}
	if(rot_state==-1){
		if(br!=0 && br!=1){
			switch(rotation){
				case 0:
				{
					int tests[5][2] = {{0,0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}};
					for(i=0;i<5;i++){
						fin_tests[i][0]=tests[i][0];
						fin_tests[i][1]=tests[i][1];
					}
					break;
				
				}
				case 1: 
				{
					int tests[5][2] = {{0,0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}};
					for(i=0;i<5;i++){
						fin_tests[i][0]=tests[i][0];
						fin_tests[i][1]=tests[i][1];
					}
					break;
				}
				case 2:
				{
					int tests[5][2] = {{0,0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}};
					for(i=0;i<5;i++){
						fin_tests[i][0]=tests[i][0];
						fin_tests[i][1]=tests[i][1];
					}
					break;
				}
				case 3:
				{
					int tests[5][2] = {{0,0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}};
					for(i=0;i<5;i++){
						fin_tests[i][0]=tests[i][0];
						fin_tests[i][1]=tests[i][1];
					}
					break;
				}
			}
		}
		if(br==0){
			switch(rotation){
				case 0:
				{
					int tests[5][2] = {{0,0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1}};
					for(i=0;i<5;i++){
						fin_tests[i][0]=tests[i][0];
						fin_tests[i][1]=tests[i][1];
					}
					break;
				
				}
				case 1: 
				{
					int tests[5][2] = {{0,0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2}};
					for(i=0;i<5;i++){
						fin_tests[i][0]=tests[i][0];
						fin_tests[i][1]=tests[i][1];
					}
					break;
				}
				case 2:
				{
					int tests[5][2] = {{0,0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1}};
					for(i=0;i<5;i++){
						fin_tests[i][0]=tests[i][0];
						fin_tests[i][1]=tests[i][1];
					}
					break;
				}
				case 3:
				{
					int tests[5][2] = {{0,0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2}};
					for(i=0;i<5;i++){
						fin_tests[i][0]=tests[i][0];
						fin_tests[i][1]=tests[i][1];
					}
					break;
				}
			}
		}
		if(br==1){
			PrintShadow(t, FALSE);
           	PrintBrick(t, FALSE); 
			if(rot[t]==0)ny[t]+=1;
			if(rot[t]==2)ny[t]-=1;
			if(rot[t]==1)nx[t]+=1;
			if(rot[t]==3)nx[t]-=1;
			rot[t]=(rot[t]+4+ROT)%4;
           	PrintShadow(t, TRUE);
           	PrintBrick(t, TRUE); 
           	return TRUE;
		}
	}
	for(i=0;i<5;i++){
		if(GetAround(nx[t] + fin_tests[i][0], ny[t] + fin_tests[i][1], br,(rotation+4+rot_state)%4, t)==EMPTY){
			PrintShadow(t, FALSE);
           	PrintBrick(t, FALSE); 
			nx[t]+=fin_tests[i][0];
			ny[t]+=fin_tests[i][1];
			rot[t]=(rot[t]+4+ROT)%4;
           	PrintShadow(t, TRUE);
           	PrintBrick(t, TRUE); 
			return TRUE;
		}
	}
	return FALSE;
}

void StrikeFlash(int t, BOOL Show){ //���� �ð��� ���� ������ ġ�� ���� ����� ��½�̰� �����ϴ� �ڵ�
	
	int i;
	int color;
	for(i=1;i<BH+1;i++){
		if(Show&&board[t][StrokeLine[t]+1][i]==BRICK)board[t][StrokeLine[t]+1][i]=ALERT_BRICK;
		if(Show&&board[t][StrokeLine[t]+1][i]==EMPTY)board[t][StrokeLine[t]+1][i]=ALERT_EMPTY;
		
		if(!Show&&board[t][StrokeLine[t]+1][i]==ALERT_BRICK)board[t][StrokeLine[t]+1][i]=BRICK;
		if(!Show&&board[t][StrokeLine[t]+1][i]==ALERT_EMPTY)board[t][StrokeLine[t]+1][i]=EMPTY;
	}
	DrawBoard(t);
	PrintBrick(t, TRUE);
	return;
}

void Strike(int t){ //������ ���� �ش� ���� ����� ��� ����� �ڵ�
	int i;
	gotoXY(BX+2*(BX+BW*2+4),BY+17);
	for(i=1;i<BH+1;i++){
		board[t][StrokeLine[t]+1][i] = ALERT_EMPTY;
	}
	DrawBoard(t);
}

