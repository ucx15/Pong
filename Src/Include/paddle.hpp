#pragma once

#include <cstdint>

#include "SDL.h"
#include "settings.hpp"


class Paddle {
	public:
		float x;
		float y;
		float w;
		float h;
		uint32_t color;

		Paddle();
		Paddle(int x, int y, int w, int h, uint32_t color);	

		void update();
		void draw(SDL_Renderer* renderer);

	private:
		SDL_Rect _rect;
};
