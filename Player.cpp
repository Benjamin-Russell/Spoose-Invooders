#include "Player.h"
#include "Game.h"

Player::Player(bool isNew)
{
	// Animations
	mpAnimation = new Animation(0.0f, true);
	for (int i = 0; i < 8; ++i)
	{
		mpAnimation->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Player"), 64, 64, Vector2((float)(i * 64), 0.0f)));
	}

	mpAnimationBasic = new Animation(0.1f, false);
	for (int i = 0; i < 4; ++i)
	{
		mpAnimationBasic->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Weapon_Basic"), 32, 18, Vector2((float)(i * 32), 0.0f)));
	}
	mpAnimationBasic->setCurrentSprite(3);

	mpAnimationMulti_Shot = new Animation(0.1f, false);
	for (int i = 0; i < 4; ++i)
	{
		mpAnimationMulti_Shot->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Weapon_Multi_Shot"), 32, 18, Vector2((float)(i * 32), 0.0f)));
	}
	mpAnimationMulti_Shot->setCurrentSprite(3);

	mpAnimationLaser = new Animation(0.07f, false);
	for (int i = 0; i < 4; ++i)
	{
		mpAnimationLaser->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Weapon_Laser"), 32, 18, Vector2((float)(i * 32), 0.0f)));
	}
	mpAnimationLaser->setCurrentSprite(3);

	mpAnimationExplosive = new Animation(0.00f, false);
	for (int i = 0; i < 2; ++i)
	{
		mpAnimationExplosive->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Weapon_Explosive"), 32, 18, Vector2((float)(i * 32), 0.0f)));
	}
	mpAnimationExplosive->setCurrentSprite(0);

	mpSpriteDestroyed = new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Player_Destroyed"), 64, 64, Vector2());

	spawnBarriers();

	mPosition.x = (float)(Game::getInstance()->getGraphicsSystem()->getWidth() / 2 - 32);
	mPosition.y = (float)(Game::getInstance()->getGraphicsSystem()->getHeight() - 64);

	mBoundingBox = Rect(Vector2((float)mPosition.x + 16, (float)mPosition.y + 20), Vector2(32, 20));

	mAmmo = 0;
	mCurrentWeapon = BASIC;
	mpProjectileLaser = NULL;
	firingLaser = false;
	timeLastFired = 0.0f;
	toMove = 0;
	mMoveSpeed = 2;
	mLives = 3;
}

Player::Player()
{
	toMove = 0;
	firingLaser = false;
	mpProjectileLaser = NULL;
	mBoundingBox = Rect(Vector2((float)mPosition.x + 16, (float)mPosition.y + 20), Vector2(32, 20));
}

Player::~Player()
{
	cleanup();
}

void Player::cheatWeapon(int num)
{
	mCurrentWeapon = (ProjectileType)num;

	switch (num)
	{
	case 0:
		mAmmo = 0;
		break;
	case 3:
		mAmmo = 1;
		break;
	default:
		mAmmo = 3;
		break;
	}

	firingLaser = false;

	if (mCurrentWeapon == EXPLOSIVE)
	{
		if (mpProjectileExplosive != NULL && mpProjectileExplosive->getVelocity().y != 0)
		{
			// Replace the already existing one in the enemy projectile vector
			Game::getInstance()->getEnemyManager()->fireProjectile(EXPLOSIVE, mpProjectileExplosive->getPosition(), mpProjectileExplosive->getVelocity());
			delete mpProjectileExplosive;
		}

		mpProjectileExplosive = new Projectile(EXPLOSIVE, Vector2(mPosition.x + 26.0f, mPosition.y), Vector2());

		mpAnimationExplosive->setCurrentSprite(0);
		mAmmo = 1;
	}
	else
	{
		// If switching weapon and I still have an explosive held
		if (mpProjectileExplosive != NULL && mpProjectileExplosive->getVelocity().y == 0)
		{
			delete mpProjectileExplosive;
			mpProjectileExplosive = NULL;
		}
	}

	Audio *audio = AudioLocator::locate();
	audio->playSound(PICKUP);
	std::cout << "Cheat Used: Weapon Added\n";
}

