// ---- Settings ----

# pragma once

#define W 840
#define H 640

#define FPS 240
constexpr int MSPF = 1000.f / FPS;  // Milliseconds per frame (Ideal)
#define UPDATE_TIME 2*1000  // in ms


#define PAD_W 10
#define PAD_H 100 

#define PADDING 5

#define PUCK_RAD 10.f
#define PUCK_VEL 1.f
#define PADDLE_VEL 2.f


#define L_TEXT_POSX  W/10
#define L_TEXT_POSY  H/12

#define R_TEXT_POSX  W - W/10
#define R_TEXT_POSY  H/12

#define WIN_SCORE 5

const uint32_t BG_COLOR = 0x0F0F10FF;
const uint32_t FG_COLOR = 0xFFFFFFFF;

// #define PUCK_COLOR 0xFA2010FF
#define PUCK_COLOR 0xAA20F0FF
#define PADDLE_COLOR FG_COLOR

#define FONT_COLOR_TITLE 0xFF9060FF
#define FONT_COLOR_TEXT FG_COLOR

#define FONT_SIZE_SCORE 86
#define FONT_SIZE_TITLE 128
#define FONT_SIZE_TEXT 48

constexpr int TRAIL_SIZE = .5*FPS;
