// Original Author: Kyle Netherland
// Used with permission by Steven Ulfelder

#ifndef COLOR_H
#define COLOR_H

#include <Trackable.h>
#include <SDL.h>

class Color : public Trackable
{
public:
	Color() {};
	unsigned int R, G, B, A;

	Color(unsigned int R, unsigned int G, unsigned int B, unsigned int A = 255){ setColor(R, G, B, A); };
	void setColor(unsigned int R, unsigned int G, unsigned int B, unsigned int A = 255);
	bool equals(unsigned R, unsigned G, unsigned B);
	unsigned getHex();
	SDL_Color getSDLColor();
};

#endif