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

#define NOTEY ((LCD_HEIGHT/2) - 2 * BigFont[1])
#define NOTEX ((LCD_WIDTH/2))
void drw_note(char* note, uint16_t hz, int cents) {
	setFont(BigFont);
	setColor(0xFF, 0xFF, 0xFF);
	setColorBg(0x00, 0x00, 0x00);
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
	xil_printf("CENTC %c", centc[0]);
	lcdPrint(hzc, NOTEX - 8 * BigFont[0], NOTEY + 2 * BigFont[1]);
	lcdPrint(centc, NOTEX + BigFont[0], NOTEY + 2 * BigFont[1]);
}

void clr_txt(void) {
	drw_clr(LCD_WIDTH / 2 - 16, LCD_HEIGHT / 2 - 16, LCD_WIDTH / 2 + 16, LCD_HEIGHT / 2 + 16);
}

void drw_clr(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	u32 bg_color = 0x333333;
	setColor((bg_color >> 16) & 0xFF, (bg_color >> 8) & 0xFF, bg_color & 0xFF);
	fillRect(x1, y1, x2, y2);

	// Draw squares
	setColor(127, 127, 127);
	for(int y = 5; y + 30 < LCD_HEIGHT; y += 40) {
		for(int x = 5; x + 30 < LCD_WIDTH; x += 40) {
			if(((y + 30) > y1) && (y < y2) && (x < x2) && ((x + 30) > x1)) {
				fillRect(max(x, x1), y, min(x + 30, x2), y + 30);
			}
		}
	}
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
	setColor((TUN_COL >> 16) & 0xFF, (TUN_COL >> 8) & 0xFF, TUN_COL & 0xFF);
	fillRect(TUN_X1, TUN_Y1, TUN_X2, TUN_Y2);
	drw_tun(tun);
}

void drw_tun(uint32_t tun) {
	setColor((TUN_COL >> 16) & 0xFF, (TUN_COL >> 8) & 0xFF, TUN_COL & 0xFF);
	fillRect(tun2pix(prev_tun), TUN_Y1, tun2pix(prev_tun) + TUN_WIDTH, TUN_Y2);

	setColor((TUN_SEL_COL >> 16) & 0xFF, (TUN_SEL_COL >> 8) & 0xFF, TUN_SEL_COL & 0xFF);
	fillRect(tun2pix(tun), TUN_Y1, tun2pix(tun) + TUN_WIDTH, TUN_Y2);

	setFont(BigFont);
	// x_size: BigFont[0]
	// y_size: BigFont[1]
	char ntext[4];
	ntext[0] = ((tun % (1000)) / 100) + '0';
	ntext[1] = ((tun % (100)) / 10) + '0';
	ntext[2] = ((tun % (10))) + '0';
	ntext[3] = '\0';
	setColorBg((TUN_COL >> 16) & 0xFF, (TUN_COL >> 8) & 0xFF, TUN_COL & 0xFF);
	setColor(0x7F, 0x7F, 0x7F);
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
	setColor((DOT_COL >> 16) & 0xFF, (DOT_COL >> 8) & 0xFF, DOT_COL & 0xFF);
	fillRect(oct2pix(prev_oct), DOT_Y, oct2pix(prev_oct) + DOT_HEIGHT, DOT_Y + DOT_HEIGHT);

	// Mark current dot as selected
	setColor((DOT_SEL_COL >> 16) & 0xFF, (DOT_SEL_COL >> 8) & 0xFF, DOT_SEL_COL & 0xFF);
	fillRect(oct2pix(oct), DOT_Y, oct2pix(oct) + DOT_HEIGHT, DOT_Y + DOT_HEIGHT);

	prev_oct = oct;
}

// Draws the entire octave-select menu (a row of 10 dots)
void drw_oct_sel(uint8_t oct) {
	setColor((DOT_COL >> 16) & 0xFF, (DOT_COL >> 8) & 0xFF, DOT_COL & 0xFF);
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
