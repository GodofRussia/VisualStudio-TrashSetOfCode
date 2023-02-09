/*
	Написать алгоритм для решения игры в “пятнашки”.
	Решением задачи является приведение к виду : [1 2 3 4] [5 6 7 8] [9 10 11 12] [13 14 15 0] , где 0 задает пустую ячейку.
	Достаточно найти хотя бы какое - то решение.
	Число перемещений костяшек не обязано быть минимальным.

	Формат входных данных
		Начальная расстановка.
	Формат выходных данных
		Если решение существует, то в первой строке выходного файла выведите минимальное число перемещений костяшек, которое нужно сделать, чтобы достичь выигрышной конфигурации,
		а во второй строке выведите соответствующую последовательность ходов:
		L означает, что костяшка сдвинулась влево, R – вправо, U – вверх, D – вниз. Если таких последовательностей несколько, то выведите любую из них.
		Если же выигрышная конфигурация недостижима, то выведите в выходной файл одно число −1.

*/

#include <iostream>
#include <array>
#include <cassert>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <utility>
#include <cstring>
#include <algorithm>

using std::array;
using std::vector;
using std::string;
using std::unordered_map;
using std::multiset;
using std::pair;

const int SideSize = 4;
const int FieldSize = SideSize * SideSize;
const array<char, FieldSize> FinishField({ 1,  2,  3,  4,
										   5,  6,  7,  8,
										   9, 10, 11, 12,
										   13, 14, 15, 0 });

class GameState {
public:
	GameState(const array<char, FieldSize>& field_);

	bool CanMoveLeft() const;
	bool CanMoveUp() const;
	bool CanMoveRight() const;
	bool CanMoveDown() const;

	GameState MoveLeft() const;
	GameState MoveUp() const;
	GameState MoveRight() const;
	GameState MoveDown() const;

	bool isFinish() const;

	bool canSolve() const;

	vector<pair<GameState, char>> getNextStates() const;

	int getHeuristic() const;
	int findNodeIndex(int value) const;

	bool operator == (const GameState& state) const { return this->field == state.field; }
	bool operator != (const GameState& state) const { return !(*this == state); }
	bool operator < (const GameState& state) const { return this->getHeuristic() < state.getHeuristic(); }

	friend struct StateHasher;
	friend std::ostream& operator << (std::ostream& out, const GameState& state) {
		for (int y = 0; y < SideSize; ++y) {
			for (int x = 0; x < SideSize; ++x) {
				out << static_cast<int>(state.field[y * SideSize + x]) << " ";
			}

			out << std::endl;
		}

		out << std::endl;
		return out;
	}

private:
	array<char, FieldSize> field;
	char zeroPos;
};

GameState::GameState(const array<char, FieldSize>& field_) : field(field_), zeroPos(-1) {
	for (size_t i = 0; i < field.size(); ++i) {
		if (static_cast<int>(field[i]) == 0) {
			zeroPos = i;
			break;
		}
	}

	assert(zeroPos != -1);
}

bool GameState::CanMoveLeft() const {
	return zeroPos % SideSize < SideSize - 1;
}

bool GameState::CanMoveUp() const {
	return (zeroPos + SideSize) < FieldSize;
}

bool GameState::CanMoveRight() const {
	return zeroPos % SideSize > 0;
}

bool GameState::CanMoveDown() const {
	return (zeroPos - SideSize) >= 0;
}

GameState GameState::MoveLeft() const {
	assert(CanMoveLeft());
	GameState newState(*this);
	std::swap(newState.field[newState.zeroPos + 1], newState.field[newState.zeroPos]);
	++newState.zeroPos;

	return newState;
}

GameState GameState::MoveUp() const {
	assert(CanMoveUp());
	GameState newState(*this);
	std::swap(newState.field[newState.zeroPos + SideSize], newState.field[newState.zeroPos]);
	newState.zeroPos += SideSize;

	return newState;
}

GameState GameState::MoveRight() const {
	assert(CanMoveRight());
	GameState newState(*this);
	std::swap(newState.field[newState.zeroPos - 1], newState.field[newState.zeroPos]);
	--newState.zeroPos;

	return newState;
}

GameState GameState::MoveDown() const {
	assert(CanMoveDown());
	GameState newState(*this);
	std::swap(newState.field[newState.zeroPos - SideSize], newState.field[newState.zeroPos]);
	newState.zeroPos -= SideSize;

	return newState;
}

bool GameState::isFinish() const {
	return field == FinishField;
}

bool GameState::canSolve() const {
	int transpSum = 0;
	for (int i = 0; i < FieldSize; ++i) {
		for (int j = i + 1; j < FieldSize; ++j) {
			if (field[j] == 0) continue;
			if (field[j] < field[i]) {
				++transpSum;
			}
		}
	}

	transpSum += (zeroPos / SideSize) + 1;

	return !(transpSum % 2);
}

