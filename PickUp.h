#ifndef _PICK_UP_H_
#define _PICK_UP_H_

#include "Projectile.h"
#include "Trackable.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

class PickUp
{
public:
	PickUp(ProjectileType type, Vector2 position);
	~PickUp();
	PickUp(){}

	void update();
	void draw(GraphicsSystem* graphics);

	inline ProjectileType getType() { return mType; }
	inline Rect getBoundingBox() { return mBoundingBox; }
	
	void destroy();
	bool getAlive();
	
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned version)
	{
		ar & mPosition;
		ar & mBoundingBox;
		ar & mType;
		ar & mSprite;
		ar & mLifeTime;
		ar & mTimeCreated;
		ar & mOnGround;
	}

	Vector2 mPosition;
	Rect mBoundingBox;
	ProjectileType mType;
	Sprite* mSprite;

	float mLifeTime;
	float mTimeCreated;
	bool mOnGround;
};

#endif