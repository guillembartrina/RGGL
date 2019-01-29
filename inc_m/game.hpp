#ifndef GAME_HPP
#define GAME_HPP

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/Audio.hpp"

#include "SceneHandler.hpp"
#include "Resources.hpp"

class Game : public sf::NonCopyable
{
public:

  Game();
  ~Game();

  void run();

private:

  static const unsigned int W;
  static const unsigned int H;
  static const std::string title;

  sf::RenderWindow _window;
  sf::Clock _clock;

  SceneHandler _sceneHandler;

  Resources _resources;

  void init();
  void loop();

  void update();
  void draw();
  void handleEvents();
};

#endif