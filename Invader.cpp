#include "Invader.h"
#include "Game.h"

Invader::Invader()
{
	inUse = false;
	firingFlash = false;
	mWeapon = BASIC_ENEMY;
	mpFlashProjectile = NULL;
	mpBunkerBusterProjectile = NULL;
}

Invader::~Invader()
{
	delete mpWeaponAnimation;

	if (firingFlash)
	{
		delete mpFlashProjectile;
		mpFlashProjectile = NULL;
	}

	delete mpBunkerBusterProjectile;
	mpBunkerBusterProjectile = NULL;
}

void Invader::drawWeapon(GraphicsSystem *graphics)
{
	graphics->drawAnimation(mpWeaponAnimation, Vector2(mPosition.x, mPosition.y));

	// Also draw my Flash
	if (firingFlash)
	{
		mpFlashProjectile->draw(graphics);
	}

	// Also draw my Bunker Buster
	if (mpBunkerBusterProjectile != NULL)
	{
		mpBunkerBusterProjectile->draw(graphics);
	}
}

void Invader::restoreOriginalPosition()
{ 
	mPosition = mOriginalPosition; 
	timeLastFired = Game::getInstance()->getCurrentTime(); 
}

bool Invader::update(const float frameTime)
{
	// Update bounding box
	mBoundingBox = Rect(Vector2((float)mPosition.x + 12, (float)mPosition.y + 16), Vector2(24, 18));

	// Destroy Barriers
	std::vector<Barrier*, std::allocator<Barrier*>> barriers = Game::getInstance()->getPlayer()->getBarriers();

	if (mPosition.y > Game::getInstance()->getPlayer()->BARRIER_Y - 48) 
	{
		for (unsigned int i = 0; i < barriers.size(); ++i)
		{
			if (mBoundingBox.checkCollision(barriers[i]->getBoundingBox()))
			{
				barriers[i]->destroy();
			}
		}
	}

	// Fire event that a wall has been reached
	if (mPosition.x <= 0 || mPosition.x >= Game::getInstance()->getGraphicsSystem()->getWidth() - 48)
	{
		gpEventSystem->fireEvent(INVADER_HIT_WALL);
	}

	// Fire event that player has been hit
	if (mBoundingBox.checkCollision(Game::getInstance()->getPlayer()->getBoundingBox()))
	{
		inUse = false;
		gpEventSystem->fireEvent(INVADER_HIT_PLAYER);
		return true;
	}

	// Update animation
	mpWeaponAnimation->update(frameTime);

	// Weapon specific, Fire a projectile
	switch (mWeapon)
	{
	case BASIC_ENEMY:
		if (timeLastFired + 1.0f <= Game::getInstance()->getCurrentTime() && (rand() % (300 - (Game::getInstance()->getDifficulty() * 40) - (Game::getInstance()->getLevel() * 5)) == 0))
		{
			mpWeaponAnimation->setCurrentSprite(0);
			timeLastFired = Game::getInstance()->getCurrentTime();

			// No sound here is better

			Game::getInstance()->getEnemyManager()->fireProjectile(BASIC_ENEMY, Vector2(mPosition.x + 20, mPosition.y + 40), Vector2(0.0f, 0.0f));
		}
		break;
	case TRIPLE_THREAT:
		if (timeLastFired + 1.0f <= Game::getInstance()->getCurrentTime() && (rand() % ((400 - (Game::getInstance()->getDifficulty() * 50) - (Game::getInstance()->getLevel() * 5))) == 0))
		{
			mpWeaponAnimation->setCurrentSprite(0);
			timeLastFired = Game::getInstance()->getCurrentTime();

			Audio *audio = AudioLocator::locate();
			audio->playSound(MULTI);

			Game::getInstance()->getEnemyManager()->fireProjectile(TRIPLE_THREAT, Vector2(mPosition.x + 20, mPosition.y + 40), Vector2(-1.0f, 0.0f));
			Game::getInstance()->getEnemyManager()->fireProjectile(TRIPLE_THREAT, Vector2(mPosition.x + 20, mPosition.y + 40), Vector2(0.0f, 0.0f));
			Game::getInstance()->getEnemyManager()->fireProjectile(TRIPLE_THREAT, Vector2(mPosition.x + 20, mPosition.y + 40), Vector2(1.0f, 0.0f));
		}
		break;
	case FLASH:
		// Set my Flash Projectile to correct position
		if (firingFlash)
		{
			if (mpFlashProjectile != NULL)
			{
				// Update my Flash
				mpFlashProjectile->setPosition(Vector2(mPosition.x + 20, mPosition.y + 42));

				float timeSinceFired = Game::getInstance()->getCurrentTime() - timeLastFired;

				if (timeSinceFired >= 0.75f && mpWeaponAnimation->getCurrentSprite() == 3)
				{
					if (timeSinceFired < 1.2f)
					{
						// Animate Weapon
						mpWeaponAnimation->setCurrentSprite(0);
					}
					else
					{
						// Finish firing
						firingFlash = false;

						delete mpFlashProjectile;
						mpFlashProjectile = NULL;
					}
				}

			}
			else
			{
				firingFlash = false;
			}
		}
		else if (timeLastFired + 7.0f <= Game::getInstance()->getCurrentTime() && (rand() % (550 - (Game::getInstance()->getDifficulty() * 50) - (Game::getInstance()->getLevel() * 5)) == 0))
		{
			firingFlash = true;
			timeLastFired = Game::getInstance()->getCurrentTime();

			mpFlashProjectile = new Projectile(FLASH, Vector2(mPosition.x + 20, mPosition.y + 40), Vector2(0.0f, 0.0f));
		}
		break;
	case BUNKER_BUSTER:
		if (mpBunkerBusterProjectile != NULL)
		{
			// Set it to the correct position
			mpBunkerBusterProjectile->setPosition(Vector2(mPosition.x + 18.0f, mPosition.y + 33.0f));

			// Drop Bunker Buster
			if (timeLastFired + 3.0f <= Game::getInstance()->getCurrentTime() && rand() % (950 - (Game::getInstance()->getDifficulty() * 100)) == 0)
			{
				// Play sound
				Audio *audio = AudioLocator::locate();
				audio->playSound(FIRE_EXPLOSIVE);

				// Switch sprite
				mpWeaponAnimation->setCurrentSprite(1);

				// Add new bunker buster to Enemy Manager
				Game::getInstance()->getEnemyManager()->fireProjectile(BUNKER_BUSTER, Vector2(mPosition.x + 18.0f, mPosition.y + 33.0f), Vector2(0.0f, 0.05f));

				// Delete my personal one
				delete mpBunkerBusterProjectile;
				mpBunkerBusterProjectile = NULL;
			}
			else if (!mpBunkerBusterProjectile->inUse)
			{
				delete mpBunkerBusterProjectile;
				mpBunkerBusterProjectile = NULL;
			}
		}
		break;
	}

	return false;
}

