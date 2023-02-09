#pragma once

#include "IGraph.h"

// ������������� ����� � ���� ������ ���������
class ListGraph : public IGraph {
public:
	ListGraph(int vertexCount);
	ListGraph(const IGraph&);

	// ���������� ����� �� from � to.
	virtual void AddEdge(int from, int to) override;

	virtual int VerticesCount() const override;

	virtual vector<int> GetNextVertices(int vertex) const override;
	virtual vector<int> GetPrevVertices(int vertex) const override;

private:
	vector<vector<int>> adjLists;
	vector<vector<int>> prevAdjLists;
};
