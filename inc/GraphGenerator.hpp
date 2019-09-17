
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

static RandomGenerator randGen;

static void getRandomPruferSequence(unsigned int num, PruferSequence& seq)
{
	seq = PruferSequence(num-1);

	for(unsigned int i = 0; i < num-2; ++i)
	{
		seq[i] = randGen.getRandInt(0, num-1);
	}

	seq[num-2] = num-1;
}

static void generateRandomUndirectedTreeFromPruferSequence(unsigned int num, const PruferSequence& seq, Graph_AL& graph)
{
	std::priority_queue<unsigned int, std::vector<unsigned int>, std::greater<unsigned int>> l;
	std::vector<unsigned int> p(num, 0);

	for(unsigned int i = 0; i < num-1; ++i) p[seq[i]]++;

	for(unsigned int i = 0; i < num; ++i)
	{
		if(p[i] == 0) l.push(i);
	}

	for(unsigned int i = 0; i < num-1; ++i)
	{
		unsigned int u = l.top();
		l.pop();
		unsigned int v = seq[i];
		graph[v].insert(graph[v].end(), u);
		graph[u].insert(graph[u].end(), v);

		p[v]--;
		if(p[v] == 0) l.push(v);
	}
}

static void generateRandomUndirectedTreeFromPruferSequence(unsigned int num, const PruferSequence& seq, Graph_AM& graph)
{
	std::priority_queue<unsigned int, std::vector<unsigned int>, std::greater<unsigned int>> l;
	std::vector<unsigned int> p(num, 0);

	for(unsigned int i = 0; i < num-1; ++i) p[seq[i]]++;

	for(unsigned int i = 0; i < num; ++i)
	{
		if(p[i] == 0) l.push(i);
	}

	for(unsigned int i = 0; i < num-1; ++i)
	{
		unsigned int u = l.top();
		l.pop();
		unsigned int v = seq[i];
		graph[v][u] = true;
		graph[u][v] = true;

		p[v]--;
		if(p[v] == 0) l.push(v);
	}
}

static void generateRandomUndirectedTree(unsigned int num, Graph_AL& graph)
{
	graph = Graph_AL(num);

	PruferSequence seq;
	
	getRandomPruferSequence(num, seq);
	generateRandomUndirectedTreeFromPruferSequence(num, seq, graph);
}

static void generateRandomUndirectedTree(unsigned int num, Graph_AM& graph)
{
	graph = Graph_AM(num, std::vector<bool>(num, false));

	PruferSequence seq;
	
	getRandomPruferSequence(num, seq);
	generateRandomUndirectedTreeFromPruferSequence(num, seq, graph);
}

static void generateRandomUndirectedGraphFromErdosRenyiModel2(unsigned int num, float prob, Graph_AL& graph)
{
	for(unsigned int i = 0; i < num; ++i)
	{
		for(unsigned int j = i+1; j < num; ++j)
		{
			float random = randGen.getRandFloat(0, 1);

			if(random < prob)
			{
				graph[i].insert(graph[i].end(), j);
				graph[j].insert(graph[j].end(), i);
			} 
		}
	}
}

static void generateRandomUndirectedGraphFromErdosRenyiModel2(unsigned int num, float prob, Graph_AM& graph)
{
	for(unsigned int i = 0; i < num; ++i)
	{
		for(unsigned int j = i+1; j < num; ++j)
		{
			float random = randGen.getRandFloat(0, 1);

			if(random < prob)
			{
				graph[i][j] = true;
				graph[j][i] = true;
			} 
		}
	}
}

static void generateRandomUndirectedGraph(unsigned int num, float prob, Graph_AL& graph)
{
	graph = Graph_AL(num);

	generateRandomUndirectedGraphFromErdosRenyiModel2(num, prob, graph);
}

static void generateRandomUndirectedGraph(unsigned int num, float prob, Graph_AM& graph)
{
	graph = Graph_AM(num, std::vector<bool>(num, false));

	generateRandomUndirectedGraphFromErdosRenyiModel2(num, prob, graph);
}

#endif