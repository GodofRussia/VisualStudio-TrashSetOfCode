/*
Необходимо написать несколько реализаций интерфейса :
	ListGraph, хранящий граф в виде массива списков смежности,
	MatrixGraph, хранящий граф в виде матрицы смежности,
	SetGraph, хранящий граф в виде массива хэш - таблиц / сбалансированных деревьев поиска,
	ArcGraph, хранящий граф в виде одного массива пар { from, to }.

Также необходимо реализовать конструктор, принимающий const IGraph& .Такой конструктор должен скопировать переданный граф в создаваемый объект.
Для каждого класса создавайте отдельные h и cpp файлы.
Число вершин графа задается в конструкторе каждой реализации.
*/

#include "IGraph.h"
#include "ListGraph.h"
#include "MatrixGraph.h"
#include "SetGraph.h"
#include "ArcGraph.h"

void BFS(const IGraph* graph, int vertex, void(*visit)(int)) {
	vector<bool> visited(graph->VerticesCount(), false);
	queue<int> queue;
	queue.push(vertex);
	visited[vertex] = true;
	while (!queue.empty()) {
		int current = queue.front();
		queue.pop();
		visit(current);
		vector<int> adjList = graph->GetNextVertices(current);
		for (int i = 0; i < adjList.size(); ++i) {
			if (!visited[adjList[i]]) {
				queue.push(adjList[i]);
				visited[adjList[i]] = true;
			}
		}
	}

}

// Функция для чтения связей графа в виде {from, to} из файла fileName
int readGraph(IGraph* graph, string fileName) {
	std::ifstream filePtr(fileName);
	int relativesCount = 0;
	if (!filePtr.is_open()) {
		std::cerr << "Error opening file";
		return EXIT_FAILURE;
	}

	filePtr >> relativesCount;
	for (int i = 0; i < relativesCount; ++i) {
		int from = 0, to = 0;
		filePtr >> from >> to;
		graph->AddEdge(from, to);
	}

	if (filePtr.bad()) {
		std::cerr << "Error reading file";
		return EXIT_FAILURE;
	}

	filePtr.close();
	return EXIT_SUCCESS;
}

int main() {
	// Test 1
	IGraph* g1 = new MatrixGraph(6);  // В матричном представлении
	if (readGraph(g1, "test1.txt")) {
		std::cout << "Something went wrong";
		return EXIT_FAILURE;
	}

	std::cout << "Test1" << std::endl << std::endl;
	std::cout << "MatrixGraph from 1 vertex:" << std::endl;
	BFS(g1, 1, [](int vertex) { std::cout << vertex << ", "; });
	std::cout << std::endl;

	ListGraph listGraph1(*g1);  // Конвертация в ListGraph
	std::cout << "ListGraph from 1 vertex:" << std::endl;
	BFS(&listGraph1, 1, [](int vertex) { std::cout << vertex << ", "; });
	std::cout << std::endl;

	ArcGraph arcGraph1(listGraph1);  // Конвертация в ArcGraph
	std::cout << "ArcGraph from 1 vertex:" << std::endl;
	BFS(&arcGraph1, 1, [](int vertex) { std::cout << vertex << ", "; });
	std::cout << std::endl;

	SetGraph setGraph1(arcGraph1);  // Конвертация в SetGraph
	std::cout << "SetGraph from 1 vertex:" << std::endl;
	BFS(&setGraph1, 1, [](int vertex) { std::cout << vertex << ", "; });
	std::cout << std::endl << std::endl;

	std::cout << "ArcGraph from 2 vertex:" << std::endl;
	BFS(&arcGraph1, 2, [](int vertex) { std::cout << vertex << ", "; });  // Дополнительный обход из 2 вершины
	std::cout << std::endl << std::endl;

	// Test 2
	IGraph* g2 = new SetGraph(11);  // В представлении массива хэш-таблиц
	if (readGraph(g2, "test2.txt")) {
		std::cout << "Something went wrong";
		return EXIT_FAILURE;
	}

	std::cout << "Test2" << std::endl << std::endl;
	std::cout << "SetGraph from 2 vertex:" << std::endl;
	BFS(g2, 2, [](int vertex) { std::cout << vertex << ", "; });
	std::cout << std::endl;

	ListGraph listGraph2(*g2);  // Конвертация в ListGraph
	std::cout << "ListGraph from 2 vertex:" << std::endl;
	BFS(&listGraph2, 2, [](int vertex) { std::cout << vertex << ", "; });
	std::cout << std::endl;

	ArcGraph arcGraph2(listGraph2);  // Конвертация в ArcGraph
	std::cout << "ArcGraph from 2 vertex:" << std::endl;
	BFS(&arcGraph2, 2, [](int vertex) { std::cout << vertex << ", "; });
	std::cout << std::endl;

	MatrixGraph matrixGraph2(arcGraph2);  // Конвертация в MatrixGraph
	std::cout << "MatrixGraph from 2 vertex:" << std::endl;
	BFS(&matrixGraph2, 2, [](int vertex) { std::cout << vertex << ", "; });
	std::cout << std::endl << std::endl;

	std::cout << "SetGraph from 4 vertex:" << std::endl;
	BFS(g2, 4, [](int vertex) { std::cout << vertex << ", "; });  // Дополнительный обход из 4 вершины
	std::cout << std::endl << std::endl;

	// Test 3
	IGraph* g3 = new ArcGraph(10);  // В представлении массива пар {from, to}
	if (readGraph(g3, "test3.txt")) {
		std::cout << "Something went wrong";
		return EXIT_FAILURE;
	}

	std::cout << "Test3" << std::endl << std::endl;
	std::cout << "ArcGraph from 3 vertex:" << std::endl;
	BFS(g3, 3, [](int vertex) { std::cout << vertex << ", "; });
	std::cout << std::endl;

	ListGraph listGraph3(*g3);  // Конвертация в ListGraph
	std::cout << "ListGraph from 1 vertex:" << std::endl;
	BFS(&listGraph3, 3, [](int vertex) { std::cout << vertex << ", "; });
	std::cout << std::endl;

	SetGraph setGraph3(listGraph3);  // Конвертация в SetGraph
	std::cout << "SetGraph from 3 vertex:" << std::endl;
	BFS(&setGraph3, 3, [](int vertex) { std::cout << vertex << ", "; });
	std::cout << std::endl;

	MatrixGraph matrixGraph3(setGraph3);  // Конвертация в MatrixGraph
	std::cout << "MatrixGraph from 3 vertex:" << std::endl;
	BFS(&matrixGraph3, 3, [](int vertex) { std::cout << vertex << ", "; });
	std::cout << std::endl << std::endl;

	std::cout << "MatrixGraph from 5 vertex:" << std::endl;
	BFS(&matrixGraph3, 5, [](int vertex) { std::cout << vertex << ", "; });  // Дополнительный обход из 6 вершины
	std::cout << std::endl << std::endl;

	delete g1;
	delete g2;
	delete g3;
	return 0;
}
