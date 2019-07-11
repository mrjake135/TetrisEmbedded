#include "button.h"

#include "sys_dbg.h"

#include "app.h"
#include "app_bsp.h"
#include "app_dbg.h"
#include "app_if.h"

#include "task_list.h"
#include "task_list_if.h"

button_t btn_mode;
button_t btn_up;
button_t btn_down;

void btn_mode_callback(void* b) {
	button_t* me_b = (button_t*)b;
	switch (me_b->state) {
	case BUTTON_SW_STATE_PRESSED: {
		APP_DBG("[btn_mode_callback] BUTTON_SW_STATE_PRESSED\n");
		task_post_pure_msg(AC_TASK_TETRIS_CONTROL,AC_TETRIS_GAME_CONTROL_START);
	}
		break;

	case BUTTON_SW_STATE_LONG_PRESSED: {
		APP_DBG("[btn_mode_callback] BUTTON_SW_STATE_LONG_PRESSED\n");
	}
		break;

	case BUTTON_SW_STATE_RELEASED: {
		APP_DBG("[btn_mode_callback] BUTTON_SW_STATE_RELEASED\n");
		if(btn_down.state == BUTTON_SW_STATE_PRESSED || btn_down.state == BUTTON_SW_STATE_LONG_PRESSED)
		{
			task_post_pure_msg(AC_TASK_TETRIS_LEVEL,AC_TETRIS_LEVEL_1);
		}
	}
		break;
	default:
		break;
	}
}

void btn_up_callback(void* b) {
	button_t* me_b = (button_t*)b;
	switch (me_b->state) {
	case BUTTON_SW_STATE_PRESSED: {
		APP_DBG("[btn_up_callback] BUTTON_SW_STATE_PRESSED\n");
		task_post_pure_msg(AC_TASK_TETRIS_CONTROL,AC_TETRIS_GAME_CONTROL_START);
	}
		break;

	case BUTTON_SW_STATE_LONG_PRESSED: {
		APP_DBG("[btn_up_callback] BUTTON_SW_STATE_LONG_PRESSED\n");
	}
		break;

	case BUTTON_SW_STATE_RELEASED: {
		APP_DBG("[btn_up_callback] BUTTON_SW_STATE_RELEASED\n");
		if(btn_mode.state == BUTTON_SW_STATE_PRESSED || btn_mode.state == BUTTON_SW_STATE_LONG_PRESSED)
		{
			task_post_pure_msg(AC_TASK_TETRIS_UI,AC_TETRIS_ROTATE);
		}
		else
		{
			task_post_pure_msg(AC_TASK_TETRIS_UI,AC_TETRIS_LEFT);
		}
	}
		break;

	default:
		break;
	}
}

void btn_down_callback(void* b) {
	button_t* me_b = (button_t*)b;
	switch (me_b->state) {
	case BUTTON_SW_STATE_PRESSED: {
		APP_DBG("[btn_down_callback] BUTTON_SW_STATE_PRESSED\n");
		task_post_pure_msg(AC_TASK_TETRIS_CONTROL,AC_TETRIS_GAME_CONTROL_START);
		if(btn_mode.state == BUTTON_SW_STATE_PRESSED || btn_mode.state == BUTTON_SW_STATE_LONG_PRESSED)
		{
			task_post_pure_msg(AC_TASK_TETRIS_LEVEL,AC_TETRIS_FAST_FALL);
		}
	}
		break;

	case BUTTON_SW_STATE_LONG_PRESSED: {
		APP_DBG("[btn_down_callback] BUTTON_SW_STATE_LONG_PRESSED\n");
		if(btn_mode.state == BUTTON_SW_STATE_PRESSED || btn_mode.state == BUTTON_SW_STATE_LONG_PRESSED)
		{
			task_post_pure_msg(AC_TASK_TETRIS_LEVEL,AC_TETRIS_FAST_FALL);
		}

	}
		break;

	case BUTTON_SW_STATE_RELEASED: {
		APP_DBG("[btn_down_callback] BUTTON_SW_STATE_RELEASED\n");
		if(btn_mode.state == BUTTON_SW_STATE_PRESSED || btn_mode.state == BUTTON_SW_STATE_LONG_PRESSED)
		{
			task_post_pure_msg(AC_TASK_TETRIS_LEVEL,AC_TETRIS_LEVEL_1);
		}
		else{
			task_post_pure_msg(AC_TASK_TETRIS_UI,AC_TETRIS_RIGHT);
		}
	}
		break;

	default:
		break;
	}
}
