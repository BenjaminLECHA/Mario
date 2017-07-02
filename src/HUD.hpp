#ifndef PROJECT_HUD_HPP
#define PROJECT_HUD_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <bits/basic_string.h>

using namespace sf;

class Character;
class Tilemap;

class HUD {
private:
    Texture livesTex;
    Sprite lives;

    Texture coinsTex;
    Sprite coins;

    Texture monsterTex;
    Sprite monster;

    Font font;

public:
    HUD();

    void drawText(RenderWindow &window, std::string message, unsigned int size, int x, int y, Color color);

    void drawHud(Character &character, RenderWindow &window, Tilemap &map);

};

#endif //PROJECT_HUD_HPP
