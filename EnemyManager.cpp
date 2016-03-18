#include "EnemyManager.h"
#include "Game.h"

EnemyManager::EnemyManager()
{
	// Populate object pool, reserving memory
	for (int i = 0; i < INVADER_ROWS * INVADER_COLUMNS; ++i)
	{
		mInvaderPool.push_back(new Invader());
	}

	// Init Invader animation
	mInvaderAnimation = new Animation(0.15f, false);

	for (int i = 0; i < 4; ++i)
	{
		mInvaderAnimation->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Invader"), 48, 48, Vector2((float)i * 48.0f, 0.0f)));
	}

	lastAnimatedInvaders = 0.0f;
	mMustSwitchDirection = false;
	mInvaderDirection = 1;
	mInvaderSpeed = 2;
}

EnemyManager::~EnemyManager()
{
	cleanup();
}

void EnemyManager::startPlay()
{
	mInvaderDirection = 1;
	mInvaderSpeed = 2;
	lastAnimatedInvaders = Game::getInstance()->getCurrentTime();

	for (int i = 0; i < INVADER_ROWS; ++i)
	{
		for (int j = 0; j < INVADER_COLUMNS; ++j)
		{
			bool foundSuitable = false;
			int k = 0;

			// Find invaders in object pool that aren't in use
			while (!foundSuitable)
			{
				if (mInvaderPool[k]->getInUse())
				{
					++k;
				}
				else
				{
					mInvaders.push_back(mInvaderPool[k]->init((float)48 + (64 * j), (float)48 + (48 * i)));
					foundSuitable = true;
				}
			}
		}
	}
}

void EnemyManager::endPlay()
{
	// Clear invaders from "active" list, and set them all to "not in use"
	mInvaders.clear();

	for (unsigned int i = 0; i < mInvaderPool.size(); i++)
	{
		mInvaderPool[i]->setInUse(false);
	}

	for (unsigned int i = 0; i < mProjectiles.size(); i++)
	{
		delete mProjectiles[i];
	}

	for (unsigned int i = 0; i < mExplosions.size(); i++)
	{
		delete mExplosions[i];
	}

	mProjectiles.clear();
	mExplosions.clear();
}

void EnemyManager::cleanup()
{
	for (unsigned int i = 0; i < mInvaderPool.size(); i++)
	{
		delete mInvaderPool[i];
	}

	for (unsigned int i = 0; i < mProjectiles.size(); i++)
	{
		delete mProjectiles[i];
	}

	for (unsigned int i = 0; i < mExplosions.size(); i++)
	{
		delete mExplosions[i];
	}

	mInvaderPool.clear();
	mProjectiles.clear();
	mExplosions.clear();
	mInvaders.clear();

	delete mInvaderAnimation;
}

Projectile* EnemyManager::fireProjectile(ProjectileType type, Vector2 position, Vector2 addedVelocity)
{
	Projectile* Proj = new Projectile(type, position, addedVelocity);
	mProjectiles.push_back(Proj);

	return Proj;
}

