#include "Game.h"
#include <math.h>

// Static Member Initializations
bool Game::msInitialized = false;
Game* Game::mspInstance = NULL;
Player* Game::mspPlayer = NULL;
GraphicsSystem* Game::mspGraphicsSystem = NULL;
GraphicsBufferManager* Game::mspGraphicsBufferManager = NULL;
MenuSystem* Game::mspMenuSystem = NULL;
InputManager* Game::mspInputManager = NULL;
InputListener* Game::mspInputListener = NULL;
GameListener* Game::mspGameListener = NULL;
EnemyManager* Game::mspEnemyManager = NULL;
SaveSystem* Game::mspSaveSystem = NULL;
TextLoader* Game::mspTextLoader = NULL;

Game::Game()
{
	// Variable inits. Some are dictated by data file, but if it fails to load then they will have default value here
	numPlayers = 1;
	mDrawBoundingBoxes = false;
	mDifficulty = NORMAL;
	mRunningLoop = false;
	mCurrentTime = 0.0f;
	mPreviousTime = 0.0f;
	mElapsedTime = 0.0f;
	mLagTime = 0.0f;
	mFpsUpdateTime = 0.0f;
	mFrameCounter = 0;
	mEndFrameTime = 0.0f;
	mLevel = 1;
	mScore = 0;
	mScorePerLife = 10000;
	mLastLifeReached = 0;
	mTimeLastLifeGained = -10.0f;

	// Data driven variables for other objects
	mPlayerStartingLife = 3;
	mPlayerMoveSpeed = 2;

	mspSaveSystem = new SaveSystem();
	mspGraphicsSystem = new GraphicsSystem(640, 480);
	mspGraphicsBufferManager = new GraphicsBufferManager();
	mspMenuSystem = new MenuSystem();
	mspInputManager = new InputManager();
	mspInputListener = new InputListener();
	mspGameListener = new GameListener();
	mspTextLoader = new TextLoader();
	AudioService *service = new AudioService();
	AudioLocator::provide(service);

	// Data driven start variables and file paths
	vector<string> gameInfo = mspTextLoader->loadFile(DATA_FILE);

	mScorePerLife = atoi(gameInfo[0].c_str());
	mDifficulty = (Difficulty)atoi(gameInfo[1].c_str());
	mPlayerStartingLife = atoi(gameInfo[2].c_str());
	mPlayerMoveSpeed = atoi(gameInfo[3].c_str());
	
	mUIFont = new Font(gameInfo[4], 36);

	mspGraphicsBufferManager->addGraphicsBuffer("Background", new GraphicsBuffer(gameInfo[5]));
	mspGraphicsBufferManager->addGraphicsBuffer("Foreground", new GraphicsBuffer(gameInfo[6]));
	mspGraphicsBufferManager->addGraphicsBuffer("Splash", new GraphicsBuffer(gameInfo[7]));
	mspGraphicsBufferManager->addGraphicsBuffer("Barrier", new GraphicsBuffer(gameInfo[8]));
	mspGraphicsBufferManager->addGraphicsBuffer("Player", new GraphicsBuffer(gameInfo[9]));
	mspGraphicsBufferManager->addGraphicsBuffer("Player_Destroyed", new GraphicsBuffer(gameInfo[10]));
	mspGraphicsBufferManager->addGraphicsBuffer("Invader", new GraphicsBuffer(gameInfo[11]));
	mspGraphicsBufferManager->addGraphicsBuffer("BoundingBoxes", new GraphicsBuffer(gameInfo[12]));
	mspGraphicsBufferManager->addGraphicsBuffer("Explosion", new GraphicsBuffer(gameInfo[13]));
	mspGraphicsBufferManager->addGraphicsBuffer("Explosion_Small", new GraphicsBuffer(gameInfo[14]));
	mspGraphicsBufferManager->addGraphicsBuffer("Projectile_Player1", new GraphicsBuffer(gameInfo[15]));
	mspGraphicsBufferManager->addGraphicsBuffer("Projectile_Multi_Shot", new GraphicsBuffer(gameInfo[16]));
	mspGraphicsBufferManager->addGraphicsBuffer("Projectile_Basic_Enemy", new GraphicsBuffer(gameInfo[17]));
	mspGraphicsBufferManager->addGraphicsBuffer("Projectile_Flash", new GraphicsBuffer(gameInfo[18]));
	mspGraphicsBufferManager->addGraphicsBuffer("Projectile_Flash_Charge", new GraphicsBuffer(gameInfo[19]));
	mspGraphicsBufferManager->addGraphicsBuffer("Projectile_Explosive", new GraphicsBuffer(gameInfo[20]));
	mspGraphicsBufferManager->addGraphicsBuffer("Weapon_Basic", new GraphicsBuffer(gameInfo[21]));
	mspGraphicsBufferManager->addGraphicsBuffer("Weapon_Multi_Shot", new GraphicsBuffer(gameInfo[22]));
	mspGraphicsBufferManager->addGraphicsBuffer("Weapon_Laser", new GraphicsBuffer(gameInfo[23]));
	mspGraphicsBufferManager->addGraphicsBuffer("Weapon_Explosive", new GraphicsBuffer(gameInfo[24]));
	mspGraphicsBufferManager->addGraphicsBuffer("Weapon_Basic_Enemy", new GraphicsBuffer(gameInfo[25]));
	mspGraphicsBufferManager->addGraphicsBuffer("Weapon_Triple_Threat", new GraphicsBuffer(gameInfo[26]));
	mspGraphicsBufferManager->addGraphicsBuffer("Weapon_Flash", new GraphicsBuffer(gameInfo[27]));
	mspGraphicsBufferManager->addGraphicsBuffer("Weapon_Bunker_Buster", new GraphicsBuffer(gameInfo[28]));

	// These need buffers to initialize
	if (!mspPlayer)
		mspPlayer = new Player(true);
	mspEnemyManager = new EnemyManager();
	mSpriteBoundingBox_8x8 = new Sprite(mspGraphicsBufferManager->getGraphicsBuffer("BoundingBoxes"), 8, 8, Vector2(0.0f, 0.0f));
	mSpriteBoundingBox_12x12 = new Sprite(mspGraphicsBufferManager->getGraphicsBuffer("BoundingBoxes"), 12, 12, Vector2(12.0f, 16.0f));
	mSpriteBoundingBox_16x16 = new Sprite(mspGraphicsBufferManager->getGraphicsBuffer("BoundingBoxes"), 16, 16, Vector2(8.0f, 0.0f));
	mSpriteBoundingBox_24x18 = new Sprite(mspGraphicsBufferManager->getGraphicsBuffer("BoundingBoxes"), 24, 18, Vector2(24.0f, 0.0f));
	mSpriteBoundingBox_32x20 = new Sprite(mspGraphicsBufferManager->getGraphicsBuffer("BoundingBoxes"), 32, 20, Vector2(48.0f, 0.0f));
	mSpriteBoundingBox_124x124 = new Sprite(mspGraphicsBufferManager->getGraphicsBuffer("BoundingBoxes"), 124, 124, Vector2(86.0f, 0.0f));
	mSpriteBoundingBox_6x480 = new Sprite(mspGraphicsBufferManager->getGraphicsBuffer("BoundingBoxes"), 6, 480, Vector2(80.0f, 0.0f));

	mCurrentState = SPLASH;
}

