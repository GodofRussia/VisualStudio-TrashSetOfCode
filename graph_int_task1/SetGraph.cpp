#include "SetGraph.h"

SetGraph::SetGraph(int vertexCount) {
	adjSetArray.resize(vertexCount);
	prevAdjSetArray.resize(vertexCount);
}

int SetGraph::VerticesCount() const {
	return adjSetArray.size();
}

SetGraph::SetGraph(const IGraph& graph) {
	adjSetArray.resize(graph.VerticesCount());
	prevAdjSetArray.resize(graph.VerticesCount());
	for (int i = 0; i < graph.VerticesCount(); ++i) {
		std::vector<int> adjList = graph.GetNextVertices(i);
		std::vector<int> prevAdjList = graph.GetPrevVertices(i);
		adjSetArray[i].insert(adjList.cbegin(), adjList.cend());
		prevAdjSetArray[i].insert(prevAdjList.cbegin(), prevAdjList.cend());
	}
}

void SetGraph::AddEdge(int from, int to) {
	assert(from >= 0 && from < this->VerticesCount());
	assert(to >= 0 && to < this->VerticesCount());
	adjSetArray[from].insert(to);
	prevAdjSetArray[to].insert(from);
}

vector<int> SetGraph::GetNextVertices(int vertex) const {
	assert(vertex >= 0 && vertex < this->VerticesCount());
	std::vector<int> adjList;
	for (int vert : adjSetArray[vertex]) {
		adjList.push_back(vert);
	}

	return adjList;
}

vector<int> SetGraph::GetPrevVertices(int vertex) const {
	assert(vertex >= 0 && vertex < this->VerticesCount());
	std::vector<int> prevAdjList;
	for (int vert : prevAdjSetArray[vertex]) {
		prevAdjList.push_back(vert);
	}

	return prevAdjList;
}
