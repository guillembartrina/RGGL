
#include "Scene_Menu.hpp"

Scene_Menu::Scene_Menu(SceneHandler* sceneHandler, Resources* resources, sf::RenderWindow* window)
: Scene(sceneHandler, resources)
, window(window)
{}

Scene_Menu::~Scene_Menu() {}

void Scene_Menu::init()
{
    type = Type::GRAPH;
    tmpType = 0;
    numNodes = 0;
    density = 0.5f;
    distribution = Distribution::GRID;
    tmpDistribution = 0;
    currentNode = nullptr;

    view = sf::Vector2f(0, 0);
    zoom = 1.f;

    dragging = false;
}

void Scene_Menu::handleEvents(const sf::Event& event)
{
    switch(event.type)
    {
        case sf::Event::KeyPressed:
        {
            switch(event.key.code)
            {
                case sf::Keyboard::Up:
                {
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && zoom < 5) zoom += 0.1f;
                    else view += (sf::Vector2f(0.f, 20.f) / zoom);
                }
                    break;
                case sf::Keyboard::Down:
                {
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && zoom > 0.1) zoom -= 0.1f;
                    else view += (sf::Vector2f(0.f, -20.f) / zoom);
                }
                    break;
                case sf::Keyboard::Left:
                {
                    view += (sf::Vector2f(20.f, 0.f) / zoom);
                }
                    break;
                case sf::Keyboard::Right:
                {
                    view += (sf::Vector2f(-20.f, 0.f) / zoom);
                }
                    break;
                default:
                    break;
            }
        }
            break;
        case sf::Event::MouseButtonPressed:
        {
            if(not ImGui::IsMouseHoveringAnyWindow())
            {
                switch(event.key.code)
                {
                    case sf::Mouse::Right:
                    {
                        for(unsigned int i = 0; i < nodes.size(); ++i)
                        {
                            if(distance(sf::Vector2f(sf::Mouse::getPosition(*window)), (nodes[i].position + view) * zoom) <= 20.f * zoom)
                            {
                                currentNode = &nodes[i];
                            }
                        }
                    }
                        break;
                    case sf::Mouse::Left:
                    {
                        dragging = true;
                        prepos = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
                    }
                        break;
                    default:
                        break;
                }
            }
        }
            break;
        case sf::Event::MouseButtonReleased:
        {
            switch(event.key.code)
            {
                case sf::Mouse::Right:
                {
                    currentNode = nullptr;
                }
                    break;
                case sf::Mouse::Left:
                {
                    dragging = false;
                }
                    break;
                default:
                    break;
            }
        }
            break;
        case sf::Event::MouseWheelScrolled:
        {
            sf::Vector2f prevPos = sf::Vector2f(sf::Mouse::getPosition(*window));

            float factor = (event.mouseWheelScroll.delta >= 0.f ? 1.1f : 0.91f);
            zoom *= factor;

            view += (sf::Vector2f(prevPos - (prevPos * factor)) / zoom);
        }
            break;
        default:
            break;
    }
}

void Scene_Menu::update(const sf::Time deltatime)
{
    const char* distributions[] = {"grid", "vertical", "physic", "round", "roundcentered"};

    ImGui::SetNextWindowSize(ImVec2(140, 180));
    ImGui::SetNextWindowPos(ImVec2(window->getSize().x - 145, 5));
    ImGui::Begin("MENU", NULL, ImGuiWindowFlags_NoResize);
    ImGui::RadioButton("graph", &tmpType, 0);
    ImGui::SameLine(0, 8.f);
    ImGui::RadioButton("tree", &tmpType, 1);
    ImGui::Separator();
    ImGui::PushItemWidth(45.f);
    ImGui::InputScalar(" nodes", ImGuiDataType_U32, &numNodes);
    ImGui::Separator();
    if(Type(tmpType) == Type::TREE)
    {
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }
    ImGui::PushItemWidth(80.f);
    ImGui::SliderFloat(" dens.", &density, 0.f, 1.f);
    if(Type(tmpType) == Type::TREE)
    {
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
    }
    ImGui::Separator();
    ImGui::PushItemWidth(80.f);
    ImGui::Combo(" dist.", &tmpDistribution, distributions, IM_ARRAYSIZE(distributions));
    ImGui::Separator();
    if(ImGui::Button("GEN", ImVec2(125, 20)))
    {
        type = Type(tmpType);
        distribution = Distribution(tmpDistribution);

        if(numNodes > 0)
        {
            generate(type, numNodes, density);
            distribute(distribution);
        }
    }
    ImGui::End();

    if(currentNode != nullptr)
    {
        currentNode->position = (sf::Vector2f(sf::Mouse::getPosition(*window)) / zoom) - view;
    }

    if(dragging)
    {
        sf::Vector2i currpos = sf::Mouse::getPosition(*window);
        view += (sf::Vector2f(currpos - prepos) / zoom);
        prepos = currpos;
    }
}

void Scene_Menu::draw(sf::RenderWindow& window) const
{
    sf::Text key;
    sf::CircleShape node;
    sf::Vertex line[2];

    key.setFont(_resources->Font("font"));
    key.setCharacterSize(28.f);
    key.setFillColor(sf::Color::White);
    key.setString("-");

    node.setRadius(20.f);
    node.setOrigin(20.f, 20.f);
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
        key.setPosition(((nodes[i].position + view) * zoom) - sf::Vector2f(12.f, 18.f));

        node.setPosition((nodes[i].position + view) * zoom);

        window.draw(node);
        window.draw(key);
    }
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

