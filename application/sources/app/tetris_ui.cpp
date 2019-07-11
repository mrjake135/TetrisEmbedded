#include <stdint.h>

#include "Adafruit_ssd1306syp.h"

#include "tetris_ui.h"
#include "task_list.h"
#include "task_dbg.h"

#include "app_dbg.h"
#include "app.h"

#include "timer.h"

Adafruit_ssd1306syp screenObj;
static void initGame();
static void startScreen();

static void currBoard();
static void updateNextPieces();
static void pieceInit();
static void chooseRandom();

static void pieceDown();
static void pieceRight();
static void pieceLeft();
static void rotate();


static bool checkBoardDown();
static bool checkBoardRight();
static bool checkBoardLeft();
static bool checkBoardRotation();
static void checkTetris();
static void checkLoss();

//PIECES
uint8_t x[4][4];
uint8_t I[4][4] = {{0,0,1,0},
				   {0,0,1,0},
				   {0,0,1,0},
				   {0,0,1,0}};

uint8_t O[4][4] = {{0,0,0,0},
				   {0,1,1,0},
				   {0,1,1,0},
				   {0,0,0,0}};

uint8_t L[4][4] = {{0,0,0,0},
				   {0,1,0,0},
				   {0,1,0,0},
				   {0,1,1,0}};

uint8_t T[4][4] = {{0,0,0,0},
				   {0,1,0,0},
				   {0,1,1,0},
				   {0,1,0,0}};

uint8_t J[4][4] = {{0,0,0,0},
				   {0,1,1,0},
				   {0,1,0,0},
				   {0,1,0,0}};

uint8_t Y[4][4] = {{0,0,0,0},
				   {0,0,1,0},
				   {0,1,1,0},
				   {0,1,0,0}};

uint8_t Z[4][4] = {{0,0,0,0},
				   {0,1,0,0},
				   {0,1,1,0},
				   {0,0,1,0}};


uint8_t board[18][14] =   {{2,0,0,0,0,0,0,0,0,0,0,0,0,2},
						   {2,0,0,0,0,0,0,0,0,0,0,0,0,2},
						   {2,0,0,0,0,0,0,0,0,0,0,0,0,2},
						   {2,0,0,0,0,0,0,0,0,0,0,0,0,2},
						   {2,0,0,0,0,0,0,0,0,0,0,0,0,2},
						   {2,0,0,0,0,0,0,0,0,0,0,0,0,2},
						   {2,0,0,0,0,0,0,0,0,0,0,0,0,2},
						   {2,0,0,0,0,0,0,0,0,0,0,0,0,2},
						   {2,0,0,0,0,0,0,0,0,0,0,0,0,2},
						   {2,0,0,0,0,0,0,0,0,0,0,0,0,2},
						   {2,0,0,0,0,0,0,0,0,0,0,0,0,2},
						   {2,0,0,0,0,0,0,0,0,0,0,0,0,2},
						   {2,0,0,0,0,0,0,0,0,0,0,0,0,2},
						   {2,0,0,0,0,0,0,0,0,0,0,0,0,2},
						   {2,0,0,0,0,0,0,0,0,0,0,0,0,2},
						   {2,0,0,0,0,0,0,0,0,0,0,0,0,2},
						   {2,0,0,0,0,0,0,0,0,0,0,0,0,2},
						   {2,2,2,2,2,2,2,2,2,2,2,2,2,2}};

uint8_t nextPieces[15][4] =  {{0,0,0,0},
							  {0,0,0,0},
							  {0,0,0,0},
							  {0,0,0,0},

							  {0,0,0,0},

							  {0,0,0,0},
							  {0,0,0,0},
							  {0,0,0,0},
							  {0,0,0,0},

							  {0,0,0,0},};

