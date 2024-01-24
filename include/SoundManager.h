#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include <SFML/Audio.hpp>
#include <string>

class SoundManager {
  public:
    sf::Music music;

    SoundManager() {
        music.openFromFile("../music/loop.ogg");
        music.setLoop(true);
        music.play();
    }

    void add_sound(std::string sound_path);
    void play_sound(std::string name) const;
};

#endif
