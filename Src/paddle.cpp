
#include "paddle.hpp"
#include "utils.hpp"


Paddle::Paddle() {}

Paddle::Paddle(int x, int y, int w, int h, uint32_t color) {
	this->x = x;
	this->y = y;
	
	this->color = color;
	this->_rect = SDL_Rect{(int)x, (int)y, (int)w, (int)h};
}


void Paddle::update() {
	this->_rect.x = this->x;
	this->_rect.y = this->y;
}

void Paddle::draw(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, to8BPC(color));
	SDL_RenderFillRect(renderer, &(_rect));
}
