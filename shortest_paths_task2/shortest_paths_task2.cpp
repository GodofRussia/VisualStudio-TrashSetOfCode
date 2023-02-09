/*
Требования: сложность O(V + E).
Формат ввода.
v : кол - во вершин(макс. 50000),
n : кол - во ребер(макс. 200000),
n пар реберных вершин,
пара вершин u, w для запроса.
Формат вывода.
Количество кратчайших путей от u к w.
*/

#include <vector>
#include <iostream>
#include <cassert>
#include <queue>
#include <limits.h>

using std::queue;
using std::vector;

// Реализовано на хранение ориентированного невзвешенного графа в виде списков смежности
class Graph {
public:
	Graph(int vertexCount);

	void AddEdge(int from, int to);
	int VerticesCount() const;
	vector<int> GetNextVertices(int vertex) const;
private:
	vector<vector<int>> adjLists;
	int _vertexCount;
};

Graph::Graph(int vertexCount) {
	adjLists.resize(vertexCount);
}

int Graph::VerticesCount() const {
	return adjLists.size();
}


void Graph::AddEdge(int from, int to) {
	assert(from >= 0 && from < adjLists.size());
	assert(to >= 0 && to < adjLists.size());
	adjLists[from].push_back(to);
	adjLists[to].push_back(from);
}

vector<int> Graph::GetNextVertices(int vertex) const {
	assert(vertex >= 0 && vertex < adjLists.size());
	return adjLists[vertex];
}

int BFSPathsCount(const Graph& graph, int from, int to) {
	vector<int> minimumPath(graph.VerticesCount(), INT_MAX);
	vector<int> minimumPathCount(graph.VerticesCount(), 0);
	queue<int> queue;
	queue.push(from);
	minimumPath[from] = 0;
	minimumPathCount[from] = 1;

	while (!queue.empty()) {
		int current = queue.front();
		queue.pop();
		vector<int> adjList = graph.GetNextVertices(current);
		for (int i = 0; i < adjList.size(); ++i) {
			if (minimumPath[adjList[i]] > minimumPath[current] + 1) {
				minimumPath[adjList[i]] = minimumPath[current] + 1;
				minimumPathCount[adjList[i]] = minimumPathCount[current];
				queue.push(adjList[i]);
			}
			else if (minimumPath[adjList[i]] == minimumPath[current] + 1) {
				minimumPathCount[adjList[i]] += minimumPathCount[current];
			}
		}
	}

	return minimumPathCount[to];
}


int main() {
	int v = 0, n = 0;  // Количество вершин, ребёр
	std::cin >> v >> n;
	assert(v <= 50000);
	assert(n <= 200000);

	Graph graph(v);
	for (int i = 0; i < n; ++i) {
		int u = 0, w = 0;
		std::cin >> u >> w;
		graph.AddEdge(u, w);
	}

	int u = 0, w = 0;  // Пара вершин откуда-куда для запроса
	std::cin >> u >> w;
	int shortestPathsCount = BFSPathsCount(graph, u, w);
	std::cout << shortestPathsCount;

	return 0;
}
