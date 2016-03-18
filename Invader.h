#ifndef _INVADER_H_
#define _INVADER_H_

#include "Projectile.h"

class Invader
{
public:
	Invader();
	~Invader();

	bool update(const float frameTime);
	void drawWeapon(GraphicsSystem *graphics);

	Invader* init(float x_, float y_);

	inline Vector2 getPosition() { return mPosition; }
	inline Rect getBoundingBox() { return mBoundingBox; }
	inline ProjectileType getWeapon() { return mWeapon; }
	inline bool getInUse() { return inUse; }
	inline void setFiringFlash(bool set) { firingFlash = set; }
	inline bool getFiringFlash() { return firingFlash; }
	inline Projectile* getFlash() { return mpFlashProjectile; }
	inline Projectile* getBunkerBuster() { return mpBunkerBusterProjectile; }
	inline void setBunkerBuster(Projectile* set) { mpBunkerBusterProjectile = set; }
	inline void setInUse(bool use) { inUse = use; }
	inline void setX(float newX) { mPosition.x = newX; }
	inline void setY(float newY) { mPosition.y = newY; }
	inline float getX() { return mPosition.x; };
	inline float getY() { return mPosition.y; };
	inline void setOriginalPosition(){ mOriginalPosition = mPosition; }
	void restoreOriginalPosition();

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned version)
	{
		ar & mPosition;
		ar & mOriginalPosition;
		ar & inUse;
		ar & mWeapon;
		ar & mpWeaponAnimation;
		ar & timeLastFired;
	}

	Vector2 mPosition;
	Vector2 mOriginalPosition;
	Rect mBoundingBox;

	bool inUse;

	ProjectileType mWeapon;
	Projectile* mpFlashProjectile;
	Projectile* mpBunkerBusterProjectile;
	Animation* mpWeaponAnimation;
	float timeLastFired;
	bool firingFlash;

};

#endif