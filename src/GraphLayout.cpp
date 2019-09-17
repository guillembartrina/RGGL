
#include "GraphLayout.hpp"

#include "SFML/Graphics.hpp"

#include "GraphGenerator.hpp"

const char* GraphLayout::distributions[6] = {(char*)0x0005, "grid", "vertical", "physic", "round", "roundcentered"};

GraphLayout::GraphLayout(sf::Font font)
{
    this->font = font;

    currentNode = nullptr;

    view = sf::Vector2f(0, 0);
    zoom = 1.f;
    nodezoom = 1.f;

    dragging = false;
}

GraphLayout::~GraphLayout() {}

void GraphLayout::load(const Graph_AL& from, Distribution mode)
{
    unsigned int size = from.size();
    nodes = std::vector<node>(size);

    for(unsigned int i = 0; i < size; i++)
    {
        nodes[i].adjancents = std::vector<unsigned int>(from[i].size());

        unsigned int j = 0;
        std::list<unsigned int>::const_iterator it = from[i].begin();
        while(it != from[i].end())
        {
            nodes[i].adjancents[j] = *it;
            j++;
            ++it;
        }
    }

    distribute(mode);
}

void GraphLayout::load(const Graph_AM& from, Distribution mode)
{
    unsigned int size = from.size();
    nodes = std::vector<node>(size);

    for(unsigned int i = 0; i < size; i++)
    {
        for(unsigned int j = 0; j < size; j++)
        {
            if(from[i][j])
            {
                nodes[i].adjancents.push_back(j);
            }
        }
    }

    distribute(mode);
}

void GraphLayout::distribute(Distribution mode)
{
    switch (mode)
    {
        case Distribution::GRID:
            distribute_grid();
            break;
        case Distribution::VERTICAL:
            distribute_vertical();
            break;
        case Distribution::PHYSIC:
        {
            distribute_physic(800 * 800);
            int max = 0;
            for(unsigned int i = 0; i < nodes.size(); i++)
            {
                if(nodes[i].adjancents.size() > nodes[max].adjancents.size()) max = i;
            }
            sf::Vector2f move = sf::Vector2f(400, 400) - nodes[max].position;
            for(unsigned int i = 0; i < nodes.size(); i++)
            {
                nodes[i].position += move;
            }
        }
            break;
        case Distribution::ROUND:
            distribute_round();
            break;
        case Distribution::ROUNDCENTERED:
            distribute_roundcentered();
            break;
        default:
            break;
    }
}