Game::~Game()
{

}

void Game::cycleDifficulty()
{
	int tmpInt = (int)(mDifficulty);
	tmpInt++;
	mDifficulty = (Difficulty)(tmpInt);

	if (mDifficulty == NUM_DIFFICULTIES_MINUS_1)
		mDifficulty = EASY;
}

void Game::initInstance()
{
	if (!msInitialized)
	{
		gpEventSystem = new EventSystem();
		mspInstance = new Game();

		mspPlayer->setNumLives(mspInstance->mPlayerStartingLife);
		mspPlayer->setMoveSpeed((float)mspInstance->mPlayerMoveSpeed);

		msInitialized = true;
	}
}

void Game::cleanUp()
{
	if (msInitialized)
	{
		msInitialized = false;

		// All destructors automatically call cleanup
		delete mspPlayer;
		delete mspMenuSystem;
		delete mspGraphicsSystem;
		delete mspGraphicsBufferManager;
		delete mspInputManager;
		delete mspInputListener;
		delete mspGameListener;
		delete mspEnemyManager;
		delete gpEventSystem;
		delete mUIFont;
		delete mspSaveSystem;
		delete mspTextLoader;
		delete mSpriteBoundingBox_8x8;
		delete mSpriteBoundingBox_12x12;
		delete mSpriteBoundingBox_16x16;
		delete mSpriteBoundingBox_24x18;
		delete mSpriteBoundingBox_32x20;
		delete mSpriteBoundingBox_124x124;
		delete mSpriteBoundingBox_6x480;
		AudioLocator::cleanup();
	}
}

