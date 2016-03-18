#include "MenuSystem.h"
#include "Game.h"

MenuSystem::MenuSystem()
{
	if (TTF_Init() == -1)
	{
		cout << "Error initializing TTF addon" << endl;
	}

	mFrameNum = 0;

	mpHeaderFont = new Font("Resources/pixelzim.ttf", 64);
	mpMainFont = new Font("Resources/pixelzim.ttf", 36);

	mSelectedIndex = 0;
	mOldIndex = 0;

	mSelectedColor = Color(255, 255, 255);
	mUnselectedColor = Color(0, 0, 0);
	mBackgroundColor = Color(100, 149, 237);

	mDrawColor = mSelectedColor;

	if (Game::getInstance()->getSaveSystem()->saveFileExists())
	{
		mMainMenuOptions.push_back("Resume");
		mMainMenuOptions.push_back("New Game");
	}
	else
		mMainMenuOptions.push_back("Play");

	mMainMenuOptions.push_back("Options");
	mMainMenuOptions.push_back("Credits");
	mMainMenuOptions.push_back("Quit");

	mPauseMenuOptions.push_back("Resume");
	mPauseMenuOptions.push_back("Options");
	mPauseMenuOptions.push_back("Credits");
	mPauseMenuOptions.push_back("Main Menu");

	mOptionsMenuOptions.push_back("Sound: On");
	mOptionsMenuOptions.push_back("Difficulty: Normal");
	mOptionsMenuOptions.push_back("Return");

	mCreditsMenuOptions.push_back("Return");

	mLoseMenuOptions.push_back("Main Menu");
}

MenuSystem::~MenuSystem()
{
	cleanup();
}

void MenuSystem::cleanup()
{
	mpMainFont->cleanup();
	mpHeaderFont->cleanup();

	TTF_Quit();
	delete mpMainFont;
	delete mpHeaderFont;
}