void Player::reset()
{
	// Does not include resetting player position

	for (unsigned int i = 0; i < mProjectiles.size(); ++i)
	{
		delete mProjectiles[i];
	}
	mProjectiles.clear();

	for (unsigned int i = 0; i < mPickUps.size(); ++i)
	{
		delete mPickUps[i];
	}
	mPickUps.clear();

	firingLaser = false;
	delete mpProjectileLaser;
	mpProjectileLaser = NULL;
	delete mpProjectileExplosive;
	mpProjectileExplosive = NULL;

	toMove = 0;
}

void Player::addPickUp(ProjectileType type, Vector2 position)
{
	mPickUps.push_back(new PickUp(type, position));
}

void Player::resetBarriers()
{
	for (unsigned int i = 0; i < mBarriers.size(); ++i)
	{
		delete mBarriers[i];
	}
	mBarriers.clear();

	spawnBarriers();
}

void Player::resetWeapon()
{
	mAmmo = 0;
	mCurrentWeapon = BASIC;
	firingLaser = false;

	if (firingLaser)
	{
		firingLaser = false;

		if (mAmmo == 0)
		{
			mCurrentWeapon = BASIC;
		}
		else
		{
			mpAnimationLaser->getSprite(3);
		}
	}
}

void Player::spawnBarriers()
{
	// Spawn Barriers
	for (int i = 0; i < 4; i++)
	{
		mBarriers.push_back(new Barrier(Vector2((float)((i * 16) + 96), (float)BARRIER_Y)));
		mBarriers.push_back(new Barrier(Vector2((float)((i * 16) + 224), (float)BARRIER_Y)));
		mBarriers.push_back(new Barrier(Vector2((float)((i * 16) + 352), (float)BARRIER_Y)));
		mBarriers.push_back(new Barrier(Vector2((float)((i * 16) + 480), (float)BARRIER_Y)));
		mBarriers.push_back(new Barrier(Vector2((float)((i * 16) + 96), (float)BARRIER_Y + 16)));
		mBarriers.push_back(new Barrier(Vector2((float)((i * 16) + 224), (float)BARRIER_Y + 16)));
		mBarriers.push_back(new Barrier(Vector2((float)((i * 16) + 352), (float)BARRIER_Y + 16)));
		mBarriers.push_back(new Barrier(Vector2((float)((i * 16) + 480), (float)BARRIER_Y + 16)));
	}

	// Additional top parts
	for (int i = 0; i < 2; i++)
	{
		mBarriers.push_back(new Barrier(Vector2((float)((i * 16) + 112), (float)BARRIER_Y - 16)));
		mBarriers.push_back(new Barrier(Vector2((float)((i * 16) + 240), (float)BARRIER_Y - 16)));
		mBarriers.push_back(new Barrier(Vector2((float)((i * 16) + 368), (float)BARRIER_Y - 16)));
		mBarriers.push_back(new Barrier(Vector2((float)((i * 16) + 496), (float)BARRIER_Y - 16)));
	}
}

void Player::subtractLife()
{

	Audio *audio = AudioLocator::locate();
	audio->playSound(DEATH);

	--mLives;

	reset();
	resetWeapon();
	Game::getInstance()->setState(DYING);
	Game::getInstance()->startTimer();

	if (mLives <= 0)
		Game::getInstance()->setState(LOSS_SCREEN);
}

void Player::becomeInvincible()
{
	mIsInvincible = true;
	mInvincibilityTimer = 0;
}

