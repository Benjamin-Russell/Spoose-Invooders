#ifndef FONT_H
#define FONT_H

#include <SDL_ttf.h>

#include <Trackable.h>
#include <string>

using namespace std;

class Font : public Trackable
{
	friend class GraphicsSystem;
public:
	Font(string path, int fontSize = 12);
	Font(){};
	~Font(){ }//TTF_CloseFont(mpSDLFont); };
	void cleanup(){ TTF_CloseFont(mpSDLFont); };

	inline int getFontSize(){ return mFontSize; };
	inline TTF_Font* getFont(){ return mpSDLFont; };

protected:
	TTF_Font* mpSDLFont;
	int mFontSize;
};

#endif