void MenuSystem::draw(GraphicsSystem* graphics, MenuType type)
{
	int textWidth = 0;
	int headerHeight = 0;
	stringstream ss;

	switch (type)
	{
	case MAIN:
		graphics->clear(mBackgroundColor);
		
		TTF_SizeText(mpHeaderFont->getFont(), MAIN_MENU_TITLE.c_str(), &textWidth, &headerHeight);
		graphics->writeText(Vector2( (float)(graphics->getWidth() / 2 - textWidth / 2), (float)Y_PADDING), *mpHeaderFont, mUnselectedColor, MAIN_MENU_TITLE);

		for (unsigned i = 0; i < mMainMenuOptions.size(); i++)
		{
			TTF_SizeText(mpMainFont->getFont(), mMainMenuOptions[i].c_str(), &textWidth, NULL);
			if (i == mSelectedIndex)
				graphics->writeText(Vector2((float)(graphics->getWidth() / 2 - textWidth / 2), (float)(headerHeight + Y_PADDING * 2 + (i * Y_PADDING))), *mpMainFont, mDrawColor, mMainMenuOptions[i]);
			else{
				graphics->writeText(Vector2((float)(graphics->getWidth() / 2 - textWidth / 2), (float)(headerHeight + Y_PADDING * 2 + (i * Y_PADDING))), *mpMainFont, mUnselectedColor, mMainMenuOptions[i]);
			}
		}

		break;
	case PAUSE:
		graphics->clear(mBackgroundColor);

		TTF_SizeText(mpHeaderFont->getFont(), PAUSE_MENU_TITLE.c_str(), &textWidth, &headerHeight);
		graphics->writeText(Vector2((float)(graphics->getWidth() / 2 - textWidth / 2), (float)Y_PADDING), *mpHeaderFont, mUnselectedColor, PAUSE_MENU_TITLE);

		for (unsigned i = 0; i < mPauseMenuOptions.size(); i++)
		{
			TTF_SizeText(mpMainFont->getFont(), mPauseMenuOptions[i].c_str(), &textWidth, NULL);
			if (i == mSelectedIndex)
				graphics->writeText(Vector2((float)(graphics->getWidth() / 2 - textWidth / 2), (float)(headerHeight + Y_PADDING * 2 + (i * Y_PADDING))), *mpMainFont, mDrawColor, mPauseMenuOptions[i]);
			else{
				graphics->writeText(Vector2((float)(graphics->getWidth() / 2 - textWidth / 2), (float)(headerHeight + Y_PADDING * 2 + (i * Y_PADDING))), *mpMainFont, mUnselectedColor, mPauseMenuOptions[i]);
			}
		}
		break;
	case OPTIONS:
		graphics->clear(mBackgroundColor);

		TTF_SizeText(mpHeaderFont->getFont(), OPTIONS_MENU_TITLE.c_str(), &textWidth, &headerHeight);
		graphics->writeText(Vector2((float)(graphics->getWidth() / 2 - textWidth / 2), (float)Y_PADDING), *mpHeaderFont, mUnselectedColor, OPTIONS_MENU_TITLE);

		for (unsigned i = 0; i < mOptionsMenuOptions.size(); i++)
		{
			TTF_SizeText(mpMainFont->getFont(), mOptionsMenuOptions[i].c_str(), &textWidth, NULL);
			if (i == mSelectedIndex)
				graphics->writeText(Vector2((float)(graphics->getWidth() / 2 - textWidth / 2), (float)(headerHeight + Y_PADDING * 2 + (i * Y_PADDING))), *mpMainFont, mDrawColor, mOptionsMenuOptions[i]);
			else{
				graphics->writeText(Vector2((float)(graphics->getWidth() / 2 - textWidth / 2), (float)(headerHeight + Y_PADDING * 2 + (i * Y_PADDING))), *mpMainFont, mUnselectedColor, mOptionsMenuOptions[i]);
			}
		}
		break;
	case CREDITS:
		graphics->clear(mBackgroundColor);

		TTF_SizeText(mpHeaderFont->getFont(), CREDITS_MENU_TITLE.c_str(), &textWidth, &headerHeight);
		graphics->writeText(Vector2((float)(graphics->getWidth() / 2 - textWidth / 2), (float)Y_PADDING), *mpHeaderFont, mUnselectedColor, CREDITS_MENU_TITLE);

		TTF_SizeText(mpMainFont->getFont(), mCreditsMenuOptions[0].c_str(), &textWidth, NULL);
		graphics->writeText(Vector2((float)(graphics->getWidth() / 2 - textWidth / 2), (float)(headerHeight + Y_PADDING * 4 + (Y_PADDING))), *mpMainFont, mDrawColor, mCreditsMenuOptions[0]);

		TTF_SizeText(mpMainFont->getFont(), CREDITS_MESSAGE.c_str(), &textWidth, NULL);
		graphics->writeText(Vector2((float)(graphics->getWidth() / 2 - textWidth / 2), (float)(headerHeight + Y_PADDING) * 1.5f), *mpMainFont, mUnselectedColor, CREDITS_MESSAGE);
		break;
	case LOSE:
		graphics->clear(mBackgroundColor);

		TTF_SizeText(mpHeaderFont->getFont(), LOSE_MENU_TITLE.c_str(), &textWidth, &headerHeight);
		graphics->writeText(Vector2((float)(graphics->getWidth() / 2 - textWidth / 2), (float)Y_PADDING), *mpHeaderFont, mUnselectedColor, LOSE_MENU_TITLE);

		TTF_SizeText(mpMainFont->getFont(), mLoseMenuOptions[0].c_str(), &textWidth, NULL);
		graphics->writeText(Vector2((float)(graphics->getWidth() / 2 - textWidth / 2), (float)(headerHeight + Y_PADDING * 4 + (Y_PADDING))), *mpMainFont, mDrawColor, mLoseMenuOptions[0]);

		ss << "SCORE: " << Game::getInstance()->getScore();
		TTF_SizeText(mpMainFont->getFont(), (ss.str()).c_str(), &textWidth, NULL);
		graphics->writeText(Vector2((float)(graphics->getWidth() / 2 - textWidth / 2), (float)Y_PADDING * 3.5f), *mpMainFont, mUnselectedColor, ss.str());
		break;
	default:
		cout << "Attempted to draw an invalid menu" << endl;
		break;
	}
}

void MenuSystem::toggleDrawColor()
{
	if (mDrawColor.equals(0, 0, 0))
		mDrawColor = mSelectedColor;
	else
		mDrawColor = mUnselectedColor;
}

void MenuSystem::menuDown()
{
	if (Game::getInstance()->getState() == GameState::CREDITS_MENU)
		return;

	Audio *audio = AudioLocator::locate();
	audio->playSound(MENU_SCROLL);

	mSelectedIndex++;
}

void MenuSystem::menuUp()
{
	if (Game::getInstance()->getState() == GameState::CREDITS_MENU)
		return;

	Audio *audio = AudioLocator::locate();
	audio->playSound(MENU_SCROLL);

	if (mSelectedIndex > 0)
		mSelectedIndex--;
	else
	{
		switch (Game::getInstance()->getState())
		{
		case MAIN_MENU:
			mSelectedIndex = mMainMenuOptions.size() - 1;
			break;
		case OPTIONS_MENU:
			mSelectedIndex = mOptionsMenuOptions.size() - 1;
			break;
		case PAUSE_MENU:
			mSelectedIndex = mPauseMenuOptions.size() - 1;
			break;
		}
	}
}

