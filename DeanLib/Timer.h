#pragma once
#include <windows.h>
#include "Trackable.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

/* Timer - high accuracy timer - uses Large Integer to prevent rollover

Dean Lawson
Champlain College
2011
*/

class Timer
{
public:

	Timer();
	~Timer();

	void start();
	void stop();
	void reset();
	double getElapsedTime() const;//returns how much time has elapsed since start
	void sleepUntilElapsed( double ms );
	void pause( bool shouldPause );
	inline double getFactor() const { return mFactor; };
	inline void multFactor( double mult ) { mLastFactor = mFactor; mFactor *= mult; };
	inline void setFactor( double theFactor ) { mLastFactor = mFactor; mFactor = theFactor; };
	inline void restoreLastFactor() { mFactor = mLastFactor; };

private:
	LARGE_INTEGER mStartTime;
	LARGE_INTEGER mEndTime;
	LARGE_INTEGER mTimerFrequency;
	double mElapsedTime;
	double mFactor;
	double mLastFactor;
	bool mPaused;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned version)
	{
		ar & mStartTime.HighPart;
		ar & mStartTime.LowPart;
		ar & mStartTime.QuadPart;
		ar & mEndTime.HighPart;
		ar & mEndTime.LowPart;
		ar & mEndTime.QuadPart;
		ar & mTimerFrequency.HighPart;
		ar & mTimerFrequency.LowPart;
		ar & mTimerFrequency.QuadPart;
		ar & mElapsedTime;
		ar & mFactor;
		ar & mLastFactor;
		ar & mPaused;
	}

	//helper function - uses current frequency for the Timer
	double calcDifferenceInMS( LARGE_INTEGER from, LARGE_INTEGER to ) const;

};