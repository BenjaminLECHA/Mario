#include "Koopa.hpp"
#include "Tilemap.hpp"
#include "GameSound.hpp"
#include "Character.hpp"
#include "Goomba.hpp"

Koopa::Koopa() {
    // Chargement spritesheet
    if (!koopaTex.loadFromFile("graphics/koopa_tiles_24_46.png"))
        std::cout << "Erreur lors du chargement du spritesheet du personnage" << std::endl;
    else
        koopa.setTexture(koopaTex);

    // Initialisation des variables d'un goomba
    netXPosition = 0;
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
    onLadder = false;
    death = false;
    netXPosition = 0;
    nextYPosition = 0;
}

int Koopa::getX() const { return x; }
int Koopa::getY() const { return y; }
int Koopa::getW() const { return w; }
int Koopa::getH() const { return h; }
int Koopa::getFrameNumber() const { return frameNumber; }
int Koopa::getFrameMax() const { return frameMax; }
int Koopa::getFrameTimer() const { return frameTimer; }
int Koopa::getState() const { return state; }
int Koopa::getDirection() const { return direction; }
int Koopa::getDeathTimer() const { return deathTimer; }
float Koopa::getNextXPosition() const { return netXPosition; }
float Koopa::getNextYPosition() const { return nextYPosition; }
bool Koopa::getOnGround() const { return onGround; }

void Koopa::initialize(int x1, int y1) {
    // Position de départ
    x = x1;
    y = y1;

    // Direction et état
    direction = LEFT;
    state = WALKING;
    base = FIX;

    // Frame où commencer
    frameNumber = 0;

    // Timer entre frames
    frameTimer = FRAME_TIMER;

    // Frames max pour l'animation WALK (1)
    frameMax = 2;

    w = KOOPA_WIDTH;
    h = KOOPA_HEIGHT;

    onGround = false;
    onLadder = false;
    death = false;
}

void Koopa::drawKoopa(Tilemap &map, RenderWindow &window) {
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

    koopa.setPosition(Vector2f(x - map.getMapStartX(), y - map.getMapStartY()));
    if (state != SHELL) {
        if (direction == LEFT) {
            koopa.setTextureRect(IntRect((frameNumber + 1) * w, state * h, -w, h));
            window.draw(koopa);
        }
        else {
            koopa.setTextureRect(IntRect(frameNumber * w, state * h, w, h));
            window.draw(koopa);
        }
    }
    else {
            koopa.setTextureRect(IntRect(frameNumber, state * h, w, h));
            window.draw(koopa);
    }
}

int Koopa::updateKoopa(Tilemap &map, Character &chara, GameSound &sound) {
    if (deathTimer == 0) {
        netXPosition = 0;
        nextYPosition += GRAVITY_SPEED;
        if(state != SHELL) {
            if (x + h < chara.getX())
                direction = RIGHT;
            else if (x > chara.getX())
                direction = LEFT;
        }

        isOnLadder(map);
        if(!onLadder || (chara.getOnGround() && (chara.getY() + chara.getH() == y + h))) {
            if (state != WALKING && state != SHELL) {
                state = WALKING;
                frameNumber = 0;
                frameTimer = FRAME_TIMER;
                frameMax = 2;
            }
            if (direction == LEFT) {
                if (state == SHELL)
                    netXPosition -= (KOOPA_SPEED + 2);
                else
                    netXPosition -= KOOPA_SPEED;
            }
            else if (direction == RIGHT) {
                if (state == SHELL)
                    netXPosition += (KOOPA_SPEED + 2);
                else
                    netXPosition += KOOPA_SPEED;
            }
            else 
            	netXPosition = 0;
        }

        else {
            if(state != SHELL) {
                if ((y + h) / 2 < ((chara.getY() + chara.getH()) / 2)) {
                    downLadder();
                }
                else {
                    climbLadder();
                }
            }
        }

        // Collisions
        mapCollideHorizontal(map, chara);
        mapCollideVertical(map, chara);

        if (collide(chara) == 1) {
            if(direction == FIX) {
                if (x > chara.getX())
                    direction = RIGHT;
                else
                    direction = LEFT;
                base = direction;
            }
            else if (base == FIX) {
                    chara.killCharacter();
            }
        }

        if (direction != base) {
            base = FIX;
        }

        if(collide(chara) == 2 || death == true) {
            if ((state == SHELL && direction != FIX) || death == true) {
                deathTimer = 1;
                death = false;
            }
            else if (state == SHELL && direction == FIX) {
                if (x > chara.getX())
                    direction = RIGHT;
                else
                 direction = LEFT;
            }
            else if (state != SHELL) {
                    state = SHELL;
                    direction = FIX;
                    onLadder = 0;
            }
            sound.playKillSound();
        }
        if(x <= 0)
            direction = RIGHT;
        if(x >= map.getMapEndX() - TILE_SIZE)
        	direction = LEFT;
    }

    // Si le monstre est mort
    if (deathTimer > 0) {
        --deathTimer;
        death = false;
        return 1;
    }
    return -1;
}

