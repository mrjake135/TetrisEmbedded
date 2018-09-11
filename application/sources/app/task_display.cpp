#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "sys_ctrl.h"
#include "sys_dbg.h"

#include "view_render.h"

#include "qrcode.h"

#include "app.h"
#include "app_dbg.h"

#include "task_list.h"
#include "task_display.h"

scr_mng_t scr_mng_app;

/* list of screen handler */
void scr_startup_handle(ak_msg_t* msg);

/* list of view dynamic function */
static void view_scr_startup();

/**
 ******************************************************************************
 * objects MAIN SCREEN
 *
 ******************************************************************************
 */

/*----------[[[ITERM]]]------------*/

view_dynamic_t dyn_view_startup = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_startup
};

/*----------[[[OBJECT]]]------------*/
view_screen_t scr_startup = {
	&dyn_view_startup,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

void task_display(ak_msg_t* msg) {
	scr_mng_dispatch(msg);
}

void scr_startup_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case AC_DISPLAY_INITIAL: {
		APP_DBG_SIG("AC_DISPLAY_INITIAL\n");
		view_render.initialize();
		view_render_display_off();
		SCREEN_NONE_UPDATE_MASK();
		task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_ON_LOGO);
	}
		break;

	case AC_DISPLAY_SHOW_ON_LOGO: {
		APP_DBG_SIG("AC_DISPLAY_SHOW_ON_LOGO\n");
		view_render_display_on();
		timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_OFF, AC_DISPLAY_LOGO_INTERVAL, TIMER_ONE_SHOT);
	}
		break;

	case AC_DISPLAY_SHOW_OFF: {
		APP_DBG_SIG("AC_DISPLAY_SHOW_OFF\n");
		view_render_display_off();
		SCREEN_NONE_UPDATE_MASK();
	}
		break;

	default:
		break;
	}
}

void view_scr_startup() {
	/* link github */
	QRCode qrcode;
	uint8_t qrcodeData[qrcode_getBufferSize(3)];
	qrcode_initText(&qrcode, qrcodeData, 3, 0, "https://github.com/thannt92/arm_m3_base.git");

	for (uint8_t y = 0; y < qrcode.size; y++) {
		for (uint8_t x = 0; x < qrcode.size; x++) {
			view_render.drawPixel(x, y, qrcode_getModule(&qrcode, x, y) ? 1 : 0);
		}
	}

	/* ak logo */
	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(38, 3);
	view_render.print("   __    _  _ ");
	view_render.setCursor(38, 10);
	view_render.print("  /__\\  ( )/ )");
	view_render.setCursor(38, 20);
	view_render.print(" /(__)\\ (   (");
	view_render.setCursor(38, 30);
	view_render.print("(__)(__)(_)\\_)");
	view_render.setCursor(42, 42);
	view_render.print("Active Kernel");
	view_render.update();
}
