/**
* @file GameApp.cpp
* @author matthew vazquez
*/


#include "pch.h"
#include "GameApp.h"
#include "GameLib/MainFrame.h"


/**
* Initializes the application.
*
* This method is called when the application starts.
* It performs the necessary initialization steps for the application, including:
* - Initializing image handlers
* - Creating and initializing the MainFrame
*
* @return True if the initialization was successful and the application can
* continue running, false otherwise. If false is returned, the application
* will terminate.
*/
bool GameApp::OnInit()
{
 if (!wxApp::OnInit())
  return false;
 wxInitAllImageHandlers();
 // Create and initialize MainFrame
 auto mainFrame = new MainFrame();
 mainFrame->Initialize();
 mainFrame->Show(true);


 return true;
}
