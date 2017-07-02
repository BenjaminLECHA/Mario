#ifndef PROJECT_MENU_HPP
#define PROJECT_MENU_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

#define MAX_LEVEL 2

class KeyboardInput;
class Tilemap;
class Character;
class GameSound;

class Menu {
private:
    // Variables du menu
    bool onMenu;
    sf::Texture screenTex;
    sf::Sprite screen;
    sf::Font font;
    sf::Text text;
    sf::Text noms;

public:
    Menu();
    ~Menu();

    bool getOnMenu() const;
    void setOnMenu(bool v);

    void updateMenu(KeyboardInput &input, Tilemap &map, Character &chara, GameSound &sound);
    void drawMenu(sf::RenderWindow &window, Tilemap &map);
};

#endif //PROJECT_MENU_HPP
