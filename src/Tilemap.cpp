#ifndef PROJECT_TILEMAP_CPP
#define PROJECT_TILEMAP_CPP

#include "Tilemap.hpp"
#include "Goomba.hpp"
#include "Koopa.hpp"
#include "Character.hpp"

using namespace std;

Tilemap::Tilemap() {
    // Tileset
    if (!tilesetTex.loadFromFile("graphics/tileset.png"))
        // Problème
        cout << "Problème lors du chargement du niveau." << endl;
    else
        tileset.setTexture(tilesetTex);

    // Variables de map
    mapStartX = 0;
    mapStartY = 0;
    mapEndX = 300;
    mapEndY = 300;
    level = 1;
    koopaNumber = 0;
    goombaNumber = 0;
    totalMonsters = 0;
    for(int i = 0; i < MAX_Y; i++)
        for(int j = 0; j < MAX_X; j++)
            tile1[i][j] = tile2[i][j] = 0;
}


// Accesseurs
int Tilemap::getLevel() const { return level; }

int Tilemap::getCharacterBeginX() const { return characterBeginX; }

int Tilemap::getCharacterBeginY() const { return characterBeginY; }

int Tilemap::getMapStartX() const { return mapStartX; }

int Tilemap::getMapStartY() const { return mapStartY; }

int Tilemap::getMapEndX() const { return mapEndX; }

int Tilemap::getMapEndY() const { return mapEndY; }

int Tilemap::getTile(int y, int x) const { return tile1[y][x]; }

int Tilemap::getMonstersNumber() const { return goombaNumber + koopaNumber; }

int Tilemap::getGoombaNumber() const { return goombaNumber; }

int Tilemap::getKoopaNumber() const { return koopaNumber; }

int Tilemap::getTotalMonsters() const { return totalMonsters; }

// Setteurs
void Tilemap::setMapStartX(int v) { mapStartX = v; }

void Tilemap::setMapStartY(int v) { mapStartY = v; }

void Tilemap::setTile(int y, int x, int v) { tile1[y][x] = v; }

void Tilemap::setLevel(int v) { level = v; }

void Tilemap::setGoombaNumber(int v) { goombaNumber = v; }

void Tilemap::setKoopaNumber(int v) { koopaNumber = v; }

void Tilemap::setTotalMonsters(int v) { totalMonsters = v; }

//###############################//
// CHARGEMENT D'UNE MAP #########//
//###############################//

void Tilemap::totalMonsterNumber() {
    for(int y = 0; y < MAX_Y; y++)
        for(int x = 0; x < MAX_X; x++) {
            if (tile1[y][x] == TILE_GOOMBA || tile1[y][x] == TILE_KOOPA)
                totalMonsters++;
        }
}

void Tilemap::loadLevel(string filename) {
    // Flux pour lire les fichiers map
    fstream fic;
    int x = 0, y = 0;

    mapEndX = 0;
    mapEndY = 0;

    // Vecteur pour les lignes
    vector<vector<int> > lines;

    // Vecteur temporaire pour accéder aux lignes
    vector<int> tmpVec;

    // Chaînes de caractères temporaires
    string strTmp2, strTmp;

    // Stringstream pour gérer les chaînes
    stringstream iostream;

    // On ouvre un fichier
    fic.open(filename.c_str(), fstream::in);

    // Test pour voir si le fichier a été ouvert
    if (!fic.is_open()) {
        cerr << "Erreur de chargement de la map." << endl;
        exit(1);
    }

    // On lit tout le fichier
    while (!fic.eof()) {
        // On récupère une ligne
        getline(fic, strTmp2);

        // Si c''est une chaîne vide, on continue
        if (strTmp2.size() == 0)
            continue;

        // Sinon on réinitialise le stringstream et on y envoie le contenu de la chaine
        iostream.clear();
        iostream.str(strTmp2);

        // On réinitialise le vector line
        tmpVec.clear();

        // On boucle pour récupérer chaque numéro de tile de la map
        while (true) {
            // On récupère le numéro de tile  entre chaque espace
            getline(iostream, strTmp, ' ');

            // On le met dans le vecteur line
            tmpVec.push_back(atoi(strTmp.c_str()));

            // On quitte la boucle quand on a fini
            if (!iostream.good())
                break;
        }

        // Si le vecteur line contient quelque chose, on l'envoie dans notre vecteur 2D
        if (tmpVec.size() != 0)
            lines.push_back(tmpVec);
    }

    // On peut fermer le fichier
    fic.close();

    // On peut charger les tiles pour la première couche
    for (y = 0; y < MAX_Y; y++)
        for (x = 0; x < MAX_X; x++) {
            // On copie la valeur du vecteur dans le tableau de tiles
            tile1[y][x] = lines[y][x];

            // On détecte si la tile est vide ou non
            if (tile1[y][x] > 0) {
                // Si elle est vide on augmente la valeur de mapEndX ou Y
                if (x > mapEndX)
                    mapEndX = x;
                if (y > mapEndY)
                    mapEndY = y;
            }
        }

    // On fait la 2ème couche
    for (y = 0; y < MAX_Y; y++)
        for (x = 0; x < MAX_X; x++)
            tile2[y][x] = lines[y + MAX_Y][x];

    // On convertit les dimensions de la map en pixels
    // (on ajoute 1 car un commence à 0, les tiles sont de taille TILE_SIZE)
    mapEndX = (mapEndX + 1) * TILE_SIZE;
    mapEndY = (mapEndY + 1) * TILE_SIZE;

    totalMonsterNumber();
}


