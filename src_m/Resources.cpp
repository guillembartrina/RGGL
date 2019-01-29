
#include "Resources.hpp"

Resources::Resources()
{
    resourcePath = "rsc/";
}

Resources::~Resources()
{
    _textures.clear();
    _fonts.clear();
    _sounds.clear();
    _musics.clear();
}

void Resources::setResourcePath(const std::string& path)
{
    resourcePath = path;
}

const sf::Texture& Resources::Texture(const std::string& id)
{
    if(_textures.find(id) == _textures.end())
    {
        if(!_textures[id].loadFromFile(resourcePath + "textures/" + id + ".png"))
        {
            std::cerr << "Error loading texture <" << id << ">." << std::endl;
        }

    }

    return _textures.at(id);
}

const sf::Font& Resources::Font(const std::string& id)
{
    if(_fonts.find(id) == _fonts.end())
    {
        if(!_fonts[id].loadFromFile(resourcePath + "fonts/" + id + ".ttf"))
        {
            std::cerr << "Error loading font <" << id << ">." << std::endl;
        }
    }

    return _fonts.at(id);
}

const sf::SoundBuffer& Resources::Sound(const std::string& id)
{
    if(_sounds.find(id) == _sounds.end())
    {
        if(!_sounds[id].loadFromFile(resourcePath + "sounds/" + id + ".wav"))
        {
            std::cerr << "Error loading sound <" << id << ">." << std::endl;
        }
    }

    return _sounds.at(id);
}

sf::Music& Resources::Music(const std::string& id)
{
    if(_musics.find(id) == _musics.end())
    {
        if(!_musics[id].openFromFile(resourcePath + "musics/" + id + ".ogg"))
        {
            std::cerr << "Error loading music <" << id << ">." << std::endl;
        }
    }

    return _musics.at(id);
}

void Resources::clearTextures()
{
    _textures.clear();
}

void Resources::clearFonts()
{
    _fonts.clear();
}

void Resources::clearSounds()
{
    _sounds.clear();
}

void Resources::clearMusics()
{
    _musics.clear();
}