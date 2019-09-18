#ifndef SCENE_OPEN_HPP
#define SCENE_OPEN_HPP

#include "Scene.hpp"

#include <vector>
#include <list>
#include <cmath>

#include "GraphLayout.hpp"

enum Encoding
{
    AL = 0,
    AM
};

class Scene_Open : public Scene
{
public:

    Scene_Open(Core core, Encoding enc, const std::string& file);
    ~Scene_Open();

    void init();

    void handleEvents(const sf::Event& event);

    void update(const sf::Time deltatime);

    void draw(sf::RenderWindow& window) const;

    void pause();

    void resume();

private:

    Encoding enc;
    std::string file;

    GraphLayout gl;

    static void readFile_AL(const std::string& file, Graph_AL& graph);
    static void readFile_AM(const std::string& file, Graph_AM& graph);

};  

#endif