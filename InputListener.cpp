#include "InputListener.h"
#include "Game.h"

int InputListener::msID = 0;

InputListener::InputListener() : EventListener()
{
	mID = msID;
	msID++;

	// Add this listener to each input event in the Event System
	gpEventSystem->addListener(LEFT_PRESSED, this);
	gpEventSystem->addListener(LEFT_HELD, this);
	gpEventSystem->addListener(LEFT_RELEASED, this);
	gpEventSystem->addListener(RIGHT_PRESSED, this);
	gpEventSystem->addListener(RIGHT_HELD, this);
	gpEventSystem->addListener(RIGHT_RELEASED, this);
	gpEventSystem->addListener(UP_PRESSED, this);
	gpEventSystem->addListener(UP_HELD, this);
	gpEventSystem->addListener(DOWN_PRESSED, this);
	gpEventSystem->addListener(DOWN_HELD, this);
	gpEventSystem->addListener(ESCAPE_PRESSED, this);
	gpEventSystem->addListener(ENTER_PRESSED, this);
	gpEventSystem->addListener(SPACE_HELD, this);
	gpEventSystem->addListener(B_PRESSED, this);
	gpEventSystem->addListener(NUM_1_PRESSED, this);
	gpEventSystem->addListener(NUM_2_PRESSED, this);
	gpEventSystem->addListener(NUM_3_PRESSED, this);
	gpEventSystem->addListener(NUM_4_PRESSED, this);
}

InputListener::~InputListener()
{

}

void InputListener::handleEvent(const Event &theEvent)
{
	// First select Game State, then check the key
	switch (Game::getInstance()->getState())
	{
	case SPLASH:
		break;
	case MAIN_MENU:
		switch (theEvent.getType())
		{
		case ESCAPE_PRESSED:
			Game::getInstance()->setState(QUIT);
			break;
		case ENTER_PRESSED:
			Game::getInstance()->getMenuSystem()->selectCurrentOption();
			break;
		case RIGHT_PRESSED:
			Game::getInstance()->getMenuSystem()->selectCurrentOption();
			break;
		case UP_PRESSED:
			Game::getInstance()->getMenuSystem()->menuUp();
			break;
		case DOWN_PRESSED:
			Game::getInstance()->getMenuSystem()->menuDown();
			break;
		}
		break;
	case OPTIONS_MENU:
		switch (theEvent.getType())
		{
		case ENTER_PRESSED:
			Game::getInstance()->getMenuSystem()->selectCurrentOption();
			break;
		case RIGHT_PRESSED:
			Game::getInstance()->getMenuSystem()->selectCurrentOption();
			break;
		case UP_PRESSED:
			Game::getInstance()->getMenuSystem()->menuUp();
			break;
		case DOWN_PRESSED:
			Game::getInstance()->getMenuSystem()->menuDown();
			break;
		}
		break;
	case PLAY:
		switch (theEvent.getType())
		{
		case ESCAPE_PRESSED:
			Game::getInstance()->setState(PAUSE_MENU);
			break;
		case LEFT_HELD:
			Game::getInstance()->getPlayer()->addMove(-1);
			break;
		case RIGHT_HELD:
			Game::getInstance()->getPlayer()->addMove(1);
			break;
		case LEFT_RELEASED:
			Game::getInstance()->getPlayer()->killAnimation();
			break;
		case RIGHT_RELEASED:
			Game::getInstance()->getPlayer()->killAnimation();
			break;
		case UP_HELD:
			Game::getInstance()->getPlayer()->fire();
			break;
		case DOWN_PRESSED:
			Game::getInstance()->getPlayer()->pickUpWeapon();
			break;
		case B_PRESSED:
			Game::getInstance()->toggleDrawingBoxes();
			break;
		case NUM_1_PRESSED:
			Game::getInstance()->getPlayer()->cheatWeapon(0);
			break;
		case NUM_2_PRESSED:
			Game::getInstance()->getPlayer()->cheatWeapon(1);
			break;
		case NUM_3_PRESSED:
			Game::getInstance()->getPlayer()->cheatWeapon(2);
			break;
		case NUM_4_PRESSED:
			Game::getInstance()->getPlayer()->cheatWeapon(3);
			break;
		}
		break;
	case PAUSE_MENU:
		switch (theEvent.getType())
		{
		case ESCAPE_PRESSED:
			Game::getInstance()->setState(PLAY);
			break;
		case ENTER_PRESSED:
			Game::getInstance()->getMenuSystem()->selectCurrentOption();
			break;
		case RIGHT_PRESSED:
			Game::getInstance()->getMenuSystem()->selectCurrentOption();
			break;
		case UP_PRESSED:
			Game::getInstance()->getMenuSystem()->menuUp();
			break;
		case DOWN_PRESSED:
			Game::getInstance()->getMenuSystem()->menuDown();
			break;
		}
		break;
	case CREDITS_MENU: case LOSS_SCREEN:
		switch (theEvent.getType())
		{
		case ENTER_PRESSED:
			Game::getInstance()->getMenuSystem()->selectCurrentOption();
			break;
		case RIGHT_PRESSED:
			Game::getInstance()->getMenuSystem()->selectCurrentOption();
			break;
		}
	}
}