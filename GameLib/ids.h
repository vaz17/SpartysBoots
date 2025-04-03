/**
* @file ids.h
 *
 * @author matthew vazquez
 *
 * This file contains unique IDs for menu commands used in the game application.
 */

#ifndef IDS_H
#define IDS_H

#include <wx/wx.h>

/**
 * Enumeration for unique menu item IDs.
 *
 * This enumeration defines unique IDs for various menu commands in the game application.
 * Each ID is associated with a specific action or menu item.
 */
enum IDs {
 // View menu
 IDM_CONTROLPOINTS = wxID_HIGHEST + 1,

 // Levels menu
 IDM_LEVEL0,
 IDM_LEVEL1,
 IDM_LEVEL2,
 IDM_LEVEL3,
 IDM_LEVEL4,
 IDM_LEVEL5,
 IDM_LEVEL6,
 IDM_LEVEL7,
 IDM_LEVEL8,

 // Gates menu
 IDM_ANDGATE,
 IDM_ORGATE,
 IDM_NOTGATE,
 IDM_SRFLIPFLOP,
 IDM_DFLIPFLOP,

 // Timers
 IDM_GAME_TIMER
};

#endif //IDS_H
