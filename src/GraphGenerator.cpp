
#include "GraphGenerator.hpp"

#include <typeinfo>

RandomGenerator GraphGenerator::randGen;

GraphGenerator::GraphGenerator() {}

GraphGenerator::~GraphGenerator() {}

void GraphGenerator::getRandomPruferSequence(PruferSequence& seq, unsigned int num)
{
	seq = PruferSequence(num-1);

	for(unsigned int i = 0; i < num-2; i++)
	{
		seq[i] = randGen.getRandInt(0, num-1);
	}

	seq[num-2] = num-1;
}

void GraphGenerator::buildUndirectedTreeFromPruferSequence(Graph_AL& graph, const PruferSequence& seq)
{
	unsigned int num = seq.size()+1;
    graph = Graph_AL(num);

	std::priority_queue<unsigned int, std::vector<unsigned int>, std::greater<unsigned int>> l;
	std::vector<unsigned int> p(num, 0);

	for(unsigned int i = 0; i < num-1; i++) p[seq[i]]++;

	for(unsigned int i = 0; i < num; i++)
	{
		if(p[i] == 0) l.push(i);
	}

	for(unsigned int i = 0; i < num-1; i++)
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

void GraphGenerator::buildUndirectedTreeFromPruferSequence(Graph_AM& graph, const PruferSequence& seq)
{
	unsigned int num = seq.size()+1;
    graph = Graph_AM(num, std::vector<bool>(num, false));

	std::priority_queue<unsigned int, std::vector<unsigned int>, std::greater<unsigned int>> l;
	std::vector<unsigned int> p(num, 0);

	for(unsigned int i = 0; i < num-1; i++) p[seq[i]]++;

	for(unsigned int i = 0; i < num; i++)
	{
		if(p[i] == 0) l.push(i);
	}

	for(unsigned int i = 0; i < num-1; i++)
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

void GraphGenerator::generateRandomUndirectedGraphFromErdosRenyiModel2(Graph_AL& graph, unsigned int num, float density)
{
    graph = Graph_AL(num);

	for(unsigned int i = 0; i < num; i++)
	{
		for(unsigned int j = i+1; j < num; j++)
		{
			float random = randGen.getRandFloat(0, 1);

			if(random < density)
			{
				graph[i].insert(graph[i].end(), j);
				graph[j].insert(graph[j].end(), i);
			} 
		}
	}
}

void GraphGenerator::generateRandomUndirectedGraphFromErdosRenyiModel2(Graph_AM& graph, unsigned int num, float density)
{
    graph = Graph_AM(num, std::vector<bool>(num, false));

	for(unsigned int i = 0; i < num; i++)
	{
		for(unsigned int j = i+1; j < num; j++)
		{
			float random = randGen.getRandFloat(0, 1);

			if(random < density)
			{
				graph[i][j] = true;
				graph[j][i] = true;
			} 
		}
	}
}