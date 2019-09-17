#ifndef SCENE_HPP
#define SCENE_HPP

#include "SFML/Window.hpp"
#include "SFML/System/Time.hpp"
#include "SFML/Graphics.hpp"

#include "SceneHandler.hpp"
#include "Resources.hpp"

class SceneHandler;

struct Core
{
  int argc;
  char** argv;
	
  sf::RenderWindow* window;
  SceneHandler* sceneHandler;
  Resources* resources;
};

class Scene
{ 
public:

    Scene(Core core);
    virtual ~Scene();

    virtual void init() = 0;

    virtual void handleEvents(const sf::Event& event) = 0;

    virtual void update(const sf::Time deltatime) = 0;

    virtual void draw(sf::RenderWindow& window) const = 0;

    virtual void pause() = 0;

    virtual void resume() = 0;

protected:

    Core core;
};

#endif