void Koopa::mapCollideGoomba(Tilemap &map, Goomba &goomba) {

    if (state == SHELL && direction != FIX) {
            if(goomba.getX() >= x && goomba.getX() <= x + w) {
                if(goomba.getY() >= y && goomba.getY() <= y + h)
                    goomba.carapaceGoomba();
            }
        }
}

void Koopa::mapCollideKoopa(Tilemap &map, Koopa &koopa) {
    if (state == SHELL && direction != FIX) {
        if(koopa.getX() >= x && koopa.getX() <= x + w) {
            if(koopa.getY() >= y && koopa.getY() <= y + h) {
                koopa.setDeath(true);
            }
        }
    }
}

void Koopa::mapCollideHorizontal(Tilemap &map, Character &chara) {
    int i, xLeft, xRight, yUp, yBottom;

    // On considère qu'on est en l'air tant qu'on est sûr de pas toucher le sol
    onGround = false;

    // On va tester les mouvements horizontaux
    // On découpe le sprite en bloc de tiles
    i = TILE_SIZE;

    //while (i != KOOPA_HEIGHT) {
    // On calcule les coins du sprite à gauche et à droite pour voir ce que ça touche
    xLeft = (int) (x + netXPosition) / TILE_SIZE;
    xRight = (int) (x + netXPosition + KOOPA_WIDTH - 1) / TILE_SIZE;

    // Pareil pour les y
    yUp = y / TILE_SIZE;
    yBottom = (y + i - 1) / TILE_SIZE;

    // Ensuite, on teste les mouvements du updateCharacter (nextXPosition et nextYPosition)
    if (xLeft >= 0 && xRight < MAX_X && yUp >= 0 && yBottom < MAX_Y) {
        ///////////////////////////////
        // Si on se déplace à droite //
        ///////////////////////////////
        if (netXPosition > 0) {
            // On vérifie si les tiles sont solides
            if ((map.getTile(yUp, xRight) > BLANK_TILE && map.getTile(yUp, xRight) != POWERUP_COIN &&
                 map.getTile(yUp, xRight) != POWERUP_MUSHROOM && map.getTile(yUp, xRight) != LADDER) ||
                (map.getTile(yBottom, xRight) > BLANK_TILE && map.getTile(yBottom, xRight) != POWERUP_COIN &&
                 map.getTile(yBottom, xRight) != POWERUP_MUSHROOM && map.getTile(yBottom, xRight) != LADDER)) {
                // Si c'est le cas on place le perso le plus proche possible
                if(direction != FIX) {
                    if (state != SHELL) {
                        x = xRight * TILE_SIZE;
                        x -= KOOPA_WIDTH + 1;
                        netXPosition = 0;
                    }
                    else
                        direction = LEFT;
                }
                else
                    direction = FIX;
            }
        }
        //////////////////////////////
        // Pareil à gauche ///////////
        //////////////////////////////
        else if (netXPosition < 0) {
            if ((map.getTile(yUp, xLeft) > BLANK_TILE && map.getTile(yUp, xLeft) != POWERUP_COIN &&
                 map.getTile(yUp, xLeft) != POWERUP_MUSHROOM && map.getTile(yUp, xLeft) != LADDER) ||
                (map.getTile(yBottom, xLeft) > BLANK_TILE && map.getTile(yBottom, xLeft) != POWERUP_COIN &&
                 map.getTile(yBottom, xLeft) != POWERUP_MUSHROOM && map.getTile(yBottom, xLeft) != LADDER)) {
                if(direction != FIX) {
                    if(state != SHELL) {
                        x = (xLeft + 1) * TILE_SIZE;
                        netXPosition = 0;
                    }
                    else
                        direction = RIGHT;
                }
                else
                    direction = FIX;
            }
        }
    }

    if(direction != FIX)
        x += netXPosition;
}

