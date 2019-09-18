
#include "Scene_Open.hpp"

#include <fstream>

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
            readFile_AL(file, graph);
            gl.load(graph, Distribution::PHYSIC);
        }
            break;
        case AM:
        {
            Graph_AM graph;
            readFile_AM(file, graph);
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

void Scene_Open::readFile_AL(const std::string& file, Graph_AL& graph)
{
    std::fstream source(file, std::ios_base::in);

    unsigned int num;
    source >> num;

    graph = Graph_AL(num);

    for(unsigned int i = 0; i < num; i++)
    {
        int adj;
        while(source >> adj and adj != -1)
        {
            graph[i].insert(graph[i].end(), adj);
        }
    }

    source.close();
}

void Scene_Open::readFile_AM(const std::string& file, Graph_AM& graph)
{
    std::fstream source(file, std::ios_base::in);

    unsigned int num;
    source >> num;

    graph = Graph_AM(num, std::vector<bool>(num, false));

    for(unsigned int i = 0; i < num; i++)
    {
        for(unsigned int j = 0; j < num; j++)
        {
            char c;
            source >> c;
            graph[i][j] = (c == 't' ? true : false);
        }
    }

    source.close();
}
