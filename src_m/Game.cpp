#include "Game.hpp"

#include "Scene_Generator.hpp"
#include "Scene_Open.hpp"

const bool Game::fullscreen = false;

const unsigned int Game::screenW = 800;
const unsigned int Game::screenH = 800;
const unsigned int Game::style = sf::Style::Default;
const std::string Game::title = "Title";

Game::Game(int argc, char** argv)
{
	core.argc = argc;
	core.argv = argv;
}

Game::~Game() {}

void Game::run()
{
  init();
  loop();
}

void Game::init()
{
  core.window = &window;
  core.sceneHandler = &sceneHandler;
  core.resources = &resources;
  
  if(fullscreen)
  {
    window.create(sf::VideoMode::getFullscreenModes()[0], title, sf::Style::Fullscreen);
  }
  else
  {
    window.create(sf::VideoMode(screenW, screenH), title);
  }

  window.setFramerateLimit(60);
  window.setKeyRepeatEnabled(false);

  ImGui::SFML::Init(window);

  switch(core.argc)
  {
    case 1:
    {
      sceneHandler.addScene(std::unique_ptr<Scene>(new Scene_Menu(core)));
    }
      break;
    case 3:
    {
      Encoding e;

      std::string str(core.argv[1]);

      if(str == "al") e = Encoding::AL;
      else if(str == "am") e = Encoding::AM;
      else usage();

      sceneHandler.addScene(std::unique_ptr<Scene>(new Scene_Open(core, e, std::string(core.argv[2]))));
    }
      break;
    default:
      usage();
      break;
  };

  sceneHandler.init();

  clock.restart();
}

void Game::loop()
{
  while (window.isOpen())
  {
    handleEvents();
    update();
    draw();

    sceneHandler.applySceneRequests();
    if(sceneHandler.exitRequest())
    {
      ImGui::SFML::Shutdown();
      window.close();
    }
  }
}

void Game::handleEvents()
{
  sf::Event event;
  while (window.pollEvent(event))
  {
    switch(event.type)
    {
      case sf::Event::Closed:
        window.close();
        break;
        
      default:
        break;
    }

    ImGui::SFML::ProcessEvent(event);

    sceneHandler.activeScene().handleEvents(event);
  }
}

void Game::update()
{
  sf::Time deltatime;
  deltatime = clock.restart();

  ImGui::SFML::Update(window, deltatime);

  sceneHandler.activeScene().update(deltatime);
}

void Game::draw()
{
  window.clear(sf::Color::Black);

  sceneHandler.activeScene().draw(window);
  
  ImGui::SFML::Render(window);

  window.display();
}

void Game::usage()
{
  std::cerr << "Usage: game.exe [al/am file]" << std::endl;
  window.close();
  exit(0);
}