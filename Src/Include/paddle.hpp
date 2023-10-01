#pragma once

#include <cstdint>

#include "SDL.h"
#include "settings.hpp"


class Paddle {
	public:
		float x;
		float y;
		uint32_t color;

		Paddle();
		Paddle(int x0, int y0, int width, int height, uint32_t c);	

		void update();
		void draw(SDL_Renderer* renderer);

	private:
		SDL_Rect _rect;
};
