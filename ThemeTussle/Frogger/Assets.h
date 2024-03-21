#ifndef BREAKOUT_ASSETS_H
#define BREAKOUT_ASSETS_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>

#include "Animation.h"


class Assets {
public:
    struct Sprite {
        std::string textureName;
        sf::IntRect textureRect;
    };

private:
    // singleton class
    Assets();
    ~Assets() = default;

public:
    static Assets& getInstance();

    // no copy or move
    Assets(const Assets&)               = delete;
    Assets(Assets&&)                    = delete;
    Assets& operator=(const Assets&)    = delete;
    Assets& operator=( Assets&&)        = delete;

private:
    std::map<std::string, std::unique_ptr<sf::Font>>            m_fontMap;
    std::map<std::string, sf::Texture>                          m_textures;
    std::map<std::string, Sprite>                               m_spriteMap;
    std::map<std::string, std::unique_ptr<sf::SoundBuffer>>     m_soundEffects;
    std::map<std::string, Animation>                            m_animationMap;
    std::map<std::string, std::vector<sf::IntRect>>             m_frameSets;


    void loadFonts(const std::string& path);
    void loadTextures(const std::string& path);
    void loadSprts(const std::string& path);
    void loadSounds(const std::string& path);
    void loadJson(const std::string& path);
    void loadAnimations(const std::string& path);

public:
    void loadFromFile(const std::string path);
    void addFont(const std::string &fontName, const std::string &path);
    void addSound(const std::string &soundEffectName, const std::string &path);
    void addTexture(const std::string& textureName, const std::string& path, bool smooth = true);
    void addSprite(const std::string& spriteName, const std::string& textureName, sf::IntRect);

    const sf::Font&             getFont(const std::string &fontName) const;
    const sf::SoundBuffer&      getSound(const std::string &fontName) const;
    const sf::Texture&          getTexture(const std::string& textureName) const;
    const Sprite&               getSprt(const std::string& sprtName) const;
    const Animation&            getAnimation(const std::string& name) const;

};


#endif //BREAKOUT_ASSETS_H
