#ifndef SFMLCLASS_MUSICPLAYER_H
#define SFMLCLASS_MUSICPLAYER_H

#include <map>
#include <string>
#include <SFML/Audio/Music.hpp>

using String = std::string;
class MusicPlayer
{

private:
    MusicPlayer();
    ~MusicPlayer() = default;

public:
    static MusicPlayer& getInstance();

    // no copy or move for singleton
    MusicPlayer(const MusicPlayer&) = delete;
    MusicPlayer(MusicPlayer&&) = delete;
    MusicPlayer& operator=(const MusicPlayer&) = delete;
    MusicPlayer& operator=( MusicPlayer&&) = delete;

    void                            addSong(const std::string& name, const std::string& path );
    void							play(String theme);
    void							stop();
    void							setPaused(bool paused);
    void							setVolume(float volume);


private:
    sf::Music						m_music;
    std::map<String, String>	    m_filenames;
    float							m_volume{25};
};


#endif //SFMLCLASS_MUSICPLAYER_H
