#include "tetris_game_control.h"
#include "app.h"

#include "task_list.h"
#include "task_dbg.h"

bool holding_left = false;
bool started_game = false;
void tetris_game_control(ak_msg_t* msg) {
	switch (msg->sig) {
		case AC_TETRIS_GAME_CONTROL_START_SCREEN:
		{
			started_game = false;
			task_post_pure_msg(AC_TASK_TETRIS_LEVEL, AC_TETRIS_LEVEL_0);
		}
			break;
		case AC_TETRIS_GAME_CONTROL_INIT:
		{
			holding_left = false;
		}
			break;
		case AC_TETRIS_GAME_CONTROL_HOLDING_LEFT_RELEASE:
		{
			holding_left = false;
			if(!started_game)
			{
				started_game = true;
				task_post_pure_msg(AC_TASK_TETRIS_UI, AC_TETRIS_INIT);
				task_post_pure_msg(AC_TASK_TETRIS_LEVEL, AC_TETRIS_LEVEL_1);
			}
		}
			break;
		case AC_TETRIS_GAME_CONTROL_HOLDING_LEFT:
		{
			holding_left = true;
		}
			break;
		case AC_TETRIS_GAME_CONTROL_MIDDLE:
		{
			if(started_game)
			{
				if(holding_left == true)
				{
					task_post_pure_msg(AC_TASK_TETRIS_UI, AC_TETRIS_ROTATE);
				}
				else
				{
					task_post_pure_msg(AC_TASK_TETRIS_UI, AC_TETRIS_LEFT);
				}
			}
			else{
				started_game = true;
				task_post_pure_msg(AC_TASK_TETRIS_UI, AC_TETRIS_INIT);
				task_post_pure_msg(AC_TASK_TETRIS_LEVEL, AC_TETRIS_LEVEL_1);
			}
		}
			break;

		case AC_TETRIS_GAME_CONTROL_RIGHT:
		{
			if(started_game)
			{
				if(holding_left == true)
				{
					task_post_pure_msg(AC_TASK_TETRIS_UI, AC_TETRIS_DOWN);
				}
				else
				{
					task_post_pure_msg(AC_TASK_TETRIS_UI, AC_TETRIS_RIGHT);
				}
			}
			else{
				started_game = true;
				task_post_pure_msg(AC_TASK_TETRIS_UI, AC_TETRIS_INIT);
				task_post_pure_msg(AC_TASK_TETRIS_LEVEL, AC_TETRIS_LEVEL_1);
			}
		}
			break;
	}
}
