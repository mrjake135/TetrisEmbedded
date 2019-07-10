#include "tetris_game_levels.h"

#include "app.h"

#include "task_list.h"
#include "task_dbg.h"
#include "timer.h"

void tetris_level(ak_msg_t* msg) {
	switch (msg->sig) {
		case AC_TETRIS_LEVEL_0:
		{

		}
			break;
		case AC_TETRIS_LEVEL_1:
		{
			timer_set(AC_TASK_TETRIS_UI,AC_TETRIS_DOWN,800,TIMER_PERIODIC);
		}
			break;
	}
}
