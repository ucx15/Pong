
#include "paddle.hpp"
#include "utils.hpp"


Paddle::Paddle() {}

Paddle::Paddle(int x0, int y0, int width, int height, uint32_t c) {

	x = x0;
	y = y0;
	
	color = c;
	_rect = SDL_Rect{x0, y0, width, height};
}


void Paddle::update() {
	_rect.x = x;
	_rect.y = y;
}

void Paddle::draw(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, to8BPC(color));
	SDL_RenderFillRect(renderer, &_rect);
}
