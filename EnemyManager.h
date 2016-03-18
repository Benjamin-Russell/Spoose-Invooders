#ifndef _ENEMY_MANAGER_H_
#define _ENEMY_MANAGER_H_

#include "Invader.h"
#include "Explosion.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <vector>

const int INVADER_ROWS = 4;
const int INVADER_COLUMNS = 5;
const int INVADER_MAX = INVADER_COLUMNS * INVADER_ROWS;
const int BOTTOM_OF_SCREEN = 380;

class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();

	void update(const float frameTime);
	void updateDeath(const float frameTime);
	void draw(GraphicsSystem *graphics);

	void setMustSwitchDirections(bool val);
	void switchDirections();
	Projectile* fireProjectile(ProjectileType type, Vector2 position, Vector2 addedVelocity);
	void fireExplosion(Vector2 position, float scale);

	void startPlay();
	void endPlay();
	void resetPositions();
	void cleanup();

	inline int getInvaderDirection() { return mInvaderDirection; }
	inline float getInvaderSpeed() { return mInvaderSpeed; }
	inline std::vector<Invader*> getInvaders() { return mInvaders; }
	inline std::vector<Projectile*> getProjectiles() { return mProjectiles; }

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned version)
	{
		ar & mInvaders;
		ar & mInvaderPool;
		ar & mProjectiles;
		ar & mExplosions;
		ar & mInvaderAnimation;
		ar & lastAnimatedInvaders;
		ar & mMustSwitchDirection;
		ar & mInvaderDirection;
		ar & mInvaderSpeed;
	}
	std::vector<Invader*> mInvaders;
	std::vector<Invader*> mInvaderPool; // Object Pool
	std::vector<Projectile*> mProjectiles;
	std::vector<Explosion*> mExplosions;
	//UFO *mUFO // EnemyManager will be in charge of the UFO or whatever we call it

	Animation* mInvaderAnimation;
	float lastAnimatedInvaders;
	bool mMustSwitchDirection;
	int mInvaderDirection;
	float mInvaderSpeed;
};

#endif