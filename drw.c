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

void drw_txt(char* text) {
	setFont(BigFont);
	lcdPrint(text, LCD_WIDTH / 2 - 8, LCD_HEIGHT / 2 - 8);
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

static uint8_t prev_vol = 0;
#define vol2pix(vol) (vol * (DELTA_VOL) / 0xFF + VOL_X1)

void clr_vol(uint8_t vol) {
	drw_clr(VOL_X1, VOL_Y1, vol2pix(vol), VOL_Y2);
}

void init_vol(uint8_t vol) {
	prev_vol = 0;
	drw_vol(vol);
}

void drw_vol(uint8_t vol) {
	if(vol == prev_vol) return;
	setColor((VOL_COL >> 16) & 0xFF, (VOL_COL >> 8) & 0xFF, VOL_COL & 0xFF);
	if(vol > prev_vol) {
		fillRect(vol2pix(prev_vol), VOL_Y1, vol2pix(vol), VOL_Y2);
	} else {
		drw_clr(vol2pix(vol), VOL_Y1, vol2pix(prev_vol), VOL_Y2);
	}
	prev_vol = vol;
}

// x-coord of left side
#define oct2pix(oct) (oct + 1) * (LCD_WIDTH - DOT_HEIGHT * NUM_DOTS) / (NUM_DOTS + 1) + oct*DOT_HEIGHT

// Avoids redundant redrawing
static uint8_t prev_oct = 0;

// Updates a single dot; assumes octave is 0-indexed
void drw_dot(uint8_t oct) {
	if(oct == prev_oct) return;
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
void drw_oct_sel() {
	setColor((DOT_COL >> 16) & 0xFF, (DOT_COL >> 8) & 0xFF, DOT_COL & 0xFF);
	for(int i = 0; i < NUM_DOTS; i++) {
		fillRect(oct2pix(i), DOT_Y, oct2pix(i) + DOT_HEIGHT, DOT_Y + DOT_HEIGHT);
	}
}

void clr_oct_sel() {
	for(int i = 0; i < NUM_DOTS; i++) {
		drw_clr(oct2pix(i), DOT_Y, oct2pix(i) + DOT_HEIGHT, DOT_Y + DOT_HEIGHT);
	}
}
