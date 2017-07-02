#ifndef PROJECT_TILEMAP_HPP
#define PROJECT_TILEMAP_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <bits/basic_string.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 480

// Nombre de tuiles sur 1 niveau, puis nombre de tuiles ligne/colonne du tileset
#define MAX_X 100
#define MAX_Y 15
#define X_NUMBER 10
#define Y_NUMBER 10

#define TILE_SIZE 32
#define TILE_GOOMBA 62
#define TILE_KOOPA 63
#define MAX_MONSTERS 15
using namespace sf;

class Character;
class Goomba;
class Koopa;

class Tilemap {

private:
    // Tileset
    Texture tilesetTex;
    Sprite tileset;

    // Point de début de la création de map
    int mapStartX, mapStartY;

    // Fin de la map
    int mapEndX, mapEndY;

    // Position de départ du personnage
    int characterBeginX, characterBeginY;

    // Tilemap
    int tile1[MAX_Y][MAX_X];

    // Tilemap2
    int tile2[MAX_Y][MAX_X];

    // Numéro du niveau
    int level;

    // Nombre de monstres
    int goombaNumber;
    int koopaNumber;
    int totalMonsters;

public:
    Tilemap();


    // Accesseurs
    int getLevel() const;
    int getCharacterBeginX() const;
    int getCharacterBeginY() const;
    int getMapStartX() const;
    int getMapStartY() const;
    int getMapEndX() const;
    int getMapEndY() const;
    int getTile(int y, int x) const;
    int getMonstersNumber() const;
    int getGoombaNumber() const;
    int getKoopaNumber() const;
    int getTotalMonsters() const;

    // Setteurs
    void setMapStartX(int v);
    void setMapStartY(int v);
    void setTile(int y, int x, int v);
    void setLevel(int v);
    void setGoombaNumber(int v);
    void setKoopaNumber(int v);
    void setTotalMonsters(int v);

    void totalMonsterNumber();

    //###############################//
    // CHARGEMENT D'UNE MAP #########//
    //###############################//
    void loadLevel(std::string filename);

    //###############################//
    // DESSIN D'UNE MAP #########//
    //###############################//
    void drawLevelLayer(int layer, RenderWindow &window, Goomba *goomba, Koopa *koopa);

    // Changer de niveau
    void changeLevel();
};


#endif //PROJECT_TILEMAP_HPP
