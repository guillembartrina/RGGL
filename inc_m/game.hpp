#ifndef GAME_HPP
#define GAME_HPP

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/Audio.hpp"

#include "SceneHandler.hpp"
#include "Resources.hpp"

#include "imgui.h"
#include "imguiSFML.h"

class Game : public sf::NonCopyable
{
public:

  Game(int argc, char** argv);
  ~Game();

  void run();

private:

  static const bool fullscreen;

  static const unsigned int screenW;
  static const unsigned int screenH;
  static const unsigned int style;
  static const std::string title;

  sf::RenderWindow window;
  sf::Clock clock;

  SceneHandler sceneHandler;

  Resources resources;

  Core core;

  void init();
  void loop();

  void handleEvents();
  void update();
  void draw();
};

#endif