#pragma once

#include "IGraph.h"

// ������������� � ���� ������� ���-������
class SetGraph: public IGraph {
public:
	SetGraph(int vertexCount);
	SetGraph(const IGraph&);

	// ���������� ����� �� from � to.
	virtual void AddEdge(int from, int to) override;

	virtual int VerticesCount() const override;

	virtual vector<int> GetNextVertices(int vertex) const override;
	virtual vector<int> GetPrevVertices(int vertex) const override;

private:
	vector<unordered_set<int>> adjSetArray;
	vector<unordered_set<int>> prevAdjSetArray;
};
