#include "ArcGraph.h"

ArcGraph::ArcGraph(int vertexCount) : _vertexCount(vertexCount), listSize(0) { }

int ArcGraph::VerticesCount() const {
	return _vertexCount;
}

ArcGraph::ArcGraph(const IGraph& graph) : listSize(0) {
	_vertexCount = graph.VerticesCount();
	for (int i = 0; i < graph.VerticesCount(); ++i) {
		std::vector<int> adjList = graph.GetNextVertices(i);
		for (int vertex : adjList) {
			pairsList.push_back(std::make_pair(i, vertex));
			++listSize;
		}
	}
}

void ArcGraph::AddEdge(int from, int to) {
	assert(from >= 0 && from < this->VerticesCount());
	assert(to >= 0 && to < this->VerticesCount());
	pairsList.push_back(std::make_pair(from, to));
	++listSize;
}

vector<int> ArcGraph::GetNextVertices(int vertex) const {
	assert(vertex >= 0 && vertex < this->VerticesCount());
	std::vector<int> adjList;
	for (int i = 0; i < listSize; ++i) {
		if (pairsList[i].first == vertex) {
			adjList.push_back(pairsList[i].second);
		}
	}

	return adjList;
}

vector<int> ArcGraph::GetPrevVertices(int vertex) const {
	assert(vertex >= 0 && vertex < this->VerticesCount());
	std::vector<int> prevAdjList;
	for (int i = 0; i < listSize; ++i) {
		if (pairsList[i].second == vertex) {
			prevAdjList.push_back(pairsList[i].first);
		}
	}

	return prevAdjList;
}
