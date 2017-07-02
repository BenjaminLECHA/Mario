#ifndef PROJECT_GOOMBA_CPP
#define PROJECT_GOOMBA_CPP

#include "Goomba.hpp"
#include "Tilemap.hpp"
#include "GameSound.hpp"
#include "Character.hpp"

Goomba::Goomba() {
    // Chargement spritesheet
    if (!goombaTex.loadFromFile("graphics/goomba_tiles_32_32.png"))
        std::cerr << "Erreur lors du chargement du spritesheet du personnage" << std::endl;
    else
        goomba.setTexture(goombaTex);

    // Initialisation des variables d'un goomba
    nextXPosition = 0;
    nextYPosition = 0;
    x = 0;
    y = 0;
    h = 0;
    w = 0;
    frameMax = 0;
    frameNumber = 0;
    frameTimer = 0;
    state = WALKING;
    direction = 0;
    deathTimer = 0;
    onGround = false;
    nextXPosition = 0;
    nextYPosition = 0;
}

int Goomba::getX() const { return x; }
int Goomba::getY() const { return y; }
int Goomba::getW() const { return w; }
int Goomba::getH() const { return h; }
int Goomba::getFrameNumber() const { return frameNumber; }
int Goomba::getFrameMax() const { return frameMax; }
int Goomba::getFrameTimer() const { return frameTimer; }
int Goomba::getState() const { return state; }
int Goomba::getDirection() const { return direction; }
int Goomba::getDeathTimer() const { return deathTimer; }
float Goomba::getNextXPosition() const { return nextXPosition; }
float Goomba::getNextYPosition() const { return nextYPosition; }
bool Goomba::getOnGround() const { return onGround; }

void Goomba::initialize(int x1, int y1) {
    // Position de départ
    x = x1;
    y = y1;

    // Direction et état
    direction = RIGHT;
    state = WALKING;

    // Frame où commencer
    frameNumber = 0;

    // Timer entre frames
    frameTimer = FRAME_TIMER;

    // Frames max pour l'animation WALK (1)
    frameMax = 2;

    w = GOOMBA_WIDTH;
    h = GOOMBA_HEIGHT;

    onGround = false;
    death = false;
}

void Goomba::drawGoomba(Tilemap &map, RenderWindow &window) {
    // Timer
    // Si le timer arrive à zéro
    if (frameTimer <= 0) {
        // On le reset
        frameTimer = FRAME_TIMER;

        // On augmente la variable qui compte les frames
        frameNumber++;

        // Si on dépasse la frame max, il faut revenir à la première :
        if (frameNumber >= frameMax)
            frameNumber = 0;
    }
        // Sinon, on réduit le timer
    else
        frameTimer--;

    goomba.setPosition(Vector2f(x - map.getMapStartX(), y - map.getMapStartY()));
    if (direction == LEFT) {
        goomba.setTextureRect(IntRect((frameNumber + 1) * w, state * h, -w, h));
        window.draw(goomba);
    }
    else {
        goomba.setTextureRect(IntRect(frameNumber * w, state * h, w, h));
        window.draw(goomba);
    }
}

int Goomba::updateGoomba(Tilemap &map, Character &chara, GameSound &sound) {
    if (deathTimer == 0) {
        nextXPosition = 0;
        nextYPosition += GRAVITY_SPEED;

        if(x + h < chara.getX())
            direction = RIGHT;
        else if (x > chara.getX())
            direction = LEFT;

        if (direction == LEFT)
            nextXPosition -= GOOMBA_SPEED;
        else
            nextXPosition += GOOMBA_SPEED;

        // Collisions
        mapCollideHorizontal(map, chara);
        mapCollideVertical(map, chara);

        if(x <= 0)
            direction = RIGHT;
        if(x >= map.getMapEndX() - TILE_SIZE)
            direction = LEFT;

        // On détecte les collisions avec le personnage
        if (collide(chara) == 1) {
            chara.killCharacter();
        }
        else if ((collide(chara) == 2 || death == true) && state != DEATH) {
            sound.playKillSound();
            deathTimer = 1;
            state = DEATH;
            frameMax = 4;
            frameTimer = FRAME_TIMER;
        }
    }

    // Pour corriger le bug où un monstre peut être en état de mort mais toujours être vivant
    if(state == DEATH && deathTimer == 0) {
        return 1;
    }

    // Si le monstre est mort
    if (deathTimer > 0) {
            --deathTimer;
            death = false;
            return 0;
        }
    return -1;
}

