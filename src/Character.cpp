#ifndef PROJECT_CHARACTER_CPP
#define PROJECT_CHARACTER_CPP

#include "Character.hpp"
#include "Tilemap.hpp"
#include "GameSound.hpp"
#include "KeyboardInput.hpp"
#include "Koopa.hpp"
#include "Goomba.hpp"

Character::Character() {
    // Chargement spritesheet
    if (!characterTex.loadFromFile("graphics/mario_spritesheet.png"))
        std::cerr << "Erreur lors du chargement du spritesheet du personnage" << std::endl;
    else
        character.setTexture(characterTex);

    // Initialisation des variables joueur
    nextXPosition = 0;
    nextYPosition = 0;
    x = 0;
    y = 300;
    frameMax = 0;
    frameNumber = 0;
    frameTimer = 0;
    state = 0;
    direction = 0;
    onGround = false;
    onLadder = false;
    nextXPosition = 0;
    nextYPosition = 0;
    deathTimer = 0;
}

// Accesseurs
int Character::getX() const { return x; }

int Character::getY() const { return y; }

int Character::getH() const { return CHARACTER_HEIGHT; }

int Character::getW() const { return CHARACTER_WIDTH; }

float Character::getNextXPosition() const { return nextXPosition; }

float Character::getNextYPosition() const { return nextYPosition; }

int Character::getOnGround() const { return onGround; }

int Character::getLives() const { return lives; }

int Character::getCoins() const { return coins; }

int Character::getDirection() const { return direction; }

int Character::getDeathTimer() const { return deathTimer; }

// Setteurs
void Character::setX(int v) { x = v; }

void Character::setY(int v) { y = v; }

void Character::setNextXPosition(float v) { nextXPosition = v; }

void Character::setNextYPosition(float v) { nextYPosition = v; }

void Character::setOnGround(bool v) { onGround = v; }

void Character::setOnLadder(bool v) { onLadder = v; }

void Character::setLives(int v) { lives = v; }

void Character::setCoins(int v) { coins = v; }

void Character::setDeathTimer(int v) { deathTimer = v; }

// Fonctions
void Character::initialize(Tilemap &map) {

    // Direction et état
    direction = RIGHT;
    state = IDLE;

    // Frame où commencer
    frameNumber = 0;

    // Timer entre frames
    frameTimer = FRAME_TIMER;

    // Frames max pour l'animation IDLE (1)
    frameMax = 1;

    // Coordonnées de départ du personnage
    x = 0;
    y = 300;

    map.setMapStartX(0);
    map.setMapStartY(0);

    onGround = false;
    onLadder = false;
    deathTimer = 0;
}

void Character::playSounds(Eve eve, GameSound &sound) {
    switch (eve) {
        case Eve::COIN:
            sound.playCoinSound();
            break;
        case Eve::JUMPING:
            sound.playJumpSound();
            break;
        case Eve::ONEUP:
            sound.playOneUpSound();
            break;
        case Eve::BUMP:
            sound.playBump();
            break;
        case Eve::STAGECLEAR:
            sound.playStageClearSound();
            break;
        case Eve::POWERUP:
            sound.playStompSound();
    }
}

void Character::killCharacter() {
    deathTimer = 60;
}

void Character::drawCharacter(Tilemap &map, RenderWindow &window) {
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

    character.setPosition(Vector2f(x - map.getMapStartX(), y - map.getMapStartY()));
    // On dessine le perso qui regarde à gauche, il faut prendre du point suivant jusqu'au début du point voulu
    if (direction == LEFT) {
        character.setTextureRect(
                IntRect((frameNumber + 1) * CHARACTER_WIDTH, state * CHARACTER_HEIGHT, -CHARACTER_WIDTH,
                        CHARACTER_HEIGHT));
        window.draw(character);
    }
        // On dessine le perso qui regarde à droite, on prend le rectangle normal
    else {
        character.setTextureRect(IntRect(frameNumber * CHARACTER_WIDTH, state * CHARACTER_HEIGHT, CHARACTER_WIDTH,
                                         CHARACTER_HEIGHT));
        window.draw(character);
    }
}

