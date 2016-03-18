#ifndef _GAME_H_
#define _GAME_H_

#include "Timer.h"
#include <PerformanceTracker.h>

#include <sstream>
#include <time.h>
#include <vector>
#include <fstream>

#include "Animation.h"
#include "GraphicsSystem.h"
#include "GraphicsBufferManager.h"
#include "MenuSystem.h"
#include "InputManager.h"
#include "InputListener.h"
#include "GameListener.h"
#include "Player.h"
#include "EnemyManager.h"
#include "SaveSystem.h"
#include "AudioLocator.h"
#include "TextLoader.h"

enum Difficulty
{
	EASY = 1,
	NORMAL,
	HARD,
	EXTREME,
	NUM_DIFFICULTIES_MINUS_1
};

const string DIFFICULTY_STRINGS[5] = { "Invalid", "Easy", "Normal", "Hard", "Extreme" };

enum GameState 
{
	SPLASH = 0,
	MAIN_MENU,
	OPTIONS_MENU,
	PLAY,
	PAUSE_MENU,
	CREDITS_MENU,
	QUIT,
	LOSS_SCREEN,
	DYING,
	NUM_GAMESTATES
};

const int UI_PADDING = 4;
const string DATA_FILE = "Resources/gameData.txt";

class Game : public Trackable
{

public:
	~Game();

	static void initInstance();
	void runGameLoop(int targetFrameRate);
	void resetGame();
	void cleanUp();

	// Sprites for bounding boxes
	Sprite* mSpriteBoundingBox_8x8;
	Sprite* mSpriteBoundingBox_12x12;
	Sprite* mSpriteBoundingBox_16x16;
	Sprite* mSpriteBoundingBox_24x18;
	Sprite* mSpriteBoundingBox_32x20;
	Sprite* mSpriteBoundingBox_124x124;
	Sprite* mSpriteBoundingBox_6x480;

	// Mutators
	void toggleDrawingBoxes();
	void increaseScoreBy(int num);
	inline void setDifficulty(Difficulty dif) { mDifficulty = dif; }
	inline void setState(GameState state) { mCurrentState = state; }
	inline void setScore(int score) { mScore = score; }
	inline void resetTimer() { mGameTimer.reset(); }
	inline void setLagTime(float time) { mLagTime = time; }
	void cycleDifficulty();
	inline void pushStateToStack(GameState state) { mStateStack.push_back(state); }
	inline GameState popStateFromStack(){ GameState toReturn = mStateStack.back(); mStateStack.pop_back(); return toReturn; }
	inline void setLevel(int level) { mLevel = level; }
	inline void startTimer() { mDeathTimer = 0; }
	inline bool incrementTimer() { mDeathTimer++; if (mDeathTimer >= 150) return true; return false; }

	// Accesors
	inline static Game* getInstance() { return mspInstance; }
	inline GraphicsSystem* getGraphicsSystem() { return mspGraphicsSystem; }
	inline GraphicsBufferManager* getGraphicsBufferManager() { return mspGraphicsBufferManager; }
	inline MenuSystem* getMenuSystem() { return mspMenuSystem; }
	inline InputManager* getInputManager() { return mspInputManager; }
	inline InputListener* getInputListener() { return mspInputListener; }
	inline EnemyManager* getEnemyManager() { return mspEnemyManager; }
	inline SaveSystem* getSaveSystem() { return mspSaveSystem; }
	inline Player* getPlayer() { return mspPlayer; }
	inline float getCurrentTime() { return mCurrentTime; }
	inline float getPreviousTime() { return mPreviousTime; }
	inline float getElapsedTime() { return mElapsedTime; }
	inline float getLagTime() { return mLagTime; }
	inline float getEndFrameTime() { return mEndFrameTime; }
	inline float getFpsUpdateTime() { return mFpsUpdateTime; }
	inline int getCurrentFps() { return mCurrentFps; }
	inline int getFrameCounter() { return mFrameCounter; }
	inline bool getInitialized() { return msInitialized; }
	inline bool getRunningLoop() { return mRunningLoop; }
	inline bool getDrawingBoxes() { return mDrawBoundingBoxes; }
	inline Difficulty getDifficulty() { return mDifficulty; }
	inline GameState getState() { return mCurrentState; }
	inline int getScore(){ return mScore; }
	inline int getLevel() { return mLevel; }

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned version)
	{
		ar & mScore;
		ar & mFrameCounter;
		ar & mDifficulty;
		ar & mCurrentTime;
		ar & mPreviousTime;
		ar & mElapsedTime;
		ar & mLagTime;
		ar & mEndFrameTime;
		ar & mFpsUpdateTime;
		ar & mGameTimer;
		ar & mLevel;
		ar & mLastLifeReached;
		ar & mTimeLastLifeGained;
	}

	Game();

	static bool msInitialized;
	bool mRunningLoop;
	vector<GameState> mStateStack;	// A stack of game states for handling menus and such
	GameState mCurrentState;

	void update(const float frameLength);
	void draw(GraphicsSystem* graphics);

	void updateGame(const float frameLength);
	void drawGame(GraphicsSystem* graphics);
	void updateSplash(const float frameLength);
	void drawSplash(GraphicsSystem* graphics);

	// Time vars in seconds (*1000)
	float mCurrentTime;
	float mPreviousTime;
	float mElapsedTime;
	float mLagTime;
	float mEndFrameTime;
	float mFpsUpdateTime;
	Timer mGameTimer;
	unsigned mDeathTimer;

	int mScore;
	int mLevel;
	int mScorePerLife;
	int mLastLifeReached;
	float mTimeLastLifeGained;

	// Data driven variables for other objects
	unsigned int mPlayerStartingLife;
	int mPlayerMoveSpeed;

	Font *mUIFont;

	int mCurrentFps;
	int	mFrameCounter;

	Difficulty mDifficulty;
	int numPlayers;
	bool mDrawBoundingBoxes;

	static Game* mspInstance;
	static Player* mspPlayer;
	static GraphicsSystem* mspGraphicsSystem;
	static GraphicsBufferManager* mspGraphicsBufferManager;
	static MenuSystem* mspMenuSystem;
	static InputManager* mspInputManager;
	static InputListener* mspInputListener;
	static GameListener* mspGameListener;
	static EnemyManager* mspEnemyManager;
	static SaveSystem* mspSaveSystem;
	static TextLoader* mspTextLoader;
};

#endif