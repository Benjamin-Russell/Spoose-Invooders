#include "Projectile.h"
#include "Game.h"

Projectile::Projectile(ProjectileType type, Vector2 position, Vector2 addedVelocity)
{
	mPosition = position;
	mBoundingBox = Rect(Vector2((float)position.x, (float)position.y), Vector2(8, 8)); // By default
	mType = type;
	killedPlayer = 0;
	mTimeCreated = Game::getInstance()->getCurrentTime();
	mFlashIsLethal = false;
	inUse = true;
	isTrash = false;

	// Define initialization characteristics based on projectile type
	switch (mType)
	{
		case BASIC:
			mpAnimation = new Animation(0.13f, true);
			mVelocity = Vector2(0.0f, -4.0f);

			// Add sprites
			for (int i = 0; i < 4; ++i)
			{
				mpAnimation->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Projectile_Player1"), 8, 8, Vector2((float)(i * 8), 0.0f)));
			}
			break;
		case MULTI_SHOT:
			mpAnimation = new Animation(0.13f, true);
			mVelocity = Vector2(0.0f, -4.0f);

			// Add sprites
			for (int i = 0; i < 4; ++i)
			{
				mpAnimation->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Projectile_Multi_Shot"), 8, 8, Vector2((float)(i * 8), 0.0f)));
			}
			break;
		case LASER:
			mpAnimation = new Animation(0.04f, true);
			mVelocity = Vector2(0.0f, 0.0f);

			// Add sprites
			for (int i = 0; i < 4; ++i)
			{
				mpAnimation->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Projectile_Flash"), 8, 480, Vector2((float)(i * 8), 0.0f)));
			}
			break;
		case EXPLOSIVE:
			mpAnimation = new Animation(0.12f, true);
			mVelocity = Vector2(0.0f, 0.0f);
			mBoundingBox = Rect(Vector2((float)position.x, (float)position.y), Vector2(12, 12));

			// Add sprites
			for (int i = 0; i < 4; ++i)
			{
				mpAnimation->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Projectile_Explosive"), 12, 12, Vector2((float)(i * 12), 0.0f)));
			}
			break;
		case BASIC_ENEMY:
			mpAnimation = new Animation(0.13f, true);
			mVelocity = Vector2(0.0f, 4.0f);

			// Add sprites
			for (int i = 0; i < 4; ++i)
			{
				mpAnimation->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Projectile_Basic_Enemy"), 8, 8, Vector2((float)(i * 8), 0.0f)));
			}
			break;
		case TRIPLE_THREAT:
			mpAnimation = new Animation(0.13f, true);
			mVelocity = Vector2(0.0f, 4.0f);

			// Add sprites
			for (int i = 0; i < 4; ++i)
			{
				mpAnimation->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Projectile_Multi_Shot"), 8, 8, Vector2((float)(i * 8), 0.0f)));
			}
			break;
		case FLASH:
			mpAnimation = new Animation(0.0f, true); // Don't bother animating 1 frame
			mVelocity = Vector2(0.0f, 0.0f);
			mBoundingBox = Rect(Vector2((float)position.x + 2, (float)position.y), Vector2(4, 480));

			mpAnimation->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Projectile_Flash_Charge"), 8, 480, Vector2((float)0.0f, 0.0f)));
			break;
		case BUNKER_BUSTER:
			mpAnimation = new Animation(0.12f, true);
			mVelocity = Vector2(0.0f, 0.0f);
			mBoundingBox = Rect(Vector2((float)position.x, (float)position.y), Vector2(12, 12));

			// Add sprites
			for (int i = 0; i < 4; ++i)
			{
				mpAnimation->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Projectile_Explosive"), 12, 12, Vector2((float)(i * 12), 0.0f)));
			}
			break;
	}

	// Add added velocity (used for multishot projectiles, which need to be given separate velocities. Also, if we want to give them player's momentum.)
	mVelocity.x += addedVelocity.x;
	mVelocity.y += addedVelocity.y;

	
}