void Character::horizontalScrolling(Tilemap &map) {
    int startCamera = x + CHARACTER_WIDTH / 2 - WINDOW_WIDTH / 2;
    int endCamera = x + CHARACTER_WIDTH / 2 + WINDOW_WIDTH / 2;
    // Si on dépasse à gauche
    if (startCamera < 0)
        // On fixe la limite
        startCamera = 0;
    // Si on dépasse à droite
    if (endCamera > map.getMapEndX())
        // On fixe la limite
        startCamera = map.getMapEndX() - WINDOW_WIDTH;

    map.setMapStartX(startCamera);
}

void Character::moveLeft(Tilemap &map) {
    direction = LEFT;
    if (x > map.getMapStartX())
        nextXPosition -= CHARACTER_SPEED;
    // Si on ne marchait pas déjà et qu'on était sur le sol
    if (state != WALK && onGround) {
        // On dit qu'on marche, et on reset l'animation
        state = WALK;
        frameNumber = 0;
        frameTimer = FRAME_TIMER;
        frameMax = 4;
    }
    if (state == CLIMB && !onLadder) {
        goToIdle();
    }
}

void Character::moveRight(Tilemap &map) {
    if (x + CHARACTER_WIDTH < map.getMapEndX() - 3)
        nextXPosition += CHARACTER_SPEED;
    direction = RIGHT;

    // Si on ne marchait pas déjà et qu'on était sur le sol
    if (state != WALK && onGround) {
        // On dit qu'on marche, et on reset l'animation
        state = WALK;
        frameNumber = 0;
        frameTimer = FRAME_TIMER;
        frameMax = 4;
    }
    if (state == CLIMB && !onLadder) {
        goToIdle();
    }
}


void Character::climbLadder() {
    nextYPosition = -CLIMBING_SPEED;
    if (state != CLIMB) {
        state = CLIMB;
        frameNumber = 0;
        frameTimer = FRAME_TIMER;
        frameMax = 2;
    }
}

void Character::downLadder() {
    nextYPosition = CLIMBING_SPEED;
    if (state != CLIMB) {
        state = CLIMB;
        frameNumber = 0;
        frameTimer = FRAME_TIMER;
        frameMax = 2;
    }
}

void Character::goToIdle() {
    // On change l'animation si le joueur n'était pas déjà inactif
    if (state != IDLE) {
        state = IDLE;
        frameNumber = 0;
        frameTimer = FRAME_TIMER;
        frameMax = 1;
    }
}

void Character::doAJump(GameSound &sound) {
    nextYPosition -= JUMP_HEIGHT;
    onGround = false;
    if (state != JUMP)
        playSounds(Eve::JUMPING, sound);
    state = JUMP;
    frameNumber = 0;
    frameTimer = FRAME_TIMER;
    frameMax = 1;
}

void Character::powerUp(int y, int x, Tilemap &map, GameSound &sound) {
    if (map.getTile(y, x) == POWERUP_MUSHROOM) {
        if (lives < 100) {
            playSounds(Eve::ONEUP, sound);
            lives++;
        }
        map.setTile(y, x, BLANK_TILE);
    }
    if (map.getTile(y, x) == POWERUP_COIN) {
        playSounds(Eve::COIN, sound);
        if (++coins >= 100) {
            coins = 0;
            if (lives < 100) {
                playSounds(Eve::ONEUP, sound);
                lives++;
            }
        }
        map.setTile(y, x, BLANK_TILE);
    }
}

void Character::itemBlock(int y, int x, Tilemap &map, GameSound &sound) {
    // On gère le cas d'un bloc d'item
    if (map.getTile(y, x) == ITEM_BLOCK) {
        // Une chance sur 10 d'avoir une vie, sinon c'est une pièce
        int randNum = (int) random() % 10;
        if (randNum < 1) {
            // Le powerup n'apparait que si la tile sur laquelle il doit apparaitre est vide
            if (map.getTile(y - 1, x) == BLANK_TILE) {
                map.setTile(y - 1, x, POWERUP_MUSHROOM);
                map.setTile(y, x, NORMAL_BLOCK);
            }
        }
        else {
            // Le powerup n'apparait que si la tile sur laquelle il doit apparaitre est vide
            if (map.getTile(y - 1, x) == BLANK_TILE) {
                map.setTile(y - 1, x, POWERUP_COIN);
                map.setTile(y, x, NORMAL_BLOCK);
                playSounds(Eve::POWERUP, sound);
            }
        }
    }
}

