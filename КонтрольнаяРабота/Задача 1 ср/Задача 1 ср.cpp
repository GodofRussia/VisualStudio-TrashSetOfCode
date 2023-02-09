#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void sort_fst_column(int** a, int m);

int main()
{
    setlocale(LC_CTYPE, "ru");
    string path = "Textfile.txt";
    fstream f(path, fstream::in | fstream :: out);
    int n, m;
    f >> n >> m;
    int** matr = new int* [n];
    for (int i = 0; i < n; i++) {
        matr[i] = new int[m];
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            f >> matr[i][j];
        }
    }
    sort_fst_column(matr, n);
    f.close();

    f.open(path, ios::app);
    f << "\n\n";

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            f << matr[i][j] << " ";
        }
        f << "\n";
    }

    f.close();
    for (int i = 0; i < n; i++) {
        delete [] matr[i];
    }
    delete[] matr;
    return 0;
}

void sort_fst_column(int** a, int m) {
    bool b = true;
    while (b) {
        b = false;
        for (int i = 0; i + 1 < m; i++) {
            if (a[i][0] < a[i + 1][0]) {
                swap(a[i][0], a[i + 1][0]);
                b = true;
            }
        }
        m--;
    }
}