#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <map>
#include <iostream>

#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/Audio/Music.hpp"

class Resources : public sf::NonCopyable
{
public:

    Resources();
    ~Resources();

    void setResourcePath(const std::string& path);

    const sf::Texture& Texture(const std::string& id);
    const sf::Font& Font(const std::string& id);
    const sf::SoundBuffer& Sound(const std::string& id);
    sf::Music& Music(const std::string& id);

    void clearTextures();
    void clearFonts();
    void clearSounds();
    void clearMusics();

private:

    std::string resourcePath;

    std::map<std::string, sf::Texture> _textures;
    std::map<std::string, sf::Font> _fonts;
    std::map<std::string, sf::SoundBuffer> _sounds;
    std::map<std::string, sf::Music> _musics;
};

#endif