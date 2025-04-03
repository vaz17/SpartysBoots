/**
 * @file MainFrame.cpp
 * @author matthew vazquez
 */

#include "pch.h"
#include "MainFrame.h"
#include "GameView.h"
#include "ids.h"
#include <wx/menu.h>

using namespace std;

/**
 * Initialize the MainFrame window.
 */
void MainFrame::Initialize()
{
    Create(nullptr, wxID_ANY, L"Sparty's Boots", wxDefaultPosition, wxSize(1000, 800));

    // Create a sizer that will lay out child windows vertically
    auto sizer = new wxBoxSizer(wxVERTICAL);

    // Create the view class object as a child of MainFrame
    auto gameView = new GameView();
    gameView->Initialize(this);

    // Add it to the sizer
    sizer->Add(gameView, 1, wxEXPAND | wxALL);

    // Set the sizer for this frame
    SetSizer(sizer);

    // Layout (place) the child windows.
    Layout();

    // Create a status bar
    CreateStatusBar(1, wxSTB_SIZEGRIP, wxID_ANY);

    // Create the menu bar
    auto menuBar = new wxMenuBar();

    // Create menus
    auto fileMenu = new wxMenu();
    auto viewMenu = new wxMenu();
    auto helpMenu = new wxMenu();
    auto gatesMenu = new wxMenu();
    auto levelMenu = new wxMenu();

    // Append items to the file menu
    fileMenu->Append(wxID_EXIT, "E&xit\tAlt-X", "Quit this program");

    // Append items to the view menu
    viewMenu->Append(IDM_CONTROLPOINTS, L"&Control Points", L"Toggle Control Points", wxITEM_CHECK);

    // Append items to the help menu
    helpMenu->Append(wxID_ABOUT, "&About\tF1", "Show about dialog");

    // Add gates menu items
    gatesMenu->Append(IDM_ANDGATE, L"&AND");
    gatesMenu->Append(IDM_ORGATE, L"&OR");
    gatesMenu->Append(IDM_NOTGATE, L"&NOT");
    gatesMenu->Append(IDM_SRFLIPFLOP, L"&SR Flip Flop");
    gatesMenu->Append(IDM_DFLIPFLOP, L"&D Flip Flop");

    // Add level menu items
    levelMenu->Append(IDM_LEVEL0, L"Level &0");
    levelMenu->Append(IDM_LEVEL1, L"Level &1");
    levelMenu->Append(IDM_LEVEL2, L"Level &2");
    levelMenu->Append(IDM_LEVEL3, L"Level &3");
    levelMenu->Append(IDM_LEVEL4, L"Level &4");
    levelMenu->Append(IDM_LEVEL5, L"Level &5");
    levelMenu->Append(IDM_LEVEL6, L"Level &6");
   levelMenu->Append(IDM_LEVEL7, L"Level &7");
 levelMenu->Append(IDM_LEVEL8, L"Level &8");

    // Append menus to the menu bar
    menuBar->Append(fileMenu, L"&File");
    menuBar->Append(viewMenu, L"&View");
    menuBar->Append(levelMenu, L"&Level");
    menuBar->Append(gatesMenu, L"&Gates");
    menuBar->Append(helpMenu, L"&Help");

    // Set the menu bar for the frame
    SetMenuBar(menuBar);



    // Bind the menu events to the event handlers
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAbout, this, wxID_ABOUT);
}

/**
 * Event handler for the Exit menu option.
 * @param event The event object.
 */
void MainFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

/**
 * Event handler for the About menu option.
 * @param event The event object.
 */
void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox(L"Welcome to the Sparty Boots Game!",
                 L"About The Game",
                 wxOK,
                 this);
}