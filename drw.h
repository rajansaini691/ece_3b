#define LCD_WIDTH 240
#define LCD_HEIGHT 320
void drw_clr(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
#define drw_bkg drw_clr(0, 0, LCD_WIDTH, LCD_HEIGHT);
void drw_tun(uint32_t tun);
#define drw_tun_clr() drw_tun(0)
void clr_tun();
void clr_txt();
void init_tun(uint32_t tun);
void drw_txt(char* text);
void drw_dot(uint8_t oct);
void drw_oct_sel(uint8_t oct);

// top y-coord of the dot row
#define NUM_DOTS 10
#define DOT_Y (3 * LCD_HEIGHT / 4)
#define DOT_HEIGHT (LCD_HEIGHT / 20)
#define DOT_SEL_COL 0xFFFFFF
#define DOT_COL 0x000000	// Unselected color

#define TUN_MIN 420
#define TUN_MAX 460
#define TUN_RANGE (TUN_MAX - TUN_MIN)
#define TUN_X1 30
#define TUN_X2 (LCD_WIDTH - TUN_X1)
#define DELTA_TUN (TUN_X2 - TUN_X1)
#define TUN_WIDTH (DELTA_TUN / TUN_RANGE)
#define TUN_HEIGHT (LCD_HEIGHT / 15)
#define TUN_Y1 DOT_Y
#define TUN_Y2 (TUN_Y1 + TUN_HEIGHT)
#define TUN_COL 0xFFFFFF
#define TUN_SEL_COL 0x000000
