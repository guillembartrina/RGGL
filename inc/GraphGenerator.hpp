
#ifndef GRAPHG_HPP
#define GRAPHG_HPP

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

	static void generate(Graph_AL& graph, Model model, Type type, int num, float density = 1.f);
	static void generate(Graph_AM& graph, Model model, Type type, int num, float density = 1.f);

private:

	static RandomGenerator randGen;

	static void getRandomPruferSequence(PruferSequence& seq, unsigned int num);

	static void buildUndirectedTreeFromPruferSequence(Graph_AL& graph, const PruferSequence& seq);
	static void buildUndirectedTreeFromPruferSequence(Graph_AM& graph, const PruferSequence& seq);

	static void generateRandomUndirectedGraphFromErdosRenyiModel2(Graph_AL& graph, unsigned int num, float density);
	static void generateRandomUndirectedGraphFromErdosRenyiModel2(Graph_AM& graph, unsigned int num, float density);
};

#endif