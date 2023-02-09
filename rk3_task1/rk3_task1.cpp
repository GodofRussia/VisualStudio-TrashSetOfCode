
#include <iostream>
#include <vector>
#include <cassert>
#include <queue>
#include <set>
#include <limits.h>

using std::vector;
using std::pair;
using std::set;
using std::queue;

class Graph {
public:
	Graph(int vertexCount);

	void AddEdge(int from, int to);
	int VerticesCount() const;
	vector<int> GetNextVertices(int vertex) const;

private:
	vector<vector<int>> adjLists;
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

bool isTree(const Graph& graph, int from) {
	vector<bool> visited(graph.VerticesCount(), false);
	vector<int> howManyChilds(graph.VerticesCount(), false);
	queue<int> queue;

	queue.push(from);
	visited[from] = true;
	while (!queue.empty()) {
		int current = queue.front();
		queue.pop();
		vector<int> nextVerteces = graph.GetNextVertices(current);
		for (auto vertex : nextVerteces) {
			if (!visited[vertex]) {
				visited[vertex] = true;

			}
		}
	}
}

int main() {

}
