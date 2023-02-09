#include "ListGraph.h"

ListGraph::ListGraph(int vertexCount) {
	adjLists.resize(vertexCount);
	prevAdjLists.resize(vertexCount);
}

int ListGraph::VerticesCount() const {
	return adjLists.size();
}

ListGraph::ListGraph(const IGraph& graph) {
	adjLists.resize(graph.VerticesCount());
	prevAdjLists.resize(graph.VerticesCount());
	for (int i = 0; i < graph.VerticesCount(); ++i) {
		adjLists[i] = graph.GetNextVertices(i);
		prevAdjLists[i] = graph.GetPrevVertices(i);
	}
}

void ListGraph::AddEdge(int from, int to) {
	assert(from >= 0 && from < adjLists.size());
	assert(to >= 0 && to < adjLists.size());
	adjLists[from].push_back(to);
	prevAdjLists[to].push_back(from);
}

vector<int> ListGraph::GetNextVertices(int vertex) const {
	assert(vertex >= 0 && vertex < adjLists.size());
	return adjLists[vertex];
}

vector<int> ListGraph::GetPrevVertices(int vertex) const {
	assert(vertex >= 0 && vertex < adjLists.size());
	return prevAdjLists[vertex];
}