float Scene_Menu::lenght(sf::Vector2f v)
{
    float d = sqrt(v.x*v.x + v.y*v.y);

    return d;
}

void Scene_Menu::generate(Type type, int n, float density)
{
    currentNode = nullptr;
    nodes = GraphLayout(n);

    Graph_AL graph;

    if(type == Type::GRAPH) generateRandomUndirectedGraph(n, density, graph);
    else if(type == Type::TREE) generateRandomUndirectedTree(n, graph);

    for(unsigned int i = 0; i < graph.size(); ++i)
    {
        nodes[i].adjancents = std::vector<unsigned int>(graph[i].size());

        int j = 0;
        std::list<unsigned int>::const_iterator it = graph[i].begin();
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
        {
            physicD(nodes, window->getSize().x * window->getSize().y);
            int max = 0;
            for(unsigned int i = 0; i < nodes.size(); ++i)
            {
                if(nodes[i].adjancents.size() > nodes[max].adjancents.size()) max = i;
            }
            sf::Vector2f move = sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2) - nodes[max].position;
            for(unsigned int i = 0; i < nodes.size(); ++i)
            {
                nodes[i].position += move;
            }
        }
            break;
        case Distribution::ROUND:
            roundD(nodes);
            break;
        case Distribution::ROUNDCENTERED:
            roundcenteredD(nodes);
            break;
        default:
            break;
    }
}

sf::Vector2f Scene_Menu::force(GraphLayout& nodes, float k, int node)
{
    int size = nodes.size();

    sf::Vector2f force = sf::Vector2f(0, 0);
    sf::Vector2f position = nodes[node].position;

    for(int i = 0; i < size; i++)
    {
        if(i != node)
        {
            sf::Vector2f direction = position - nodes[i].position;
            float modulo = lenght(direction);
            force += (direction / modulo) * float(pow(k, 2) / modulo);
        }
    }

    for(unsigned int i = 0; i < nodes[node].adjancents.size(); i++)
    {
        sf::Vector2f direction = nodes[nodes[node].adjancents[i]].position - position;
        float modulo = lenght(direction);
        force += (direction / modulo) * float(pow(modulo, 2) / k);
    }

    return force;
}

void Scene_Menu::gridD(GraphLayout& nodes)
{
    for(unsigned int i = 0; i < nodes.size(); ++i)
    {
        nodes[i].position = sf::Vector2f(100 + 100*(i%6), 100 + 100*(i/6));
    }
}

void Scene_Menu::verticalD(GraphLayout& nodes)
{
    std::map<int, std::list<unsigned int>> levels;
    std::vector<bool> vis(nodes.size(), false);

    //BFS

    std::queue<std::pair<int,int>> q;
    q.push(std::make_pair(0, 0));

    while(!q.empty())
    {
        std::pair<int,int> c = q.front();
        q.pop();

        vis[c.first] = true;

        for(unsigned int i = 0; i < nodes[c.first].adjancents.size(); ++i)
        {
            if(!vis[nodes[c.first].adjancents[i]])
            {
                q.push(std::make_pair(nodes[c.first].adjancents[i], c.second+1));
            }
        }

        levels[c.second].insert(levels[c.second].end(), c.first);
    }

    int y = 100;

    for(unsigned int i = 0; i < levels.size(); ++i)
    {
        int sep = 800/levels[i].size();

        int x = 0;

        std::list<unsigned int>::const_iterator it = levels[i].begin();
        while(it != levels[i].end())
        {
            nodes[*it].position = sf::Vector2f(sep/2 + x*sep, y);
            ++x;
            ++it;
        }

        y += 100;
    }
}

void Scene_Menu::physicD(GraphLayout& nodes, float area)
{
    RandomGenerator randEngine;

    int size = nodes.size();

    for(unsigned int i = 0; i < nodes.size(); ++i)
    {
        nodes[i].position = sf::Vector2f(randEngine.getRandInt(20, 780), randEngine.getRandInt(20, 780));
    }

    float k = sqrt(area / size);
    float t = 10 * sqrt(size);

    while(t > 1.f)
    {
        for(unsigned int i = 0; i < size; i++)
        {
            sf::Vector2f f = force(nodes, k, i);
            float modulus = lenght(f);
            nodes[i].position = nodes[i].position + ((f / modulus) * std::min(modulus, t));
        }
        t = std::max(t * 0.95f, 1.f);
    }
}

void Scene_Menu::roundD(GraphLayout& nodes)
{
    std::vector<bool> vis(nodes.size(), false);
    nodes[0].position = sf::Vector2f(400, 400);
    vis[0] = true;

    roundDrec(nodes, sf::Vector2f(400, 400), vis, 0, 2*PI, 1, 0);
}

void Scene_Menu::roundDrec(GraphLayout& nodes, sf::Vector2f c, std::vector<bool>& v, float aI, float aF, int o, int n)
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

void Scene_Menu::roundcenteredD(GraphLayout& nodes)
{
    int max = 0;
    for(unsigned int i = 0; i < nodes.size(); ++i)
    {
        if(nodes[i].adjancents.size() > nodes[max].adjancents.size()) max = i;
    }

    std::vector<bool> vis(nodes.size(), false);
    nodes[max].position = sf::Vector2f(400, 400);
    vis[max] = true;

    roundDrec(nodes, sf::Vector2f(400, 400), vis, 0, 2*PI, 1, max);
}
