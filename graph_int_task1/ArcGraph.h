#pragma once

#include "IGraph.h"

// Представление в виде массива пар {from, to}
class ArcGraph : public IGraph {
public:
	ArcGraph(int vertexCount);
	ArcGraph(const IGraph&);

	// Добавление ребра от from к to.
	virtual void AddEdge(int from, int to) override;

	virtual int VerticesCount() const override;

	virtual vector<int> GetNextVertices(int vertex) const override;
	virtual vector<int> GetPrevVertices(int vertex) const override;

private:
	vector<pair<int, int>> pairsList;
	int _vertexCount;
	int listSize;
};
