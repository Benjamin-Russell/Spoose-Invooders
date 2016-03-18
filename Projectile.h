#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "Animation.h"
#include "Vector2.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

enum ProjectileType
{
	// Ally
	BASIC = 0,
	MULTI_SHOT,
	LASER,
	EXPLOSIVE,
	RAPID_FIRE,
	FLAK_GUN, // Possible edition, great for killing last invaders

	// Enemy
	ENEMY_THRESHOLD, // Use this to check if mType is ally (mType < ENEMY_THRESHOLD) or enemy (mType > ENEMY THRESHOLD)

	BASIC_ENEMY,
	TRIPLE_THREAT,
	FLASH,
	BUNKER_BUSTER,
	FAST_FREDDY, // Fires in a wave, name is non-desciptive so I wouldn't mind a name change
	ENEMY_EQUIVALENT_OF_FLAK_GUN // Maybe leaves a spike or something on the ground
};

class Projectile
{
public:
	Projectile(ProjectileType type, Vector2 position, Vector2 addedVelocity);
	Projectile(Projectile* toCopy);
	Projectile(){ killedPlayer = 0; };
	~Projectile();

	// Flags for deletion:
	bool inUse;   // This flag lets the projectile's manager know to scrap it.
	bool isTrash; // This flag is set by other objects, so it gets a chance to update before being scrapped.

	void cleanUp();
	void update(const float frameTime);
	void draw(GraphicsSystem *graphics);

	inline Animation* getAnimation() { return mpAnimation; }
	inline Rect getBoundingBox() { return mBoundingBox; }
	inline ProjectileType getType() { return mType; }
	inline int getKilledPlayer() { return killedPlayer; }
	inline Vector2 getVelocity() { return mVelocity; }
	inline Vector2 getPosition() { return mPosition; }
	inline void setPosition(Vector2 position) { mPosition = position; }
	inline void setVelocity(Vector2 velocity) { mVelocity = velocity; }
	inline float getTimeCreated() { return mTimeCreated; }

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned version)
	{
		ar & mPosition;
		ar & mVelocity;
		ar & mType;
		ar & inUse;
		ar & isTrash;
		ar & mpAnimation;
		ar & mTimeCreated;
		ar & mFlashIsLethal;
		//ar & killedPlayer;
	}
	Vector2 mPosition;
	Vector2 mVelocity;
	Rect mBoundingBox;
	Animation* mpAnimation;

	ProjectileType mType;
	float mTimeCreated;
	bool mFlashIsLethal;
	int killedPlayer;
};

#endif