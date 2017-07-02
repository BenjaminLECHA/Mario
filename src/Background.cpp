#ifndef PROJECT_BACKGROUND_CPP
#define PROJECT_BACKGROUND_CPP

#include "Background.hpp"

// Constructeur de map
Background::Background() {
    // Background
    if (!backgroundTex.loadFromFile("graphics/background.png"))
        // Problème
        std::cerr << "Problème lors du chargement du background." << std::endl;
    else
        background.setTexture(backgroundTex);
}

// Méthode pour construire le background
void Background::buildBackground(RenderWindow &window) {
    window.draw(background);
}

#endif