uint8_t arrNext[2] = {0};
int8_t r;
int8_t c;
uint8_t lines;
void tetris_ui(ak_msg_t* msg) {
	switch (msg->sig) {
		case AC_TETRIS_START_SCREEN:{
			APP_DBG("START SCREEN\n");
			startScreen();
		}
			break;

		case AC_TETRIS_INIT:{
			APP_DBG("INIT\n");
			initGame();
			task_post_pure_msg(AC_TASK_TETRIS_UI,AC_TETRIS_NEW_PIECE);
		}
			break;

		case AC_TETRIS_NEW_PIECE:{
			screenObj.setCursor(63,5);
			screenObj.fillRect(63,5,50,14,BLACK);
			screenObj.print("Lines: ");
			screenObj.print(lines);
			r = 0;
			c = 20;
			APP_DBG("NEW PIECE\n");
			pieceInit();
			updateNextPieces();
			task_post_pure_msg(AC_TASK_TETRIS_UI,AC_TETRIS_UPDATE);
		}
			break;

		case AC_TETRIS_UPDATE:{
			APP_DBG("UPDATE\n");
			currBoard();
			screenObj.update();
		}
			break;

		case AC_TETRIS_ROTATE:{
			APP_DBG("ROTATE\n");
			rotate();
			task_post_pure_msg(AC_TASK_TETRIS_UI,AC_TETRIS_UPDATE);
		}
			break;

		case AC_TETRIS_DOWN: {
			APP_DBG("DOWN\n");
			pieceDown();
			task_post_pure_msg(AC_TASK_TETRIS_UI,AC_TETRIS_UPDATE);
		}
			break;

		case AC_TETRIS_RIGHT: {
			APP_DBG("RIGHT\n");
			pieceRight();
			task_post_pure_msg(AC_TASK_TETRIS_UI,AC_TETRIS_UPDATE);
		}
			break;

		case AC_TETRIS_LEFT: {
			APP_DBG("LEFT\n");
			pieceLeft();
			task_post_pure_msg(AC_TASK_TETRIS_UI,AC_TETRIS_UPDATE);
		}
			break;
	}
}

void initGame()
{
	lines = 0;
	for(int i = 0; i < 17; i++)
	{
		for(int j = 1; j < 13; j++)
		{
			board[i][j] = 0;
		}
	}
	screenObj.clear();
	screenObj.drawRect(60,0,60,60,WHITE);
	arrNext[0] = rand() % 7;
	arrNext[1] = rand() % 7;
	updateNextPieces();
}

void updateNextPieces()
{
	for(int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if(arrNext[0] == 0) nextPieces[i][j] = I[i][j];
			if(arrNext[0] == 1) nextPieces[i][j] = O[i][j];
			if(arrNext[0] == 2)	nextPieces[i][j] = L[i][j];
			if(arrNext[0] == 3) nextPieces[i][j] = T[i][j];
			if(arrNext[0] == 4) nextPieces[i][j] = J[i][j];
			if(arrNext[0] == 5) nextPieces[i][j] = Y[i][j];
			if(arrNext[0] == 6) nextPieces[i][j] = Z[i][j];
		}
	}

	for(int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if(arrNext[1] == 0) nextPieces[i+5][j] = I[i][j];
			if(arrNext[1] == 1) nextPieces[i+5][j] = O[i][j];
			if(arrNext[1] == 2)	nextPieces[i+5][j] = L[i][j];
			if(arrNext[1] == 3) nextPieces[i+5][j] = T[i][j];
			if(arrNext[1] == 4) nextPieces[i+5][j] = J[i][j];
			if(arrNext[1] == 5) nextPieces[i+5][j] = Y[i][j];
			if(arrNext[1] == 6) nextPieces[i+5][j] = Z[i][j];
		}
	}
	for(int i = 0; i < 10; i ++)
	{
		for(int j = 0; j < 4; j++)
		{
			//board is larger than screen thus the 12 addition
			if(nextPieces[i][j] == 0)
				screenObj.drawRect(4*j+80,4*i+15,4,4,BLACK);
			if(nextPieces[i][j] == 1)
				screenObj.drawRect(4*j+80,4*i+15,4,4,WHITE);
		}
	}
	screenObj.update();
}
void startScreen()
{
	screenObj.initialize();
	screenObj.clear();
	screenObj.drawRect(0,0,120,60,WHITE);
	screenObj.setCursor(2,10);
	screenObj.print("Tetris by Khoa Phan");
	screenObj.update();
}
void chooseRandom()
{
	if(arrNext[0] == 0) memcpy(x,I,16);
	if(arrNext[0] == 1) memcpy(x,O,16);
	if(arrNext[0] == 2) memcpy(x,L,16);
	if(arrNext[0] == 3) memcpy(x,T,16);
	if(arrNext[0] == 4) memcpy(x,J,16);
	if(arrNext[0] == 5) memcpy(x,Y,16);
	if(arrNext[0] == 6) memcpy(x,Z,16);

	uint8_t randNum = rand() % 7;
	arrNext[0] = arrNext[1];
	arrNext[1] = randNum;
}
void rotate(){
	if(checkBoardRotation())
	{
		for(int i = 0; i < 4; i ++)
		{
			for(int j = 0; j < 4; j++)
			{
				board[(r+4*i)/4][(c+4*j)/4] =  board[(r+4*i)/4][(c+4*j)/4] - x[i][j];
			}
		}
		for(int i = 0; i < 2; i++)
		{
			for(int j = i; j< 4-i-1; j++)
			{
				int temp = x[i][j];
				x[i][j] = x[j][4-i-1];
				x[j][4-i-1] = x[4-i-1][4-j-1];
				x[4-i-1][4-j-1]= x[4-j-1][i];
				x[4-j-1][i] = temp;
			}
		}
		for(int i = 0; i < 4; i ++)
		{
			for(int j = 0; j < 4; j++)
			{
				board[(r+4*i)/4][(c+4*j)/4] = board[(r+4*i)/4][(c+4*j)/4] + x[i][j];
			}
		}
	}
}
void currBoard(){
	for(int i = 0; i < 18; i ++)
	{
		for(int j = 0; j < 14; j++)
		{
			//board is larger than screen thus the 12 addition
			if(board[i][j] == 0)
				screenObj.drawRect(4*j,4*i-8,4,4,BLACK);
			if(board[i][j] == 1)
				screenObj.drawRect(4*j,4*i-8,4,4,WHITE);
		}
	}
	screenObj.drawRect(4,0,48,60,WHITE);

}

