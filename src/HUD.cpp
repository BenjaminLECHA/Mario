#ifndef PROJECT_HUD_CPP
#define PROJECT_HUD_CPP

#include "HUD.hpp"
#include "Tilemap.hpp"
#include "Character.hpp"

HUD::HUD() {
    // Chargement de la police du jeu
    if (!font.loadFromFile("font/mario.ttf"))
        // Erreur
        std::cerr << "Problème lors du chargement de la police." << std::endl;

    // Chargement du HUD
    if (!coinsTex.loadFromFile("graphics/coins.png"))
        // Erreur
        std::cerr << "Problème lors du chargement du HUD (coins)." << std::endl;
    else
        coins.setTexture(coinsTex);

    if (!livesTex.loadFromFile("graphics/lives.png"))
        // Erreur
        std::cerr << "Problème lors du chargement du HUD (lives)." << std::endl;
    else
        lives.setTexture(livesTex);

    if (!monsterTex.loadFromFile("graphics/monster.png"))
        // Erreur
        std::cerr << "Problème lors du chargement du HUD (monsters)." << std::endl;
    else
        monster.setTexture(monsterTex);
}

void HUD::drawText(RenderWindow &window, std::string message, unsigned int size, int x, int y, Color color)
{
    Text text;

    // Police
    text.setFont(font);

    // Chaîne de caractères à afficher
    text.setString(message);

    // Taille des caractères (pixels)
    text.setCharacterSize(size);

    // Position du texte
    text.setPosition(x, y);

    // Couleur
    text.setColor(color);

    window.draw(text);
}

void HUD::drawHud(Character &character, RenderWindow &window, Tilemap &map) {
    // Nombre de vies en haut à gauche
    lives.setPosition(10, 10);
    window.draw(lives);
    drawText(window, std::to_string(character.getLives()), 25, 50, 10, Color::Black);

    coins.setPosition(100, 10);
    window.draw(coins);
    drawText(window, std::to_string(character.getCoins()), 25, 140, 10, Color::Black);

    monster.setPosition(190, 10);
    window.draw(monster);
    drawText(window, std::to_string(map.getTotalMonsters()), 25, 250, 10, Color::Black);

}

#endif