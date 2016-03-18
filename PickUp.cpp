#include "PickUp.h"
#include "Game.h"

PickUp::PickUp(ProjectileType type, Vector2 position)
{
	mPosition = position;
	mBoundingBox = Rect(Vector2(mPosition.x, mPosition.y), Vector2(16.0f, 16.0f));
	mType = type;

	switch (mType)
	{
	case MULTI_SHOT:
		mSprite = new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Weapon_Triple_Threat"), 16, 16, Vector2(160.0f, 27.0f));
		break;
	case LASER:
		mSprite = new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Weapon_Flash"), 16, 16, Vector2(160.0f, 27.0f));
		break;
	case EXPLOSIVE:
		mSprite = new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Weapon_Bunker_Buster"), 20, 16, Vector2(14.0f, 26.0f));
		break;
	}

	mLifeTime = 15.0f;
	mTimeCreated = Game::getInstance()->getCurrentTime();
	mOnGround = false;
}

PickUp::~PickUp()
{
	delete mSprite;
}

void PickUp::destroy()
{
	// Set time created to a time to be destroyed.
	mTimeCreated = Game::getInstance()->getCurrentTime() - mLifeTime - 1.0f;
}

void PickUp::update()
{
	if (!mOnGround)
	{
		if (mPosition.y < Game::getInstance()->getGraphicsSystem()->getHeight() - 16)
		{
			mPosition.y += 4;

			// Update bounding box
			mBoundingBox = Rect(Vector2(mPosition.x, mPosition.y), Vector2(16.0f, 16.0f));

			if (mPosition.y >= Game::getInstance()->getGraphicsSystem()->getHeight() - 16)
			{
				mOnGround = true;

				// Switch sprite
				delete mSprite;

				switch (mType)
				{
				case MULTI_SHOT:
					mSprite = new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Weapon_Multi_Shot"), 16, 16, Vector2(104.0f, 4.0f));
					break;
				case LASER:
					mSprite = new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Weapon_Laser"), 16, 16, Vector2(104.0f, 3.0f));
					break;
				case EXPLOSIVE:
					mSprite = new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Weapon_Explosive"), 20, 16, Vector2(6.0f, 2.0f));
					break;
				}
			}
		}
	}

	// Check for barriers
	if (!mOnGround)
	{

		std::vector<Barrier*, std::allocator<Barrier*>> barriers = Game::getInstance()->getPlayer()->getBarriers();
		bool hitBarrier = false;

		for (unsigned int i = 0; !hitBarrier && i < barriers.size(); ++i)
		{
			while (mBoundingBox.checkCollision(barriers[i]->getBoundingBox()))
			{
				// Move above barrier
				--mPosition.y;
				--mBoundingBox.mPosition.y;
				hitBarrier = true;
			}
		}
	}
}

void PickUp::draw(GraphicsSystem* graphics)
{
	float timeAlive = Game::getInstance()->getCurrentTime() - mTimeCreated;

	// Flash during last 5 seconds
	if (timeAlive <= mLifeTime - 5.0f || (int)(timeAlive * 10.0f) % 10 < 5)
	{
		graphics->drawSprite(mSprite, mPosition);
	}

	if (Game::getInstance()->getDrawingBoxes())
	{
		if (mType == EXPLOSIVE)
		{
			graphics->drawSprite(Game::getInstance()->mSpriteBoundingBox_16x16, Vector2(mPosition.x + 2, mPosition.y));
		}
		else
		{
			graphics->drawSprite(Game::getInstance()->mSpriteBoundingBox_16x16, Vector2(mPosition.x, mPosition.y));
		}
	}
}

bool PickUp::getAlive()
{
	return (Game::getInstance()->getCurrentTime() - mTimeCreated < mLifeTime);
}