//###############################//
// DESSIN D'UNE MAP #########//
//###############################//

void Tilemap::drawLevelLayer(int layer, RenderWindow &window, Goomba goomba[], Koopa koopa[]) {
    int x, y, mapX, xStartDraw, xEndDraw, mapY, y1, y2, xSource, ySource, tileNum;

    /* Coordonnées de départ pour l'affichage de la map
     * détermine à quelles coordonnées il faut découper la première colonne du tileset */
    xStartDraw = (mapStartX % TILE_SIZE) * -1;

    /* Calcul des coordonnées de la fin de la map (à dessiner)*/
    xEndDraw = xStartDraw + WINDOW_WIDTH + TILE_SIZE;

    // Pareil pour les Y
    mapY = mapStartY / TILE_SIZE;
    y1 = (mapStartY % TILE_SIZE) * -1;
    y2 = y1 + WINDOW_HEIGHT + TILE_SIZE;
    // On dessine la carte en commençant par mapStartX et Y

    if (layer == 1) {
        for (y = y1; y < y2; y += TILE_SIZE) {
            /* A chaque début de ligne on réinitialise le mapX (qui contient la colonne 0 au début) */
            mapX = mapStartX / TILE_SIZE;

            /* A chaque colonne de tile, on dessine la bonne tile */
            for (x = xStartDraw; x < xEndDraw; x += TILE_SIZE) {
                // Suivant le numéro de tile, on découpe le tileset
                tileNum = tile1[mapY][mapX];
                if(tileNum != 0) {
                    if(tileNum == TILE_GOOMBA) {
                        if(goombaNumber < MAX_MONSTERS) {
                            goomba[goombaNumber].initialize(mapX * TILE_SIZE, mapY * TILE_SIZE);
                            goombaNumber++;
                        }
                        tileNum = 0;
                        tile1[mapY][mapX] = 0;
                    }
                    else if(tileNum == TILE_KOOPA) {
                        if(koopaNumber < MAX_MONSTERS) {
                            koopa[koopaNumber].initialize(mapX * TILE_SIZE, mapY * TILE_SIZE);
                            koopaNumber++;
                        }
                        tileNum = 0;
                        tile1[mapY][mapX] = 0;
                    }
                }
                // Calcul pour obtenir son y et son x (tileset de 32 tiles par ligne/colonne)
                ySource = tileNum / Y_NUMBER * TILE_SIZE;
                xSource = tileNum % X_NUMBER * TILE_SIZE;

                // On pose la bonne tile au bon endroit
                tileset.setPosition(Vector2f(x, y));
                tileset.setTextureRect(IntRect(xSource, ySource, TILE_SIZE, TILE_SIZE));
                window.draw(tileset);

                mapX++;
            }
            mapY++;
        }
    }

    else if (layer == 2) {
        for (y = y1; y < y2; y += TILE_SIZE) {
            /* A chaque début de ligne on réinitialise le mapX (qui contient la colonne 0 au début) */
            mapX = mapStartX / TILE_SIZE;

            /* A chaque colonne de tile, on dessine la bonne tile */
            for (x = xStartDraw; x < xEndDraw; x += TILE_SIZE) {
                // Suivant le numéro de tile, on découpe le tileset (a = numéro de tile)
                tileNum = tile2[mapY][mapX];

                // Calcul pour obtenir son y et son x (tileset de 32 tiles par ligne/colonne)
                ySource = tileNum / Y_NUMBER * TILE_SIZE;
                xSource = tileNum % X_NUMBER * TILE_SIZE;

                // On pose la bonne tile au bon endroit
                tileset.setPosition(Vector2f(x, y));
                tileset.setTextureRect(IntRect(xSource, ySource, TILE_SIZE, TILE_SIZE));
                window.draw(tileset);

                mapX++;
            }
            mapY++;
        }
    }
}

// Changer de niveau
void Tilemap::changeLevel() {
    loadLevel("maps/map" + to_string(level) + ".txt");
}

#endif