void pieceInit(){
	chooseRandom();
	for(int i = 0; i < 4; i ++)
	{
		for(int j = 0; j < 4; j++)
		{
			board[(r+4*i)/4][(c+4*j)/4] = x[i][j];
		}
	}
}

void pieceDown()
{
	if(checkBoardDown())
	{
		for(int i = 0; i < 4; i ++)
		{
			for(int j = 0; j < 4; j++)
			{
				board[(r+4*i)/4][(c+4*j)/4] = board[(r+4*i)/4][(c+4*j)/4] - x[i][j];
			}
		}
		r= r+4;
		for(int i = 0; i < 4; i ++)
		{
			for(int j = 0; j < 4; j++)
			{
				board[(r+4*i)/4][(c+4*j)/4] = board[(r+4*i)/4][(c+4*j)/4] + x[i][j];
			}
		}
	}
	else
	{
		for(int i = 0; i < 4; i ++)
		{
			for(int j = 0; j < 4; j++)
			{
				board[(r+4*i)/4][(c+4*j)/4] = board[(r+4*i)/4][(c+4*j)/4] + x[i][j];
			}
		}
		checkTetris();
		checkLoss();
		task_post_pure_msg(AC_TASK_TETRIS_UI,AC_TETRIS_NEW_PIECE);
	}
}

void pieceRight()
{
	if(checkBoardRight())
	{
		for(int i = 0; i < 4; i ++)
		{
			for(int j = 0; j < 4; j++)
			{
				board[(r+4*i)/4][(c+4*j)/4] = board[(r+4*i)/4][(c+4*j)/4] - x[i][j] ;
			}
		}
		c= c+4;
		for(int i = 0; i < 4; i ++)
		{
			for(int j = 0; j < 4; j++)
			{
				board[(r+4*i)/4][(c+4*j)/4] = board[(r+4*i)/4][(c+4*j)/4] + x[i][j];
			}
		}
	}
}

void pieceLeft()
{
	if(checkBoardLeft())
	{
		for(int i = 0; i < 4; i ++)
		{
			for(int j = 0; j < 4; j++)
			{
				board[(r+4*i)/4][(c+4*j)/4] = board[(r+4*i)/4][(c+4*j)/4] - x[i][j] ;
			}
		}
		c= c-4;
		for(int i = 0; i < 4; i ++)
		{
			for(int j = 0; j < 4; j++)
			{
				board[(r+4*i)/4][(c+4*j)/4] = board[(r+4*i)/4][(c+4*j)/4] + x[i][j];
			}
		}
	}
}


bool checkBoardDown()
{
	bool ans = true;
	r += 4;
	for(int i = 0; i < 4; i ++)
	{
		for(int j = 0; j < 4; j++)
		{
			board[(r+4*i)/4][(c+4*j)/4] = board[(r+4*i)/4][(c+4*j)/4] + x[i][j];
		}
	}
	for(int i = 0; i < 4; i ++)
	{
		for(int j = 0; j < 4; j++)
		{
			if(board[(r+4*i)/4][(c+4*j)/4] >= 3)
			{
				ans = false;
			}
			board[(r+4*i)/4][(c+4*j)/4] = board[(r+4*i)/4][(c+4*j)/4] - x[i][j];
		}
	}
	r-=4;
	return ans;
}

