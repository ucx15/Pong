#pragma once


#include "SDL.h"
#include "settings.hpp"


class Puck {
	public:
		float x;
		float y;
		float r;

		float velX;
		float velY;

		uint32_t color;
		uint32_t trail_color;

		Puck();
		Puck(float x, float y, float w, uint32_t color);
		
		void resetTrails();
		void update(float dt);

		void draw(SDL_Surface* surface);


	private:
		int trails_itr;
		float *trails_x;
		float *trails_y;
};
