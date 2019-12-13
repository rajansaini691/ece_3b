#include "lib/lcd.h"
#include "drw.h"
#define set_pix(x, y) fillRect(x, y, x, y)

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })
#define BG_COL 0x07, 0x36, 0x42
#define FG_COL 0x83, 0x94, 0x96
#define HL_COL 0xd3, 0x36, 0x82

#define NOTEY ((LCD_HEIGHT/2) - 2 * BigFont[1])
#define NOTEX ((LCD_WIDTH/2))
void drw_note(char* note, uint16_t hz, int cents) {
	setFont(BigFont);
	setColor(FG_COL);
	setColorBg(BG_COL);
	note[3] = '\0';
	lcdPrint(note, NOTEX, NOTEY);
	char hzc[9] = "12345 Hz";
	char centc[6] = " 12 c";
	int mod = 1;
	for(int i = 4; i >= 0; i--) {
		hzc[i] = ((hz % (10 * mod)) / mod) + '0';
		mod *= 10;
	}
	if(cents < 0) {
		centc[0] = '-';
		cents = -cents;
	}
	centc[1] = ((cents % 100)/10) + '0';
	centc[2] = (cents % 10) + '0';
	hzc[8] = '\0';
	lcdPrint(hzc, NOTEX - 8 * BigFont[0], NOTEY + 2 * BigFont[1]);
	centc[5] = '\0';
	lcdPrint(centc, NOTEX + BigFont[0], NOTEY + 2 * BigFont[1]);
}

void clr_txt(void) {
	drw_clr(LCD_WIDTH / 2 - 16, LCD_HEIGHT / 2 - 16, LCD_WIDTH / 2 + 16, LCD_HEIGHT / 2 + 16);
}

void drw_clr(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	setColor(BG_COL);
	fillRect(x1, y1, x2, y2);

}

static uint32_t prev_tun = 420;
//#define tun2pix(tun) ((tun - TUN_MIN) * (DELTA_TUN) / TUN_RANGE + TUN_X1)
uint32_t tun2pix(uint32_t tun) {
	return ((tun - TUN_MIN) * (DELTA_TUN) / TUN_RANGE + TUN_X1);
}

void clr_tun() {
	drw_clr(TUN_X1, TUN_Y1, TUN_X2 + TUN_WIDTH, TUN_Y2);
}

void init_tun(uint32_t tun) {
	setColor(FG_COL);
	fillRect(TUN_X1, TUN_Y1, TUN_X2, TUN_Y2);
	drw_tun(tun);
}

void drw_tun(uint32_t tun) {
	setColor(FG_COL);
	fillRect(tun2pix(prev_tun), TUN_Y1, tun2pix(prev_tun) + TUN_WIDTH, TUN_Y2);

	setColor(HL_COL);
	fillRect(tun2pix(tun), TUN_Y1, tun2pix(tun) + TUN_WIDTH, TUN_Y2);

	setFont(BigFont);
	// x_size: BigFont[0]
	// y_size: BigFont[1]
	char ntext[4];
	ntext[0] = ((tun % (1000)) / 100) + '0';
	ntext[1] = ((tun % (100)) / 10) + '0';
	ntext[2] = ((tun % (10))) + '0';
	ntext[3] = '\0';
	setColorBg(FG_COL);
	setColor(0x58, 0x6e, 0x75);

	lcdPrint(ntext, 120 - BigFont[0] - BigFont[0]/2, TUN_Y2 - (TUN_HEIGHT -BigFont[1])/2 - BigFont[1]);

	//xil_printf("%d\n\r", tun2pix(tun));
	prev_tun = tun;
}

// x-coord of left side
#define oct2pix(oct) (oct + 1) * (LCD_WIDTH - DOT_HEIGHT * NUM_DOTS) / (NUM_DOTS + 1) + oct*DOT_HEIGHT

// Avoids redundant redrawing
static uint8_t prev_oct = 0;

// Updates a single dot; assumes octave is 0-indexed
void drw_dot(uint8_t oct) {
	uint8_t x = oct2pix(oct);

	// Reset last dot
	setColor(FG_COL);
	fillRect(oct2pix(prev_oct), DOT_Y, oct2pix(prev_oct) + DOT_HEIGHT, DOT_Y + DOT_HEIGHT);

	// Mark current dot as selected
	setColor(HL_COL);
	fillRect(oct2pix(oct), DOT_Y, oct2pix(oct) + DOT_HEIGHT, DOT_Y + DOT_HEIGHT);

	prev_oct = oct;
}

// Draws the entire octave-select menu (a row of 10 dots)
void drw_oct_sel(uint8_t oct) {
	setColor(FG_COL);
	for(int i = 0; i < NUM_DOTS; i++) {
		fillRect(oct2pix(i), DOT_Y, oct2pix(i) + DOT_HEIGHT, DOT_Y + DOT_HEIGHT);
	}
	drw_dot(oct);
}

void clr_oct_sel() {
	for(int i = 0; i < NUM_DOTS; i++) {
		drw_clr(oct2pix(i), DOT_Y, oct2pix(i) + DOT_HEIGHT, DOT_Y + DOT_HEIGHT);
	}
}
