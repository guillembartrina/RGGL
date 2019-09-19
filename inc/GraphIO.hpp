
#ifndef GRAPHIO_HPP
#define GRAPHIO_HPP

#include <fstream>

typedef std::vector<std::list<unsigned int>> Graph_AL;
typedef std::vector<std::vector<bool>> Graph_AM;

static void readFileAux(Graph_AL& graph, const std::string& file)
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

static void readFileAux(Graph_AM& graph, const std::string& file)
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

static void writeFileAux(const Graph_AL& graph, const std::string& file)
{
    std::fstream destination(file, std::ios_base::out | std::ios_base::trunc);

    unsigned int num = graph.size();

    destination << num << std::endl;

    for(unsigned int i = 0; i < num; i++)
    {
        std::list<unsigned int>::const_iterator it = graph[i].cbegin();
        while(it != graph[i].cend())
        {
            destination << *it << ' ';
            it++;
        }
        destination << -1 << std::endl;
    }

    destination.close();
}

static void writeFileAux(const Graph_AM& graph, const std::string& file)
{
    std::fstream destination(file, std::ios_base::out | std::ios_base::trunc);

    unsigned int num = graph.size();

    destination << num << std::endl;

    for(unsigned int i = 0; i < num; i++)
    {
        for(unsigned int j = 0; j < num; j++)
        {
            destination << (graph[i][j] ? 't' : 'f') << ' ';
        }
        destination << std::endl;
    }

    destination.close();
}

template<typename Graph_T>
static void readFile(Graph_T& graph, const std::string& file)
{
    static_assert(std::is_same<Graph_T, Graph_AL>::value or std::is_same<Graph_T, Graph_AM>::value, "Not a type able to hold graphs");

    readFileAux(graph, file);
}

template<typename Graph_T>
static void writeFile(const Graph_T& graph, const std::string& file)
{
    static_assert(std::is_same<Graph_T, Graph_AL>::value or std::is_same<Graph_T, Graph_AM>::value, "Not a type able to hold graphs");

    writeFileAux(graph, file);
}

#endif