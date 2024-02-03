#include <cmath>
#include "puck.hpp"
#include "utils.hpp"

Puck::Puck() {}

Puck::Puck(float x0, float y0, float width, uint32_t c) {	
	x = x0;
	y = y0;
	r = width;

	color = c;

	velX = 0.f;
	velY = 0.f;

	// trials
	trail_color = color;

	trails_itr = 0;
	trails_x = new float[TRAIL_SIZE];
	trails_y = new float[TRAIL_SIZE];
}

void Puck::resetTrails() {
	trails_itr = 0;
}

void Puck::update() {
	// Eulers integration
	x += velX;
	y += velY;

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


void Puck::draw(SDL_Surface* surface) {
	uint8_t cR, cG, cB, cA;
	uint32_t tColor;
	uint32_t* drawBuffer = (uint32_t*)surface->pixels;
	float t;
	int tx, ty, tr;

	for (int i=0; i<trails_itr; i++) {
		t = (float)i/trails_itr;
		// t = sqrtf(t);    // Easing function
		t = t*t;    // Easing function

		tx = trails_x[i];
		ty = trails_y[i];
		tr = roundf(r*t);

		toComponents(trail_color, cR, cG, cB, cA);
		fromComponents(tColor, (uint8_t) cR*(t), (uint8_t) cG*(1-t), cB, cA);

		drawCircle(drawBuffer, tx, ty, tr, tColor);
	}

	drawCircle(drawBuffer, int(x), int(y), PUCK_RAD, color);
}