bool checkBoardRight()
{
	bool ans = true;
	c += 4;
	for(int i = 0; i < 4; i ++)
	{
		for(int j = 0; j < 4; j++)
		{
			board[(r+4*i)/4][(c+4*j)/4] = board[(r+4*i)/4][(c+4*j)/4] + x[i][j];
		}
	}
	for(int i = 0; i < 4; i ++)
	{
		for(int j = 0; j < 4; j++)
		{
			if(board[(r+4*i)/4][(c+4*j)/4] >= 3)
			{
				ans = false;
			}
			board[(r+4*i)/4][(c+4*j)/4] = board[(r+4*i)/4][(c+4*j)/4] - x[i][j];
		}
	}
	c-=4;
	return ans;
}

bool checkBoardLeft()
{
	bool ans = true;
	c -= 4;
	for(int i = 0; i < 4; i ++)
	{
		for(int j = 0; j < 4; j++)
		{
			board[(r+4*i)/4][(c+4*j)/4] = board[(r+4*i)/4][(c+4*j)/4] + x[i][j];
		}
	}
	for(int i = 0; i < 4; i ++)
	{
		for(int j = 0; j < 4; j++)
		{
			if(board[(r+4*i)/4][(c+4*j)/4] >= 3)
			{
				ans = false;
			}
			board[(r+4*i)/4][(c+4*j)/4] = board[(r+4*i)/4][(c+4*j)/4] - x[i][j];
		}
	}
	c+=4;
	return ans;
}

bool checkBoardRotation()
{
	bool ans = true;
	for(int i = 0; i < 2; i++)
	{
		for(int j = i; j< 4-i-1; j++)
		{
			int temp = x[i][j];
			x[i][j] = x[j][4-i-1];
			x[j][4-i-1] = x[4-i-1][4-j-1];
			x[4-i-1][4-j-1]= x[4-j-1][i];
			x[4-j-1][i] = temp;
		}
	}
	for(int i = 0; i < 4; i ++)
	{
		for(int j = 0; j < 4; j++)
		{
			board[(r+4*i)/4][(c+4*j)/4] = board[(r+4*i)/4][(c+4*j)/4] + x[i][j];
		}
	}
	for(int i = 0; i < 4; i ++)
	{
		for(int j = 0; j < 4; j++)
		{
			if(board[(r+4*i)/4][(c+4*j)/4] >= 3)
			{
				ans = false;
			}
		}
	}
	for(int i = 0; i < 4; i ++)
	{
		for(int j = 0; j < 4; j++)
		{
			board[(r+4*i)/4][(c+4*j)/4] = board[(r+4*i)/4][(c+4*j)/4] - x[i][j];
		}
	}
	for(int i = 0; i < 2; i++)
	{
		for(int j = i; j< 4-i-1; j++)
		{
			int temp =x[i][j];
			x[i][j] = x[4-j-1][i];
			x[4-j-1][i] = x[4-i-1][4-j-1];
			x[4-i-1][4-j-1]= x[j][4-i-1];
			x[j][4-1-i] = temp;
		}
	}
	return ans;
}

void checkTetris()
{
	for(int i = 2; i < 17; i ++)
	{
		bool hasnoHole = true;
		for(int j = 1; j < 13; j++)
		{
			if(board[i][j] == 0) hasnoHole = false;
		}
		if(hasnoHole)
		{
			for(int k = i; k > 2; k--)
			{
				for(int j = 1; j < 13; j++)
				{
					board[k][j] = board[k-1][j];
				}
			}
			lines +=1;
		}
	}
}

void checkLoss()
{
	bool loss = false;
	for(int j = 1; j < 13; j++)
	{
		if(board[4][j] > 0) loss = true;
	}
	if(loss)
	{
		task_post_pure_msg(AC_TASK_TETRIS_LEVEL, AC_TETRIS_LEVEL_0);
		timer_set(AC_TASK_TETRIS_CONTROL, AC_TETRIS_GAME_CONTROL_START_SCREEN,1000,TIMER_ONE_SHOT);
		timer_set(AC_TASK_TETRIS_UI, AC_TETRIS_START_SCREEN,1000,TIMER_ONE_SHOT);
	}
}