void EnemyManager::update(const float frameTime)
{
	// Start play if no invaders are found
	if (Game::getInstance()->getState() == PLAY && mInvaders.size() == 0)
	{
		Game::getInstance()->getPlayer()->reset();
		Game::getInstance()->getPlayer()->resetBarriers();
		Game::getInstance()->setLevel(Game::getInstance()->getLevel() + 1);

		endPlay();
		startPlay();
	}
	
	// Set Invader speed
	mInvaderSpeed = (float)((float)INVADER_MAX / (float)mInvaders.size() * Game::getInstance()->getDifficulty() * 0.5f);

	// Invader Animation
	if (Game::getInstance()->getCurrentTime() - lastAnimatedInvaders > 2.0f)
	{
		mInvaderAnimation->setCurrentSprite(0);
		lastAnimatedInvaders += 1.0f;
	}

	mInvaderAnimation->update(frameTime);

	// Switch direction
	if (mMustSwitchDirection)
	{
		switchDirections();
		mMustSwitchDirection = false;
	}

	bool playRestarted = false;

	// Update enemy projectiles
	for (unsigned int i = 0; !playRestarted && i < mProjectiles.size();)
	{
		if (mProjectiles[i]->inUse)
		{
			mProjectiles[i]->update(frameTime);
			++i;
		}
		else
		{
			// Signal to quit updating projectiles because play needs to be restarted
			if (mProjectiles[i]->getKilledPlayer() == 1)
			{
				playRestarted = true;
			}
			else
			{
				delete mProjectiles[i];
				mProjectiles.erase(mProjectiles.begin() + i);
			}
		}
	}

	// Update personal Projectiles
	for (unsigned int i = 0; !playRestarted && i < mInvaders.size();)
	{
		if (mInvaders[i]->getWeapon() == FLASH && mInvaders[i]->getFiringFlash())
		{
			mInvaders[i]->getFlash()->update(frameTime);

			if (mInvaders[i]->getFlash()->isTrash && mInvaders[i]->getFlash()->getKilledPlayer() == 1)
			{
				playRestarted = true;
			}
		}

		if (mInvaders[i]->getWeapon() == BUNKER_BUSTER && mInvaders[i]->getBunkerBuster() != NULL)
		{
			mInvaders[i]->getBunkerBuster()->update(frameTime);

			// A bunker buster does not itself kill the player. Its explosion does. So no need to check here for playerKilled
		}

		++i;
	}

	// Update Explosions
	for (unsigned int i = 0; !playRestarted && i < mExplosions.size();)
	{
		if (!mExplosions[i]->isTrash)
		{
			mExplosions[i]->update(frameTime);

			if (mExplosions[i]->getKilledPlayer() == 1)
			{
				playRestarted = true;
			}
		}
		else
		{
			delete mExplosions[i];
			mExplosions.erase(mExplosions.begin() + i);
		}

		++i;
	}

	// This event must be handled outside of projectile update or the projectile mid-update will be deleted and break game
	if (playRestarted)
	{
		gpEventSystem->fireEvent(PLAYERS_DEAD);
	}

	// If invader positions are restarted, updating their list must stop so that they are in sync
	playRestarted = false;

	for (unsigned int i = 0; i < mInvaders.size() && !playRestarted;)
	{
		if (mInvaders[i]->getInUse())
		{
			mInvaders[i]->setX(mInvaders[i]->getX() + (mInvaderSpeed * (float)mInvaderDirection));
			playRestarted = mInvaders[i]->update(frameTime);
			++i;
		}
		else
		{
			// Remove laser projectile if it was firing
			if (mInvaders[i]->getFiringFlash())
			{
				delete mInvaders[i]->getFlash();
				mInvaders[i]->setFiringFlash(false);
			}

			// Remove from list, but do not delete
			mInvaders.erase(mInvaders.begin() + i);
		}
	}
}

void EnemyManager::updateDeath(const float frameTime)
{
	// Delete Explosions
	for (unsigned int i = 0; i < mExplosions.size(); ++i)
	{
		delete mExplosions[i];
		mExplosions.erase(mExplosions.begin() + i);
	}
}

void EnemyManager::draw(GraphicsSystem *graphics)
{
	// Draw projectiles
	for (unsigned int i = 0; i < mProjectiles.size(); ++i)
	{
		mProjectiles[i]->draw(graphics);
	}

	// Draw invader animation at all invader locations, plus their weapon animations
	for (unsigned int i = 0; i < mInvaders.size(); ++i)
	{
		graphics->drawAnimation(mInvaderAnimation, Vector2((float)mInvaders[i]->getX(), (float)mInvaders[i]->getY()));
		mInvaders[i]->drawWeapon(graphics);

		if (Game::getInstance()->getDrawingBoxes())
		{
			graphics->drawSprite(Game::getInstance()->mSpriteBoundingBox_24x18, Vector2((float)mInvaders[i]->getX() + 12, (float)mInvaders[i]->getY() + 16));
		}
	}

	// Draw Explosions
	for (unsigned int i = 0; i < mExplosions.size(); ++i)
	{
		mExplosions[i]->draw(graphics);
	}
}

void EnemyManager::fireExplosion(Vector2 position, float scale)
{
	mExplosions.push_back(new Explosion(position, scale));
}

void EnemyManager::setMustSwitchDirections( bool val )
{
	mMustSwitchDirection = val;
}

void EnemyManager::switchDirections()
{
	mInvaderDirection *= -1;

	for (unsigned int i = 0; i < mInvaders.size(); ++i)
	{
		mInvaders[i]->setY(mInvaders[i]->getY() + 12);
	}
}

void EnemyManager::resetPositions()
{
	for (unsigned int i = 0; i < mInvaders.size(); i++)
	{
		if (mInvaders[i]->getFiringFlash())
		{
			mInvaders[i]->setFiringFlash(false);
			delete mInvaders[i]->getFlash();
		}
	}

	for (unsigned int i = 0; i < mProjectiles.size(); i++)
	{
		delete mProjectiles[i];
	}
	mProjectiles.clear();

	mInvaderDirection = 1;
	Game::getInstance()->getPlayer()->subtractLife();
}