void GraphLayout::handleEvents(const sf::Event& event)
{
    switch(event.type)
    {
        case sf::Event::MouseButtonPressed:
        {
            switch(event.key.code)
            {
                case sf::Mouse::Right:
                {
                    for(unsigned int i = 0; i < nodes.size(); i++)
                    {
                        sf::Vector2f pos = (sf::Vector2f(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)) / zoom) - view;
                        if(distance(pos, nodes[i].position) <= 20.f * nodezoom)
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
			float factor = (event.mouseWheelScroll.delta >= 0.f ? 1.1f : 0.91f);

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			{
				nodezoom *= factor;
			}
			else
			{
				sf::Vector2f prevPos = sf::Vector2f(sf::Vector2i(event.mouseWheelScroll.x, event.mouseWheelScroll.y));
				zoom *= factor;
				view += (sf::Vector2f(prevPos - (prevPos * factor)) / zoom);
			}
        }
            break;
        default:
            break;
    }
}

void GraphLayout::update(sf::Vector2i mouse)
{
    if(currentNode != nullptr)
    {
        currentNode->position = (sf::Vector2f(mouse) / zoom) - view;
    }

    if(dragging)
    {
        sf::Vector2i currpos = mouse;
        view += (sf::Vector2f(currpos - prepos) / zoom);
        prepos = currpos;
    }
}

void GraphLayout::draw(sf::RenderWindow& window) const
{
    sf::Text key;
    sf::CircleShape node;
    sf::Vertex line[2];

    key.setFont(font);
    key.setCharacterSize(28.f * nodezoom);
    key.setFillColor(sf::Color::White);
    key.setString("-");

    node.setRadius(20.f * nodezoom);
    node.setOrigin(20.f * nodezoom, 20.f * nodezoom);
    node.setFillColor(sf::Color::Red);

    for(unsigned int i = 0; i < nodes.size(); i++)
    {
        line[0] = (nodes[i].position + view) * zoom;

        for(unsigned int j = 0; j < nodes[i].adjancents.size(); j++)
        {
            line[1] = (nodes[nodes[i].adjancents[j]].position + view) * zoom;
            window.draw(line, 2, sf::Lines);
        }  
    }

    for(unsigned int i = 0; i < nodes.size(); i++)
    {
        key.setString(std::to_string(i));
        key.setPosition(((nodes[i].position + view) * zoom) - (sf::Vector2f(12.f, 18.f) * nodezoom));

        node.setPosition((nodes[i].position + view) * zoom);

        window.draw(node);
        window.draw(key);
    }
}

const char** GraphLayout::getDistributions() const
{
    return distributions;
}

//---------------------------------DISTRIBUTIONS--------------------------------------------

void GraphLayout::distribute_grid() //MODIFY
{
    for(unsigned int i = 0; i < nodes.size(); i++)
    {
        nodes[i].position = sf::Vector2f(100 + 100*(i%6), 100 + 100*(i/6));
    }
}

void GraphLayout::distribute_vertical()
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

        for(unsigned int i = 0; i < nodes[c.first].adjancents.size(); i++)
        {
            if(!vis[nodes[c.first].adjancents[i]])
            {
                q.push(std::make_pair(nodes[c.first].adjancents[i], c.second+1));
            }
        }

        levels[c.second].insert(levels[c.second].end(), c.first);
    }

    int y = 100;

    for(unsigned int i = 0; i < levels.size(); i++)
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

void GraphLayout::distribute_physic(float area)
{
    RandomGenerator randEngine;

    int size = nodes.size();

    for(unsigned int i = 0; i < nodes.size(); i++)
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

void GraphLayout::distribute_round()
{
    std::vector<bool> vis(nodes.size(), false);
    nodes[0].position = sf::Vector2f(400, 400);
    vis[0] = true;

    distribute_roundRec(sf::Vector2f(400, 400), vis, 0, 2*PI, 1, 0);
}

void GraphLayout::distribute_roundRec(sf::Vector2f c, std::vector<bool>& v, float aI, float aF, int o, int n)
{
    float aN;
    if(o == 1) aN = (aF-aI)/(nodes[n].adjancents.size());
    else aN = (aF-aI)/(nodes[n].adjancents.size()-1);

    int j = 0;

    for(unsigned int i = 0; i < nodes[n].adjancents.size(); i++)
    {
        if(!v[nodes[n].adjancents[i]])
        {
            float aCI = aI + aN*j;
            float aCF = aI + aN*(j+1);
            float aC = (aCI+aCF)/2;

            sf::Vector2f vC = sf::Vector2f(cos(aC), sin(aC));
            vC = vC / float(sqrt(vC.x*vC.x + vC.y*vC.y));

            nodes[nodes[n].adjancents[i]].position = c + (vC * float(o) * 100.f);
            v[nodes[n].adjancents[i]] = true;

            j++;

            distribute_roundRec(c, v, aCI, aCF, o+1, nodes[n].adjancents[i]);
        }
    }
}

void GraphLayout::distribute_roundcentered()
{
    int max = 0;
    for(unsigned int i = 0; i < nodes.size(); i++)
    {
        if(nodes[i].adjancents.size() > nodes[max].adjancents.size()) max = i;
    }

    std::vector<bool> vis(nodes.size(), false);
    nodes[max].position = sf::Vector2f(400, 400);
    vis[max] = true;

    distribute_roundRec(sf::Vector2f(400, 400), vis, 0, 2*PI, 1, max);
}

//------------------------------------------------------------------------------------------

float GraphLayout::distance(sf::Vector2f p1, sf::Vector2f p2)
{
    float dX = p1.x - p2.x;
    float dY = p1.y - p2.y;

    float d = sqrt(dX*dX + dY*dY);

    return d;
}

float GraphLayout::lenght(sf::Vector2f v)
{
    float d = sqrt(v.x*v.x + v.y*v.y);

    return d;
}

sf::Vector2f GraphLayout::force(const std::vector<node>& nodes, float k, int node)
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