Projectile::Projectile(Projectile* toCopy)
{
	mPosition = toCopy->getPosition();
	mVelocity = toCopy->getVelocity();
	mBoundingBox = Rect(Vector2((float)mPosition.x, (float)mPosition.y), Vector2(8, 8)); // By default
	mType = toCopy->getType();
	killedPlayer = 0;
	mTimeCreated = Game::getInstance()->getCurrentTime();
	mFlashIsLethal = toCopy->mFlashIsLethal;

	// Define initialization characteristics based on projectile type
	switch (mType)
	{
	case BASIC:
		mpAnimation = new Animation(0.13f, true);
		mVelocity = Vector2(0.0f, -4.0f);

		// Add sprites
		for (int i = 0; i < 4; ++i)
		{
			mpAnimation->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Projectile_Player1"), 8, 8, Vector2((float)(i * 8), 0.0f)));
		}
		break;
	case MULTI_SHOT:
		mpAnimation = new Animation(0.13f, true);
		mVelocity = Vector2(0.0f, -4.0f);

		// Add sprites
		for (int i = 0; i < 4; ++i)
		{
			mpAnimation->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Projectile_Multi_Shot"), 8, 8, Vector2((float)(i * 8), 0.0f)));
		}
		break;
	case LASER:
		mpAnimation = new Animation(0.04f, true);
		mVelocity = Vector2(0.0f, 0.0f);

		// Add sprites
		for (int i = 0; i < 4; ++i)
		{
			mpAnimation->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Projectile_Flash"), 8, 480, Vector2((float)(i * 8), 0.0f)));
		}
		break;
	case BASIC_ENEMY:
		mpAnimation = new Animation(0.13f, true);
		mVelocity = Vector2(0.0f, 4.0f);

		// Add sprites
		for (int i = 0; i < 4; ++i)
		{
			mpAnimation->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Projectile_Basic_Enemy"), 8, 8, Vector2((float)(i * 8), 0.0f)));
		}
		break;
	case TRIPLE_THREAT:
		mpAnimation = new Animation(0.13f, true);
		mVelocity = Vector2(0.0f, 4.0f);

		// Add sprites
		for (int i = 0; i < 4; ++i)
		{
			mpAnimation->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Projectile_Multi_Shot"), 8, 8, Vector2((float)(i * 8), 0.0f)));
		}
		break;
	case FLASH:
		mpAnimation = new Animation(0.0f, true); // Don't bother animating 1 frame
		mVelocity = Vector2(0.0f, 0.0f);
		mBoundingBox = Rect(Vector2((float)mPosition.x + 2, (float)mPosition.y), Vector2(4, 480));

		mpAnimation->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Projectile_Flash_Charge"), 8, 480, Vector2((float)0.0f, 0.0f)));
		break;
	case BUNKER_BUSTER:
		mpAnimation = new Animation(0.12f, true);
		mVelocity = Vector2(0.0f, 0.0f);
		mBoundingBox = Rect(Vector2((float)mPosition.x, (float)mPosition.y), Vector2(12, 12));

		// Add sprites
		for (int i = 0; i < 4; ++i)
		{
			mpAnimation->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Projectile_Explosive"), 12, 12, Vector2((float)(i * 12), 0.0f)));
		}
		break;
	}

	inUse = toCopy->inUse;
	isTrash = toCopy->isTrash;
	cout << isTrash << endl;
}

Projectile::~Projectile()
{
	cleanUp();
}

void Projectile::cleanUp()
{
	delete mpAnimation;
}

