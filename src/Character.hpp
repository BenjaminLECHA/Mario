#ifndef PROJECT_CHARACTER_HPP
#define PROJECT_CHARACTER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>


// Pour les animations
#define FRAME_TIMER 5

// Taille du sprite de Mario
#define CHARACTER_WIDTH 26
#define CHARACTER_HEIGHT 50

// Vitesse
#define CHARACTER_SPEED 4
#define CLIMBING_SPEED 3
// Gravité
#define GRAVITY_SPEED 0.45

// Les états + la direction
#define IDLE 0
#define WALK 1
#define JUMP 2
#define CLIMB 3

#define RIGHT 1
#define LEFT 2

#define JUMP_HEIGHT 11
#define MAX_FALLING_SPEED 11
#define MAX_JUMPING_SPEED -11

#define BLANK_TILE 0
#define POWERUP_COIN 47
#define POWERUP_MUSHROOM 57
#define ITEM_BLOCK 11
#define NORMAL_BLOCK 60
#define LADDER 61

#define CAMERA_SPEED 4

#define LEVEL_MAX 2

using namespace sf;

class Tilemap;
class GameSound;
class KeyboardInput;
class Goomba;
class Koopa;


class Character {

private:
    Texture characterTex;
    Sprite character;

    // Les variables joueur
    int lives;
    int coins; // 100 pièces : 1 vie

    // Coordonnées du personnage (point haut gauche du sprite)
    int x, y;

    // Pour les animations
    int frameNumber, frameMax, frameTimer;

    // État d'un sprite, direction dans laquelle regarde Mario
    int state, direction;

    // Pour la gestion des collisions
    bool onGround, onLadder;

    // Déplacement temporaire avant la détection des collisions
    float nextXPosition, nextYPosition;

    // Décompte quand on est mort
    int deathTimer;


public:
    enum class Eve {
        JUMPING,
        ONEUP,
        COIN,
        POWERUP,
        STAGECLEAR,
        BUMP
    };

    Character();

    // Accesseurs
    int getX() const;
    int getY() const;
    int getH() const;
    int getW() const;
    float getNextXPosition() const;
    float getNextYPosition() const;
    int getOnGround() const;
    int getLives() const;
    int getCoins() const;
    int getDirection() const;
    int getDeathTimer() const;

    // Setteurs
    void setX(int v);
    void setY(int v);
    void setNextXPosition(float v);
    void setNextYPosition(float v);
    void setOnGround(bool v);
    void setOnLadder(bool v);
    void setLives(int v);
    void setCoins(int v);
    void setDeathTimer(int v);

    // Fonctions
    void initialize(Tilemap &map);

    void playSounds(Eve eve, GameSound &sound);

    void killCharacter();

    void drawCharacter(Tilemap &map, RenderWindow &window);

    void horizontalScrolling(Tilemap &map);

    void moveLeft(Tilemap &map);

    void moveRight(Tilemap &map);

    void climbLadder();

    void downLadder();

    void goToIdle();

    void doAJump(GameSound &sound);

    void powerUp(int y, int x, Tilemap &map, GameSound &sound);

    void itemBlock(int y, int x, Tilemap &map, GameSound &sound);

    void isOnLadder(Tilemap &map);

    void mapCollideHorizontal(Tilemap &map, GameSound &sound);

    void mapCollideVertical(Tilemap &map, KeyboardInput &input, GameSound &sound, Goomba* goomba, Koopa* koopa);

    void nextLevel(Tilemap &map, GameSound &sound);

    int updateCharacter(KeyboardInput &input, Tilemap &map, GameSound &sound, Goomba* goomba, Koopa* koopa);


};

#endif //PROJECT_CHARACTER_HPP
