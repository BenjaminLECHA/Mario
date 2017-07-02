#ifndef PROJECT_GAME_HPP
#define PROJECT_GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cstdlib>

#include "KeyboardInput.hpp"
#include "Background.hpp"
#include "Tilemap.hpp"
#include "HUD.hpp"
#include "Character.hpp"
#include "GameSound.hpp"
#include "Goomba.hpp"
#include "Koopa.hpp"
#include "Menu.hpp"

#define MAX_MONSTERS 15

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 480

using namespace sf;


class Game {
private:
    RenderWindow    window;
    KeyboardInput   input;
    Background      background;
    Tilemap         tilemap;
    Character       character;
    HUD             hud;
    GameSound       gameSound;
    Goomba          goomba[MAX_MONSTERS];
    Koopa           koopa[MAX_MONSTERS];
    Menu            menu;

public:
    Game();

    void update();

    void render();

    void processEvents();

    void run();

    ~Game();
};

#endif //PROJECT_GAME_HPP
