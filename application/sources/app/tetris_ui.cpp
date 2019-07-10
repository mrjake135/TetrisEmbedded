#include <stdint.h>

#include "Adafruit_ssd1306syp.h"

#include "tetris_ui.h"
#include "task_list.h"
#include "task_dbg.h"

#include "app_dbg.h"
#include "app.h"

#include "timer.h"
Adafruit_ssd1306syp screenObj;

static void startScreen();

static void rotate();
static void currBoard();

static void pieceInit();
static void chooseRandom();
static void pieceDown();
static void pieceRight();
static void pieceLeft();


static bool checkBoardDown();
static bool checkBoardRight();
static bool checkBoardLeft();
static bool checkBoardRotation();
static void checkTetris();

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


uint8_t board[18][14] = {{2,0,0,0,0,0,0,0,0,0,0,0,0,2},
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
int r;
int c;

void tetris_ui(ak_msg_t* msg) {
	switch (msg->sig) {
		case AC_TETRIS_START_SCREEN:{
			startScreen();
		}
			break;
		case AC_TETRIS_INIT:{
			screenObj.clear();
			APP_DBG("INIT\n");
			task_post_pure_msg(AC_TASK_TETRIS_UI,AC_TETRIS_NEW_PIECE);
		}
			break;
		case AC_TETRIS_NEW_PIECE:{
			r = 0;
			c = 20;
			APP_DBG("NEW PIECE\n");
			pieceInit();
			task_post_pure_msg(AC_TASK_TETRIS_UI,AC_TETRIS_UPDATE);
		}
			break;
		case AC_TETRIS_UPDATE:{
			APP_DBG("row: %d\n",r);
			APP_DBG("col: %d\n",c);
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

void startScreen()
{
	screenObj.initialize();
	screenObj.clear();
	screenObj.drawRect(0,0,120,60,WHITE);
	screenObj.setCursor(40,10);
	screenObj.print("Tetris");
	screenObj.update();
}
void chooseRandom()
{
	int randNum = rand() % 7;
	if(randNum == 0) memcpy(x,I,16);
	if(randNum == 1) memcpy(x,O,16);
	if(randNum == 2) memcpy(x,L,16);
	if(randNum == 3) memcpy(x,T,16);
	if(randNum == 4) memcpy(x,J,16);
	if(randNum == 5) memcpy(x,Y,16);
	if(randNum == 6) memcpy(x,Z,16);
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
				screenObj.drawRect(4*j-4,4*i-8,4,4,BLACK);
			if(board[i][j] == 1)
				screenObj.drawRect(4*j-4,4*i-8,4,4,WHITE);
		}
	}
	screenObj.drawRect(0,0,48,60,WHITE);
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
		}
	}
}
