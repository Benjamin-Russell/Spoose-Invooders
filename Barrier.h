#ifndef BARRIER_H
#define BARRIER_H

#include "Trackable.h"
#include "Vector2.h"
#include "Sprite.h"
#include "GraphicsSystem.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

class Barrier
{
public:
	Barrier(Vector2 location);
	~Barrier();
	Barrier();

	void cleanup();
	void draw(GraphicsSystem *graphics);

	inline bool checkAlive() { return mLife > 0; }
	inline Rect getBoundingBox() { return mBoundingBox; }
	inline void subtractLife() { --mLife; }
	inline void subtractLife(int amount) { mLife -= amount; }
	inline void destroy() { mLife = 0; }
	inline Vector2 getPosition() { return mPosition; }

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned version)
	{
		ar & mPosition;
		ar & mSprite;
		ar & mLife;
		ar & mBoundingBox;
	}
	Vector2 mPosition;
	Sprite *mSprite;
	Rect mBoundingBox;

	int mLife;
};

#endif