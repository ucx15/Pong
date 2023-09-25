#include "puck.hpp"
#include "utils.hpp"

#include <iostream>


Puck::Puck() {}

Puck::Puck(float x, float y, float w, float h, uint32_t color) {	
	this->w = w;
	this->h = h;
	this->color = color;

	this->x = x;
	this->y = y;

	this->velX = 0.f;
	this->velY = 0.f;

	this->_rect = SDL_Rect{(int)x, (int)y, (int)(w), (int)(h)};

	// trials
	this->trail_color = color;
	this->trails_itr = 0;
	this->trails_x = new float[TRAIL_SIZE];
	this->trails_y = new float[TRAIL_SIZE];
	this->_trail_rect = SDL_Rect{(int)x, (int)y, (int)(w), (int)(h)};
}

void Puck::resetTrails() {
	this->trails_itr = 0;
}

void Puck::update() {
	// Eulers integration
	this->x += this->velX;
	this->y += this->velY;

	this->_rect.x = x;
	this->_rect.y = y;

	if (this->trails_itr < TRAIL_SIZE) {
		this->trails_x[this->trails_itr] = x;
		this->trails_y[this->trails_itr++] = y;
	}

	else {
		shiftLeft2Arrays(this->trails_x, this->trails_y, TRAIL_SIZE);

		this->trails_x[TRAIL_SIZE-1] = x;
		this->trails_y[TRAIL_SIZE-1] = y;
	}
}


void Puck::draw(SDL_Renderer* renderer) {
	uint8_t cR, cG, cB, cA;
	uint32_t tColor;

	float t;

	for (int i=0; i<this->trails_itr; i++) {
		t = (float)i/this->trails_itr;

		this->_trail_rect.x = this->trails_x[i];
		this->_trail_rect.y = this->trails_y[i];

		this->_trail_rect.w = (int) ((this->w)*t);
		this->_trail_rect.h = (int) ((this->h)*t);

		toComponents(this->trail_color, cR, cG, cB, cA);
		fromComponents(tColor, (uint8_t) cR*(t), (uint8_t) cG*(1-t), cB, cA);

		SDL_SetRenderDrawColor(renderer, to8BPC(tColor));
		SDL_RenderFillRect(renderer, &(this->_trail_rect));
	}

	SDL_SetRenderDrawColor(renderer, to8BPC(this->color));
	SDL_RenderFillRect(renderer, &(this->_rect));
}
