#ifndef PROJECT_KOOPA_HPP
#define PROJECT_KOOPA_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

// Pour les animations
#define FRAME_TIMER 5

// Taille du sprite du koopa
#define KOOPA_WIDTH 24
#define KOOPA_HEIGHT 46

// Vitesse
#define KOOPA_SPEED 3

// Les états + la direction
#define WALKING 0
#define CLIMBING 1
#define SHELL 2

#define FIX 0
#define RIGHT 1
#define LEFT 2

using namespace sf;

class Tilemap;
class Character;
class GameSound;
class Goomba;

class Koopa {

private:

    Texture koopaTex;
    Sprite koopa;

    // Coordonnées de sprite
    int x, y;
    // Largeur, hauteur sprite
    int w, h;

    // Pour les animations
    int frameNumber, frameMax, frameTimer;

    // État d'un sprite, direction dans laquelle regarde un koopa
    int state, direction;

    // Pour la gestion des collisions
    bool onGround, onLadder;

    // Pour la mort
    int deathTimer;

    // Déplacement temporaire avant la détection des collisions
    float netXPosition, nextYPosition;

    bool death = false;

    int base;

public:
    Koopa();

    int getX() const;
    int getY() const;
    int getW() const;
    int getH() const;
    int getFrameNumber() const;
    int getFrameMax() const;
    int getFrameTimer() const;
    int getState() const;
    int getDirection() const;
    int getDeathTimer() const;
    void setDeath(bool b) { death = b; };
    float getNextXPosition() const;
    float getNextYPosition() const;
    bool getOnGround() const;

    void initialize(int x1, int y1);

    void drawKoopa(Tilemap &map, RenderWindow &window);

    int updateKoopa(Tilemap &map, Character &chara, GameSound &sound);

    void mapCollideHorizontal(Tilemap &map, Character &chara);

    void mapCollideGoomba(Tilemap &map, Goomba &goomba);

    void mapCollideKoopa(Tilemap &map, Koopa &koopa);

    void mapCollideVertical(Tilemap &map, Character &chara);

    int collide(Character &chara);

    void copy(Koopa &koopa);

    void isOnLadder(Tilemap &map);

    void climbLadder();

    void downLadder();

    void testBottomCollide(int xF, int yF, Tilemap& map, GameSound& sound);
};
#endif //PROJECT_KOOPA_HPP
