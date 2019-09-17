#ifndef SCENE__MENU_HPP
#define SCENE__MENU_HPP

#include "Scene.hpp"

#include "imgui.h"
#include "imgui_internal.h"
#include "imguiSFML.h"

#include "SFML/Graphics.hpp"

#include <vector>
#include <list>
#include <cmath>

#include "RandomGenerator.hpp"
#include "GraphGenerator.hpp"

#define PI 3.14159265358979323846

struct node
{
    sf::Vector2f position;
    std::vector<unsigned int> adjancents;
};

typedef std::vector<node> GraphLayout;

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

    Type type;
    int tmpType;
    int numNodes;
    float density;
    Distribution distribution;
    int tmpDistribution;

    GraphLayout nodes;
    node* currentNode;

    sf::Vector2f view;
    float zoom;

    bool dragging;
    sf::Vector2i prepos;

    static float distance(sf::Vector2f p1, sf::Vector2f p2);
    static float lenght(sf::Vector2f v);

    void generate(Type type, int nodes, float density);
    void distribute(Distribution mode);

    static sf::Vector2f force(GraphLayout& nodes, float k, int node);

    static void gridD(GraphLayout& nodes);
    static void verticalD(GraphLayout& nodes);
    static void physicD(GraphLayout& nodes, float area);
    static void roundD(GraphLayout& nodes);
    static void roundDrec(GraphLayout& nodes, sf::Vector2f c, std::vector<bool>& v, float aI, float aF, int o, int n);
    static void roundcenteredD(GraphLayout& nodes);

};

#endif