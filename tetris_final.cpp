/****************************************************************************
* @version 1.0
* @date - 2018/4/16
* @brief 비주얼 C++ 환경에서 터보 C 스타일의 함수를 정의한다.
*****************************************************************************/

/****************************************************************************
*테트리스 였던 것,
*@version 1.0.0
*@date - 2022/12/06
*@by - 2315 최민종, 2513 정해찬 
*****************************************************************************/


#ifndef TURBOC_HEADER //TURBOC_HEADER 매크로가 정의되어 있지 않으면 아래 문장 컴파일
#define TURBOC_HEADER
#include <stdio.h>
#include <stdlib.h>	
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>
	
   typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } cursor_type;

   void clearScreen();         // 화면을 모두 지운다. //tc->clrscr();
   void gotoXY(int x, int y);   // 커서를 x,y좌표로 이동시킨다.
   int whereX();            // 커서의 x 좌표를 조사한다.
   int whereY();            // 커서의 y 좌표를 조사한다.
   void setCursorType(cursor_type c);   // 커서를 숨기거나 다시 표시한다.

   #define delay(n) Sleep(n)                     // n/1000초만큼 시간 지연
   #define randomize() srand((unsigned)time(NULL))      // 난수 발생기 clearScreen
                        //<stdlib.h> srand함수 : 난수 발생기를 초기화 한다.
   #define random(n) (rand() % (n))               //0~n까지의 난수 발생
                   //<stdlib.h> rand함수 : 난수표에서 난수를 선택한다.

   // 이 매크로가 정의되어 있으면 함수의 원형만 선언하고 정의는 하지 않는다.
   #ifndef TURBOC_PROTOTYPE_ONLY
      /**
      * @brief clearScreen 화면을 지우고 커서를 (1,1)로 옮긴다.
      */
      void clearScreen()
      {
         system("cls");
      }

      /**
      * @brief gotoXY 콘솔화면에서 커서를 (x,y)로 이동시킨다.
      * @param int 콘솔화면에서의 가로 위치를 지정
      * @param int 콘솔화면에서의 세로 위치를 지정
      */
      void gotoXY(int x, int y)
      {
         COORD Cur;
         Cur.X = x;
         Cur.Y = y;
         SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
      }

      /**
      * @brief whereX 커서의 X 좌표를 조사한다.
      * @return int 현재 X 좌표
      */
      int whereX()
      {
         CONSOLE_SCREEN_BUFFER_INFO BufInfo;

         GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufInfo);
         return BufInfo.dwCursorPosition.X;
      }

      /**
      * @brief whereY 커서의 Y 좌표를 조사한다.
      * @return int 현재 Y 좌표
      */
      int whereY()
      {
         CONSOLE_SCREEN_BUFFER_INFO BufInfo;

         GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufInfo);
         return BufInfo.dwCursorPosition.Y;
      }

      /**
      * @brief setCursorType 커서의 모양을 결정한다.
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

#define BX 5 //게임판의 위치
#define BY 5
#define BW 10 //게임판의 크기
#define BH 20
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80

enum { EMPTY, BRICK, WALL , ALERT_EMPTY, ALERT_BRICK}; 
const char *arTile[5]={". ","■","□", "□", "■"};  
int board[2][BW+2][BH+2];

int nx[2], ny[2];
int brick[2], rot[2], boom[2];
int BoomArange = 3;
int PlayerNum=1;
int cleared[2], sent[2];
//tenet 관련 변수 
int GoRight = 1, GoLeft=-1,ROT=1;
struct Point {
   int x,y;
};

struct Point Shape[][4][4]={
   { {0,0,2,0,1,0,-1,0}, {0,0,0,1,0,-2,0,-1},
   {0,0,-2,0,-1,0,1,0},  {0,0,0,-1,0,2,0,1}}, //0번 벽돌 - I
   { {0,0,1,0,0,1,1,1}, {0,0,-1,0,-1,1,0,1},
   {0,0,-1,0,0,-1,-1,-1}, {0,0,1,0,0,-1,1,-1} }, //1번 벽돌 - O
   { {0,0,-1,0,0,-1,1,-1}, {0,0,0,-1,1,0,1,1},
   {0,0,-1,1,0,1,1,0}, {0,0,0,1,-1,0,-1,-1}}, //2번 벽돌 - S
   
   { {0,0,-1,-1,0,-1,1,0}, {0,0,0,1,1,0,1,-1},
   {0,0,-1,0,1,1,0,1}, {0,0,-1,0,-1,1,0,-1} }, //3번 벽돌 - Z
   { {0,0,-1,0,1,0,-1,-1}, {0,0,0,-1,0,1,1,-1},
   {0,0,-1,0,1,0,1,1}, {0,0,0,-1,0,1,-1,1}}, //4 벽돌번 - J
   { {0,0,1,0,-1,0,1,-1}, {0,0,0,-1,0,1,1,1},
   {0,0,1,0,-1,0,-1,1}, {0,0,0,1,0,-1,-1,-1}}, //5 벽돌번 -L 
   { {0,0,-1,0,1,0,0,-1}, {0,0,0,-1,0,1,1,0},
   {0,0,-1,0,1,0,0,1}, {0,0,-1,0,0,-1,0,1}}//6 벽돌 번 -T
};

int bag[10000];

int keyTime = 100;
clock_t time1, time2;

int Mx;

//폭탄 관련 변수 
int BOOMNUM=5;
int IsBoom=0;
int LIGHTNING = 5;

int strokes[2];
int StrokeTime[2]={10e6, 10e6}, StrokeLine[2]={7, 9};

void PrintBrick(int t, BOOL Show);//블록을 지우고 그리기
void tenet(); // 조작 키 반전
void PrintShadow(int t, BOOL Show);//블록의 그림자 그리고 지우기
void PrintBoomShadow(int t, BOOL Show);//폭탄의 파편 도착 지점 그리고 지우기
void SevenBag();//블럭 순서 랜덤으로 정하기
int GetAround(int x, int y, int b, int r, int type);//해당 칸이 어떤 칸인지 반환
void DrawScreen();//스크린 그리기
void DrawBoard(int t);//보드 그리기
void TestFull(int t);//줄이 없어졌는지 확인하고, 상대 필드에 줄 추가
BOOL MoveDown(int t);//블럭 한칸씩 내리기
int ProcessKey();//키 입력받기
void start();//게임 시작
void splash(int x, int y, int b, int r, int type);//폭탄 파편 날리기
void crash(int x, int y, int type);//파편 터뜨리기
int Abs();//절댓값 반환
BOOL SqBoom();//파편의 폭탄 반경 체크
BOOL PointCheck(int type, int x, int y, int pointType);//해당 좌표의 칸 확인
BOOL SRS(int br, int rotation, int rot_state, int type);//회전 시스템 개선
void StrikeFlash(int t, BOOL Show); //번개 공격 보여주기
void Strike(int t);//번개 공격
void PlayerSelect();   //플레이어 선택
int SelectSpeed();//속도 선택
void SelectBoom();//폭탄 빈도 선택

int main()
{
	clearScreen();
	gotoXY(27,14);
	puts("'테트리스'였던 것");
	gotoXY(60,30); 
	puts("SSHS 33기 최민종 정해찬 제작");
	int i=0;
	for(i=5;i>0;i--){
		gotoXY(40,16);
		printf("%d 초 후 시작...",i);
		delay(1000);
	} 
	clearScreen();//스크린 클리어
	PlayerSelect();//플레이어 선택
	start();//게임 시작
}



void PlayerSelect(){
	gotoXY(27,14);puts("press 1 to play alone, 2 to play 2");
	while(1){//1,2선택할때까지 대기
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
	gotoXY(27,16);puts("□□□□□");
	gotoXY(27,18);puts("press SPACE to ");
	int speed = 0,tw=0;
	while(!GetAsyncKeyState(VK_RETURN)){//좌우키로 속도 선택
		tw++;
		tw%=4;
		if(GetAsyncKeyState(VK_RIGHT)){
			if(speed<5)speed++;
		}
		if(GetAsyncKeyState(VK_LEFT)){
			if(speed>0)speed--;
		}
		int i=0;
		for(i=0;i<5;i++){//선택된 속도를 가이드 바로 보여줌
			gotoXY(27+2*i,16); puts("□");
		}
		for(i=0;i<speed;i++){
			gotoXY(27+2*i,16); puts("■");
		}
		if(speed>3&&tw%2){//속도가 일정 이상이면 빨강색으로 점멸
			CONSOLE_SCREEN_BUFFER_INFO buff;
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buff);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  (buff.wAttributes & 0xf0) | (4 & 0xf));
			for(i=0;i<speed;i++){
				gotoXY(27+2*i,16); puts("■");
			}
			if(speed>4){//속도가 최대일 시 하드 모드 안내 메세지 점멸
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
void SelectBoom(){//폭탄 빈도를 좌우 키로 설정
	clearScreen();
	gotoXY(27,15);puts("SELECT 폭탄 빈도    press < >");
	gotoXY(27,16);puts("□□□□□");
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
			gotoXY(27+2*i,16); puts("□");
		}
		for(i=0;i<boom;i++){
			gotoXY(27+2*i,16); puts("■");//선택한 값을 가이드 바로 보여줌
		}
		delay(200);
		if(GetAsyncKeyState(VK_SPACE)) break;//스페이스바로 선택 완료
	}
	BOOMNUM=18;
	BOOMNUM/=boom+1;//선택한 값을 반영
} 



void start()
{
	Mx=2*BW+2*BX+4+(PlayerNum-1)*(2*BW+BX+4);
	SevenBag();
	//블럭 순서 랜덤 배열 저장

   	int x,y,winner;
   	int b1=0, b2=0;
   	setCursorType(NOCURSOR); // 커서를 안보이게 하는 함수
   	clearScreen(); //화면을 깨끗이 지우는 함수 
   	int isTenet=0;GoRight = 1;GoLeft=-1;ROT=1;//tenet 관련 변수 초기화 
	
   	for (x=0;x<BW+2;x++) { // board 배열 초기화 
      	for (y=0;y<BH+2;y++) {//보드 초기화
         	board[0][x][y] = (y==0 || y==BH+1 || x==0 ||x==BW+1) ? WALL : EMPTY;
         	if(PlayerNum-1) board[1][x][y] = (y==0 || y==BH+1 || x==0 ||x==BW+1) ? WALL : EMPTY;
      	}   
   	}
   
   	int nFrame, nStay;//벽돌이 내려가는 속도
   
   	srand((unsigned)time(NULL));//난수발생기 초기화
   
   	nFrame=50;//fps
   	int speedCon=SelectSpeed();//속도 선택 
   	SelectBoom();//폭탄 빈도 선택 
	
   	clearScreen();
   	sent[0] = sent[1] = 0;
   	cleared[0] = cleared[1] = 0;
   	DrawScreen();
	DrawBoard(0);
	//스크린 초기화 및 스크린, 보드 그리기

  	if(PlayerNum-1)DrawBoard(1);
  	// 벽돌 선택하기
  	brick[0]=bag[b1++];
	if(PlayerNum-1)brick[1]=bag[b2++];
	
	//boom 초기화 0이면 일반 블록, 1이면 폭 탄 
	boom[0]=0;
	if(PlayerNum-1)boom[1]=0; 
	
  	nx[0]=nx[1]=BW/2;  // 벽돌의 초기 x좌표
  	ny[0]=ny[1]=2; // 벽돌의 초기 y좌표 
  	rot[0]=rot[1]=0;  //벽돌의 회전모양 번호
  	PrintBrick(0, TRUE); // 벽돌 그리기 함수 호출
  	PrintShadow(0, TRUE);//그림자 그리기
    if(PlayerNum-1)PrintBrick(1, TRUE); 
	if(PlayerNum-1)PrintShadow(1, TRUE); 
    nStay=nFrame/speedCon;//선택한 속도 반영

    int TIME=0;//시간 초기화
    for (;2;) {
	gotoXY(Mx, BY+8);printf("TIME: %.2f",(float)TIME/(float)1000);//시간 안내
    	gotoXY(Mx, BY+10);printf("SPEED: %d",TIME/30000+1);//속도 안내
    	
    	if((TIME/1000)%70>=32&&(TIME/1000)%70<=50){ 	
			if((TIME/1000)%70<=34){
				CONSOLE_SCREEN_BUFFER_INFO buff;
				GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buff);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  (buff.wAttributes & 0xf0) | (5 & 0xf));
				int i;
				for(i=3;i>0;i--){
					gotoXY(Mx/3, BY+6);printf("%d초 후 이동 반전!!!",i);
					delay(1000);
				}
				TIME+=3000;
				GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buff);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  (buff.wAttributes & 0xf0) | (15 & 0xf));
			}
			else{
				gotoXY(Mx ,BY+12);printf("%d초 후 반전 종료!!!",50-(TIME/1000)%70);
			}
		}
		else{
			gotoXY(Mx,BY+12);printf("                    ");
		}//키 반전 안내 메세지
		
    	if(TIME%70000==35000) tenet();
    	if(TIME%70000==50000) tenet();
    	//키 반전

    	int t0 = TIME - StrokeTime[0], t1 = TIME - StrokeTime[1];
	    if(PlayerNum==2){ // 줄을 지운 시점에서 일정 시간 내에 번개가 번쩍이는 효과 주기
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

        if (--nStay == 0) {//nStay 프레임마다 블럭 다운 실행
nStay=nFrame/(TIME/30000+1);//30초마다 속도 증가
            nStay/=speedCon; //초기 선택 속도 반영
             
            if (MoveDown(0)) 
            {
            	boom[0]=1;
            	PrintBrick(0, TRUE);
				brick[0]=bag[b1++];
				boom[0]+=TIME/13;
				boom[0]%=BOOMNUM;// 폭탄인지의 여부를 초기화 당시 시간의 누적합으로 유사 랜덤 구현 
            	nx[0]=BW/2;  // 벽돌의 초기 x좌표
		      	ny[0]=2; // 벽돌의 초기 y좌표 
		      	rot[0]=0;  //벽돌의 회전모양 번호
		      	 // 벽돌 그리기 함수 호출
		      	PrintBrick(0, TRUE);
		      	PrintShadow(0, TRUE);
		      	PrintShadow(1, TRUE);
            	if (GetAround(nx[0],ny[0],brick[0],rot[0], 0) != EMPTY){ //새로 들어온 블록 주위에 블록이 쌓여있다면 패배 처리.
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
				boom[1]%=BOOMNUM;// 폭탄인지의 여부를 초기화 당시 시간의 누적합으로 유사 랜덤 구현 
            	nx[1]=BW/2;  // 벽돌의 초기 x좌표
		      	ny[1]=2; // 벽돌의 초기 y좌표 
		      	rot[1]=0;  //벽돌의 회전모양 번호
		      	PrintBrick(1, TRUE); // 벽돌 그리기 함수 호출
				PrintShadow(1, TRUE);  
				PrintShadow(0, TRUE);
				if (GetAround(nx[1],ny[1],brick[1],rot[1], 1) != EMPTY) {//새로 들어온 블록 주위에 블록이 쌓여있다면 패배 처리.
            		winner=0;
            		break;
				}
			}
			
		}
		if(cleared[0]/LIGHTNING > strokes[0]){//지운 줄 수를 카운팅해 번개를 내릴 여부를 결정한다.
      		strokes[0] = cleared[0]/LIGHTNING;
      		StrokeTime[1] = TIME;
      		StrokeLine[1] = rand()%10;
		}
		if(cleared[1]/LIGHTNING > strokes[1]){//지운 줄 수를 카운팅해 번개를 내릴 여부를 결정한다.
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
			boom[0]%=BOOMNUM;// 폭탄인지의 여부를 초기화 당시 시간의 누적합으로 유사 랜덤 구현 
         	nx[0]=BW/2;  // 벽돌의 초기 x좌표
      		ny[0]=2; // 벽돌의 초기 y좌표 
      		rot[0]=0;  //벽돌의 회전모양 번호
        	PrintShadow(0, TRUE);//그림자 그리기 함수 호출
        	PrintBrick(0, TRUE); // 벽돌 그리기 함수 호출
         	if (GetAround(nx[0],ny[0],brick[0],rot[0], 0) != EMPTY){//게임 종료 여부 판별하는 코드
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
			boom[1]%=BOOMNUM;// 폭탄인지의 여부를 초기화 당시 시간의 누적합으로 유사 랜덤 구현 
         	nx[1]=BW/2;  // 벽돌의 초기 x좌표
      		ny[1]=2; // 벽돌의 초기 y좌표 
      		rot[1]=0;  //벽돌의 회전모양 번호
      		PrintShadow(1, TRUE);  //그림자 그리기 함수 호출
      		PrintBrick(1, TRUE); // 벽돌 그리기 함수호출
         	if (GetAround(nx[1],ny[1],brick[1],rot[1], 1) != EMPTY){//게임 종료 여부를 판별하는 코드
            		winner=0;
            		break;
				}
     	}
     	int boomInd=0;//폭탄 관련 로직 실행 플레이어 인덱스 선언,초기화
     	for(boomInd=0;boomInd<PlayerNum;boomInd++){//블럭이 폭탄이면 폭탄 그림자 그리기
     		if(boom[boomInd]==0) PrintBoomShadow(boomInd,true);
		 }
        delay(1000/nFrame);
        
        for(boomInd=0;boomInd<PlayerNum;boomInd++){//블럭이 폭탄이면 폭탄 그림자 지우기
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
	if(GetAsyncKeyState(VK_SPACE)){//게임을 재시작하는 코드
		main();
	}
	delay(20);
   }
   setCursorType(NORMALCURSOR);
}

void tenet() //블록의 이동방향을 반전시키는 함수
{
	GoRight*=-1;
	GoLeft*=-1;
	ROT*=-1;
}
int max(int a, int b){
	return a>b?a:b;
}
int GetAround(int x, int y, int b, int r, int type)//블록 주위에 다른 벽돌이 있는지 확인하는 함수
{
   int i, k=EMPTY;
   if(x<0||x>BW||y<0||y>BH) return WALL;
   for (i=0;i<4;i++) {//블럭의 모든 칸을 순회하며 해당 칸 확인
      k=max(k, board[type][x+Shape[b][r][i].x][y+Shape[b][r][i].y]%3);   
   }
   return k;
}

void DrawScreen()//전체적인 스크린을 그리는 함수
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
   //기타 안내 메세지
   gotoXY(Mx,BY+3);puts("Tetris Ver 1.0");
   gotoXY(Mx,BY+5);puts("player I: WASD + SPACE"); 
   if(PlayerNum-1){
		gotoXY(Mx,BY+6);puts("player II: 방향키 + ENTER");
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

void PrintBrick(int t, BOOL Show)//플레이어의 벽돌을 그리는 함수
{
    int i;
	CONSOLE_SCREEN_BUFFER_INFO buff;
	
	if(boom[t]==0 && Show){//폭탄 그리기
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buff);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  (buff.wAttributes & 0xf0) | (5 & 0xf));
	}
	
	if(t==0){//폭발 원점 보라색
		gotoXY(BX+(Shape[brick[0]][rot[0]][0].x+nx[0])*2,BY+Shape[brick[0]][rot[0]][0].y+ny[0]);
		puts(arTile[Show ? BRICK:EMPTY]);
	}
	else{//일반 폭발원점
		gotoXY(BX*2+BW*2+4+(Shape[brick[1]][rot[1]][0].x+nx[1])*2,BY+Shape[brick[1]][rot[1]][0].y+ny[1]);
		puts(arTile[Show ? BRICK:EMPTY]);
	}
	
	
	if(boom[t]==0 && Show){
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buff);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  (buff.wAttributes & 0xf0) | (4 & 0xf));
	}
	
	if(t==0){//파편 그리기
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
	
	if(boom[t]==0 && Show){//색 리셋
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buff);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  (buff.wAttributes & 0xf0) | (15 & 0xf));
	}
}

void DrawBoard(int t)//플레이어의 보드를 그리는 함수
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

void TestFull(int t) //한 줄이 가득 찼는지를 확인, 줄을 지우는 함수
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
   	if(PlayerNum==2){ //2인 기준으로, 일정 수의 줄을 지우면 상대에게 쓰레기줄을 보내는 코드
   		
   		
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

BOOL MoveDown(int t) //블록을 한 칸 아래로 내리는 함수
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

int ProcessKey() //사용자 정의 입력을 받는 함수
{ 
   	int ch, trot1, trot2; 
   	bool one=false, two=false;
	if(GetAsyncKeyState('A')){
		if (GetAround(nx[0] + GoLeft, ny[0], brick[0], rot[0], 0) == EMPTY) {  
			clock_t now=clock();
			if(now-time1>=keyTime){
				time1 = now;
				PrintShadow(0, FALSE);
				PrintBrick(0, FALSE); //벽돌 지우기  
           		nx[0]+=GoLeft ;
           		PrintShadow(0, TRUE);
           		PrintBrick(0, TRUE); //벽돌 그리기  
			}
        } 
	}
	if(GetAsyncKeyState('D')){
		if (GetAround(nx[0]+ GoRight, ny[0], brick[0], rot[0], 0) == EMPTY) {
			clock_t now=clock();
			if(now-time1>=keyTime){  
				time1 = now;
           		PrintShadow(0, FALSE);		
           		PrintBrick(0, FALSE); //벽돌 지우기  
           		nx[0]+=GoRight ;
           		PrintShadow(0, TRUE);
           		PrintBrick(0, TRUE); //벽돌 그리기  
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
	           		PrintBrick(1, FALSE); //벽돌 지우기  
	           		nx[1]+=GoLeft;
	           		PrintShadow(1, TRUE);
	           		PrintBrick(1, TRUE); //벽돌 그리기  
	           	}
	        } 
		}
		if(GetAsyncKeyState(VK_RIGHT)){
			if (GetAround(nx[1]+GoRight, ny[1], brick[1], rot[1], 1) == EMPTY) {  
	           	clock_t now=clock();
				if(now-time2>=keyTime){  
					time2 = now;
	           		PrintShadow(1, FALSE);
	           		PrintBrick(1, FALSE); //벽돌 지우기  
	           		nx[1]+=GoRight;
	           		PrintShadow(1, TRUE);
	           		PrintBrick(1, TRUE); //벽돌 그리기  
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

void PrintShadow(int t, BOOL Show)//블록이 떨어질 위치를 그림자로 미리 표시해주는 코드
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
{//폭탄 파편의 도착지점을 표시해주는 함수
	int b=brick[t],r=rot[t],x=nx[t],y=ny[t],type=t;
	struct Point BOOM[4] = {Shape[b][r][0],Shape[b][r][1],Shape[b][r][2],Shape[b][r][3]	};
	int i;
	
	CONSOLE_SCREEN_BUFFER_INFO buff;
	if(Show){
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buff);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  (buff.wAttributes & 0xf0) | (6 & 0xf));
	}
	
	for(i=1;i<4;i++){//파편의 도착 지점을 계산
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

void SevenBag()//블록이 나오는 순서를 아예 랜덤이 아니라, 7개씩 묶어 그 내부에서만 셔플하는 코드. 
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

BOOL PointCheck(int type, int x, int y, int pointType){//좌표가 입력받은 종류의 칸인지 확인
	if(x<0||x>BW||y<0||y>BH){
		if(pointType==WALL) return true;
		return false;
	}
	if(board[type][x][y]==pointType) return true;
	return false;
}

void splash(int x, int y, int type, int b, int r)//폭발시 파편을 뿌리고, 파편이 순차적으로 터지게 하는 함수
{
	struct Point BOOM[4] = {Shape[b][r][0],Shape[b][r][1],Shape[b][r][2],Shape[b][r][3]	};
	int i;
	
	
	for(i=1;i<4;i++){//파편 뿌리기
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
	for (i=1;i<4;i++) { //파편 터뜨리기
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

BOOL SqBoom(int x, int y, int Range){//좌표가 폭발 반경에 해당하는지 체크
	if((Abs(x)==Range&&Abs(y)<=Range)||(Abs(y)==Range&&Abs(x)<=Range)) return true;
	return false;
}

void crash(int x, int y, int type)
{//파편 폭발 함수
	
	CONSOLE_SCREEN_BUFFER_INFO buff;
	
	int i=-BoomArange,j=BoomArange,Range=0;
	
	for(Range=0;Range<=BoomArange;Range++){//폭발의 퍼지는 이펙트
		
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buff);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  (buff.wAttributes & 0xf0) | (5 & 0xf));
		
		for(i=-BoomArange;i<=BoomArange;i++){//폭발 연기 그리기
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
		
		for(i=-BoomArange;i<=BoomArange;i++){//연기 지우고 해당 칸을 빈칸으로 설정, 그리기
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

BOOL SRS(int br, int rotation, int rot_state, int t){ //Standard Rotation System: 더 자연스러운 블록 회전을 위해 사용

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

void StrikeFlash(int t, BOOL Show){ //현재 시간에 따라 번개가 치는 열의 블록을 번쩍이게 변경하는 코드
	
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

void Strike(int t){ //번개를 내려 해당 열의 블록을 모두 지우는 코드
	int i;
	gotoXY(BX+2*(BX+BW*2+4),BY+17);
	for(i=1;i<BH+1;i++){
		board[t][StrokeLine[t]+1][i] = ALERT_EMPTY;
	}
	DrawBoard(t);
}

