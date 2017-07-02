#ifndef PROJECT_KEYBOARDINPUT_CPP
#define PROJECT_KEYBOARDINPUT_CPP

#include "KeyboardInput.hpp"

// Constructeur, on initialise toutes les entrées à faux
KeyboardInput::KeyboardInput() {
    button.left = false;
    button.right = false;
    button.up = false;
    button.down = false;
    button.jump = false;
    button.enter = false;
}

// Accesseur des entrées
KeyboardInput::Button KeyboardInput::getButton() const {
    return button;
}

// Pour gérer le clavier
void KeyboardInput::inputManagement(RenderWindow &window) { getKeyboardInput(window); }

// Permet de récupérer les input depuis la fenêtre de jeu
void KeyboardInput::getKeyboardInput(RenderWindow &window) {
    // Boucle des événements
    while (window.pollEvent(event)) {
        // Gestion des différents cas d'événements
        switch (event.type) {
            // Si on ferme la fenêtre
            case Event::Closed:
                window.close();
                break;

                // Si on appuie sur une touche du clavier
            case Event::KeyPressed:
                // On regarde sur quelle touche on a appuyé
                switch (event.key.code) {
                    case Keyboard::Space:
                        button.jump = true;
                        break;

                    case Keyboard::Up:
                        button.up = true;
                        break;

                    case Keyboard::Down:
                        button.down = true;
                        break;

                    case Keyboard::Left:
                        button.left = true;
                        break;

                    case Keyboard::Right:
                        button.right = true;
                        break;

                    case Keyboard::Return:
                        button.enter = true;
                        break;

                    default:
                        break;
                }
                break;

                // Si on relâche une touche du clavier
            case Event::KeyReleased:
                // On regarde quelle touche a été relâchée
                switch (event.key.code) {
                    case Keyboard::Space:
                        button.jump = false;
                        break;

                    case Keyboard::Up:
                        button.up = false;
                        break;

                    case Keyboard::Down:
                        button.down = false;
                        break;

                    case Keyboard::Left:
                        button.left = false;
                        break;

                    case Keyboard::Right:
                        button.right = false;
                        break;

                    case Keyboard::Return:
                        button.enter = false;
                        break;

                    default:
                        break;
                }
                break;
                // Pas d'autre événement
            default:
                break;
        }
    }
}

#endif