#include "MatrixGraph.h"

MatrixGraph::MatrixGraph(int vertexCount) {
	adjMatrix.resize(vertexCount * vertexCount, 0);
	prevAdjMatrix.resize(vertexCount * vertexCount, 0);
}

int MatrixGraph::VerticesCount() const {
	return static_cast<int>(sqrt(adjMatrix.size()));
}

MatrixGraph::MatrixGraph(const IGraph& graph) {
	adjMatrix.resize(graph.VerticesCount() * graph.VerticesCount());
	prevAdjMatrix.resize(graph.VerticesCount() * graph.VerticesCount());
	for (int i = 0; i < graph.VerticesCount(); ++i) {
		std::vector<int> adjList = graph.GetNextVertices(i);
		std::vector<int> prevAdjList = graph.GetPrevVertices(i);
		for (int j = 0; j < adjList.size(); ++j) {
			adjMatrix[i * graph.VerticesCount() + adjList[j]] = 1;
		}
		for (int j = 0; j < prevAdjList.size(); ++j) {
			prevAdjMatrix[i * graph.VerticesCount() + prevAdjList[j]] = 1;
		}
	}
}

void MatrixGraph::AddEdge(int from, int to) {
	assert(from >= 0 && from < this->VerticesCount());
	assert(to >= 0 && to < this->VerticesCount());
	adjMatrix[from * this->VerticesCount() + to] = 1;
	prevAdjMatrix[to * this->VerticesCount() + from] = 1;
}

vector<int> MatrixGraph::GetNextVertices(int vertex) const {
	assert(vertex >= 0 && vertex < this->VerticesCount());
	std::vector<int> adjList;
	for (int i = 0; i < this->VerticesCount(); ++i) {
		if (adjMatrix[vertex * this->VerticesCount() + i] == 1) {
			adjList.push_back(i);
		}
	}

	return adjList;
}

vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
	assert(vertex >= 0 && vertex < this->VerticesCount());
	std::vector<int> prevAdjList;
	for (int i = 0; i < this->VerticesCount(); ++i) {
		if (prevAdjMatrix[vertex * this->VerticesCount() + i] == 1) {
			prevAdjList.push_back(i);
		}
	}

	return prevAdjList;
}
