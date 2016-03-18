#include "GraphicsSystem.h"

GraphicsSystem::GraphicsSystem()
{
	mDisplay = SDL_SetVideoMode(1920, 1080, 32, SDL_SWSURFACE | SDL_DOUBLEBUF);
	mBackBuffer = SDL_GetVideoSurface();
	fullScreen = false;

	if (!mDisplay)
	{
		std::cout << "Error creating display: SDL_SetVideoMode() failed" << std::endl;
	}
}

GraphicsSystem::GraphicsSystem(int width, int height)
{
	mDisplay = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE | SDL_DOUBLEBUF);
	mBackBuffer = SDL_GetVideoSurface();
	fullScreen = false;

	if (!mDisplay)
	{
		std::cout << "Error creating display: SDL_SetVideoMode() failed" << std::endl;
	}
}

GraphicsSystem::~GraphicsSystem()
{
	cleanUp();
}

void GraphicsSystem::cleanUp()
{
	// SDL_QUIT takes care of mDisplay, no need to free it
	SDL_Quit();
}

void GraphicsSystem::clear(Color color)
{
	SDL_FillRect(mDisplay, NULL, color.getHex());
}

void GraphicsSystem::clear(GraphicsBuffer& buffer, Color color)
{
	SDL_FillRect(buffer.getSurface(), NULL, color.getHex());
}

void GraphicsSystem::drawBuffer(GraphicsBuffer* buffer, Vector2 location)
{
	SDL_Rect dest;
	dest.x = (int)location.x;
	dest.y = (int)location.y;

	SDL_BlitSurface(buffer->getSurface(), NULL, mDisplay, &dest);
}

void GraphicsSystem::drawSprite(Sprite* sprite, Vector2 location)
{
	SDL_Rect dest;
	dest.x = (int)location.x;
	dest.y = (int)location.y;

	SDL_Rect source;
	source.x = (int)sprite->getSource().x;
	source.y = (int)sprite->getSource().y;
	source.w = sprite->getWidth();
	source.h = sprite->getHeight();

	SDL_BlitSurface(sprite->getBuffer()->getSurface(), &source, mDisplay, &dest);
}

void GraphicsSystem::drawAnimation(Animation* ani, Vector2 location)
{
	if (ani->getNumSprites() > 0)
	{
		SDL_Rect dest;
		dest.x = (int)location.x;
		dest.y = (int)location.y;

		SDL_Rect source;
		source.x = (int)ani->getSprite(ani->getCurrentSprite())->getSource().x;
		source.y = (int)ani->getSprite(ani->getCurrentSprite())->getSource().y;
		source.w = ani->getSprite(ani->getCurrentSprite())->getWidth();
		source.h = ani->getSprite(ani->getCurrentSprite())->getHeight();

		SDL_BlitSurface(ani->getSprite(ani->getCurrentSprite())->getBuffer()->getSurface(), &source, mDisplay, &dest);
	}
}

void GraphicsSystem::toggleFullscreen()
{
	if (fullScreen)
	{
		SDL_SetVideoMode(1920, 1080, 32, SDL_SWSURFACE | SDL_DOUBLEBUF);

		fullScreen = false;
	}
	else
	{
		SDL_SetVideoMode(1920, 1080, 32, SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);

		fullScreen = true;
	}
}

void GraphicsSystem::writeText(Vector2 location, Font font, Color color, string text)
{
	SDL_Rect dest;
	dest.x = (int)location.x;
	dest.y = (int)location.y;

	SDL_Surface* tmpSurface = NULL;
	tmpSurface = TTF_RenderText_Solid(font.getFont(), text.c_str(), color.getSDLColor());

	SDL_BlitSurface(tmpSurface, NULL, mDisplay, &dest);
}

int initSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	// Set the window caption
	SDL_WM_SetCaption("WAIzards", NULL);

	return 0;
}