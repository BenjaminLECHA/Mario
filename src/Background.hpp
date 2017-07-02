#ifndef PROJECT_BACKGROUND_HPP
#define PROJECT_BACKGROUND_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <bits/basic_string.h>

using namespace sf;

class Background {
private:

    // Background de la map
    Texture backgroundTex;
    Sprite background;

public:

    // Constructeur de map
    Background();

    // Méthode pour construire le background
    void buildBackground(RenderWindow &window);

};

#endif //PROJECT_BACKGROUND_HPP
