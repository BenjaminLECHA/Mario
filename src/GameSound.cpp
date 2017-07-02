#ifndef PROJECT_SOUND_CPP
#define PROJECT_SOUND_CPP

#include "GameSound.hpp"

GameSound::GameSound() {
    if (!_musicBackground.openFromFile("sounds/bgMusic.wav"))
        std::cerr << L"Music Background n'a pas pu être chargée" << std::endl;
    else
        _musicBackground.setLoop(true);

    if (!_coinSoundB.loadFromFile("sounds/coin.wav"))
        std::cerr << L"Le son Coin n'a pas pu être chargé" << std::endl;

    if (!_jumpSoundB.loadFromFile("sounds/jump.wav"))
        std::cerr << L"Le son pour Jump na pas pu être chargé" << std::endl;

    if (!_oneUPSoundB.loadFromFile("sounds/oneUP.wav"))
        std::cerr << L"Le son ONE up n'a pas pu être chargé" << std::endl;

    if (!_bumpSoundB.loadFromFile("sounds/bump.wav"))
        std::cerr << L"Le son Bump n'a pas pu être chargé" << std::endl;

    if (!_stageClearSoundB.loadFromFile("sounds/stageClear.wav"))
        std::cerr << L"Le son pour le StageClear n'a pas pu être chargé" << std::endl;

    if(!_stompSoundB.loadFromFile("sounds/stomp.wav"))
        std::cerr << L"Le son de Stomp n'a pas pu être chargé" << std::endl;

    if(!_killSoundB.loadFromFile("sounds/kill.wav"))
        std::cerr << L"Le son de Kill n'a pas pu être chargé" << std::endl;
}

void GameSound::playJumpSound() {
    _jumpSound.setBuffer(_jumpSoundB);
    _jumpSound.setVolume(30);
    _jumpSound.play();
}

void GameSound::playBackgroundMusic() {
    _musicBackground.setVolume(50);
    _musicBackground.play();
}

void GameSound::playCoinSound() {
    _coinSound.setVolume(50);
    _coinSound.setBuffer(_coinSoundB);
    _coinSound.play();
}

void GameSound::playOneUpSound() {
    _oneUPSound.setBuffer(_oneUPSoundB);
    _oneUPSound.play();
}

void GameSound::playBump() {
    _bumpSound.setBuffer(_bumpSoundB);
    _bumpSound.play();
}

void GameSound::playStageClearSound() {
    _stageClearSound.setBuffer(_stageClearSoundB);
    _stageClearSound.play();
}

void GameSound::playStompSound() {
    _stompSound.setBuffer(_stompSoundB);
    _stompSound.play();
}

void GameSound::playKillSound() {
    _killSound.setVolume(50);
    _killSound.setBuffer(_killSoundB);
    _killSound.play();
}

#endif