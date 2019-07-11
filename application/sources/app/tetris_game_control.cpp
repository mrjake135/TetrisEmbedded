#include "tetris_game_control.h"
#include "app.h"

#include "task_list.h"
#include "task_dbg.h"

#include "app_dbg.h"
bool started_game = false;

void tetris_game_control(ak_msg_t* msg) {
	switch (msg->sig) {
		case AC_TETRIS_GAME_CONTROL_START_SCREEN:
		{
			started_game = false;
		}
			break;
		case AC_TETRIS_GAME_CONTROL_START:
		{
			if(!started_game)
			{
				started_game = true;
				task_post_pure_msg(AC_TASK_TETRIS_UI, AC_TETRIS_INIT);
				task_post_pure_msg(AC_TASK_TETRIS_LEVEL, AC_TETRIS_LEVEL_1);
			}
		}
			break;
	}
}
