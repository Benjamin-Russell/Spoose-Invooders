#ifndef PLAYER_H
#define PLAYER_H

#include "Trackable.h"
#include "Animation.h"
#include "Vector2.h"
#include "Projectile.h"
#include "Barrier.h"
#include "PickUp.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>

class Player : public Trackable
{
public:
	Player(bool isNew);
	Player();
	~Player();

	const int BARRIER_Y = 364;
	const int INVINCIBILITY_TIME = 300;

	void cleanup();
	void reset();
	void resetBarriers();
	void resetWeapon();
	void spawnBarriers();
	void addPickUp(ProjectileType type, Vector2 position);
	void update(const float frameTime);
	void draw(GraphicsSystem *graphics);
	void fire();
	void pickUpWeapon();
	void becomeInvincible();
	inline bool isInvincible(){ return mIsInvincible; }

	// Cheats
	void cheatWeapon(int num);

	inline void setPosition(Vector2 pos) { mPosition = pos; }
	inline Vector2 getPosition() { return mPosition; }
	inline void killAnimation() { mpAnimation->setAnimationSpeed(0.0f); }
	inline void addMove(int move) { toMove += move; }
	inline void setMove(int move) { toMove = move; }
	inline void setNumLives(int newLives) { mLives = newLives; }
	inline void setMoveSpeed(float speed) { mMoveSpeed = speed; }
	inline void addLife() { mLives++; }
	inline void setFiringLaser(bool set) { firingLaser = set; }
	inline bool getFiringLaser() { return firingLaser; }
	inline void setProjectileExplosive(Projectile* newProj) { mpProjectileExplosive = newProj; }
	void subtractLife();
	
	inline Rect getBoundingBox() { return mBoundingBox; }
	inline std::vector<Barrier*> getBarriers() { return mBarriers; }
	inline int getNumLives() { return mLives; }
	inline std::vector<Projectile*> getProjectiles() { return mProjectiles; }
	inline Animation* getLaserWeaponAnimation() { return mpAnimationLaser; }
	inline Projectile* getProjectileLaser() { return mpProjectileLaser; }
	inline Projectile* getProjectileExplosive() { return mpProjectileExplosive; }
	inline ProjectileType getCurrentWeapon() { return mCurrentWeapon; }
	inline std::vector<PickUp*> getPickUps() { return mPickUps; }
	inline Animation* getAnimationExplosive() { return mpAnimationExplosive; }
	inline Animation* getAnimation() { return mpAnimation; }
	inline int getAmmo() { return mAmmo; }
	inline void setAmmo(int ammo) { mAmmo = ammo; }
	inline Sprite* getSpriteDestroyed() { return mpSpriteDestroyed; }

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned version)
	{
		ar & mPosition;
		ar & mpAnimation;
		ar & mpAnimationBasic;
		ar & mpAnimationMulti_Shot;
		ar & mpAnimationLaser;
		ar & mpAnimationExplosive;
		ar & mpSpriteDestroyed;
		ar & mPickUps;
		ar & mBarriers;
		ar & mProjectiles;
		ar & mAmmo;
		ar & mCurrentWeapon;
		ar & timeLastFired;
		ar & mLives;
	}

	Vector2 mPosition;
	Animation *mpAnimation;
	Animation *mpAnimationBasic;
	Animation *mpAnimationMulti_Shot;
	Animation *mpAnimationLaser;
	Animation *mpAnimationExplosive;
	Sprite *mpSpriteDestroyed;
	Rect mBoundingBox;

	// Invincibility (for preventing deaths after reloading)
	bool mIsInvincible;
	int mInvincibilityTimer;

	std::vector<PickUp*> mPickUps;
	std::vector<Barrier*> mBarriers;
	std::vector<Projectile*> mProjectiles;

	int mAmmo;
	ProjectileType mCurrentWeapon;
	Projectile* mpProjectileLaser;
	Projectile* mpProjectileExplosive;
	bool firingLaser;
	float timeLastFired;

	int toMove; // Accounts for left and right keys in combination
	float mMoveSpeed;
	int mLives;
};

#endif