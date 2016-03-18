// Original Author: Kyle Netherland
// Used with permission by Steven Ulfelder

#include "Color.h"
#include <string>
#include <sstream>

using namespace std;

void Color::setColor(unsigned int R, unsigned int G, unsigned int B, unsigned int A)
{
	this->R = R;
	this->G = G;
	this->B = B;
	if (A > 255)
		this->A = A;
	else
		this->A = 255;
}

bool Color::equals(unsigned R, unsigned G, unsigned B)
{
	if (R == this->R && G == this->G && B == this->B)
		return true;

	return false;
}

unsigned Color::getHex()
{
	int rgbVal = ((this->R & 0xff) << 16) | ((this->G & 0xff) << 8) | (this->B & 0xff);
	static string hexDigits = "0123456789ABCDEF";
	string rgbStr = "";
	for (int i = (3 * 2) - 1; i >= 0; i--)
		rgbStr += hexDigits[((rgbVal >> i * 4) & 0xF)];
	
	if (rgbStr == "000000")
		return (unsigned)000000;

	unsigned toReturn;
	istringstream iss(rgbStr);
	iss >> hex >> toReturn;

	return toReturn;
}

SDL_Color Color::getSDLColor()
{
	SDL_Color tmpColor = { this->R, this->G, this->B };

	return tmpColor;
}