void Character::isOnLadder(Tilemap &map) {
    int xF = (x + (CHARACTER_WIDTH / 2)) / TILE_SIZE;
    int yF = y / TILE_SIZE;
    int yF2 = (y + CHARACTER_WIDTH + 10) / TILE_SIZE;
    onLadder = (map.getTile(yF, xF) == LADDER || map.getTile(yF2, xF) == LADDER);
}

void Character::mapCollideHorizontal(Tilemap &map, GameSound &sound) {
    int i, xLeft, xRight, yUp, yDown;

    // On limite
    if(nextYPosition < MAX_JUMPING_SPEED)
        nextYPosition = MAX_JUMPING_SPEED;
    if(nextYPosition > MAX_FALLING_SPEED)
        nextYPosition = MAX_FALLING_SPEED;
    
    // On considère qu'on est pas sur une échelle
    onLadder = false;
    // On va tester les mouvements horizontaux
    // On découpe le sprite en bloc de tiles
    i = TILE_SIZE;

    // On calcule les coins du sprite à gauche et à droite pour voir ce que ça touche
    xLeft = (int) (x + nextXPosition) / TILE_SIZE;
    xRight = (int) (x + nextXPosition + CHARACTER_WIDTH - 1) / TILE_SIZE;

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
                x -= CHARACTER_WIDTH + 1;
                nextXPosition = 0;
            }
                // Si c'est un powerup, on appelle powerup et on enlève la tile
            else if (map.getTile(yUp, xRight) == POWERUP_COIN || map.getTile(yUp, xRight) == POWERUP_MUSHROOM
                     || map.getTile(yDown, xRight) == POWERUP_COIN || map.getTile(yDown, xRight) == POWERUP_MUSHROOM) {
                powerUp(yDown, xRight, map, sound);
                powerUp(yUp, xRight, map, sound);
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
            else if (map.getTile(yUp, xLeft) == POWERUP_COIN || map.getTile(yUp, xLeft) == POWERUP_MUSHROOM
                     || map.getTile(yDown, xLeft) == POWERUP_COIN || map.getTile(yDown, xLeft) == POWERUP_MUSHROOM) {
                powerUp(yDown, xLeft, map, sound);
                powerUp(yUp, xLeft, map, sound);
            }
        }
    }


    // Ensuite on applique le déplacement
    x += nextXPosition;

}

