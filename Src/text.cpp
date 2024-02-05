#include <iostream>

#include "text.hpp"

Text::Text() {};

Text::Text(const char* font_path, int font_size, SDL_Color color) {
	_color = color;
	_font = TTF_OpenFont(font_path, font_size);
}


void Text::update(std::string msg) {
	_text_surface = TTF_RenderText_Solid(_font, msg.c_str(), _color);
	_text_rect.w = _text_surface->w;
	_text_rect.h = _text_surface->h;
}


void Text::draw(SDL_Surface* surface, int x, int y) {
	_text_rect.x = x;
	_text_rect.y = y;
	SDL_BlitSurface(_text_surface, NULL, surface, &_text_rect);
}

float Text::Width() {
	return _text_rect.w;
}

float Text::Height() {
	return _text_rect.h;
}