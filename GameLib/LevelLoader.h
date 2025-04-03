/**
 * @file LevelLoader.h
 * @author Shawn_Porto
 *
 * Class for loading game levels from Xml files.
 */
 
#ifndef LEVELLOADER_H
#define LEVELLOADER_H

// Forward declaration.
class Game;

/**
 * Objects of this class are responsible for loading levels within the game
 * Loads files and applies them to game instance.
 */
class LevelLoader
{
private:

public:
    void LoadLevel(const wxString &filename, Game* game);
};

#endif //LEVELLOADER_H