void Game::increaseScoreBy(int num) 
{ 
	mScore += num * mDifficulty; 

	// Add a life
	if (mScore - mScorePerLife >= mLastLifeReached)
	{
		mLastLifeReached += mScorePerLife;
		mspPlayer->addLife();
		mTimeLastLifeGained = Game::getInstance()->getCurrentTime();

		Audio *audio = AudioLocator::locate();
		audio->playSound(LIFE_UP);
	}
}

void Game::toggleDrawingBoxes()
{
	std::cout << "Cheat Used: toggle Bounding Box Draw Mode.\n";

	if (mDrawBoundingBoxes)
		mDrawBoundingBoxes = false;
	else
		mDrawBoundingBoxes = true;
}

void Game::resetGame()
{
	// Different player reset functions for different situations
	mspPlayer->setPosition(Vector2((float)(Game::getInstance()->getGraphicsSystem()->getWidth() / 2 - 32), (float)(Game::getInstance()->getGraphicsSystem()->getHeight() - 64)));
	mspPlayer->reset();
	mspPlayer->resetBarriers();
	mspPlayer->resetWeapon();

	mspSaveSystem->destroySaveFile();

	mspEnemyManager->endPlay();
	mspPlayer->setNumLives(3);
	mLastLifeReached = 0;
	mTimeLastLifeGained = -10.0f;
}

void Game::drawGame(GraphicsSystem* gs)
{
	mspGraphicsSystem->clear(Color(0, 0, 0));

	mspGraphicsSystem->drawBuffer(mspGraphicsBufferManager->getGraphicsBuffer("Background"), Vector2(0, 0));
	mspGraphicsSystem->drawBuffer(mspGraphicsBufferManager->getGraphicsBuffer("Foreground"), Vector2(0, (float)mspGraphicsSystem->getHeight() - 64.0f));

	mspPlayer->draw(gs);
	mspEnemyManager->draw(gs);

	float timeSinceLifeGained = Game::getInstance()->getCurrentTime() - mTimeLastLifeGained;
	char addedLifeMark = ' ';

	if (timeSinceLifeGained < 2.0f && (int)(timeSinceLifeGained * 10.0f) % 4 >= 2)
		addedLifeMark = '+';

	stringstream ss;
	ss << "Lives: " << mspPlayer->getNumLives() << addedLifeMark << " FPS: " << mCurrentFps << "  Score: " << mScore;
	gs->writeText(Vector2((float)UI_PADDING * 10, (float)UI_PADDING * 2), *mUIFont, Color(255, 255, 255), ss.str());
}

void Game::drawSplash(GraphicsSystem* graphics)
{
	graphics->drawBuffer(mspGraphicsBufferManager->getGraphicsBuffer("Splash"), Vector2(0, 0));
}

void Game::updateSplash(const float frameLength)
{
	if (mCurrentTime > 3.0f)
		mCurrentState = MAIN_MENU;
}

void Game::updateGame(const float frameLength)
{
	// Update as many times as needed to catch up
	while (mLagTime >= frameLength)
	{
		mspPlayer->update(mCurrentTime);
		mspEnemyManager->update(mCurrentTime);

		mLagTime -= frameLength;
	}
}

