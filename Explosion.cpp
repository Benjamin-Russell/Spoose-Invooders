#include "Explosion.h"
#include "Game.h"

Explosion::Explosion(Vector2 position, float scale)
{
	mPosition = position;
	mScale = scale;

	// Looks better when landing at bottom of screen if raised
	if (mPosition.y > 380)
		mPosition.y = 380;

	float padding = 10.0f;
	mBoundingBox = Rect(Vector2(mPosition.x + padding, mPosition.y + padding), Vector2(144.0f - (padding * 2.0f), 144.0f - (padding * 2.0f)));

	killedPlayer = 0;
	isTrash = false;
	collisionsChecked = false;

	mpAnimation = new Animation(0.12f, false);

	for (int i = 0; i < 4; ++i)
	{
		if (mScale == 1.0f)
		{
			mpAnimation->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Explosion"), 144, 144, Vector2((float)(i * 144), 0.0f)));
		}
		else
		{	
			mpAnimation->addSprite(new Sprite(Game::getInstance()->getGraphicsBufferManager()->getGraphicsBuffer("Explosion_Small"), 72, 72, Vector2((float)(i * 72), 0.0f)));
		}
	}
}

Explosion::~Explosion()
{
	delete mpAnimation;
}

void Explosion::update(const float frameTime)
{
	// Animation
	mpAnimation->update(frameTime);

	if (mpAnimation->getCurrentSprite() == 3)
	{
		isTrash = true;
	}

	// Gravity
	mPosition.y += 1.0f;

	Audio *audio = AudioLocator::locate();

	if (!collisionsChecked)
	{
		if (mScale == 1.0f)
		{
			// Barriers
			std::vector<Barrier*, std::allocator<Barrier*>> barriers = Game::getInstance()->getPlayer()->getBarriers();

			for (unsigned int i = 0; i < barriers.size(); ++i)
			{
				if (mBoundingBox.checkCollision(barriers[i]->getBoundingBox()))
				{
					// Deal damage based on distance
					Vector2 mCenter = Vector2(mPosition.x + 72.0f, mPosition.y + 72.0f);
					Vector2 barrierCenter = Vector2(barriers[i]->getPosition().x + 6.0f, barriers[i]->getPosition().y + 6.0f);
					int damage = (int)(4.0f - ((float)mCenter.getDistanceTo(barrierCenter) / 110.0f * 4.0f));

					if (mCenter.getDistanceTo(barrierCenter) < 40 && damage > 0)
						barriers[i]->subtractLife(damage);
				}
			}

			std::vector<Invader*, std::allocator<Invader*>> invaders = Game::getInstance()->getEnemyManager()->getInvaders();
			std::vector<Projectile*, std::allocator<Projectile*>> projectiles = Game::getInstance()->getEnemyManager()->getProjectiles();
			std::vector<PickUp*, std::allocator<PickUp*>> pickUps = Game::getInstance()->getPlayer()->getPickUps();

			// Enemy Projectiles
			for (unsigned int i = 0; i < projectiles.size(); ++i)
			{
				if (projectiles[i]->getType() != FLASH && mBoundingBox.checkCollision(projectiles[i]->getBoundingBox()))
				{
					projectiles[i]->isTrash = true;
				}
			}

			// Alied Projectiles
			projectiles = Game::getInstance()->getPlayer()->getProjectiles();

			for (unsigned int i = 0; i < projectiles.size(); ++i)
			{
				if (projectiles[i]->getType() != LASER && mBoundingBox.checkCollision(projectiles[i]->getBoundingBox()))
				{
					projectiles[i]->isTrash = true;
				}
			}

			// Invaders
			for (unsigned int i = 0; i < invaders.size(); ++i)
			{
				if (mBoundingBox.checkCollision(invaders[i]->getBoundingBox()) && invaders[i]->getInUse())
				{
					// If its bunker buster exists, give it to the Enemy Manager
					if (invaders[i]->getWeapon() == BUNKER_BUSTER && invaders[i]->getBunkerBuster() != NULL && invaders[i]->getBunkerBuster()->isTrash == false)
					{
						Projectile* newProj = Game::getInstance()->getEnemyManager()->fireProjectile(BUNKER_BUSTER, invaders[i]->getBunkerBuster()->getPosition(), Vector2(0.0f, 0.1f));

						// If the personal bunker buster is also in the explosion
						if (mBoundingBox.checkCollision(newProj->getBoundingBox()))
						{
							newProj->isTrash = true;
						}

						delete invaders[i]->getBunkerBuster();
						invaders[i]->setBunkerBuster(NULL);
					}

					Game::getInstance()->getEnemyManager()->fireExplosion(Vector2(invaders[i]->getPosition().x - 12.0f, invaders[i]->getPosition().y - 12.0f), 0.5f);

					invaders[i]->setInUse(false);
					Game::getInstance()->increaseScoreBy(100);
				}
			}

			// Pick Ups
			for (unsigned int i = 0; i < pickUps.size(); ++i)
			{
				if (mBoundingBox.checkCollision(pickUps[i]->getBoundingBox()))
				{
					pickUps[i]->destroy();
				}
			}

			// Players
			if (mBoundingBox.checkCollision(Game::getInstance()->getPlayer()->getBoundingBox()))
			{
				isTrash = true;
				killedPlayer = 1; // Could be 2 for player 2
			}

			audio->playSound(EXPLOSION);
			collisionsChecked = true;
		}
		else
		{
			audio->playSound(INVADER_DEAD);
			collisionsChecked = true;
		}
	}
}

void Explosion::draw(GraphicsSystem* graphics)
{
	graphics->drawAnimation(mpAnimation, mPosition);

	if (Game::getInstance()->getDrawingBoxes() && mScale == 1.0f)
	{
		graphics->drawSprite(Game::getInstance()->mSpriteBoundingBox_124x124, Vector2(mPosition.x + 10.0f, mPosition.y + 10.0f));
	}
}