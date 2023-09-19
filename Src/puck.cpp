#include "puck.hpp"
#include "utils.hpp"


Puck::Puck() {}

Puck::Puck(float x, float y, float w, float h, uint32_t color) {
	this->x = x;
	this->y = y;

	this->velX = 0.f;
	this->velY = 0.f;

	this->w = w;
	this->h = h;

	this->color = color;
	this->_rect = SDL_Rect{(int)x, (int)y, (int)(w), (int)(h)};
}


void Puck::update() {
	// Eulers integration
	this->x += this->velX;
	this->y += this->velY;

	this->_rect.x = x;
	this->_rect.y = y;
}

void Puck::draw(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, to8BPC(this->color));
	SDL_RenderFillRect(renderer, &(this->_rect));
}