void Koopa::mapCollideVertical(Tilemap &map, Character &chara) {
    // Et on refait pareil avec l'axe vertical (pas besoin de while car la largeur du sprite est plus petite qu'une tile
    int i = KOOPA_WIDTH;

    int xLeft = x / TILE_SIZE;
    int xRight = (x + i) / TILE_SIZE;

    int yUp = (int) (y + nextYPosition) / TILE_SIZE;
    int yDown = (int) (y + nextYPosition + KOOPA_HEIGHT) / TILE_SIZE;

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
                y -= KOOPA_HEIGHT;
                nextYPosition = 0;
                onGround = true;
            }
        }
        //////////////////////////////
        // Déplacement vers le haut //
        //////////////////////////////
        else {
            if(nextYPosition < 0) {
                if(state == SHELL)
                    nextYPosition = 0;
               else if((map.getTile(yUp, xLeft) > BLANK_TILE && map.getTile(yUp, xLeft) != POWERUP_COIN &&
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
    if (y > map.getMapEndY())
        deathTimer = 1;
}

int Koopa::collide(Character &chara) {
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

    else if (chara.getY() + chara.getH() <= y + 10 && state != SHELL)  {
        chara.setNextYPosition(-JUMP_HEIGHT);
        return 2;
    }
    else if (chara.getY() + chara.getH() <= y + 23 && state == SHELL) {
        chara.setNextYPosition(-JUMP_HEIGHT);
        return 2;
    }

    else
        return 1;
}


void Koopa::copy(Koopa &koopa) {
    netXPosition = koopa.getNextXPosition();
    nextYPosition = koopa.getNextYPosition();
    x = koopa.getX();
    y = koopa.getY();
    h = koopa.getH();
    w = koopa.getW();
    frameNumber = koopa.getFrameNumber();
    frameTimer = koopa.getFrameTimer();
    frameMax = koopa.getFrameMax();
    state = koopa.getState();
    direction = koopa.getDirection();
    onGround = koopa.getOnGround();
}

void Koopa::isOnLadder(Tilemap &map) {
    int xF = (x + w + 1) / TILE_SIZE;
    int yF = y / TILE_SIZE;
    int yF2 = (y + KOOPA_HEIGHT) / TILE_SIZE;
    if(direction == LEFT && state != SHELL)
        onLadder = (map.getTile(yF, xF) == LADDER || map.getTile(yF2, xF) == LADDER);
    else if (direction == RIGHT && state != SHELL)
        onLadder = (map.getTile(yF, xF - 1) == LADDER || map.getTile(yF2, xF - 1) == LADDER);
}

void Koopa::climbLadder() {
    nextYPosition = -CLIMBING_SPEED;
    if (state != CLIMBING && state != SHELL) {
        state = CLIMBING;
        frameNumber = 0;
        frameTimer = FRAME_TIMER;
        frameMax = 2;
    }
}

void Koopa::downLadder() {
    nextYPosition = CLIMBING_SPEED;
    if (state != CLIMBING && state != SHELL) {
        state = CLIMBING;
        frameNumber = 0;
        frameTimer = FRAME_TIMER;
        frameMax = 2;
    }
}

void Koopa::testBottomCollide(int xF, int yF, Tilemap& map, GameSound& sound) {
    if(((((x + KOOPA_WIDTH + 10) / TILE_SIZE) == yF) || (((x + KOOPA_WIDTH - 10) / TILE_SIZE) == yF)
       || ((x + KOOPA_WIDTH) / TILE_SIZE == yF)) && (y + KOOPA_HEIGHT) / TILE_SIZE == xF) {
        if(state == SHELL)
            deathTimer = 1;
        else {
            state = SHELL;
            direction = FIX;
        }
        sound.playKillSound();
    }
}