// #include <iostream>

#include "puck.hpp"
#include "utils.hpp"

Puck::Puck() {}

Puck::Puck(float x0, float y0, float width, float height, uint32_t c) {	
	w = width;
	h = height;
	color = c;

	x = x0;
	y = y0;

	velX = 0.f;
	velY = 0.f;

	_rect = SDL_Rect{(int)x, (int)y, (int)(w), (int)(h)};

	// trials
	trail_color = color;

	trails_itr = 0;
	trails_x = new float[TRAIL_SIZE];
	trails_y = new float[TRAIL_SIZE];
	_trail_rect = SDL_Rect{(int)x, (int)y, (int)(w), (int)(h)};
}

void Puck::resetTrails() {
	trails_itr = 0;
}

void Puck::update() {
	// Eulers integration
	x += velX;
	y += velY;

	_rect.x = x;
	_rect.y = y;

	if (trails_itr < TRAIL_SIZE) {
		trails_x[trails_itr] = x;
		trails_y[trails_itr++] = y;
	}

	else {
		shiftLeft2Arrays(trails_x, trails_y, TRAIL_SIZE);

		trails_x[TRAIL_SIZE-1] = x;
		trails_y[TRAIL_SIZE-1] = y;
	}
}


void Puck::drawCircle(SDL_Renderer* renderer, SDL_Rect &rect) {
    int xSt = rect.x-rect.w;
    int xEn = rect.x+rect.h;
    int ySt = rect.y-rect.w;
    int yEn = rect.y+rect.h;

    for (int py = ySt; py < yEn; py++){
        for (int px = xSt; px < xEn; px++) {
            
            if ( (sq(px-rect.x) + sq(py-rect.y)) < sq(PUCK_RAD) ) {
                SDL_RenderDrawPoint(renderer, px, py);
            }
        }
    }
}


void Puck::draw(SDL_Renderer* renderer) {
	uint8_t cR, cG, cB, cA;
	uint32_t tColor;
	float t;

 
	for (int i=0; i<trails_itr; i++) {
		t = (float)i/trails_itr;

		_trail_rect.x = trails_x[i];
		_trail_rect.y = trails_y[i];
		_trail_rect.w = (int)(w)*t;
		_trail_rect.h = (int)(h)*t;

		toComponents(trail_color, cR, cG, cB, cA);
		fromComponents(tColor, (uint8_t) cR*(t), (uint8_t) cG*(1-t), cB, cA);
	
		SDL_SetRenderDrawColor(renderer, to8BPC(tColor));
		this->drawCircle(renderer, _trail_rect);
	}

	SDL_SetRenderDrawColor(renderer, to8BPC(color));
	this->drawCircle(renderer, _rect);
}
