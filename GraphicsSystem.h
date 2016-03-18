#ifndef _GRAPHICS_SYSTEM_H_
#define _GRAPHICS_SYSTEM_H_

#include "Animation.h"
#include "Color.h"
#include "Font.h"

#include <Trackable.h>

int initSDL();

class GraphicsSystem : public Trackable
{
public:
	GraphicsSystem(int width, int height);
	GraphicsSystem();
	~GraphicsSystem();

	void drawBuffer(GraphicsBuffer* buffer, Vector2 location);
	void drawSprite(Sprite* sprite, Vector2 location);
	void drawAnimation(Animation* ani, Vector2 location);
	void writeText(Vector2 location, Font font, Color color, string text);
	void toggleFullscreen();
	void cleanUp();
	void clear(Color color);
	void clear(GraphicsBuffer &buffer, Color color);

	inline int getWidth() { return mDisplay->w; };
	inline int getHeight() { return mDisplay->h; };
	inline void flipDisplay() { SDL_Flip(mBackBuffer); };

private:
	SDL_Surface* mDisplay;
	SDL_Surface* mBackBuffer;

	bool fullScreen;
};

#endif