void Player::update(const float frameTime)
{
	if (mIsInvincible)
	{
		mInvincibilityTimer++;
		if (mInvincibilityTimer >= INVINCIBILITY_TIME)
			mIsInvincible = false;
	}
		
	// Move
	mPosition.x += toMove * mMoveSpeed;

	if (toMove != 0)
		mpAnimation->setAnimationSpeed(0.2f / mMoveSpeed);
	else
		mpAnimation->setAnimationSpeed(0.0f);

	// Update bounding box
	mBoundingBox = Rect(Vector2((float)mPosition.x + 16, (float)mPosition.y + 20), Vector2(32, 20));

	// Hit a wall
	if (mPosition.x > Game::getInstance()->getGraphicsSystem()->getWidth() - 64)
	{
		mPosition.x = (float)(Game::getInstance()->getGraphicsSystem()->getWidth() - 64);
		mpAnimation->setAnimationSpeed(0.0f);
	}
	else if (mPosition.x < 0)
	{
		mPosition.x = 0.0f;
		mpAnimation->setAnimationSpeed(0.0f);
	}

	// Needed here so laser will change back to normal only after laser is done firing
	if (mCurrentWeapon == LASER && mAmmo <= 0 && !firingLaser)
	{
		mCurrentWeapon = BASIC;
	}

	// Animations
	mpAnimation->update(frameTime);
	mpAnimationBasic->update(frameTime);
	mpAnimationMulti_Shot->update(frameTime);
	mpAnimationLaser->update(frameTime);

	// Manage Projectiles
	std::vector<Projectile*>::iterator iter;
	for (iter = mProjectiles.begin(); iter != mProjectiles.end();)
	{
		if ((*iter)->inUse)
		{
			(*iter)->update(frameTime);
			++iter;
		}
		else
		{
			delete *iter;
			iter = mProjectiles.erase(iter);
		}
	}

	// Update my laser
	if (firingLaser)
	{
		if (mpProjectileLaser->inUse)
		{
			mpProjectileLaser->setPosition(Vector2(mPosition.x + 28.0f, mPosition.y - 476.0f));
			mpProjectileLaser->update(frameTime);
		}
		else
		{
			firingLaser = false;

			delete mpProjectileLaser;
			mpProjectileLaser = NULL;
		}
	}

	// Update my explosive
	if (mpProjectileExplosive != NULL)
	{
		if (mpProjectileExplosive->inUse)
		{
			if (mpProjectileExplosive->getVelocity().y == 0.0f)
			{
				mpProjectileExplosive->setPosition(Vector2(mPosition.x + 26.0f, mPosition.y));
			}

			mpProjectileExplosive->update(frameTime);
		}
		else
		{
			delete mpProjectileExplosive;
			mpProjectileExplosive = NULL;
		}
	}

	// Manage Barriers
	std::vector<Barrier*>::iterator iter2;
	for (iter2 = mBarriers.begin(); iter2 != mBarriers.end();)
	{
		if ((*iter2)->checkAlive())
		{
			++iter2;
		}
		else
		{
			delete *iter2;
			iter2 = mBarriers.erase(iter2);
		}
	}

	// Manage PickUps
	std::vector<PickUp*>::iterator iter3;
	for (iter3 = mPickUps.begin(); iter3 != mPickUps.end();)
	{
		if ((*iter3)->getAlive())
		{
			(*iter3)->update();
			++iter3;
		}
		else
		{
			delete *iter3;
			iter3 = mPickUps.erase(iter3);
		}
	}
}

