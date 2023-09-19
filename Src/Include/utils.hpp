# pragma once


// ---- Macros ----
#define sq(x) ((x)*(x))

#define to8BPC(x) ((x & 0xff000000) >> 24), ((x & 0x00ff0000) >> 16), ((x & 0x0000ff00) >> 8), (x & 0x000000ff)
