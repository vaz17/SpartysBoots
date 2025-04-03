/**
 * @file LevelLoader.cpp
 * @author Shawn_Porto
 */

#include "pch.h"
#include "LevelLoader.h"
#include "Game.h"

/**
 * Load the game from XML file
 *
 * Opens XML file and reads nodes, creating all items.
 *
 * @param filename The filename of the XML file the level is loaded from
 * @param game the pointer to game instance.
 */
void LevelLoader::LoadLevel(const wxString &filename, Game *game)
{
    wxXmlDocument xmlDoc;

    if (!xmlDoc.Load(filename))
    {
        wxMessageBox(L"Unable to load level");
        return;
    }

    game->Clear();

    auto root = xmlDoc.GetRoot();

    game->XmlGame(root);

    auto itemsChild = root->GetChildren();

    auto child = itemsChild->GetChildren();
    for ( ; child; child = child->GetNext())
    {
        game->XmlItem(child);
    }
}