#ifndef _Animation_H_
#define _Animation_H_

#include <vector>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>

#include "Sprite.h"

class Animation
{
public:
	Animation(float aniSpeed, bool looping);
	~Animation();
	Animation(){ };

	void update(float currentTime);

	inline Sprite* getSprite(int index) { return mSprites[index]; }
	inline float getAnimationSpeed() { return mAnimationSpeed; }
	inline int getCurrentSprite() { return mCurrentSprite; }
	inline int getNumSprites() { return mSprites.size(); }
	inline bool getLooping() { return mLooping; }

	void addSprite(Sprite* newSprite);
	void clearSprites();
	void setAnimationSpeed(float speed);
	void setCurrentSprite(unsigned int index);
	void setLooping(bool loop);

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned version)
	{
		ar & mCurrentSprite;
		ar & mSprites;
		ar & mAnimationSpeed;
		ar & timeLastUpdated;
		ar & timeUntilNextSprite;
		ar & mLooping;
	}

	std::vector<Sprite*> mSprites;
	float mAnimationSpeed;
	float timeLastUpdated;
	float timeUntilNextSprite;
	unsigned int mCurrentSprite;
	bool mLooping;
};

#endif