#include "Vector2.h"

Vector2::Vector2()
{
	x = 0.0f;
	y = 0.0f;
}

Vector2::Vector2(float x_, float y_)
{
	x = x_;
	y = y_;
}

Vector2::~Vector2()
{

}

int Vector2::getDistanceTo(Vector2 vector2)
{
	return (int)(sqrt( pow( vector2.x - x, 2) + pow( vector2.y - y, 2)));
}

Rect::Rect()
{
	mPosition = Vector2(0.0f, 0.0f);
	mSize = Vector2(0.0f, 0.0f);
}

Rect::Rect(Vector2 pos, Vector2 size)
{
	mPosition = pos;
	mSize = size;
}

Rect::~Rect()
{

}

bool Rect::checkCollision(Rect otherRect)
{
	if (mPosition.x + mSize.x >= otherRect.mPosition.x && mPosition.x <= otherRect.mPosition.x + otherRect.mSize.x
		&& mPosition.y + mSize.y >= otherRect.mPosition.y && mPosition.y <= otherRect.mPosition.y + otherRect.mSize.y)
		return true;
	else
		return false;
}