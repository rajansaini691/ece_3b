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
	u32 bg_color = 0x000000;
	setColor((bg_color >> 16) & 0xFF, (bg_color >> 8) & 0xFF, bg_color & 0xFF);
	fillRect(x1, y1, x2, y2);

	// Draw squares
	setColor(0x00, 0x00, 0x00);
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
	setColor((DOT_BDR >> 16) & 0xFF, (DOT_BDR >> 8) & 0xFF, DOT_BDR & 0xFF);
	fillRect(oct2pix(prev_oct), DOT_Y, oct2pix(prev_oct) + DOT_HEIGHT, DOT_Y + DOT_HEIGHT);
	setColor((DOT_COL >> 16) & 0xFF, (DOT_COL >> 8) & 0xFF, DOT_COL & 0xFF);
	fillRect(oct2pix(prev_oct) + 2, DOT_Y + 2, oct2pix(prev_oct) + DOT_HEIGHT - 2, DOT_Y + DOT_HEIGHT - 2);

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

void drw_bar() {
	setColor((BAR_COL >> 16) & 0xFF, (BAR_COL >> 8) & 0xFF, BAR_COL & 0xFF);
	fillRect(BAR_X1, BAR_Y, BAR_X2, BAR_Y + 3);
}


#define cents2pix((cents + 100) * LCD_WIDTH / BAR_RANGE + (LCD_WIDTH - BAR_RANGE) / 2
int16_t prev_cents = 0;
void drw_dial(int16_t cents) {
	clr_pix(cents2pix(prev_cents), BAR_Y - DIAL_HEIGHT / 2, cents2pix(prev_cents) + DIAL_HEIGHT, BAR_Y + DIAL_HEIGHT / 2);
	setColor((BAR_COL >> 16) & 0xFF, (BAR_COL >> 8) & 0xFF, BAR_COL & 0xFF);
	fillRect(cents2pix(prev_cents), BAR_Y, cents2pix(prev_cents) + DIAL_HEIGHT, BAR_Y + 3);
	fillRect(cents2pix(cents), BAR_Y - DIAL_HEIGHT / 2, cents2pix(cents) + DIAL_HEIGHT, BAR_Y + DIAL_HEIGHT / 2);
}

void clr_bar() {
	clr_pix(BAR_X1, BAR_Y, BAR_X2, BAR_Y + 3);
}

void drw_decoration() {
	setColor(0xFF, 0xFF, 0xFF);
	fillRect(LCD_WIDTH / 6, LCD_HEIGHT / 5, 5 * LCD_WIDTH / 6, LCD_HEIGHT / 5 + 10);
}
