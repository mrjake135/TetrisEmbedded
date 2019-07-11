#include "tetris_game_levels.h"

#include "app.h"
#include "app_dbg.h"


#include "task_list.h"
#include "task_dbg.h"
#include "timer.h"
#include "time.h"

void tetris_level(ak_msg_t* msg) {
	switch (msg->sig) {
		case AC_TETRIS_LEVEL_0:
		{
			timer_remove_attr(AC_TASK_TETRIS_UI,AC_TETRIS_DOWN);
		}
			break;
		case AC_TETRIS_LEVEL_1:
		{
			timer_remove_attr(AC_TASK_TETRIS_UI,AC_TETRIS_DOWN);
			APP_DBG("SLOW FALLING\n");
			timer_set(AC_TASK_TETRIS_UI,AC_TETRIS_DOWN,800,TIMER_PERIODIC);
		}
			break;
		case AC_TETRIS_FAST_FALL:
		{
			timer_remove_attr(AC_TASK_TETRIS_UI,AC_TETRIS_DOWN);
			APP_DBG("FAST FALLING\n");
			timer_set(AC_TASK_TETRIS_UI,AC_TETRIS_DOWN,100,TIMER_PERIODIC);
		}
			break;
	}
}
