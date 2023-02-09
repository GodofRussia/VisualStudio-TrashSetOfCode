#pragma once

#include <vector>
#include <iostream>
#include <cassert>
#include <queue>
#include <cmath>
#include <unordered_set>
#include <fstream>
#include <string>

using std::unordered_set;
using std::queue;
using std::vector;
using std::string;
using std::pair;

struct IGraph {
	virtual ~IGraph() {}

	// Добавление ребра от from к to.
	virtual void AddEdge(int from, int to) = 0;

	virtual int VerticesCount() const = 0;

	virtual vector<int> GetNextVertices(int vertex) const = 0;
	virtual vector<int> GetPrevVertices(int vertex) const = 0;
};
