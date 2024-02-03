#include <iostream>
#include "settings.hpp"
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



void drawRect(uint32_t *buffer, int x, int y, int w, int h, uint32_t color) {
    int xSt = std::min(W, std::max(0, x));
    int xEn = std::min(W, std::max(0, x+w));
    int ySt = std::min(H, std::max(0, y));
    int yEn = std::min(H, std::max(0, y+h));

    for (int i=xSt; i<xEn; i++) {
        buffer[y*W + i] = color;
        buffer[(y+h-1)*W + i] = color;
    }

    for (int i=ySt; i<yEn; i++) {
        buffer[i*W + x] = color;
        buffer[i*W + (x+w-1)] = color;
    }

}

void fillRect(uint32_t *buffer, int x, int y, int w, int h, uint32_t color) {
    int xSt = std::min(W, std::max(0, x));
    int xEn = std::min(W, std::max(0, x+w));
    int ySt = std::min(H, std::max(0, y));
    int yEn = std::min(H, std::max(0, y+h));

    for (int py=ySt; py<yEn; py++){
        for (int px=xSt; px<xEn; px++) {
            buffer[py*W + px] = color;
        }
    }	
}


void drawCircle(uint32_t *buffer, int x, int y, int r, uint32_t color) {
    int xSt = std::min(W, std::max(0, (x-r) ));
    int xEn = std::min(W, std::max(0, (x+r) ));
    int ySt = std::min(H, std::max(0, (y-r) ));
    int yEn = std::min(H, std::max(0, (y+r) ));

    for (int py=ySt; py<yEn; py++){
        for (int px=xSt; px<xEn; px++) {
            if ( (sq(px-x) + sq(py-y)) < sq(r) ) {
				buffer[py*W + px] = color;
            }
        }
    }
}

void fillCircle(uint32_t *buffer, int x, int y, int r, uint32_t color) {
    int xSt = std::min(W, std::max(0, (x-r) ));
    int xEn = std::min(W, std::max(0, (x+r) ));
    int ySt = std::min(H, std::max(0, (y-r) ));
    int yEn = std::min(H, std::max(0, (y+r) ));

    for (int py=ySt; py<yEn; py++){
        for (int px=xSt; px<xEn; px++) {
            if ( (sq(px-x) + sq(py-y)) < sq(r) ) {
				buffer[py*W + px] = color;
            }
        }
    }
}
