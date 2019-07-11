#include "tetris_game_levels.h"

#include "app.h"
#include "app_dbg.h"


#include "task_list.h"
#include "task_dbg.h"
#include "timer.h"
#include "time.h"

#include "tetris_ui.h"
u_int16_t timeDrop;
void tetris_level(ak_msg_t* msg) {
	switch (msg->sig) {
		case AC_TETRIS_LEVEL_0:
		{
			timer_remove_attr(AC_TASK_TETRIS_UI,AC_TETRIS_DOWN);
		}
			break;
		case AC_TETRIS_LEVEL_1:
		{
			if(lines == 0) timeDrop = 800;
			if(lines == 10) timeDrop = 700;
			if(lines == 20) timeDrop = 600;
			if(lines == 30) timeDrop = 500;
			if(lines == 40) timeDrop = 400;
			if(lines == 50) timeDrop = 300;
			if(lines == 60) timeDrop = 200;
			if(lines == 70) timeDrop = 100;
			timer_remove_attr(AC_TASK_TETRIS_UI,AC_TETRIS_DOWN);
			APP_DBG("SLOW FALLING\n");
			timer_set(AC_TASK_TETRIS_UI,AC_TETRIS_DOWN,timeDrop,TIMER_PERIODIC);
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
