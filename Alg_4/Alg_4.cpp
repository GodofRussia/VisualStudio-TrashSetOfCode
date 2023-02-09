#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>

class Algorithm
{
public:
	char* name = _strdup("Algorithm"); // Название алгоритма
	std::vector<std::vector<int>> data; // Массив значений (матрица)
	Algorithm();
	Algorithm(std::vector<std::vector<int>>);
	Algorithm(char*);
	bool LoadData(std::vector<std::vector<int>>);
	bool LoadData(char*);
	virtual void Run(); // Метод для запуска алгоритма
protected:
	int GetStrCount(std::ifstream&); // Считываем количество строк в файле
	int GetColCount(std::ifstream&); // Считываем количество столбцов в файле
	virtual bool validateData(); // Метод для проверки значений в матрице. Вызывается перед Run()
};

class LittleAlgorithm : public Algorithm
{
public:
	std::vector<std::pair<int, int>> result; // Результат. Здесь будет храниться искомый цикл
	LittleAlgorithm();
	LittleAlgorithm(std::vector<std::vector<int>>);
	LittleAlgorithm(char*);
	LittleAlgorithm(std::ifstream& is);
	virtual void Run();
private:
	enum check { Row, Col };
	int getMin(std::vector<std::vector<int>>, int, check); // Поиск минимального элемента столбца/строки
	void matrixProcedure(std::vector<std::vector<int>>); // Метод в котором идет поиск цикла
	void showMatrix(std::vector<std::vector<int>>); // Вывод матрицы
	int getResultSum(); // Считывание суммы всех выбранных дуг
	virtual bool validateData();
};

LittleAlgorithm::LittleAlgorithm(std::vector<std::vector<int>> data) {
	for (size_t i = 0; i < data.size(); ++i)
		for (size_t j = 0; j < data[i].size(); ++j) {
			this->data[i][j] = data[i][j];
		}
}
LittleAlgorithm::LittleAlgorithm(std::ifstream& is) {
	int rows = 0;
	is >> rows;
	data.resize(rows);
	for (auto& string : data) {
		for (auto& element : string) {
			is >> element;
		}
	}
}

void LittleAlgorithm::Run()
{
	name = _strdup("Little algorithm");
	Algorithm::Run();

	matrixProcedure(std::vector<std::vector<int>>(data));
}
int LittleAlgorithm::getMin(std::vector<std::vector<int>> matrix, int sel, check pos)
{
	int min = INT32_MAX;
	for (int i = 0; i < matrix[sel].size() - 1; i++)
		switch (pos)
		{
		case LittleAlgorithm::Row:
			if (min > matrix[sel][i])
				min = matrix[sel][i];
			break;
		case LittleAlgorithm::Col:
			if (min > matrix[i][sel])
				min = matrix[i][sel];
			break;
		}
	return min;
}
void LittleAlgorithm::showMatrix(std::vector<std::vector<int>> temp)
{
	std::cout << std::endl;
	std::cout << "\t";
	for (int i = 0; i < temp[temp.size() - 1].size() - 1; i++) {
		std::cout << temp[temp.size() - 1][i] << "\t";
	}
	std::cout << std::endl;
	std::cout << "\t";
	for (int i = 0; i < temp[0].size(); i++)
		for (int j = 0; j < 6; j++) std::cout << "_";
	std::cout << std::endl << std::endl;
	for (int i = 0; i < temp.size() - 1; i++) {
		std::cout << temp[i][temp.size() - 1] << " | " << "\t";
		for (int j = 0; j < temp[i].size() - 1; j++)
			if (temp[i][j] != INT32_MAX && j != temp.size() - 1)std::cout << temp[i][j] << "\t";
			else std::cout << "inf" << "\t";
		std::cout << std::endl;
	}
	std::cout << std::endl << std::endl;
}
int LittleAlgorithm::getResultSum()
{
	int sum = 0;
	for (int i = 0; i < result.size(); i++)
		sum += data[result[i].first - 1][result[i].second - 1];
	return sum;
}
bool LittleAlgorithm::validateData()
{
	//Добавляем в нашу матрицу столбец и строку с нумерацией для отслеживания удаляемых ребер
	for (int i = 0; i < data.size(); i++)
		for (int j = 0; j < data[i].size(); j++)
			if (data[i][j] == 0)
				data[i][j] = INT32_MAX;

	std::vector<std::vector<int>> temp(data);
	for (int i = 0; i < data.size(); i++)
		data[i].push_back(i + 1);
	std::vector<int> numeration;
	for (int i = 0; i < data[0].size(); i++)
		numeration.push_back(i + 1);
	data.push_back(numeration);

	return true;
}

