
#ifndef GRAPHGENERATOR_HPP
#define GRAPHGENERATOR_HPP

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <cstdlib>

#include "RandomGenerator.hpp"

typedef std::vector<std::list<unsigned int>> Graph_AL;
typedef std::vector<std::vector<bool>> Graph_AM;

typedef std::vector<unsigned int> PruferSequence;

enum Model
{
    GRAPH = 0,
    TREE
};

enum Type
{
    DIRECTED = 0,
    UNDIRECTED
};

class GraphGenerator
{
public:

	GraphGenerator();
	~GraphGenerator();

	template <typename Graph_T>
	static void generate(Graph_T& graph, Model model, Type type, unsigned int num, float density = 1.f);

private:

	static RandomGenerator randGen;

	static void getRandomPruferSequence(PruferSequence& seq, unsigned int num);

	static void buildUndirectedTreeFromPruferSequence(Graph_AL& graph, const PruferSequence& seq);
	static void buildUndirectedTreeFromPruferSequence(Graph_AM& graph, const PruferSequence& seq);

	static void generateRandomUndirectedGraphFromErdosRenyiModel2(Graph_AL& graph, unsigned int num, float density);
	static void generateRandomUndirectedGraphFromErdosRenyiModel2(Graph_AM& graph, unsigned int num, float density);
};

template <typename Graph_T>
void GraphGenerator::generate(Graph_T& graph, Model model, Type type, unsigned int num, float density)
{
	static_assert(std::is_same<Graph_T, Graph_AL>::value or std::is_same<Graph_T, Graph_AM>::value, "Not a type able to hold graphs");

    switch(type)
    {
        case DIRECTED:
        {
            //IMPLEMENT
        }
            break;
        case UNDIRECTED:
        {
            switch(model)
            {
                case GRAPH:
                {
                    generateRandomUndirectedGraphFromErdosRenyiModel2(graph, num, density);
                }
                    break;
                case TREE:
                {
                    PruferSequence seq;
                    getRandomPruferSequence(seq, num);
                    buildUndirectedTreeFromPruferSequence(graph, seq);
                }
                    break;   
            };
        }
            break;   
    };
}

#endif