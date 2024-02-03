# pragma once

 #include <string>

#include "SDL.h"
#include "SDL_ttf.h"

class Text {

	// Attributes
	private:
		TTF_Font *_font;
		SDL_Surface *_text_surface;
		SDL_Rect _text_rect;
		SDL_Color _color;


	// Methods
	public:
		Text();
		Text(const char* font_path, int font_size, SDL_Color color);

		void update(std::string text);
		void display(SDL_Surface* surface,int x, int y);
		
		float Width();
		float Height();
};