void Goomba::carapaceGoomba() {
    death = true;
}

void Goomba::mapCollideHorizontal(Tilemap &map, Character &chara) {
    int i, xLeft, xRight, yUp, yDown;

    // On considère qu'on est en l'air tant qu'on est sûr de pas toucher le sol
    onGround = false;

    // On va tester les mouvements horizontaux
    // On découpe le sprite en bloc de tiles
    i = TILE_SIZE;

    //while (i != GOOMBA_HEIGHT) {
        // On calcule les coins du sprite à gauche et à droite pour voir ce que ça touche
        xLeft = (int) (x + nextXPosition) / TILE_SIZE;
        xRight = (int) (x + nextXPosition + GOOMBA_WIDTH - 1) / TILE_SIZE;

        // Pareil pour les y
        yUp = y / TILE_SIZE;
        yDown = (y + i - 1) / TILE_SIZE;

        // Ensuite, on teste les mouvements du updateCharacter (nextXPosition et nextYPosition)
        if (xLeft >= 0 && xRight < MAX_X && yUp >= 0 && yDown < MAX_Y) {
            ///////////////////////////////
            // Si on se déplace à droite //
            ///////////////////////////////
            if (nextXPosition > 0) {
                // On vérifie si les tiles sont solides
                if ((map.getTile(yUp, xRight) > BLANK_TILE && map.getTile(yUp, xRight) != POWERUP_COIN &&
                     map.getTile(yUp, xRight) != POWERUP_MUSHROOM && map.getTile(yUp, xRight) != LADDER) ||
                    (map.getTile(yDown, xRight) > BLANK_TILE && map.getTile(yDown, xRight) != POWERUP_COIN &&
                     map.getTile(yDown, xRight) != POWERUP_MUSHROOM && map.getTile(yDown, xRight) != LADDER)) {
                    // Si c'est le cas on place le perso le plus proche possible
                    x = xRight * TILE_SIZE;
                    x -= GOOMBA_WIDTH + 1;
                    nextXPosition = 0;
                }
            }

            //////////////////////////////
            // Pareil à gauche ///////////
            //////////////////////////////
            else if (nextXPosition < 0) {
                if ((map.getTile(yUp, xLeft) > BLANK_TILE && map.getTile(yUp, xLeft) != POWERUP_COIN &&
                     map.getTile(yUp, xLeft) != POWERUP_MUSHROOM && map.getTile(yUp, xLeft) != LADDER) ||
                    (map.getTile(yDown, xLeft) > BLANK_TILE && map.getTile(yDown, xLeft) != POWERUP_COIN &&
                     map.getTile(yDown, xLeft) != POWERUP_MUSHROOM && map.getTile(yDown, xLeft) != LADDER)) {
                    x = (xLeft + 1) * TILE_SIZE;
                    nextXPosition = 0;
                }
            }
        }

    x += nextXPosition;

}

