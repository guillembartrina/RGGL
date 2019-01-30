
#include "Scene_Menu.hpp"

Scene_Menu::Scene_Menu(SceneHandler* sceneHandler, Resources* resources, sf::RenderWindow* window) : Scene(sceneHandler, resources), window(window) {}

Scene_Menu::~Scene_Menu() {}

void Scene_Menu::init()
{
    menuText.setFont(_resources->Font("font"));
    menuText.setCharacterSize(24.f);
    menuText.setFillColor(sf::Color(255, 102, 0));
    menuText.setString("MENU");
    menuText.setPosition(704, 15);

    menuRect.setSize(sf::Vector2f(110.f, 170.f));
    menuRect.setFillColor(sf::Color(0, 204, 153));
    menuRect.setPosition(680, 10);

    type = Type::GRAPH;

    typeText.setFont(_resources->Font("font"));
    typeText.setCharacterSize(20.f);
    typeText.setFillColor(sf::Color::Black);
    typeText.setString("Type:");
    typeText.setPosition(690, 50);

    typeValue.setFont(_resources->Font("font"));
    typeValue.setCharacterSize(20.f);
    typeValue.setFillColor(sf::Color::Blue);
    typeValue.setString(typeStrings[type]);
    typeValue.setPosition(745, 50);

    nodesNum = 0;

    nodesText.setFont(_resources->Font("font"));
    nodesText.setCharacterSize(20.f);
    nodesText.setFillColor(sf::Color::Black);
    nodesText.setString("Nod:");
    nodesText.setPosition(690, 80);

    nodesValue.setFont(_resources->Font("font"));
    nodesValue.setCharacterSize(20.f);
    nodesValue.setFillColor(sf::Color::Blue);
    nodesValue.setString(std::to_string(nodesNum));
    nodesValue.setPosition(745, 80);

    onWrite = false;

    distribution = Distribution::GRID;

    distributionText.setFont(_resources->Font("font"));
    distributionText.setCharacterSize(20.f);
    distributionText.setFillColor(sf::Color::Black);
    distributionText.setString("Dist:");
    distributionText.setPosition(690, 110);

    distributionValue.setFont(_resources->Font("font"));
    distributionValue.setCharacterSize(20.f);
    distributionValue.setFillColor(sf::Color::Blue);
    distributionValue.setString(distributionStrings[distribution]);
    distributionValue.setPosition(745, 110);

    genText.setFont(_resources->Font("font"));
    genText.setCharacterSize(30.f);
    genText.setFillColor(sf::Color(255, 0, 102));
    genText.setString("GEN");
    genText.setPosition(705, 140);
    genText.setOutlineThickness(1.f);
    genText.setOutlineColor(sf::Color::Black);

    view = sf::Vector2f(0, 0);
    zoom = 1.f;

    physic = false;
    world = new b2World(gravity);

    b2PolygonShape shape = createRectangle(b2Vec2(window->getSize().x - 40, 20));

    walls[0] = createBody(*world, b2Vec2(20, 0), b2BodyType::b2_staticBody, &shape, 10.f, 0.1f, 0.f);
    walls[1] = createBody(*world, b2Vec2(20, window->getSize().y - 20), b2BodyType::b2_staticBody, &shape, 10.f, 0.1f, 0.f);

    shape = createRectangle(b2Vec2(20, window->getSize().y - 40));

    walls[2] = createBody(*world, b2Vec2(0, 20), b2BodyType::b2_staticBody, &shape, 10.f, 0.1f, 0.f);
    walls[3] = createBody(*world, b2Vec2( window->getSize().x - 20, 20), b2BodyType::b2_staticBody, &shape, 10.f, 0.1f, 0.f);

}

