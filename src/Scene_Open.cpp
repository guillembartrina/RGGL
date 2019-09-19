
#include "Scene_Open.hpp"

#include "GraphIO.hpp"

Scene_Open::Scene_Open(Core core, Encoding enc, const std::string& file)
: Scene(core)
, gl(core.resources->Font("font"))
{
    this->enc = enc;
    this->file = file;
}

Scene_Open::~Scene_Open() {}

void Scene_Open::init()
{
    switch(enc)
    {
        case AL:
        {
            Graph_AL graph;
            readFile<Graph_AL>(graph, file);
            gl.load(graph, Distribution::PHYSIC);
        }
            break;
        case AM:
        {
            Graph_AM graph;
            readFile<Graph_AM>(graph, file);
            gl.load(graph, Distribution::PHYSIC);
        }
            break;
    };
}

void Scene_Open::handleEvents(const sf::Event& event)
{
    gl.handleEvents(event);
}

void Scene_Open::update(const sf::Time deltatime)
{
    gl.update(sf::Mouse::getPosition(*core.window));
}

void Scene_Open::draw(sf::RenderWindow& window) const
{
    gl.draw(window);
}

void Scene_Open::pause() {}

void Scene_Open::resume() {}