Invader* Invader::init(float x_, float y_)
{
	inUse = true;
	mPosition.x = x_;
	mPosition.y = y_;
	mOriginalPosition.x = x_;
	mOriginalPosition.y = y_;
	timeLastFired = Game::getInstance()->getCurrentTime();
	mBoundingBox = Rect(Vector2((float)mPosition.x + 12, (float)mPosition.y + 16), Vector2(24, 18));

	if (firingFlash)
	{
		// Should be redundant, but you never know.
		delete mpFlashProjectile;
	}
	mpFlashProjectile = NULL;
	firingFlash = false;

	delete mpBunkerBusterProjectile;
	mpBunkerBusterProjectile = NULL;

	// For resetting
	delete mpWeaponAnimation;

	// Chance to have special weapon
	int random = rand() % 100;

	if (random < 15)
	{
		mWeapon = TRIPLE_THREAT;

		mpWeaponAnimation = new Animation(0.1f, false);

		for (int i = 0; i < 4; ++i)
		{
			mpWeaponAnimation->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Weapon_Triple_Threat"), 48, 48, Vector2((float)i * 48, 0.0f)));
		}
	}
	else if (random < 28)
	{
		mWeapon = FLASH;

		mpWeaponAnimation = new Animation(0.3f, false);

		for (int i = 0; i < 4; ++i)
		{
			mpWeaponAnimation->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Weapon_Flash"), 48, 48, Vector2((float)i * 48, 0.0f)));
		}
	}
	else if (random < 40)
	{
		mWeapon = BUNKER_BUSTER;

		mpWeaponAnimation = new Animation(0.0f, false);
		for (float i = 0; i < 2; ++i)
		{
			mpWeaponAnimation->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Weapon_Bunker_Buster"), 48, 48, Vector2(i * 48.0f, 0.0f)));

		}

		mpBunkerBusterProjectile = new Projectile(BUNKER_BUSTER, Vector2(mPosition.x + 19.0f, mPosition.y + 33.0f), Vector2());
	}
	else
	{
		mWeapon = BASIC_ENEMY;

		mpWeaponAnimation = new Animation(0.2f, false);
		for (int i = 0; i < 4; ++i)
		{
			mpWeaponAnimation->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Weapon_Basic_Enemy"), 48, 48, Vector2((float)i * 48, 0.0f)));
		}
	}
	
	if (mWeapon != BUNKER_BUSTER)
	{
		mpWeaponAnimation->setCurrentSprite(3);
	}

	return this;
}