void Scene_Menu::handleEvents(const sf::Event& event)
{
    switch(event.type)
    {
        case sf::Event::MouseButtonPressed:
        {
            switch(event.key.code)
            {
                case sf::Mouse::Left:
                {
                    if(genText.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
                    {
                        if(nodesNum != 0)
                        {
                            physic = false;
                            generate(nodesNum);
                            distribute(distribution);
                        }
                        onWrite = false;
                    }
                    else if(typeValue.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
                    {
                        type = Type(type+1);
                        type = Type(type % Type::T_NUM);
                        typeValue.setString(typeStrings[type]);
                    }
                    else if(nodesValue.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
                    {
                        onWrite = true;
                        nodesNum = 0;
                        nodesValue.setString("_");
                    }
                    else if(distributionValue.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
                    {
                        distribution = Distribution(distribution+1);
                        distribution = Distribution(distribution % Distribution::D_NUM);
                        distributionValue.setString(distributionStrings[distribution]);
                    }

                    for(unsigned int i = 0; i < nodes.size(); ++i)
                    {
                        if(distance(sf::Vector2f(sf::Mouse::getPosition(*window)), (nodes[i].position + view) * zoom) <= 20.f * zoom)
                        {
                            currentNode = &nodes[i];
                            //currentNode->body->SetType(b2BodyType::b2_kinematicBody);
                        }
                    }
                }
                default:
                    break;
            }
        }
            break;
        case sf::Event::MouseButtonReleased:
        {
            switch(event.key.code)
            {
                case sf::Mouse::Left:
                {
                    currentNode = nullptr;
                }
                default:
                    break;
            }
        }
            break;
        
        case sf::Event::KeyPressed:
        {
            if(onWrite)
            {
                switch(event.key.code)
                {
                    default:
                    {
                        if(event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9)
                        {
                            nodesNum *= 10;
                            nodesNum += event.key.code - sf::Keyboard::Num0;
                        }
                    }
                        break;
                }
            }
            else
            {
                switch(event.key.code)
                {
                    case sf::Keyboard::Up:
                    {
                        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && zoom < 5) zoom += 0.1f;
                        else view = view + sf::Vector2f(0.f, 20.f);
                    }
                        break;
                    case sf::Keyboard::Down:
                    {
                        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && zoom > 0.1) zoom -= 0.1f;
                        else view = view + sf::Vector2f(0.f, -20.f);
                    }
                        break;
                    case sf::Keyboard::Left:
                    {
                        view = view + sf::Vector2f(20.f, 0.f);
                    }
                        break;
                    case sf::Keyboard::Right:
                    {
                        view = view + sf::Vector2f(-20.f, 0.f);
                    }
                        break;
                    default:
                        break;
                }
            }
        }
            break;
        default:
            break;
    }
}

void Scene_Menu::update(const sf::Time deltatime)
{
    if(currentNode != nullptr)
    {
        currentNode->position = (sf::Vector2f(sf::Mouse::getPosition(*window)) /zoom) - view;
    }

    if(onWrite && nodesNum > 0)
    {
        nodesValue.setString(to_string(nodesNum));
    }
}

void Scene_Menu::draw(sf::RenderWindow& window) const
{
    sf::Text key;
    sf::CircleShape node;
    sf::Vertex line[2];

    key.setFont(_resources->Font("font"));
    key.setCharacterSize(18.f * zoom);
    key.setFillColor(sf::Color::White);
    key.setString("-");

    node.setRadius(20.f * zoom);
    node.setOrigin(20.f * zoom, 20.f * zoom);
    node.setFillColor(sf::Color::Red);

    line[0] = sf::Vertex(sf::Vector2f(0, 0));
    line[1] = sf::Vertex(sf::Vector2f(0, 0));

    for(unsigned int i = 0; i < nodes.size(); ++i)
    {
        line[0] = (nodes[i].position + view) * zoom;

        for(unsigned int j = 0; j < nodes[i].adjancents.size(); ++j)
        {
            line[1] = (nodes[nodes[i].adjancents[j]].position + view) * zoom;
            window.draw(line, 2, sf::Lines);
        }  
    }

    for(unsigned int i = 0; i < nodes.size(); ++i)
    {
        key.setString(std::to_string(i));
        key.setPosition((nodes[i].position - sf::Vector2f(10.f, 10.f) + view) * zoom);

        node.setPosition((nodes[i].position + view) * zoom);

        window.draw(node);
        window.draw(key);
    }

    window.draw(menuRect);
    window.draw(menuText);
    window.draw(genText);
    window.draw(typeText);
    window.draw(typeValue);
    window.draw(nodesText);
    window.draw(nodesValue);
    window.draw(distributionText);
    window.draw(distributionValue);
}

void Scene_Menu::pause() {}

void Scene_Menu::resume() {}

float Scene_Menu::distance(sf::Vector2f p1, sf::Vector2f p2)
{
    float dX = p1.x - p2.x;
    float dY = p1.y - p2.y;

    float d = sqrt(dX*dX + dY*dY);

    return d;
}

sf::Vector2f Scene_Menu::force1(int node)
{
    int size = nodes.size();
    std::vector<bool> checked(size, false);

    sf::Vector2f position = nodes[node].position;
    sf::Vector2f force = sf::Vector2f(0, 0);

    float k = sqrt(window->getSize().x * window->getSize().y / size);

    for(unsigned int i = 0; i < nodes[node].adjancents.size(); i++)
    {
        sf::Vector2f destination = nodes[nodes[node].adjancents[i]].position;

        sf::Vector2f direction = destination - position; //current -> adjacent (force)
        float angle = atan2f(direction.y, direction.x);

        float tmpForce = pow(distance(position, destination), 2) / k;

        force += sf::Vector2f(tmpForce * cosf(angle), tmpForce * sinf(angle));

        checked[nodes[node].adjancents[i]] = true;
    }

    for(unsigned int i = 0; i < size; i++)
    {
        if(i != node and not checked[i])
        {
            sf::Vector2f destination = nodes[i].position;

            sf::Vector2f direction = destination - position; //current <- adjacent (force)
            float angle = atan2f(direction.y, direction.x);

            float tmpForce = -1 * (pow(k, 2) / distance(position, destination));

            force += sf::Vector2f(tmpForce * cosf(angle), tmpForce * sinf(angle));

            checked[i] = true;
        }
    }

    return force;
}

sf::Vector2f Scene_Menu::force2(std::vector<node>& nodes, float k, int node)
{
    int size = nodes.size();

    sf::Vector2f force = sf::Vector2f(0, 0);
    sf::Vector2f position = nodes[node].position;

    for(int i = 0; i < size; i++)
    {
        if(i != node)
        {
            sf::Vector2f direction = position - nodes[i].position;
            float modulo = tob2Vec2(direction).Length();
            force += (direction / modulo) * float(pow(k, 2) / modulo);
        }
    }

    for(unsigned int i = 0; i < nodes[node].adjancents.size(); i++)
    {
        sf::Vector2f direction = nodes[nodes[node].adjancents[i]].position - position;
        float modulo = tob2Vec2(direction).Length();

        force += (direction / modulo) * float(pow(modulo, 2) / k);
    }

    return force;
}

void Scene_Menu::generate(int n)
{
    currentNode = nullptr;
    nodes = std::vector<node>(n);

    Graph graph;

    if(type == Type::GRAPH) generateRandomUndirectedGraph(n, 0.5, graph);
    else if(type == Type::TREE) generateRandomUndirectedTree(n, graph);

    for(unsigned int i = 0; i < graph.size(); ++i)
    {
        nodes[i].adjancents = std::vector<int>(graph[i].size());

        int j = 0;
        std::list<int>::const_iterator it = graph[i].begin();
        while(it != graph[i].end())
        {
            nodes[i].adjancents[j] = *it;
            ++j;
            ++it;
        }
    }
}

void Scene_Menu::distribute(Distribution mode)
{
    switch (mode)
    {
        case Distribution::GRID:
            gridD(nodes);
            break;
        case Distribution::VERTICAL:
            verticalD(nodes);
            break;
        case Distribution::PHYSIC:
            physicD(nodes, world, area);
            physic = true;
            break;
        case Distribution::ROUND:
        case Distribution::ROUNDCENTERED:
            roundD(nodes, mode);
            break;
    
        default:
            break;
    }
}

void Scene_Menu::gridD(std::vector<node>& nodes)
{
    for(unsigned int i = 0; i < nodes.size(); ++i)
    {
        nodes[i].position = sf::Vector2f(100 + 100*(i%6), 100 + 100*(i/6));
    }
}

void Scene_Menu::verticalD(std::vector<node>& nodes)
{
    map<int, list<int>> levels;
    vector<bool> vis(nodes.size(), false);

    //BFS

    queue<pair<int,int>> q;
    q.push(make_pair(0, 0));

    while(!q.empty())
    {
        pair<int,int> c = q.front();
        q.pop();

        vis[c.first] = true;

        for(unsigned int i = 0; i < nodes[c.first].adjancents.size(); ++i)
        {
            if(!vis[nodes[c.first].adjancents[i]])
            {
                q.push(make_pair(nodes[c.first].adjancents[i], c.second+1));
            }
        }

        levels[c.second].insert(levels[c.second].end(), c.first);
    }

    int y = 100;

    for(unsigned int i = 0; i < levels.size(); ++i)
    {
        int sep = 800/levels[i].size();

        int x = 0;

        list<int>::const_iterator it = levels[i].begin();
        while(it != levels[i].end())
        {
            nodes[*it].position = sf::Vector2f(sep/2 + x*sep, y);
            ++x;
            ++it;
        }

        y += 100;
    }
}

void Scene_Menu::physicD(std::vector<node>& nodes, b2World* world, float area)
{
    RandomGenerator randEngine;

    for(unsigned int i = 0; i < nodes.size(); ++i)
    {
        nodes[i].position = sf::Vector2f(randEngine.getRandInt(20, 780), randEngine.getRandInt(20, 780));

        b2CircleShape circle = createCircle(20.f);

        nodes[i].body = createBody(*world, tob2Vec2(nodes[i].position), b2BodyType::b2_dynamicBody, &circle, 1.f, 0.1f, 0.1f);
    }

    float k = sqrt(area / nodes.size());
    float t = 24.f;

    while(t > 0.001f)
    {
        for(unsigned int i = 1; i < nodes.size(); i++) //BEGIN AT ONE
        {
            sf::Vector2f force = force2(nodes, k, i);
            nodes[i].position = nodes[i].position + ((force / tob2Vec2(force).Length()) * min(tob2Vec2(force).Length(), t));
            //nodes[i].position.x = min(800 / 2, max(-1 * 800 / 2, int(nodes[i].position.x)));
            //nodes[i].position.y = min(800 / 2, max(-1 * 800 / 2, int(nodes[i].position.y)));
        }
        t = max(t - 0.1f, 0.001f);
    }
}

void Scene_Menu::roundD(std::vector<node>& nodes, Distribution mode)
{
    int max = 0;

    if(mode == Distribution::ROUNDCENTERED)
    {
        for(unsigned int i = 0; i < nodes.size(); ++i)
        {
            if(nodes[i].adjancents.size() > nodes[max].adjancents.size()) max = i;
        }
    }

    vector<bool> vis(nodes.size(), false);
    nodes[max].position = sf::Vector2f(400, 400);
    vis[max] = true;

    roundDrec(nodes, sf::Vector2f(400, 400), vis, 0, 2*PI, 1, max);
}

void Scene_Menu::roundDrec(std::vector<node>& nodes, sf::Vector2f c, vector<bool>& v, float aI, float aF, int o, int n)
{
    float aN;
    if(o == 1) aN = (aF-aI)/(nodes[n].adjancents.size());
    else aN = (aF-aI)/(nodes[n].adjancents.size()-1);

    int j = 0;

    for(unsigned int i = 0; i < nodes[n].adjancents.size(); ++i)
    {
        if(!v[nodes[n].adjancents[i]])
        {
            float aCI = aI + aN*j;
            float aCF = aI + aN*(j+1);
            float aC = (aCI+aCF)/2;

            sf::Vector2f vC = sf::Vector2f(cos(aC), sin(aC));
            vC = vC / (sqrt(vC.x*vC.x + vC.y*vC.y));

            nodes[nodes[n].adjancents[i]].position = c + (vC * float(o) * 100.f);
            v[nodes[n].adjancents[i]] = true;

            ++j;

            roundDrec(nodes, c, v, aCI, aCF, o+1, nodes[n].adjancents[i]);
        }
    }
}


