#include <iostream>

#include "text.hpp"

Text::Text() {};

Text::Text(SDL_Renderer* renderer, const char* font_path, int font_size, SDL_Color color) {
	_renderer = renderer;
	_color = color;
	_font = TTF_OpenFont(font_path, font_size);
}


void Text::update(SDL_Renderer* renderer, std::string msg) {
	auto text_surface = TTF_RenderText_Solid(_font, msg.c_str(), _color);
	_text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_FreeSurface(text_surface);

	SDL_QueryTexture(_text_texture, nullptr, nullptr, &_text_rect.w, &_text_rect.h);
}


void Text::display(SDL_Renderer* renderer, int x, int y) {
	_text_rect.x = x;
	_text_rect.y = y;

	SDL_RenderCopy(renderer, _text_texture, nullptr, &_text_rect);
}

float Text::Width() {
	return _text_rect.w;
}

float Text::Height() {
	return _text_rect.h;
}