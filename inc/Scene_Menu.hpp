#ifndef SCENE__MENU_HPP
#define SCENE__MENU_HPP

#include <vector>
#include <list>
#include <cmath>

#include "SFML/Graphics.hpp"
#include "Box2D/Box2D.h"

#include "Scene.hpp"

#include "RandomGenerator.hpp"
#include "GraphGenerator.hpp"
#include "Box2DUtils.hpp"

#define PI 3.14159265358979323846

struct node
{
    sf::Vector2f position;
    b2Body* body;
    std::vector<int> adjancents;
};

enum Type
{
    GRAPH = 0,
    TREE,
    T_NUM
};

enum Distribution
{
    GRID = 0,
    VERTICAL,
    PHYSIC,
    ROUND,
    ROUNDCENTERED,
    D_NUM
};

class Scene_Menu : public Scene
{
public:

    Scene_Menu(SceneHandler* sceneHandler, Resources* resources, sf::RenderWindow* window);
    ~Scene_Menu();

    void init();

    void handleEvents(const sf::Event& event);

    void update(const sf::Time deltatime);

    void draw(sf::RenderWindow& window) const;

    void pause();

    void resume();

private:

    sf::RenderWindow* window;

    sf::Text menuText;
    sf::RectangleShape menuRect;
    sf::Text genText;

    sf::Text typeText;
    sf::Text typeValue;
    Type type;
    const char typeStrings[3] = { 'G', 'T'};

    sf::Text nodesText;
    sf::Text nodesValue;
    int nodesNum;
    bool onWrite;

    sf::Text distributionText;
    sf::Text distributionValue;
    Distribution distribution;
    const char distributionStrings[5] = { 'Q', 'V', 'F', 'R', 'C'};

    std::vector<node> nodes;
    node* currentNode;

    sf::Vector2f view;
    float zoom;

    const b2Vec2 gravity = b2Vec2(0.f, 0.f);
    const float area = window->getSize().x * window->getSize().y;

    bool physic;
    b2World* world;

    b2Body* walls[4];

    static float distance(sf::Vector2f p1, sf::Vector2f p2);
    sf::Vector2f force1(int node);
    static sf::Vector2f force2(std::vector<node>& nodes, float k, int node);

    void generate(int nodes);
    void distribute(Distribution mode);

    static void gridD(std::vector<node>& nodes);
    static void verticalD(std::vector<node>& nodes);
    static void physicD(std::vector<node>& nodes, b2World* world, float area);
    static void roundD(std::vector<node>& nodes, Distribution mode);
    static void roundDrec(std::vector<node>& nodes, sf::Vector2f c, vector<bool>& v, float aI, float aF, int o, int n);
};

#endif