# pragma once

// ---- Macros ----
#define sq(x) ((x)*(x))

#define to8BPC(x) ((x & 0xff000000) >> 24), ((x & 0x00ff0000) >> 16), ((x & 0x0000ff00) >> 8), (x & 0x000000ff)


void shiftLeftArray(float* arr, int N);
void shiftLeft2Arrays(float* arr1, float* arr2, int N);

void toComponents(const uint32_t &c, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a);
void fromComponents(uint32_t &c, const uint8_t &r, const uint8_t &g, const uint8_t &b, const uint8_t &a);