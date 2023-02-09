#pragma once

#include "IGraph.h"

// ����������� �� �������� ���������������� ������������� ����� � ���� ������� ���������
class MatrixGraph : public IGraph {
public:
	MatrixGraph(int vertexCount);
	MatrixGraph(const IGraph&);

	// ���������� ����� �� from � to.
	virtual void AddEdge(int from, int to) override;

	virtual int VerticesCount() const override;

	virtual vector<int> GetNextVertices(int vertex) const override;
	virtual vector<int> GetPrevVertices(int vertex) const override;

private:
	vector<int> adjMatrix;
	vector<int> prevAdjMatrix;
};
