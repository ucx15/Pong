// ---- Settings ----

# pragma once

#include <cstdint>

#define W 840
#define H 640
#define PADDING 5						// Additional padding b/w Window and playable area
#define FPS 240

constexpr int MSPF = 1000.f / FPS;  	// Milliseconds per frame (Ideal)
#define UPDATE_TIME 2*1000				// in ms

#define ANGLE_CUTOFF .4f				//  cos(a) < ANGLE_CUTOFF won't be a valid; prevents vertical launch
#define PADDLE_SPEEDING_FACTOR 3000.f	//  paddle_vel += PADDLE_VEL * paddle_hits / factor
#define PUCK_SPEEDING_FACTOR .05f		//  puck.vel   -= PUCK_VEL * factor;
#define COLOR_FADING_FACTOR 0.8f		//  dull trail color by this much


#define WIN_SCORE 4
constexpr int TRAIL_SIZE = .8 * FPS;


#define PAD_W 10
#define PAD_H 100 
#define PUCK_RAD 12.f

#define PUCK_VEL 300.f
#define PADDLE_VEL 300.f


#define L_TEXT_POSX  W/10
#define L_TEXT_POSY  H/12

#define R_TEXT_POSX  W - W/10
#define R_TEXT_POSY  H/12


const uint32_t BG_COLOR = 0x0F0F10FF;
const uint32_t FG_COLOR = 0xFFFFFFFF;

#define PUCK_COLOR 0x8F20FDFF
#define PADDLE_COLOR FG_COLOR

#define FONT_COLOR_TITLE 0xFF9060FF
#define FONT_COLOR_TEXT FG_COLOR

#define FONT_SIZE_SCORE 86
#define FONT_SIZE_TITLE 128
#define FONT_SIZE_TEXT 48