void Game::draw(GraphicsSystem* graphics)
{
	switch (mCurrentState)
	{
	case SPLASH:
		drawSplash(graphics);
		break;
	case MAIN_MENU:
		mspMenuSystem->draw(mspGraphicsSystem, MAIN);
		break;
	case OPTIONS_MENU:
		mspMenuSystem->draw(mspGraphicsSystem, OPTIONS);
		break;
	case PAUSE_MENU:
		mspMenuSystem->draw(mspGraphicsSystem, PAUSE);
		break;
	case CREDITS_MENU:
		mspMenuSystem->draw(mspGraphicsSystem, CREDITS);
		break;
	case PLAY:
		drawGame(graphics);
		break;
	case QUIT:
		mspSaveSystem->saveGame();
		mRunningLoop = false;
		break;
	case LOSS_SCREEN:
		mspMenuSystem->draw(mspGraphicsSystem, LOSE);
		break;
	case DYING:
		drawGame(graphics);
		mspGraphicsSystem->drawSprite(mspPlayer->getSpriteDestroyed(), mspPlayer->getPosition());
		break;
	default:
		cout << "Game attempted to draw to an invalid state" << endl;
		break;
	}

	mspGraphicsSystem->flipDisplay();
}

void Game::update(const float frameLength)
{
	switch (mCurrentState)
	{
	case SPLASH:
		updateSplash(frameLength);
		break;
	case MAIN_MENU:
		mspMenuSystem->update(frameLength, MAIN);
		break;
	case OPTIONS_MENU:
		mspMenuSystem->update(frameLength, OPTIONS);
		break;
	case PAUSE_MENU:
		mspMenuSystem->update(frameLength, PAUSE);
		break;
	case CREDITS_MENU:
		mspMenuSystem->update(frameLength, CREDITS);
		break;
	case PLAY:
		updateGame(frameLength);
		break;
	case QUIT:
		mRunningLoop = false;
		break;
	case LOSS_SCREEN:
		resetGame();
		mspMenuSystem->update(frameLength, LOSE);
		break;
	case DYING:
		if (incrementTimer())
		{
			// Reset Invader positions now
			for (unsigned int i = 0; i < mspEnemyManager->getInvaders().size(); ++i)
			{
				mspEnemyManager->getInvaders()[i]->restoreOriginalPosition();
			}

			setState(PLAY);
		}
		else
		{
			mspEnemyManager->updateDeath(frameLength);
		}
		break;
	default:
		cout << "Game attempted to update to an invalid state" << endl;
		break;
	}
}

// The 10% of the code that's being run 90% of the time
void Game::runGameLoop(int targetFrameRate)
{
	const float frameLength = 1.0f / (float)targetFrameRate;
	mCurrentFps = targetFrameRate;

	mGameTimer.start();

	mRunningLoop = true;

	while (mRunningLoop)
	{
		// Time Management
		mCurrentTime = (float)(mGameTimer.getElapsedTime() / 1000.0f);
		mElapsedTime = mCurrentTime - mPreviousTime;

		// Stop adding time for the game when not in Play
		if (mCurrentState == PLAY)
			mLagTime += mElapsedTime;

		// FrameRate
		mFpsUpdateTime += mElapsedTime;

		if (mFpsUpdateTime >= 1.0f)
		{
			mFpsUpdateTime -= 1.0f;
			mCurrentFps = mFrameCounter;
			mFrameCounter = 0;
		}

		//--------------Get input--------------//
		mspInputManager->update();

		//---------------Update---------------//
		update(frameLength);

		//----------------Draw----------------//
		draw(mspGraphicsSystem);

		// FrameRate
		++mFrameCounter;

		//-------Sleep Until Next Frame-------//
		mEndFrameTime = (float)(mGameTimer.getElapsedTime() / 1000.0f);
		if (mEndFrameTime - mCurrentTime < frameLength)
		{
			// If things don't run smoothly, we can scrap this conversion. We lose .5ms worth of accuracy, but I hate compiler warnings
			Uint32 tmpU32 = (Uint32)(((mCurrentTime + frameLength) - mEndFrameTime) * 1000.0f);
			SDL_Delay(tmpU32);
		}

		mPreviousTime = mCurrentTime;
	}

	mGameTimer.stop();
}