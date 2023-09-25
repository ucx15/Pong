#include <iostream>
#include "utils.hpp"


void shiftLeftArray(float* arr, int N) {
	for (int i=0; i<N; i++) {
		arr[i] = arr[i+1];
	}
	arr[N-1] = 0;
}


void shiftLeft2Arrays(float* arr1, float* arr2, int N) {
	for (int i=0; i<N; i++) {
		arr1[i] = arr1[i+1];
		arr2[i] = arr2[i+1];
	}
}

void toComponents(const uint32_t &c, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a) {
	r = (uint8_t) (c >> 24);
	g = (uint8_t) (c >> 16);
	b = (uint8_t) (c >> 8);
	a = (uint8_t) (c & 0x000000ff);
}

void fromComponents(uint32_t &c, const uint8_t &r, const uint8_t &g, const uint8_t &b, const uint8_t &a) {
	c = (uint32_t) ((r<<24) |  (g<<16) | (b<<8) | a);
}
