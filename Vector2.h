#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include "Trackable.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

struct Vector2
{
public:
	Vector2();
	Vector2(float x_, float y_);
	~Vector2();

	float x,
		  y;

	int getDistanceTo(Vector2 vector2);
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned version)
	{
		ar & x;
		ar & y;
	}
};

struct Rect : public Trackable
{
public:
	Rect();
	Rect(Vector2 pos, Vector2 size);
	~Rect();

	Vector2 mPosition;
	Vector2 mSize; // Width and Height

	bool checkCollision(Rect otherRect);
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned version)
	{
		ar & mPosition;
		ar & mSize;
	}
};

#endif