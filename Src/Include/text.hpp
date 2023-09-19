# pragma once

 #include <string>

#include "SDL.h"
#include "SDL_ttf.h"

class Text {

	public:
		Text();
		Text(SDL_Renderer* renderer, const char* font_path, int font_size, SDL_Color color);

		void update(SDL_Renderer* renderer, std::string text);
		void display(SDL_Renderer* renderer,int x, int y);
		
		float Width();
		float Height();


	private:
		TTF_Font* _font;
		SDL_Texture* _text_texture = nullptr;
		SDL_Rect _text_rect;
		SDL_Color _color;
		SDL_Renderer* _renderer;
};
