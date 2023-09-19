// ---- Settings ----

# pragma once

#define W 840
#define H 640

#define FPS 240
constexpr int MSPF = 1000.f / FPS;  // Milliseconds per frame (Ideal)


#define PAD_W 10
#define PAD_H 100 

#define PADDING 10

#define PUCK_RAD 10.f
#define PUCK_VEL 3.f
#define PADDLE_VEL 2.f


#define L_TEXT_POSX  W/10
#define L_TEXT_POSY  H/12

#define R_TEXT_POSX  W - W/10
#define R_TEXT_POSY  H/12

#define WIN_SCORE 5

#define BG_COLOR 0x0F0F10FF
#define FG_COLOR 0xFFFFFFFF

#define PUCK_COLOR 0xFA2010FF
#define PADDLE_COLOR FG_COLOR

#define FONT_COLOR_TITLE 0xFF9060FF
#define FONT_COLOR_TEXT FG_COLOR

#define FONT_SIZE_SCORE 72
#define FONT_SIZE_TITLE 128
#define FONT_SIZE_TEXT 48
