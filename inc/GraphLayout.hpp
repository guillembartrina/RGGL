#ifndef GRAPHLAYOUT_HPP
#define GRAPHLAYOUT_HPP

#include <vector>
#include <list>
#include <cmath>

#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

#define PI 3.14159265358979323846

typedef std::vector<std::list<unsigned int>> Graph_AL;
typedef std::vector<std::vector<bool>> Graph_AM;

enum Distribution
{
    GRID = 0,
    VERTICAL,
    PHYSIC,
    ROUND,
    ROUNDCENTERED
};

class GraphLayout
{
public:

    GraphLayout(sf::Font font);
    ~GraphLayout();

    void load(const Graph_AL& from, Distribution mode = Distribution::GRID);
    void load(const Graph_AM& from, Distribution mode = Distribution::GRID);

    void distribute(Distribution mode);

    void handleEvents(const sf::Event& event);
    void update(sf::Vector2i mouse);
    void draw(sf::RenderWindow& window) const;

    const char** getDistributions() const; //return[0] = numDistributions, return[1...numDistributions] = distributions

private:

    struct node
    {
        sf::Vector2f position;
        std::vector<unsigned int> adjancents;
    };

    sf::Font font;

    std::vector<node> nodes;
    node* currentNode;

    sf::Vector2f view;
    float zoom;
    float nodezoom;

    bool dragging;
    sf::Vector2i prepos;

    void distribute_grid();
    void distribute_vertical();
    void distribute_physic(float area);
    void distribute_round();
    void distribute_roundRec(sf::Vector2f c, std::vector<bool>& v, float aI, float aF, int o, int n);
    void distribute_roundcentered();

    static float distance(sf::Vector2f p1, sf::Vector2f p2);
    static float lenght(sf::Vector2f v);

    static sf::Vector2f force(const std::vector<node>& nodes, float k, int node);

    static const char* distributions[6];
};

#endif