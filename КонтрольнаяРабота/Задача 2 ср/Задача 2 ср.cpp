#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Students {
    string surname;
    int sum_b;
};

int main()
{
    string path = "File.txt";
    setlocale(LC_CTYPE, "ru");
    fstream f(path);
    char test[100];
    int n = 0;
    while (f.getline(test, 100)) {
        n++;
    }
    f.close();

    f.open(path);
    Students* a = new Students[n];
    for (int i = 0; i < n; i++) {
        string surname, name;
        int ball;
        f >> surname >> name;
        a[i].surname = surname;
        f >> a[i].sum_b;
        for (int j = 0; j < 9; j++) {
            f >> ball;
            a[i].sum_b += ball;
        }
    }
    f.close();

    int r = n;
    bool b = true;
    while (b) {
        b = false;
        for (int i = 0; i + 1 < r; i++) {
            if (a[i].sum_b < a[i+1].sum_b || (a[i].sum_b == a[i + 1].sum_b && a[i].surname > a[i+1].surname)) {
                swap(a[i], a[i+1]);
                b = true;
            }
        }
        r--;
    }
    f.open(path, ios::app);
    f << "\n";
    for (int i = 0; i < n; i++) {
        f << a[i].surname << " " << a[i].sum_b<< "\n";
    }
    f.close();
    delete[] a;
    return 0;
}