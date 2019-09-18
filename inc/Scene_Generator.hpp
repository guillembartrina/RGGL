#ifndef SCENE__MENU_HPP
#define SCENE__MENU_HPP

#include "Scene.hpp"

#include <vector>
#include <list>
#include <cmath>

#include "imgui.h"
#include "imgui_internal.h"
#include "imguiSFML.h"

#include "GraphGenerator.hpp"
#include "GraphLayout.hpp"

class Scene_Menu : public Scene
{
public:

    Scene_Menu(Core core);
    ~Scene_Menu();

    void init();

    void handleEvents(const sf::Event& event);

    void update(const sf::Time deltatime);

    void draw(sf::RenderWindow& window) const;

    void pause();

    void resume();

private:

    Model model;
    int tmpModel;
    int numNodes;
    float density;
    Distribution distribution;
    int tmpDistribution;

    GraphLayout gl;
};

#endif