void Player::draw(GraphicsSystem *graphics)
{
	// Draw my projectiles
	for (unsigned int i = 0; i < mProjectiles.size(); ++i)
	{
		mProjectiles[i]->draw(graphics);
	}

	// Draw my barriers
	for (unsigned int i = 0; i < mBarriers.size(); i++)
	{
		mBarriers[i]->draw(graphics);
	}

	// Draw myself
	if (Game::getInstance()->getState() != DYING)
	{
		graphics->drawAnimation(mpAnimation, mPosition);
	}

	// Draw my Pick ups
	for (unsigned int i = 0; i < mPickUps.size(); i++)
	{
		mPickUps[i]->draw(graphics);
	}

	// Draw my weapon
	if (Game::getInstance()->getState() != DYING)
	{
		switch (mCurrentWeapon)
		{
		case BASIC:
			graphics->drawAnimation(mpAnimationBasic, Vector2(mPosition.x + 16, mPosition.y + 2));
			break;
		case MULTI_SHOT:
			graphics->drawAnimation(mpAnimationMulti_Shot, Vector2(mPosition.x + 16, mPosition.y + 1));
			break;
		case LASER:
			graphics->drawAnimation(mpAnimationLaser, Vector2(mPosition.x + 16, mPosition.y + 1));
			break;
		case EXPLOSIVE:
			graphics->drawAnimation(mpAnimationExplosive, Vector2(mPosition.x + 16, mPosition.y + 1));
			break;
		}
	}

	// Draw my Laser
	if (firingLaser)
	{
		mpProjectileLaser->draw(graphics);
	}

	// Draw my Explosive
	if (mpProjectileExplosive != NULL)
	{
		mpProjectileExplosive->draw(graphics);
	}

	if (Game::getInstance()->getDrawingBoxes())
	{
		graphics->drawSprite(Game::getInstance()->mSpriteBoundingBox_32x20, Vector2(mPosition.x + 16, mPosition.y + 20));
	}

	// Draw Ammo
	for (int i = 0; i < mAmmo; ++i)
	{
		switch (mCurrentWeapon)
		{
		case MULTI_SHOT:
			graphics->drawAnimation(mpAnimationMulti_Shot, Vector2((32.0f) + (i * 16.0f), 32.0f));
			break;
		case LASER:
			graphics->drawAnimation(mpAnimationLaser, Vector2((32.0f) + (i * 16.0f), 32.0f));
			break;
		}
	}
}

void Player::cleanup()
{
	delete mpAnimation;
	delete mpAnimationBasic;
	delete mpAnimationMulti_Shot;
	delete mpAnimationLaser;
	delete mpAnimationExplosive;

	delete mpProjectileLaser;
	firingLaser = false;
	delete mpProjectileExplosive;

	for (unsigned int i = 0; i < mProjectiles.size(); ++i)
	{
		delete mProjectiles[i];
	}
	for (unsigned int i = 0; i < mBarriers.size(); ++i)
	{
		delete mBarriers[i];
	}
	for (unsigned int i = 0; i < mPickUps.size(); ++i)
	{
		delete mPickUps[i];
	}

	mBarriers.clear();
	mProjectiles.clear();
	mPickUps.clear();
}

