
#include "paddle.hpp"
#include "utils.hpp"


Paddle::Paddle() {}

Paddle::Paddle(int x0, int y0, int width, int height, uint32_t c) {

	x = x0;
	y = y0;
	w = width;
	h = height;
	
	color = c;
}


void Paddle::update() {
	x = x;
	y = y;
}

void Paddle::draw(SDL_Surface *surface) {
	fillRect((uint32_t*)surface->pixels, x, y, w, h, color);
}
