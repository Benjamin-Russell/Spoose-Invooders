#ifndef MENUSYSTEM_H
#define MENUSYSTEM_H

#include "GraphicsSystem.h"
#include "Font.h"
#include "Color.h"
#include <Trackable.h>
#include <vector>
#include <string>

using namespace std;

enum MenuType
{
	MAIN,
	OPTIONS,
	PAUSE,
	LOSE,
	CREDITS
};
class MenuSystem : public Trackable
{
public:
	MenuSystem();
	~MenuSystem();

	void draw(GraphicsSystem* graphics, MenuType type);
	void update(const float frameTime, MenuType type);

	// Public interface for controlling menu
	void menuDown();
	void menuUp();
	void selectCurrentOption();

	void cleanup();

private:
	vector<string> mMainMenuOptions;
	vector<string> mOptionsMenuOptions;
	vector<string> mPauseMenuOptions;
	vector<string> mCreditsMenuOptions;
	vector<string> mLoseMenuOptions;

	const string MAIN_MENU_TITLE = "Spoose Invooders";
	const string PAUSE_MENU_TITLE = "Game Paused";
	const string OPTIONS_MENU_TITLE = "Options";
	const string CREDITS_MENU_TITLE = "Created By";
	const string LOSE_MENU_TITLE = "Game Over";

	const int Y_PADDING = 64;
	const string CREDITS_MESSAGE = "Ben Russell & Stevie Ulfelder";

	int mSelectedIndex, mOldIndex;

	// TODO: add m, p in front of these

	Font* mpHeaderFont;
	Font* mpMainFont;

	Color mBackgroundColor;
	Color mUnselectedColor;
	Color mSelectedColor;
	Color mDrawColor;

	void toggleDrawColor();

	int mFrameNum;
};
#endif