void Player::fire()
{
	if (timeLastFired + 1.0f <= Game::getInstance()->getCurrentTime()) // Check the same time that BASIC requires
	{
		// Switch to basic if empty on Explosive
		if (mCurrentWeapon == EXPLOSIVE)
		{
			if (mpProjectileExplosive == NULL)
			{
				mCurrentWeapon = BASIC;
			}
			else
			{
				// Switch my Explosive to enemy projectile vector
				if (mpProjectileExplosive->getVelocity().y != 0.0f)
				{
					mCurrentWeapon = BASIC;
					Audio *audio = AudioLocator::locate();
					audio->playSound(FIRE_EXPLOSIVE);
					Game::getInstance()->getEnemyManager()->fireProjectile(EXPLOSIVE, mpProjectileExplosive->getPosition(), mpProjectileExplosive->getVelocity());
					delete mpProjectileExplosive;
					mpProjectileExplosive = NULL;
				}
			}
		}
	}

	switch (mCurrentWeapon)
	{
	case BASIC:
		if (timeLastFired + 1.0f <= Game::getInstance()->getCurrentTime())
		{
			timeLastFired = Game::getInstance()->getCurrentTime();

			mpAnimationBasic->setCurrentSprite(0);
			Audio *audio = AudioLocator::locate();
			audio->playSound(FIRE_DEFAULT);
			mProjectiles.push_back(new Projectile(BASIC, Vector2(mPosition.x + 28.0f, mPosition.y + 8.0f), Vector2(0.0f, 0.0f)));
		}
		break;
	case MULTI_SHOT:
		if (timeLastFired + 0.75f <= Game::getInstance()->getCurrentTime())
		{
			timeLastFired = Game::getInstance()->getCurrentTime();

			Audio *audio = AudioLocator::locate();
			audio->playSound(MULTI);

			mpAnimationMulti_Shot->setCurrentSprite(0);
			mProjectiles.push_back(new Projectile(MULTI_SHOT, Vector2(mPosition.x + 20.0f, mPosition.y + 8.0f), Vector2(-1.0f, 0.0f)));
			mProjectiles.push_back(new Projectile(MULTI_SHOT, Vector2(mPosition.x + 28.0f, mPosition.y + 8.0f), Vector2(0.0f, 0.0f)));
			mProjectiles.push_back(new Projectile(MULTI_SHOT, Vector2(mPosition.x + 36.0f, mPosition.y + 8.0f), Vector2(1.0f, 0.0f)));

			--mAmmo;
		}
		break;
	case LASER:
		if (timeLastFired + 2.0f <= Game::getInstance()->getCurrentTime())
		{
			timeLastFired = Game::getInstance()->getCurrentTime();

			mpAnimationLaser->setCurrentSprite(0);
			
			Audio *audio = AudioLocator::locate();
			audio->playSound(FIRE_LASER);
			mpProjectileLaser = new Projectile(LASER, Vector2(mPosition.x + 28.0f, mPosition.y - 478.0f), Vector2());
			firingLaser = true;

			--mAmmo;
		}
		break;
	case EXPLOSIVE:
		if (timeLastFired + 0.5f <= Game::getInstance()->getCurrentTime() && mpProjectileExplosive != NULL && mpProjectileExplosive->getVelocity().y == 0.0f)
		{
			timeLastFired = Game::getInstance()->getCurrentTime();

			Audio *audio = AudioLocator::locate();
			audio->playSound(FIRE_EXPLOSIVE);
			mpProjectileExplosive->setVelocity(Vector2((float)toMove, -8.0f));
			mpAnimationExplosive->setCurrentSprite(1);

			--mAmmo;
		}
		break;
	}

	if (mAmmo <= 0 && !firingLaser && mCurrentWeapon != EXPLOSIVE)
	{
		mCurrentWeapon = BASIC;
	}
}

void Player::pickUpWeapon()
{
	bool foundWeapon = false;

	Rect checkRect = mBoundingBox;
	checkRect.mSize.y += 32;

	for (unsigned int i = 0; !foundWeapon && i < mPickUps.size(); ++i)
	{
		if (checkRect.checkCollision(mPickUps[i]->getBoundingBox()))
		{
			Audio *audio = AudioLocator::locate();
			audio->playSound(PICKUP);
			foundWeapon = true;
			timeLastFired = 0.0f;
			mPickUps[i]->destroy();

			switch (mPickUps[i]->getType())
			{
			case MULTI_SHOT:
				mCurrentWeapon = MULTI_SHOT;
				mAmmo = (rand() % 3) + 2;
				break;
			case LASER:
				mCurrentWeapon = LASER;
				mAmmo = 1;

				// One in four chance of having 2
				if (rand() % 4 == 0)
					++mAmmo;

				break;
			case EXPLOSIVE:
				mCurrentWeapon = EXPLOSIVE;
				mAmmo = 1;
				mpAnimationExplosive->setCurrentSprite(0);

				if (mpProjectileExplosive != NULL && mpProjectileExplosive->getVelocity().y != 0)
				{
					// Replace the already existing one in the enemy projectile vector
					Game::getInstance()->getEnemyManager()->fireProjectile(EXPLOSIVE, mpProjectileExplosive->getPosition(), mpProjectileExplosive->getVelocity());
					delete mpProjectileExplosive;
				}

				mpProjectileExplosive = new Projectile(EXPLOSIVE, Vector2(mPosition.x + 26.0f, mPosition.y), Vector2());
				break;
			}

			// If switching weapon and I still have an explosive held
			if (mCurrentWeapon != EXPLOSIVE)
			{
				if (mpProjectileExplosive != NULL && mpProjectileExplosive->getVelocity().y == 0)
				{
					delete mpProjectileExplosive;
					mpProjectileExplosive = NULL;
				}
			}
		}
	}
}