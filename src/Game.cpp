#ifndef PROJECT_GAME_CPP
#define PROJECT_GAME_CPP

#include "Game.hpp"

Game::Game() : window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mario Game") {
    // Création de la fenêtre du jeu
    // Limite 60FPS et synchro verticale
    window.setFramerateLimit(60);
}

void Game::update() {
    if(menu.getOnMenu()) {
        menu.updateMenu(input, tilemap, character, gameSound);
    }
    else {
        // Update des infos du personnage
        // Si le personnage est mort on retire les monstres qui eexistaient déjà
        if (character.updateCharacter(input, tilemap, gameSound, goomba, koopa) > 0) {
            tilemap.setGoombaNumber(0);
            tilemap.setKoopaNumber(0);
        }
        // Update des goomba
        for (int i = 0; i < tilemap.getGoombaNumber(); i++) {
            if ( goomba[i].updateGoomba(tilemap, character, gameSound) == 1) {
                // Cas où le goomba doit mourir
                // On copie à sa place le dernier monstre avant d'en retirer un
                goomba[i].copy(goomba[tilemap.getGoombaNumber() - 1]);
                tilemap.setGoombaNumber(tilemap.getGoombaNumber() - 1);
                tilemap.setTotalMonsters(tilemap.getTotalMonsters() - 1);
                if (tilemap.getTotalMonsters() == 0)
                    character.nextLevel(tilemap, gameSound);
            }
        }
        // Idem pour les koopa
        for (int i = 0; i < tilemap.getKoopaNumber(); i++) {
            for (int j = 0; j < tilemap.getGoombaNumber(); j++) {
                    koopa[i].mapCollideGoomba(tilemap, goomba[j]);
            }
            int tmp = tilemap.getKoopaNumber() - 1;
            if(tmp >= 1 && tmp != i) {
                koopa[i].mapCollideKoopa(tilemap, koopa[tmp]);
                koopa[tmp].mapCollideKoopa(tilemap, koopa[i]);
            }
            if (koopa[i].updateKoopa(tilemap, character, gameSound) == 1) {
                koopa[i].copy(koopa[tilemap.getKoopaNumber() - 1]);
                tilemap.setKoopaNumber(tilemap.getKoopaNumber() - 1);
                tilemap.setTotalMonsters(tilemap.getTotalMonsters() - 1);
                if (tilemap.getTotalMonsters() == 0)
                    character.nextLevel(tilemap, gameSound);
            }
        }

        if(character.getLives() == 0) {
            menu.setOnMenu(true);
            tilemap.setTotalMonsters(0);
        }
    }
}

void Game::render() {
    // Création des graphismes
    window.clear();
    if(menu.getOnMenu()) {
        menu.drawMenu(window, tilemap);
    }
    else {
        // Background
        background.buildBackground(window);

        // Deuxième couche et première couche du level
        tilemap.drawLevelLayer(2, window, goomba, koopa);
        tilemap.drawLevelLayer(1, window, goomba, koopa);

        // Personnage
        character.drawCharacter(tilemap, window);

        // HUD
        hud.drawHud(character, window, tilemap);

        for (int i = 0; i < tilemap.getGoombaNumber(); i++)
            goomba[i].drawGoomba(tilemap, window);
        for (int i = 0; i < tilemap.getKoopaNumber(); i++)
            koopa[i].drawKoopa(tilemap, window);
    }

    window.display();

}

void Game::processEvents() {
    // Gestion des inputs
    input.inputManagement(window);
}

void Game::run() {
    // On initialise les informations
    //initialize();
    // Boucle principale du jeu, ne se termine pas tant que la fenêtre est ouverte
    while (window.isOpen()) {
        processEvents();
        render();
        update();
    }
}

Game::~Game() {}


#endif