void LittleAlgorithm::matrixProcedure(std::vector<std::vector<int>> matrix)
{
	//Определяем точку возврата и удаляем необходимое ребро
	if (matrix.size() - 1 > 2) {
		std::vector<int> vertexes;
		for (int i = 0; i < result.size(); i++) {
			vertexes.push_back(result[i].first);
			vertexes.push_back(result[i].second);
		}
		for (int i = 0; i < vertexes.size(); i++) {
			std::pair<int, int> elem(INT32_MAX, INT32_MAX), elem1(INT32_MAX, INT32_MAX);
			for (int j = 0; j < vertexes.size(); j++) {
				if (vertexes[i] != vertexes[j]) {
					for (int k = 0; k < matrix[matrix.size() - 1].size() - 1; k++) {
						if (vertexes[i] == matrix[k][matrix[k].size() - 1]) elem.first = k;
						if (vertexes[j] == matrix[k][matrix[k].size() - 1]) elem1.first = k;
					}
					for (int k = 0; k < matrix.size() - 1; k++) {
						if (vertexes[i] == matrix[matrix.size() - 1][k]) elem.second = k;
						if (vertexes[j] == matrix[matrix.size() - 1][k]) elem1.second = k;
					}
				}
			}
			for (int i = 0; i < matrix.size() - 1; i++)
				for (int j = 0; j < matrix.size() - 1; j++)
					if (i == elem1.first && j == elem1.second)
						matrix[elem1.first][elem1.second] = INT32_MAX;
			for (int i = 0; i < matrix.size() - 1; i++)
				for (int j = 0; j < matrix.size() - 1; j++)
					if (i == elem.first && j == elem.second)
						matrix[elem.first][elem.second] = INT32_MAX;
		}
	}

	//Вычитаем из каждой строки минимальное значение
	for (int i = 0; i < matrix.size() - 1; i++) {
		int min = 0;
		if ((min = getMin(matrix, i, check::Row)) == INT32_MAX) {
			showMatrix(matrix);
			std::cout << std::endl << "Bad road" << std::endl;
			return;
		}
		if ((min = getMin(matrix, i, check::Row)) != 0)
			for (int j = 0; j < matrix[i].size() - 1; j++)
				if (matrix[i][j] != INT32_MAX) matrix[i][j] -= min;
	}

	//Вычитаем из каждого столбца минимальное значение
	for (int i = 0; i < matrix[matrix.size() - 1].size() - 1; i++) {
		int min = 0;
		if ((min = getMin(matrix, i, check::Col)) == INT32_MAX) {
			showMatrix(matrix);
			std::cout << std::endl << "Bad road" << std::endl;
			return;
		}
		if ((min = getMin(matrix, i, check::Col)) != 0)
			for (int j = 0; j < matrix.size() - 1; j++)
				if (matrix[j][i] != INT32_MAX) matrix[j][i] -= min;
	}

	//Находим максимально оцененный ноль
	int Max = 0;
	for (int i = 0; i < matrix.size() - 1; i++)
		for (int j = 0; j < matrix[i].size() - 1; j++)
			if (matrix[i][j] == 0) {
				matrix[i][j] = INT32_MAX;
				int max = (getMin(matrix, i, check::Row) == INT32_MAX || getMin(matrix, j, check::Col) == INT32_MAX) ? INT32_MAX : getMin(matrix, i, check::Row) + getMin(matrix, j, check::Col);
				if (max > Max) Max = max;
				matrix[i][j] = 0;
			}

	//Находим все нули максимальная оценка которых равна Max
	std::vector<std::pair<int, int>> Maxs;
	for (int i = 0; i < matrix.size() - 1; i++)
		for (int j = 0; j < matrix[i].size() - 1; j++)
			if (matrix[i][j] == 0) {
				matrix[i][j] = INT32_MAX;
				int max = (getMin(matrix, i, check::Row) == INT32_MAX || getMin(matrix, j, check::Col) == INT32_MAX) ? INT32_MAX : getMin(matrix, i, check::Row) + getMin(matrix, j, check::Col);
				if (max == Max) Maxs.push_back(std::pair<int, int>(matrix[i][matrix.size() - 1], matrix[matrix.size() - 1][j]));
				matrix[i][j] = 0;
			}
	//Вывод координат выбранных нулей
	std::cout << "Maxs - ";
	for (int i = 0; i < Maxs.size(); i++)
		std::cout << Maxs[i].first << " " << Maxs[i].second << "\t";
	std::cout << std::endl;
	//Вывод матрицы
	showMatrix(matrix);
	std::cout << std::endl;
	//Завершаем выполнение данной ветви если нету нулей
	if (Maxs.size() == 0) {
		std::cout << "Bad road." << std::endl;
		return;
	}

	for (int i = 0; i < Maxs.size(); i++) {
		//Добавляем вершину в массив с результатом
		result.push_back(Maxs[i]);
		//Если размер матрицы порядка 1, выводим результат и завершаем текущию ветвь
		if (matrix.size() - 1 == 1) {
			for (int i = 0; i < result.size(); i++)
				std::cout << "(" << result[i].first << ", " << result[i].second << ")\t";
			std::cout << std::endl;
			std::cout << "Result: " << getResultSum() << std::endl;
			result.pop_back();
			return;
		}
		//Создаем копию текущей матрицы и удаляем из нее строку и столбец выбранного нуля
		std::vector<std::vector<int>>  temp(matrix);
		std::pair<int, int> elem(INT32_MAX, INT32_MAX), elem1(INT32_MAX, INT32_MAX);
		for (int j = 0; j < temp[temp.size() - 1].size() - 1; j++) {
			if (Maxs[i].first == temp[j][temp[j].size() - 1]) elem.first = j;
			if (Maxs[i].second == temp[j][temp[j].size() - 1]) elem1.first = j;
		}
		for (int j = 0; j < temp.size() - 1; j++) {
			if (Maxs[i].second == temp[temp.size() - 1][j]) elem.second = j;
			if (Maxs[i].first == temp[temp.size() - 1][j]) elem1.second = j;
		}

		for (int i = 0; i < temp.size() - 1; i++)
			for (int j = 0; j < temp.size() - 1; j++)
				if (i == elem1.first && j == elem1.second)
					temp[elem1.first][elem1.second] = INT32_MAX;

		for (int j = 0; j < temp[temp.size() - 1].size(); j++)
			temp[j].erase(temp[j].begin() + elem.second);
		temp.erase(temp.begin() + elem.first);
		//Вызываем рекурсивно эту же функцию для уже новой матрицы
		matrixProcedure(temp);
		//Удаляем последние значение из массива с результатом 
		result.pop_back();
	}
}

int main()
{
	std::ifstream inputFile("input.txt");
	LittleAlgorithm voyazher(inputFile);
	inputFile.close();
	voyazher.Run();
    return 0;
}