int GameState::findNodeIndex(int value) const {
	if (value == 0)
		return FieldSize - 1;

	return int(value - 1);
}


int GameState::getHeuristic() const {
	int generalHeuristic = 0;

	for (int i = 0; i < FieldSize; ++i) {
		// Ситуация нахождения костяшки на чужом месте
		int finalPosition = findNodeIndex(field[i]);
		if (i != finalPosition) {
			generalHeuristic += (abs(i / SideSize - finalPosition / SideSize) + abs(i % SideSize - finalPosition % SideSize));
		}

		int linearConflicts = 0;

		if ((i / SideSize == finalPosition / SideSize)) {
			for (int j = i + 1; j < SideSize; ++j) {
				int secondFinalPos = findNodeIndex(field[j]);
				if (j / SideSize == secondFinalPos / SideSize) {
					if (field[i] > field[j])
						linearConflicts += 2;
				}
			}
		}

		if ((i % SideSize == finalPosition % SideSize)) {
			for (int j = i + SideSize; j < FieldSize; j += SideSize) {
				int secondFinalPos = findNodeIndex(field[j]);
				if (j % SideSize == secondFinalPos % SideSize) {
					if (field[i] > field[j])
						linearConflicts += 2;
				}
			}
		}


		generalHeuristic += linearConflicts;

		if (linearConflicts == 0) {
			if ((findNodeIndex(1) != 0) && ((i == 1 && field[i] == 2) || (i == SideSize && i == findNodeIndex(field[i])))) {
				generalHeuristic += 2;
			}
			else if ((findNodeIndex(SideSize) != SideSize - 1) && ((i == SideSize - 2 && i == findNodeIndex(field[i])) || (i == 2 * SideSize - 1 && i == findNodeIndex(field[i])))) {
				generalHeuristic += 2;
			}
			else if ((findNodeIndex(FieldSize - SideSize + 1) != (FieldSize - SideSize)) && ((i == FieldSize - 2 * SideSize && i == findNodeIndex(field[i])) || (i == SideSize * (SideSize - 1) + 1 && i == findNodeIndex(field[i])))) {
				generalHeuristic += 2;
			}
		}
	}

	return generalHeuristic;
}


vector<pair<GameState, char>> GameState::getNextStates() const {
	vector<pair<GameState, char>> nextStates;
	if (CanMoveLeft()) {
		nextStates.push_back(std::make_pair(MoveLeft(), 'L'));
	}

	if (CanMoveUp()) {
		nextStates.push_back(std::make_pair(MoveUp(), 'U'));
	}

	if (CanMoveRight()) {
		nextStates.push_back(std::make_pair(MoveRight(), 'R'));
	}

	if (CanMoveDown()) {
		nextStates.push_back(std::make_pair(MoveDown(), 'D'));
	}

	return nextStates;
}

struct StateHasher {
	size_t operator()(const GameState& state) const {
		size_t hash = 0;
		for (size_t i = 0; i < FieldSize; ++i) {
			hash = hash * 17 + state.field[i];
		}
		return hash;
	}
};


string get16thSolution(const GameState& state) {
	if (!state.canSolve()) {
		return "bad";
	}

	multiset<GameState> queue;
	queue.insert(state);
	unordered_map<GameState, char, StateHasher> visited;
	visited[state] = 'S';
	bool hasSolution = false;
	while (!queue.empty()) {
		GameState current = *queue.begin();
		queue.erase(queue.begin());
		if (current.isFinish()) {
			hasSolution = true;
			break;
		}

		vector<pair<GameState, char>> nextStates = current.getNextStates();
		for (auto nextState : nextStates) {
			if (visited.find(nextState.first) == visited.end()) {
				visited[nextState.first] = nextState.second;
				queue.insert(nextState.first);
			}
		}
	}

	assert(hasSolution);

	string result;
	GameState current(FinishField);
	char move = visited[current];
	while (move != 'S') {
		result += move;
		switch (move) {
		case 'L':
			current = current.MoveRight();
			break;
		case 'U':
			current = current.MoveDown();
			break;
		case 'R':
			current = current.MoveLeft();
			break;
		case 'D':
			current = current.MoveUp();
			break;
		default:
			break;
		}

		move = visited[current];
	}

	std::reverse(result.begin(), result.end());
	return result;
}

int main() {
	std::array<char, FieldSize> initStateArr;
	for (int i = 0; i < FieldSize; ++i) {
		int symbol;
		std::cin >> symbol;
		initStateArr[i] = symbol;
	}

	//{ 3, 5, 4, 10, 1, 11, 7, 9, 13, 8, 0, 6, 15, 14, 2, 12 }
	GameState state(initStateArr);
	string result = get16thSolution(state);
	if (result == "bad") {
		std::cout << -1;
		return EXIT_FAILURE;
	}

	std::cout << result.size() << std::endl;
	std::cout << result << std::endl;

	return EXIT_SUCCESS;
}
