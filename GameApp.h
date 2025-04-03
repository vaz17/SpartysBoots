/**
* @file GameApp.h
 * @author Matthew Vazquez
 * Main application class for initializing the game application.
 *
 * The GameApp class is responsible for initializing the game environment and managing
 * the application lifecycle.
 */

#ifndef GAMEAPP_H
#define GAMEAPP_H

#include <wx/wx.h>

/**
 * Class representing the main game application.
 *
 * GameApp is a wxWidgets application class that manages the initialization and
 * execution of the game. It overrides the `OnInit` method to set up the game environment.
 */
class GameApp : public wxApp {
public:
 /**
  * Initializes the game application.
  *
  * This method is called when the application is launched and is responsible for
  * setting up the main game window and environment.
  */
 bool OnInit() override;
};

#endif // GAMEAPP_H
