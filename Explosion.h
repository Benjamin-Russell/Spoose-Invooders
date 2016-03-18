#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "Trackable.h"
#include "Animation.h"
#include "GraphicsSystem.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

class Explosion
{
public:
	Explosion(Vector2 position, float scale);
	Explosion(){ };
	~Explosion();

	void update(const float frameTime);
	void draw(GraphicsSystem *graphics);

	inline Rect getBoundingBox() { return mBoundingBox; }
	inline int getKilledPlayer() { return killedPlayer; }
	inline bool getCollisionsChecked() { return collisionsChecked; }

	bool isTrash;

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned version)
	{
		ar & mPosition;
		ar & killedPlayer;
		ar & collisionsChecked;
		ar & isTrash;
		ar & mpAnimation;
		ar & mScale;
	}
	Vector2 mPosition;
	Rect mBoundingBox;
	Animation* mpAnimation;

	int killedPlayer;
	bool collisionsChecked;
	float mScale;
};

#endif