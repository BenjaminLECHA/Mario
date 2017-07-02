#ifndef PROJECT_SOUND_HPP
#define PROJECT_SOUND_HPP

#include <SFML/Audio.hpp>
#include <iostream>

using namespace sf;

class GameSound {
private:
    Music _musicBackground;
    Sound _coinSound;
    Sound _stageClearSound;
    Sound _oneUPSound;
    Sound _bumpSound;
    Sound _jumpSound;
    Sound _stompSound;
    Sound _killSound;
    SoundBuffer _coinSoundB;
    SoundBuffer _stageClearSoundB;
    SoundBuffer _oneUPSoundB;
    SoundBuffer _bumpSoundB;
    SoundBuffer _jumpSoundB;
    SoundBuffer _stompSoundB;
    SoundBuffer _killSoundB;

public:
    GameSound();

    void playJumpSound();

    void playBackgroundMusic();

    void playCoinSound();

    void playOneUpSound();

    void playBump();

    void playStageClearSound();

    void playStompSound();

    void playKillSound();
};

#endif //PROJECT_SOUND_HPP