void Projectile::update(const float frameTime)
{
	// If in use still
	if (!isTrash)
	{
		// Move
		mPosition.x += mVelocity.x;
		mPosition.y += mVelocity.y;

		// Outside level
		if (mType != FLASH && mType != LASER)
		{
			if (mPosition.x <= -8.0f || mPosition.x > Game::getInstance()->getGraphicsSystem()->getWidth()
				|| mPosition.y <= -8.0f || mPosition.y > Game::getInstance()->getGraphicsSystem()->getHeight())
			{
				isTrash = true;
			}
		}

		// Gravity
		if (mType == BUNKER_BUSTER || mType == EXPLOSIVE)
		{
			if (mVelocity.y != 0.0f)
			{
				mVelocity.y += 0.1f;

				if (mVelocity.y == 0.0f) // Don't revert to zero-gravity state
					mVelocity.y += 0.01f;
			}
		}

		// Update my bounding box
		switch (mType)
		{
		case LASER:
			mBoundingBox = Rect(Vector2((float)mPosition.x + 2, (float)mPosition.y), Vector2(4, 480));
			break;
		case FLASH:
			mBoundingBox = Rect(Vector2((float)mPosition.x + 2, (float)mPosition.y), Vector2(4, 480));
			break;
		case EXPLOSIVE:
			mBoundingBox = Rect(Vector2((float)mPosition.x, (float)mPosition.y), Vector2(12, 12));
			break;
		case BUNKER_BUSTER:
			mBoundingBox = Rect(Vector2((float)mPosition.x, (float)mPosition.y), Vector2(12, 12));
			break;
		default:
			mBoundingBox = Rect(Vector2((float)mPosition.x, (float)mPosition.y), Vector2(8, 8));
			break;
		}

		// Flash turn lethal after 1 second
		if (mType == FLASH && !mFlashIsLethal && Game::getInstance()->getCurrentTime() - mTimeCreated >= 1.0f)
		{
			mFlashIsLethal = true;

			Audio *audio = AudioLocator::locate();
			audio->playSound(FIRE_LASER);

			delete mpAnimation;
			mpAnimation = new Animation(0.1f, true);

			// Add sprites
			for (int i = 0; i < 4; i++)
			{
				mpAnimation->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Projectile_Flash"), 8, 480, Vector2((float)(i * 8), 0.0f)));
			}
		}

		// Laser finish off
		if (mType == LASER && Game::getInstance()->getPlayer()->getLaserWeaponAnimation()->getCurrentSprite() == 3)
		{
			isTrash = true;
			inUse = false;
		}

		// Check collisions
		std::vector<Projectile*, std::allocator<Projectile*>> otherProjectiles;

		// Barriers
		if (mType != FLASH && mType != LASER)
		{
			std::vector<Barrier*, std::allocator<Barrier*>> barriers = Game::getInstance()->getPlayer()->getBarriers();

			for (unsigned int i = 0; !isTrash && i < barriers.size(); ++i)
			{
				if (mBoundingBox.checkCollision(barriers[i]->getBoundingBox()))
				{
					barriers[i]->subtractLife();
					isTrash = true;

					// If the player is destroying barrier, kill it faster
					if (mType < ENEMY_THRESHOLD)
					{
						barriers[i]->subtractLife();
					}
				}
			}
		}

		if (mType < ENEMY_THRESHOLD)
		{
			std::vector<Invader*, std::allocator<Invader*>> invaders = Game::getInstance()->getEnemyManager()->getInvaders();

			if (mType != LASER)
			{
				// Check for collisions with enemy projectiles
				otherProjectiles = Game::getInstance()->getEnemyManager()->getProjectiles();

				for (unsigned int i = 0; !isTrash && i < otherProjectiles.size(); ++i)
				{
					if (!otherProjectiles[i]->isTrash && otherProjectiles[i]->inUse && otherProjectiles[i]->getType() != FLASH && otherProjectiles[i] != this && mBoundingBox.checkCollision(otherProjectiles[i]->mBoundingBox))
					{
						otherProjectiles[i]->isTrash = true;
						isTrash = true;

						if (otherProjectiles[i]->getType() != BUNKER_BUSTER)
						{
							Game::getInstance()->increaseScoreBy(10);
						}
						else
						{
							Game::getInstance()->increaseScoreBy(50);
						}
					}
				}

				// Check for collisions with invaders' personal projectiles
				for (unsigned int i = 0; !isTrash && i < invaders.size(); ++i)
				{
					if (invaders[i]->getBunkerBuster() != NULL)
					{
						if (mBoundingBox.checkCollision(invaders[i]->getBunkerBuster()->getBoundingBox()))
						{
							invaders[i]->getBunkerBuster()->isTrash = true;
							Game::getInstance()->increaseScoreBy(50);
							isTrash = true;
						}
					}
				}
			}

			// Check for collisions with invaders
			if (!isTrash)
			{
				for (unsigned int i = 0; !isTrash && i < invaders.size(); ++i)
				{
					if (mBoundingBox.checkCollision(invaders[i]->getBoundingBox()) && invaders[i]->getInUse())
					{
						if (mType == EXPLOSIVE && invaders[i]->getWeapon() == BUNKER_BUSTER && invaders[i]->getBunkerBuster() == NULL)
						{
							// Catch it
							isTrash = true;
							inUse = false;

							invaders[i]->setBunkerBuster(new Projectile(BUNKER_BUSTER, Vector2(invaders[i]->getPosition().x + 19.0f, invaders[i]->getPosition().y + 33.0f), Vector2()));
						}
						else
						{
							// Add PickUp
							switch (invaders[i]->getWeapon())
							{
							case TRIPLE_THREAT:
								Game::getInstance()->getPlayer()->addPickUp(MULTI_SHOT, Vector2(invaders[i]->getPosition().x + 16, invaders[i]->getPosition().y + 27));
								break;
							case FLASH:
								Game::getInstance()->getPlayer()->addPickUp(LASER, Vector2(invaders[i]->getPosition().x + 16, invaders[i]->getPosition().y + 27));
								break;
							case BUNKER_BUSTER:
								Game::getInstance()->getPlayer()->addPickUp(EXPLOSIVE, Vector2(invaders[i]->getPosition().x + 16, invaders[i]->getPosition().y + 27));

								// If its bunker buster exists, give it to the Enemy Manager
								if (invaders[i]->getBunkerBuster() != NULL)
								{
									Game::getInstance()->getEnemyManager()->fireProjectile(BUNKER_BUSTER, invaders[i]->getBunkerBuster()->getPosition(), Vector2(0.0f, 0.1f));

									delete invaders[i]->getBunkerBuster();
									invaders[i]->setBunkerBuster(NULL);
								}
								break;
							}

							Game::getInstance()->getEnemyManager()->fireExplosion(Vector2(invaders[i]->getPosition().x - 12.0f, invaders[i]->getPosition().y - 12.0f), 0.5f);

							invaders[i]->setInUse(false);
							Game::getInstance()->increaseScoreBy(100);

							if (mType != LASER)
							{
								isTrash = true;
							}
						}
					}
				}
			}

			// Check for player to be caught again
			if (!isTrash && mType == EXPLOSIVE && Game::getInstance()->getPlayer()->getProjectileExplosive() == this)
			{
				if (mBoundingBox.checkCollision(Game::getInstance()->getPlayer()->getBoundingBox()))
				{
					mVelocity = Vector2(0.0f, 0.0f);
					Game::getInstance()->getPlayer()->getAnimationExplosive()->setCurrentSprite(0);
					Game::getInstance()->getPlayer()->setAmmo(1);

					// Play sound
					Audio *audio = AudioLocator::locate();
					audio->playSound(FIRE_EXPLOSIVE);
				}
			}
		}
		else // mType > ENEMY_THRESHOLD
		{			
			// Check for collision with players
			if (!isTrash && !Game::getInstance()->getPlayer()->isInvincible())
			{
				if (mType != FLASH || mFlashIsLethal)
				{
					if (mBoundingBox.checkCollision(Game::getInstance()->getPlayer()->getBoundingBox()))
					{
						if ((mType != BUNKER_BUSTER) || (Game::getInstance()->getPlayer()->getCurrentWeapon() != EXPLOSIVE || (Game::getInstance()->getPlayer()->getProjectileExplosive() != NULL && Game::getInstance()->getPlayer()->getProjectileExplosive()->getVelocity().y == 0.0f)))
						{
							isTrash = true;
							killedPlayer = 1; // Could be 2 for player 2
						}
						else
						{
							// Get caught

							// If player has a personal in the air, replace it in the enemy vector
							if (Game::getInstance()->getPlayer()->getProjectileExplosive() != NULL)
							{
								Game::getInstance()->getEnemyManager()->fireProjectile(EXPLOSIVE, Game::getInstance()->getPlayer()->getProjectileExplosive()->getPosition(), Game::getInstance()->getPlayer()->getProjectileExplosive()->getVelocity());
								delete Game::getInstance()->getPlayer()->getProjectileExplosive();
							}
							
							Game::getInstance()->getPlayer()->getAnimationExplosive()->setCurrentSprite(0);
							Game::getInstance()->getPlayer()->setAmmo(1);
							inUse = false;
							Game::getInstance()->getPlayer()->setProjectileExplosive(new Projectile(EXPLOSIVE, Vector2(), Vector2()));

							// Play sound
							Audio *audio = AudioLocator::locate();
							audio->playSound(FIRE_EXPLOSIVE);
						}
					}
				}
			}
		}

		// Animation
		mpAnimation->update(frameTime);
	}
	// Use last update to do something based on type
	else
	{
		inUse = false;

		switch (mType)
		{
		case EXPLOSIVE:
			Game::getInstance()->getEnemyManager()->fireExplosion(Vector2(mPosition.x - 66.0f, mPosition.y - 66.0f), 1.0f);
			break;
		case BUNKER_BUSTER:
			Game::getInstance()->getEnemyManager()->fireExplosion(Vector2(mPosition.x - 66.0f, mPosition.y - 66.0f), 1.0f);
			break;
		}
	}
}

void Projectile::draw(GraphicsSystem *graphics)
{
	if (!isTrash)
	{
		graphics->drawAnimation(mpAnimation, mPosition);

		if (Game::getInstance()->getDrawingBoxes())
		{
			if (mType != FLASH || mFlashIsLethal)
			{
				switch (mType)
				{
				case LASER:
					graphics->drawSprite(Game::getInstance()->mSpriteBoundingBox_6x480, Vector2(mPosition.x + 1, mPosition.y));
					break;
				case FLASH:
					graphics->drawSprite(Game::getInstance()->mSpriteBoundingBox_6x480, Vector2(mPosition.x + 1, mPosition.y));
					break;
				case EXPLOSIVE:
					graphics->drawSprite(Game::getInstance()->mSpriteBoundingBox_12x12, Vector2(mPosition.x, mPosition.y));
					break;
				case BUNKER_BUSTER:
					graphics->drawSprite(Game::getInstance()->mSpriteBoundingBox_12x12, Vector2(mPosition.x, mPosition.y));
					break;
				default:
					graphics->drawSprite(Game::getInstance()->mSpriteBoundingBox_8x8, Vector2(mPosition.x, mPosition.y));
					break;
				}
			}
		}
	}
}