#ifndef _GRAPHICS_BUFFER_H_
#define _GRAPHICS_BUFFER_H_

#include <Trackable.h>
#include <SDL.h>
#include <SDL_image.h>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <string>

using namespace std;

class GraphicsBuffer
{
public:
	GraphicsBuffer();
	GraphicsBuffer(const char* file);
	GraphicsBuffer(string file);
	~GraphicsBuffer();

	inline string getName() { return mName; }
	inline void setName(string newName) { mName = newName; };
	inline int getWidth() { return mpSurface->w; }
	inline int getHeight() { return mpSurface->h; }
	inline SDL_Surface* getSurface() { return mpSurface; }

private:
	friend class GraphicsSystem;
	SDL_Surface* mpSurface;
	string mName;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned version)
	{
		ar & mName;
	}
};

#endif