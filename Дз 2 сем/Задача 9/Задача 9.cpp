#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void show_matr(double** A, int n); //прототип функции для отображения матриц

double** build_matr(double** A, int n); // прототип функции построения матрицы B

int main()
{
	setlocale(LC_CTYPE, "ru");
	int n;
	string path = "Matrix.txt";
	ifstream f(path);
	f >> n; //читаем размерность квадратной матрицы
	double** A = new double*[n]; //выделяем под неё память
	if (A == nullptr) //проверка, что память выделилась, в ней нет необходимости 
	{
		cout << "Ошибка обращения к динамической памяти";
		return 1;
	}
	for (int i = 0; i < n; i++) 
	{
		A[i] = new double[n];
	}
	//заполняем матрицу A символами из файла
	for (int i = 0; i < n; i++) 
	{
		for (int j = 0; j < n; j++)
		{
			f >> A[i][j];
		}
	}
	cout << "Исходная матрица:\n";
	show_matr(A, n);
	double** B = build_matr(A, n); //матрице B присваиваем искомую матрицу
	cout << "\nПолученная матрица: \n";
	show_matr(B, n);
	f.close();
	//далее удаления памяти, выделенной под матрицы
	for (int i = 0; i < n; i++) {
		delete A[i];
	}
	delete[] A;
	for (int i = 0; i < n; i++) {
		delete B[i];
	}
	delete[] B;
	return 0;
}

void show_matr(double** A, int n)  //обычный вывод матрице в виде таблички
{
	//cout.width(8);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			cout << A[i][j] << " ";
		cout << endl;
	}
}

double** build_matr(double** A, int n) 
{
	double** B = new double* [n];
	for (int i = 0; i < n; i++) {
		B[i] = new double[n];
	}
    /*
	Создали вторую матрицу и далее идём по всем элементам a(i,j) первой, для каждого находя значение b(i,j) 
	*/
	for(int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) 
		{
			B[i][j] = A[i][j]; //записали значение a[i,j] в b[i,j]
			double sum = 0; //сумма элементов, лежащих в заданной области относительно a[i,j]
			int m = 0; //количество этих элементов
			for (int l = 0; l <= j; l++) //тут мы идём по элеметам a[k,l], k и l принадлежат нашей области (как по рисунку), 
			{                                                                                        //ищем параметры, нужные для определения b[i,j] 
				for (int k = 0; k <= -l + j + i; k++) {
					if (k >= 0 && k <= n - 1) {
						sum += A[k][l];
						m++;
					}
				}
			}
			if (m!= 0)
			B[i][j] = sum / m; //получаем среднее значение в области
		}
	return B; //возвращаем полученную матрицу
}