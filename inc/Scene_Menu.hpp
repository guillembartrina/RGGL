#ifndef SCENE__MENU_HPP
#define SCENE__MENU_HPP

#include "Scene.hpp"

#include "Box2D/Box2D.h"
#include "Box2DUtils.hpp"

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

    Type type;
    int tmpType;
    int numNodes;
    float density;
    Distribution distribution;
    int tmpDistribution;

    std::vector<node> nodes;
    node* currentNode;

    sf::Vector2f view;
    float zoom;

    static float distance(sf::Vector2f p1, sf::Vector2f p2);

    void generate(Type type, int nodes, float density);
    void distribute(Distribution mode);

    static sf::Vector2f force(std::vector<node>& nodes, float k, int node);

    static void gridD(std::vector<node>& nodes);
    static void verticalD(std::vector<node>& nodes);
    static void physicD(std::vector<node>& nodes, float area);
    static void roundD(std::vector<node>& nodes);
    static void roundDrec(std::vector<node>& nodes, sf::Vector2f c, vector<bool>& v, float aI, float aF, int o, int n);
    static void roundcenteredD(std::vector<node>& nodes);

};

#endif