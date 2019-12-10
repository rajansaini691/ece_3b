#include "lib/qpn_port.h"
#include "drw.h"
#include "bsp.h"
#include "hsm.h"

static QState hsm_init(hsm *mcn);
static QState hsm_on(hsm *mcn);
static QState hsm_listen(hsm *mcn);
static QState hsm_configure(hsm *mcn);
 
void hsm_ctor(void)  {
	QActive_ctor(&(machine.super_), (QStateHandler)& hsm_init);
	machine.note = "A";
	machine.ticks = 0;
	machine.octave = 4;
	machine.tuning = 440;
}

QState hsm_init(hsm *mcn) {
	xil_printf("Initializing HSM\r\n");
	return Q_TRAN(&hsm_on);
}

QState hsm_on(hsm *mcn) {
	switch (Q_SIG(mcn)) {
		case Q_ENTRY_SIG:
			drw_clr(0, 0, LCD_WIDTH, LCD_HEIGHT);
		case Q_INIT_SIG:
			return Q_HANDLED();
		case TICK_SIG:
			return Q_HANDLED();
		case ENC_BTN_SIG:
			return Q_HANDLED();

	}
	return Q_SUPER(&QHsm_top);
}

QState hsm_listen(hsm *mcn) {
	switch (Q_SIG(mcn)) {
		case Q_ENTRY_SIG:
			mcn->disp_tmr = INACTIVE_TICKS;
			return Q_HANDLED();
		case Q_EXIT_SIG:
			return Q_HANDLED();
		case LEFT_SIG:
			return Q_HANDLED();
			break;
		case RIGHT_SIG:
			return Q_HANDLED();
			break;
		case TICK_SIG:
			return Q_HANDLED();
	}
	return Q_SUPER(&hsm_on);
}

QState hsm_configure(hsm *mcn) {
	switch (Q_SIG(mcn)) {
		case Q_ENTRY_SIG:
			return Q_HANDLED();
		case Q_EXIT_SIG: 
			return Q_HANDLED();
		case TICK_SIG:
			return Q_HANDLED();
		case LEFT_SIG:
			return Q_HANDLED();
		case RIGHT_SIG:
			return Q_HANDLED();
	}
	return Q_SUPER(&hsm_on);
}
