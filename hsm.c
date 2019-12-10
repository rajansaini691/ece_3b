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
			return Q_TRAN(hsm_oct_cfg);
		case RIGHT_SIG:
			return Q_TRAN(hsm_oct_cfg);
		case ENC_BTN_SIG:
			return Q_TRAN(hsm_tun_cfg);
		case TICK_SIG:
			// Draw colored word

			// Wait 50 ms (simulate FFT)
			return Q_HANDLED();
	}
	return Q_SUPER(&hsm_on);
}

QState hsm_configure(hsm *mcn) {
	switch (Q_SIG(mcn)) {
		case Q_ENTRY_SIG:
			mcn->ticks = 0;
			return Q_HANDLED();
		case Q_EXIT_SIG: 
			return Q_HANDLED();
		case LEFT_SIG:
			mcn->ticks = 0;
			return Q_HANDLED();
		case RIGHT_SIG:
			mcn->ticks = 0;
			return Q_HANDLED();
		case TICK_SIG:
			if(mcn->ticks > 30) {	// TODO Make a macro somewhere
				return Q_TRAN(hsm_listen);
			} else {
				mcn->ticks++;
			}
			return Q_HANDLED();
	}
	return Q_SUPER(&hsm_on);
}

// Configure tuning frequency
QState hsm_tun_cfg(hsm *mcn) {
	switch (Q_SIG(mcn)) {
		case LEFT_SIG:
			if(mcn->tuning > 420) {
				mcn->tuning--;
				// TODO Draw bar

			}
		case RIGHT_SIG:
			if(mcn->tuning < 460) {
				mcn->tuning++;
				// TODO Draw bar

			}
	}
	return Q_SUPER(hsm_configure);
}

// Set the octave
QState hsm_oct_cfg(hsm *mcn) {
	switch (Q_SIG(mcn)) {
		case LEFT_SIG:
			if(mcn->octave > 420) {
				mcn->octave--;
				// TODO Draw bar

			}
		case RIGHT_SIG:
			if(mcn->octave < 460) {
				mcn->octave++;
				// TODO Draw bar

			}
	}
	return Q_SUPER(hsm_configure);
}
