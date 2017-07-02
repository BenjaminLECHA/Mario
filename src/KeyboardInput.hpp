#ifndef PROJECT_KEYBOARDINPUT_HPP
#define PROJECT_KEYBOARDINPUT_HPP

#include <SFML/Graphics.hpp>

using namespace sf;

class KeyboardInput {

private:
    struct Button {
        bool up, down, right, left, jump, enter;
    };

    // Variables pour les événements et les bouttons du jeu
    Event event;
    Button button;

    enum {
        up, down, right, left, jump, enter
    };

public:

    // Constructeur, on initialise toutes les entrées à faux
    KeyboardInput();

    // Accesseur des entrées
    Button getButton() const;

    // Pour gérer le clavier
    void inputManagement(RenderWindow &window);

    // Permet de récupérer les input depuis la fenêtre de jeu
    void getKeyboardInput(RenderWindow &window);

};

#endif //PROJECT_KEYBOARDINPUT_HPP
