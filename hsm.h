#ifndef hsm_h
#define hsm_h
void hsm_ctor(void);
// 2 seconds  * ticks per second
#define CFG_TIMEOUT 2
#include "fft/note.h"


enum hsm_sig {
	LEFT_SIG = Q_USER_SIG,
	RIGHT_SIG,
	LEFT_BTN_SIG,
	RIGHT_BTN_SIG,
	A_SIG,
	B_SIG,
	C_SIG,
	D_SIG,
	E_SIG,
	TICK_SIG,
	FFT_SIG
};

typedef struct hsmTag {               //Lab2A State machine
	QActive super_; // inheritance from QP
	uint32_t disp_tmr; // display timer
	uint16_t ticks;
	uint8_t octave;
	uint32_t tuning;
	uint32_t color;
	int cents;	// -100 - 100
	int note; // 0 - 11
} hsm;

hsm machine;
#endif  