void Character::mapCollideVertical(Tilemap &map, KeyboardInput &input, GameSound &sound, Goomba goomba[],
                                   Koopa koopa[]) {
    // Et on refait pareil avec l'axe vertical (pas besoin de while car la largeur du sprite est plus petite qu'une tile
    int i = CHARACTER_WIDTH;

    int xLeft = x / TILE_SIZE;
    int xRight = (x + i) / TILE_SIZE;

    int yUp = (int) (y + nextYPosition) / TILE_SIZE;
    int yDown = (int) (y + nextYPosition + CHARACTER_HEIGHT) / TILE_SIZE;

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
                y -= CHARACTER_HEIGHT;
                nextYPosition = 0;
                onGround = true;
            }
            else if (map.getTile(yDown, xLeft) == POWERUP_COIN || map.getTile(yDown, xLeft) == POWERUP_MUSHROOM
                     || map.getTile(yDown, xRight) == POWERUP_COIN || map.getTile(yDown, xRight) == POWERUP_MUSHROOM) {
                powerUp(yDown, xLeft, map, sound);
                powerUp(yDown, xRight, map, sound);
            }
                // Cas spécial échelles, si on est sur le haut d'une échelle et quu'on ne veut pas descendre
            else if ((map.getTile(yDown, xRight) == LADDER) &&
                     (map.getTile(yDown - 1, xRight) == BLANK_TILE) &&
                     !input.getButton().down) {
                // On colle le joueur au sol et on le déclare au sol
                y = yDown * TILE_SIZE;
                y -= CHARACTER_HEIGHT;
                nextYPosition = 0;
                onGround = true;
            }

        }
        //////////////////////////////
        // Déplacement vers le haut //
        //////////////////////////////
        else {
            if (nextYPosition < 0) {
                if ((map.getTile(yUp, xLeft) > BLANK_TILE && map.getTile(yUp, xLeft) != POWERUP_COIN &&
                     map.getTile(yUp, xLeft) != POWERUP_MUSHROOM && map.getTile(yUp, xLeft) != LADDER) ||
                    (map.getTile(yUp, xRight) > BLANK_TILE && map.getTile(yUp, xRight) != POWERUP_COIN &&
                     map.getTile(yUp, xRight) != POWERUP_MUSHROOM && map.getTile(yUp, xRight) != LADDER)) {

                    // On gère les cas de collision avec un bloc item
                    itemBlock(yUp, xLeft, map, sound);
                    itemBlock(yUp, xRight, map, sound);

                    y = (yUp + 1) * TILE_SIZE;
                    nextYPosition = 0;
                    playSounds(Eve::BUMP, sound);
                    for (int n = 0; n < map.getGoombaNumber(); n++)
                        goomba[n].testBottomCollide(yUp, xLeft, map, sound);
                    for (int n = 0; n < map.getKoopaNumber(); n++)
                        koopa[n].testBottomCollide(yUp, xLeft, map, sound);
                }
                else if (map.getTile(yUp, xLeft) == POWERUP_COIN || map.getTile(yUp, xLeft) == POWERUP_MUSHROOM
                         || map.getTile(yUp, xRight) == POWERUP_COIN || map.getTile(yUp, xRight) == POWERUP_MUSHROOM) {
                    powerUp(yUp, xLeft, map, sound);
                    powerUp(yUp, xRight, map, sound);
                }
            }
        }
    }

    y += nextYPosition;

    // Si on tombe dans un trou
    if (y > map.getMapEndY())
        deathTimer = 60;
}

void Character::nextLevel(Tilemap &map, GameSound &sound) {
    if (map.getLevel() < LEVEL_MAX) {
        map.setLevel(map.getLevel() + 1);
        map.changeLevel();
        initialize(map);
        playSounds(Eve::STAGECLEAR, sound);
    }
}

int Character::updateCharacter(KeyboardInput &input, Tilemap &map, GameSound &sound, Goomba goomba[], Koopa koopa[]) {
    // Si le timer vaut 0, ça veut dire que le jeu a repris
    if (!deathTimer) {
        nextXPosition = 0;
        if (state != CLIMB)
            nextYPosition += GRAVITY_SPEED;

        // Si on va à gauche
        if (input.getButton().left)
            moveLeft(map);
        // Si on va à droite
        else if (input.getButton().right)
            moveRight(map);

        // Si on grimpe à une échelle
        else if (onLadder) {
            if (input.getButton().up)
                climbLadder();
            else if (input.getButton().down)
                downLadder();
            else if (state == CLIMB) {
                nextYPosition = 0;
                frameTimer = FRAME_TIMER;
            }
            else
                goToIdle();
        }

        else if (state == CLIMB && !onLadder)
            goToIdle();

            // Si on fait rien et qu'on est sur le sol
        else if (!input.getButton().right && !input.getButton().left && onGround)
            goToIdle();

        // Le saut
        if (input.getButton().jump && onGround)
            doAJump(sound);

        // On gère les collisions
        mapCollideHorizontal(map, sound);
        mapCollideVertical(map, input, sound, goomba, koopa);
        isOnLadder(map);

        // On gère le scrolling
        horizontalScrolling(map);
    }
        // Si on est mort
    else {
        deathTimer--;
        if (deathTimer == 0) {
            lives--;
            map.changeLevel();
            initialize(map);
            map.setTotalMonsters(0);
            map.totalMonsterNumber();
        }
    }

    return deathTimer;
}


#endif