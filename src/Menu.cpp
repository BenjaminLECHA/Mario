#ifndef PROJECT_MENU_CPP
#define PROJECT_MENU_CPP

#include "Menu.hpp"
#include "KeyboardInput.hpp"
#include "Tilemap.hpp"
#include "Character.hpp"
#include "GameSound.hpp"

using namespace std;

Menu::Menu() {
    // Image menu
    if(!screenTex.loadFromFile("graphics/menu.png")) 
        cerr << "Problème lors du chargement du menu" << endl;
    else
        screen.setTexture(screenTex);

    // Chargement de la police du jeu
    if (!font.loadFromFile("font/mario.ttf"))
        // Erreur
        std::cerr << "Problème lors du chargement de la police." << std::endl;

    onMenu = true;
}

bool Menu::getOnMenu() const { return onMenu; }
void Menu::setOnMenu(bool v) { onMenu = v; }

Menu::~Menu() {}

void Menu::updateMenu(KeyboardInput &input, Tilemap &map, Character &chara, GameSound &sound) {
    if(input.getButton().enter) {
        // On commence au premier niveau
        map.setLevel(1);
        map.changeLevel();

        // Personnage à 5 vies
        chara.initialize(map);
        chara.setCoins(0);
        chara.setLives(5);

        // Son
        sound.playBackgroundMusic();
        onMenu = false;
    }
}
void Menu::drawMenu(sf::RenderWindow &window, Tilemap &map) {
    window.draw(screen);
    text.setFont(font);
    text.setString("Appuyez sur entree pour jouer");
    text.setPosition(100, 325);
    text.setColor(sf::Color::White);
    
    noms.setFont(font);
    noms.setString("Geoffrey Baudelet\nBenjamin Lecha\nCoralie Le Foll\nPierre Kurzawski\nGaetan Pouilly");
    noms.setPosition(70, 5);
    noms.setColor(sf::Color::Black);
    noms.setCharacterSize(12);

    window.draw(text);
    window.draw(noms);
}

#endif
