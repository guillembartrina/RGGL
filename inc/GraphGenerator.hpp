
#ifndef GRAPHG_HPP
#define GRAPHG_HPP

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <cstdlib>

#include "RandomGenerator.hpp"

using namespace std;

typedef vector< list<int> > Graph;

static RandomGenerator randGen;

static void getRandomPruferSequence(int num, vector<int>& seq)
{
	for(int i = 0; i < num-2; ++i)
	{
		seq[i] = randGen.getRandInt(0, num-1);
	}
}

static void desequencePruferSequence(int num, const vector<int>& seq, Graph& graph)
{
	priority_queue<int, vector<int>, greater<int>> l;
	vector<int> p(num, 0);
	for(int i = 0; i < num-1; ++i) p[seq[i]]++;
	for(int i = 0; i < num; ++i) if(p[i] == 0) l.push(i);

	for(int i = 0; i < num-1; ++i)
	{
		int u = l.top();
		l.pop();
		int v = seq[i];
		graph[v].insert(graph[v].end(), u);
		graph[u].insert(graph[u].end(), v);

		p[v]--;
		if(p[v] == 0) l.push(v);
	}
}

static void generateRandomUndirectedTree(int num, Graph& graph)
{
	graph = Graph(num);
	
	vector<int> seq(num-1);
	
	getRandomPruferSequence(num, seq);
	seq[num-2] = num-1;
	
	desequencePruferSequence(num, seq, graph);
}

static void applyErdosRenyiModel2(int num, int prob, Graph& graph)
{
	for(int i = 0; i < num; ++i)
	{
		for(int j = i+1; j < num; ++j)
		{
			int random = randGen.getRandInt(0, 9);

			if(random < prob)
			{
				graph[i].insert(graph[i].end(), j);
				graph[j].insert(graph[j].end(), i);
			} 
		}
	}
}

static void generateRandomUndirectedGraph(int num, float prob, Graph& graph)
{
	graph = Graph(num);

	applyErdosRenyiModel2(num, prob*10, graph);
}

#endif