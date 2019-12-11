#include "lib/qpn_port.h"
#include "drw.h"
#include "bsp.h"
#include "hsm.h"

static QState hsm_init(hsm *mcn);
static QState hsm_listen(hsm *mcn);
static QState hsm_oct_cfg(hsm *mcn);
static QState hsm_tun_cfg(hsm *mcn);
 
void hsm_ctor(void)  {
	QActive_ctor(&(machine.super_), (QStateHandler)& hsm_init);
	machine.note = "A";
	machine.ticks = 0;
	machine.octave = 4;
	machine.tuning = 440;
	machine.color = 0x000000;
}

QState hsm_init(hsm *mcn) {
	xil_printf("Initializing HSM\r\n");
	return Q_TRAN(&hsm_listen);
}

QState hsm_listen(hsm *mcn) {
	switch (Q_SIG(mcn)) {
		case Q_ENTRY_SIG:
			// TODO Draw on program entry, not every time we go back
			// to this state
			drw_clr(0, 0, LCD_WIDTH, LCD_HEIGHT);
			return Q_HANDLED();
		case Q_EXIT_SIG:
			clr_txt();
			return Q_HANDLED();
		case D_SIG:
			return Q_TRAN(hsm_oct_cfg);
		case C_SIG:
			return Q_TRAN(hsm_tun_cfg);
		case TICK_SIG:
			// Wait 50 ms (simulate FFT)

			// Determine color (no idea how this will turn out)
			mcn->color += 1;
			mcn->color &= 0xFF;
			mcn->color |= ~(mcn->color & 0xFF) << 8;
			mcn->color |= (mcn->color & 0xFF) << 16;

			// Draw colored word (obviously more sensibly than this)
			setColor(mcn->color >> 16 & 0xFF, ~(mcn->color >> 8 & 0xFF), mcn->color & 0xFF);
			drw_txt("A");
			return Q_HANDLED();
	}
	return Q_SUPER(&QHsm_top);
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
			if(mcn->ticks > CFG_TIMEOUT) {
				return Q_TRAN(hsm_listen);
			} else {
				mcn->ticks++;
			}
			return Q_HANDLED();
	}
	return Q_SUPER(&QHsm_top);
}

// Configure tuning frequency
QState hsm_tun_cfg(hsm *mcn) {
	switch (Q_SIG(mcn)) {
		case LEFT_SIG:
			if(mcn->tuning > 420) {
				mcn->tuning--;
				// TODO Draw bar
				xil_printf("%d\n\r", mcn->tuning);
			}
			break;
		case RIGHT_SIG:
			if(mcn->tuning < 460) {
				mcn->tuning++;
				// TODO Draw bar
				xil_printf("%d\n\r", mcn->tuning);
			}
			break;
	}
	return Q_SUPER(hsm_configure);
}

// Set the octave
QState hsm_oct_cfg(hsm *mcn) {
	switch (Q_SIG(mcn)) {
		case Q_ENTRY_SIG:
			// TODO Make sure parent's ENTRY still gets called
			// (parent is hsm_configure)

			// Draws entire octave menu
			drw_oct_sel();
			break;
		case Q_EXIT_SIG:
			clr_oct_sel();
			break;
		case LEFT_SIG:
			if(mcn->octave > 0) {
				mcn->octave--;

				// Delete when done debugging
				xil_printf("%d\n\r", mcn->octave);
				drw_dot(mcn->octave);
			}
			break;
		case RIGHT_SIG:
			if(mcn->octave < 9) {
				mcn->octave++;
				xil_printf("%d\n\r", mcn->octave);
				drw_dot(mcn->octave);
			}
			break;
	}
	return Q_SUPER(hsm_configure);
}