void Goomba::mapCollideVertical(Tilemap &map, Character &chara) {
    // Et on refait pareil avec l'axe vertical (pas besoin de while car la largeur du sprite est plus petite qu'une tile
    int i = GOOMBA_WIDTH;

    int xLeft = x / TILE_SIZE;
    int xRight = (x + i) / TILE_SIZE;

    int yUp = (int) (y + nextYPosition) / TILE_SIZE;
    int yDown = (int) (y + nextYPosition + GOOMBA_HEIGHT) / TILE_SIZE;

    if (xLeft >= 0 && xRight < MAX_X && yUp >= 0 && yDown < MAX_Y) {
        //////////////////////////////////
        // Si on se déplace vers le bas //
        //////////////////////////////////
        if (nextYPosition > 0) {
            if ((map.getTile(yDown, xLeft) > BLANK_TILE && map.getTile(yDown, xLeft) != POWERUP_COIN &&
                 map.getTile(yDown, xLeft) != POWERUP_MUSHROOM && map.getTile(yDown, xLeft) != LADDER) ||
                (map.getTile(yDown, xRight) > BLANK_TILE && map.getTile(yDown, xRight) != POWERUP_COIN &&
                 map.getTile(yDown, xRight) != POWERUP_MUSHROOM && map.getTile(yDown, xRight) != POWERUP_MUSHROOM &&
                 map.getTile(yDown, xRight) != LADDER)) {
                // On colle le joueur au sol et on le déclare au sol
                y = yDown * TILE_SIZE;
                y -= GOOMBA_HEIGHT;
                nextYPosition = 0;
                onGround = true;
            }
        }
        //////////////////////////////
        // Déplacement vers le haut //
        //////////////////////////////
        else {
            if(nextYPosition < 0) {
                if((map.getTile(yUp, xLeft) > BLANK_TILE && map.getTile(yUp, xLeft) != POWERUP_COIN &&
                    map.getTile(yUp, xLeft) != POWERUP_MUSHROOM && map.getTile(yUp, xLeft) != LADDER) ||
                   (map.getTile(yUp, xRight) > BLANK_TILE && map.getTile(yUp, xRight) != POWERUP_COIN &&
                    map.getTile(yUp, xRight) != POWERUP_MUSHROOM && map.getTile(yUp, xRight) != LADDER)) {

                    y = (yUp + 1) * TILE_SIZE;
                    nextYPosition = 0;
                }
            }
        }
    }

    y += nextYPosition;

    // Si on tombe dans un trou
    if (y > map.getMapEndY() && state != DEATH) {
        deathTimer = 10;
        state = DEATH;
    }
}

int Goomba::collide(Character &chara) {
    // On gère les collisions monstre / joueur
    // return 0 s'il n'y a pas de collisions
    // return 1 si le joueur se fait toucher
    // return 2 si le goomba est touché

    if((chara.getX() >= x + w)
       || (chara.getX() + chara.getW() <= x)
       || (chara.getY() > y + h)
       || (chara.getY() + chara.getH() <= y)) {
        return 0;
    }

    else if (chara.getY() + chara.getH() <= y + 10) {
        chara.setNextYPosition(-JUMP_HEIGHT);
        return 2;
    }

    else
        return 1;
}

void Goomba::copy(Goomba &goomba) {
    nextXPosition = goomba.getNextXPosition();
    nextYPosition = goomba.getNextYPosition();
    x = goomba.getX();
    y = goomba.getY();
    h = goomba.getH();
    w = goomba.getW();
    frameNumber = goomba.getFrameNumber();
    frameTimer = goomba.getFrameTimer();
    frameMax = goomba.getFrameMax();
    state = goomba.getState();
    direction = goomba.getDirection();
    onGround = goomba.getOnGround();
}

void Goomba::testBottomCollide(int xF, int yF, Tilemap& map, GameSound& sound) {
    if(((((x + GOOMBA_WIDTH + 10) / TILE_SIZE) == yF) || (((x + GOOMBA_WIDTH - 10) / TILE_SIZE) == yF)
       || ((x + GOOMBA_WIDTH) / TILE_SIZE == yF)) && (y + GOOMBA_HEIGHT) / TILE_SIZE == xF) {
        deathTimer = 10;
        state = DEATH;
        frameMax = 4;
        frameTimer = FRAME_TIMER;
        sound.playKillSound();
    }
}

#endif