void MenuSystem::selectCurrentOption()
{
	Audio *audio = AudioLocator::locate();
	audio->playSound(MENU_SELECT);

	string selection;
	if (Game::getInstance()->getState() == MAIN_MENU)
	{
		selection = mMainMenuOptions[mSelectedIndex];

		if (selection == "Resume")
		{
			Game::getInstance()->getSaveSystem()->loadGame();
			Game::getInstance()->setState(PLAY);
		}
		if (selection == "Play" || selection == "New Game")
		{
			Game::getInstance()->resetGame();
			Game::getInstance()->setState(PLAY);
			Game::getInstance()->setLevel(0);
			Game::getInstance()->setScore(0);
		}
		else if (selection == "Options")
		{
			mOldIndex = mSelectedIndex;
			mSelectedIndex = 0;
			Game::getInstance()->pushStateToStack(MAIN_MENU);
			Game::getInstance()->setState(OPTIONS_MENU);
		}
		else if (selection == "Credits")
		{
			mOldIndex = mSelectedIndex;
			mSelectedIndex = 0;
			Game::getInstance()->pushStateToStack(MAIN_MENU);
			Game::getInstance()->setState(CREDITS_MENU);
		}
		else if (selection == "Quit")
		{
			Game::getInstance()->setState(QUIT);
		}

	}
	else if (Game::getInstance()->getState() == PAUSE_MENU)
	{
		selection = mPauseMenuOptions[mSelectedIndex];
		if (selection == "Resume")
		{
			Game::getInstance()->setState(PLAY);
		}
		else if (selection == "Options")
		{
			mOldIndex = mSelectedIndex;
			mSelectedIndex = 0;
			Game::getInstance()->pushStateToStack(PAUSE_MENU);
			Game::getInstance()->setState(OPTIONS_MENU);
		}
		else if (selection == "Credits")
		{
			mOldIndex = mSelectedIndex;
			mSelectedIndex = 0;
			Game::getInstance()->pushStateToStack(PAUSE_MENU);
			Game::getInstance()->setState(CREDITS_MENU);
		}
		else if (selection == "Main Menu")
		{
			Game::getInstance()->getSaveSystem()->saveGame();
			//Game::getInstance()->resetGame();
			Game::getInstance()->setState(MAIN_MENU);
			mSelectedIndex = 0;
		}
	}
	else if (Game::getInstance()->getState() == OPTIONS_MENU)
	{

		selection = mOptionsMenuOptions[mSelectedIndex];
		if (selection.substr(0, 1) == "S")
		{
			if(mOptionsMenuOptions[mSelectedIndex] == "Sound: On")
			{
				mOptionsMenuOptions[mSelectedIndex] = "Sound: Off";
				AudioLocator::provide(NULL);
			}
			else
			{
				mOptionsMenuOptions[mSelectedIndex] = "Sound: On";
				AudioService *service = new AudioService();
				AudioLocator::provide(service);
			}

		}
		else if (selection == "Difficulty: " + DIFFICULTY_STRINGS[Game::getInstance()->getDifficulty()])
		{
			Game::getInstance()->cycleDifficulty();
		}
		else if (selection == "Return")
		{
			mSelectedIndex = mOldIndex;
			mOldIndex = 0;
			Game::getInstance()->setState(Game::getInstance()->popStateFromStack());
		}
	}
	else if (Game::getInstance()->getState() == CREDITS_MENU)
	{
		mSelectedIndex = mOldIndex;
		mOldIndex = 0;
		Game::getInstance()->setState(Game::getInstance()->popStateFromStack());
	}
	else if (Game::getInstance()->getState() == LOSS_SCREEN)
	{
		mSelectedIndex = 0;
		mOldIndex = 0;
		Game::getInstance()->setState(MAIN_MENU);
		Game::getInstance()->setScore(0);
	}
}

void MenuSystem::update(const float frameTime, MenuType type)
{
	if (!Game::getInstance()->getSaveSystem()->saveFileExists())
	{
		if (mMainMenuOptions[0] == "Resume")
		{
			mMainMenuOptions.erase(mMainMenuOptions.begin());
			mMainMenuOptions[0] = "Play";
		}
	}
	else
	{
		if (mMainMenuOptions[0] == "Play")
		{
			mMainMenuOptions.insert(mMainMenuOptions.begin(), "Resume");
			mMainMenuOptions[1] = "New Game";
		}
	}

	mFrameNum++;

	if (mFrameNum % 25 == 0)
	{
		toggleDrawColor();
		
		mFrameNum = 0;
	}
	switch (type)
	{
	case MAIN:
		if (mSelectedIndex >= (int)mMainMenuOptions.size())
			mSelectedIndex = 0;
		break;
	case PAUSE:
		if (mSelectedIndex >= (int)mPauseMenuOptions.size())
			mSelectedIndex = 0;
		break;
	case OPTIONS:
		mOptionsMenuOptions[1] = "Difficulty: " + DIFFICULTY_STRINGS[Game::getInstance()->getDifficulty()];
		if (mSelectedIndex >= (int)mOptionsMenuOptions.size())
			mSelectedIndex = 0;
		break;
	case CREDITS:
		if (mSelectedIndex != 0)
			mSelectedIndex = 0;
		break;
	case LOSE:
		if (mSelectedIndex != 0)
			mSelectedIndex = 0;
		break;
	default:
		cout << "Attempted to update an invalid menu" << endl;
		break;
	}
}