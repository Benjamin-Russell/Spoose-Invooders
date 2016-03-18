#include "Barrier.h"
#include "Game.h"

Barrier::Barrier(Vector2 location)
{
	mLife = 5;
	mPosition = location;
	mBoundingBox = Rect(mPosition, Vector2(16, 16));
	mSprite = new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Barrier"), 16, 16, Vector2());
}

Barrier::Barrier()
{
	mBoundingBox = Rect(mPosition, Vector2(16, 16));
}

Barrier::~Barrier()
{
	cleanup();
}

void Barrier::cleanup()
{
	delete mSprite;
}

void Barrier::draw(GraphicsSystem *graphics)
{
	mSprite->setSource(Vector2((mLife * 16.0f) - 16.0f, 0.0f));

	graphics->drawSprite(mSprite, mPosition);

	if (Game::getInstance()->getDrawingBoxes())
	{
		graphics->drawSprite(Game::getInstance()->mSpriteBoundingBox_16x16, mPosition);
	}
}