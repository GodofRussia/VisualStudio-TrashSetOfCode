/*
Требуется отыскать самый выгодный маршрут между городами.
Требования: время работы O((N+M)logN), где N-количество городов, M-известных дорог между ними.
Формат входных данных.
Первая строка содержит число N – количество городов.
Вторая строка содержит число M - количество дорог.
Каждая следующая строка содержит описание дороги (откуда, куда, время в пути).
Последняя строка содержит маршрут (откуда и куда нужно доехать).
Формат выходных данных.
Вывести длину самого выгодного маршрута
*/

#include <vector>
#include <iostream>
#include <cassert>
#include <queue>
#include <set>
#include <limits.h>

using std::priority_queue;
using std::vector;
using std::pair;
using std::set;

struct Edge {
	int destVertex;
	int weight;

	Edge(int to_, int weight_) : destVertex(to_), weight(weight_) { }
};

// Реализовано на хранение ориентированного невзвешенного графа в виде списков смежности
class Graph {
public:
	Graph(int vertexCount);

	void AddEdge(int from, int to, int weigth);
	int VerticesCount() const;
	vector<int> GetNextVertices(int vertex) const;
	vector<Edge> GetNextEdges(int vertex) const;

private:
	vector<vector<Edge>> adjLists;
};

Graph::Graph(int vertexCount) {
	adjLists.resize(vertexCount);
}

int Graph::VerticesCount() const {
	return adjLists.size();
}

void Graph::AddEdge(int from, int to, int weight) {
	assert(from >= 0 && from < adjLists.size());
	assert(to >= 0 && to < adjLists.size());
	Edge edge1(to, weight);
	adjLists[from].push_back(edge1);
	Edge edge2(from, weight);
	adjLists[to].push_back(edge2);
}

vector<int> Graph::GetNextVertices(int vertex) const {
	assert(vertex >= 0 && vertex < adjLists.size());
	std::vector<int> vertexList;
	for (int i = 0; i < adjLists[vertex].size(); ++i) {
		vertexList.push_back(adjLists[vertex][i].destVertex);
	}

	return vertexList;
}

vector<Edge> Graph::GetNextEdges(int vertex) const {
	assert(vertex >= 0 && vertex < adjLists.size());
	return adjLists[vertex];
}

int Dijkstra(const Graph& graph, int from, int to) {
	vector<int> d(graph.VerticesCount(), INT_MAX);
	set<pair<int, int>> queue;
	d[from] = 0;
	queue.insert(std::make_pair(d[from], from));
	while (!queue.empty()) {
		int current = queue.begin()->second;
		queue.erase(queue.begin());
		vector<Edge> adjList = graph.GetNextEdges(current);
		for (int i = 0; i < adjList.size(); ++i) {
			if (d[adjList[i].destVertex] == INT_MAX) {
				d[adjList[i].destVertex] = d[current] + adjList[i].weight;
				queue.insert(std::make_pair(d[adjList[i].destVertex], adjList[i].destVertex));
			} else if (d[adjList[i].destVertex] > d[current] + adjList[i].weight) {
				queue.erase(std::make_pair(d[adjList[i].destVertex], adjList[i].destVertex));
				d[adjList[i].destVertex] = d[current] + adjList[i].weight;
				queue.insert(std::make_pair(d[adjList[i].destVertex], adjList[i].destVertex));
			}
		}
	}

	return d[to];
}


int main() {
	int N = 0, M = 0;  // Количество вершин, ребёр (городов и дорог между ними)
	std::cin >> N >> M;
	assert(N <= 10000);
	assert(M <= 250000);

	Graph graph(N);
	for (int i = 0; i < M; ++i) {
		int u = 0, w = 0, weight = 0;
		std::cin >> u >> w >> weight;
		graph.AddEdge(u, w, weight);
	}

	int u = 0, w = 0;  // Пара вершин откуда-куда для запроса
	std::cin >> u >> w;
	int shortestLength = Dijkstra(graph, u, w);
	std::cout << shortestLength;

	return 0;
}
