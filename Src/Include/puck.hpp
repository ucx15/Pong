#pragma once


#include "SDL.h"
#include "settings.hpp"


class Puck {
	public:
		float x;
		float y;

		float velX;
		float velY;

		float w;
		float h;
		uint32_t color;
		uint32_t trail_color;

		Puck();
		Puck(float x, float y, float w, float h, uint32_t color);
		
		void resetTrails();
		void update();
		void draw(SDL_Renderer* renderer);


	private:
		SDL_Rect _rect;
		SDL_Rect _trail_rect;
		
		int trails_itr;
		float *trails_x;
		float *trails_y;
};
