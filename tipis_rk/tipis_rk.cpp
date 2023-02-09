// tipis.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include<iomanip>
using namespace std;
int main()
{
    setlocale(LC_ALL, "Rus");
    int m, l;
    cout << "m" << endl;
    cin >> m;
    cout << "l" << endl;
    cin >> l;
    float* Yj = new float[m];
    float* Xi = new float[l];
    cout << "интервалы Yj " << endl;
    for (int i = 0; i < m; i++) {
        cin >> Yj[i];
    }
    cout << "интервалы Xi " << endl;
    for (int i = 0; i < l; i++) {
        cin >> Xi[i];
    }

    int** mat = new int* [l];
    for (int i = 0; i < l; i++) {
        mat[i] = new int[m];
    }

    cout << "матрицу по строкам " << endl;
    for (int i = 0; i < l; i++) {
        for (int j = 0; j < m; j++) {
            cin >> mat[i][j];
        }
    }

    int* ni = new int[l];
    int* nj = new int[m];
    for (int i = 0; i < l; i++) {
        ni[i] = 0;
        for (int j = 0; j < m; j++) {
            ni[i] += mat[i][j];
        }
    }

    for (int j = 0; j < m; j++) {
        nj[j] = 0;
        for (int i = 0; i < l; i++) {
            nj[j] += mat[i][j];
        }
    }


    int n = 0;
    for (int i = 0; i < l; i++) {
        n += ni[i];
    }

    float* Ygrsr = new float[l];
    for (int i = 0; i < l; i++) {
        Ygrsr[i] = 0;
        for (int j = 0; j < m; j++) {
            Ygrsr[i] += mat[i][j] * Yj[j];
        }
        Ygrsr[i] /= ni[i];
    }

    float* Xgrsr = new float[m];
    for (int j = 0; j < m; j++) {
        Xgrsr[j] = 0;
        for (int i = 0; i < l; i++) {
            Xgrsr[j] += Xi[i] * mat[i][j];
        }
        Xgrsr[j] /= nj[j];
    }

    //вывод матрицы
    std::cout << std::setw(8) << "xi,yj";
    for (int j = 0; j < m; j++) {
        std::cout << std::setw(8) << Yj[j];
    }
    std::cout << std::setw(8) << "всni";
    std::cout << std::setw(8) << "грсрYi" << endl;
    for (int i = 0; i < l; i++) {
        std::cout << std::setw(8) << Xi[i];
        for (int j = 0; j < m; j++) {
            std::cout << std::setw(8) << mat[i][j];
        }
        std::cout << std::setw(8) << ni[i];
        std::cout << std::setw(8) << Ygrsr[i];
        cout << endl;
    }
    std::cout << std::setw(8) << "вс nj";
    for (int j = 0; j < m; j++) {
        std::cout << std::setw(8) << nj[j];
    }
    std::cout << std::setw(8) << n << endl;
    std::cout << std::setw(8) << "грсрXj";
    for (int j = 0; j < m; j++) {
        std::cout << std::setw(8) << Xgrsr[j];
    }
    cout << endl;
    //конец вывода матрицы
    float XYsr = 0;
    for (int i = 0; i < l; i++) {
        for (int j = 0; j < m; j++) {
            XYsr += Xi[i] * Yj[j] * mat[i][j];
        }
    }
    XYsr /= n;
    cout << "XYsr=" << XYsr << endl;

    float Xsr = 0;
    for (int i = 0; i < l; i++) {
        Xsr += Xi[i] * ni[i];
    }
    Xsr /= n;
    cout << "Xsr=" << Xsr << endl;

    float Ysr = 0;
    for (int j = 0; j < m; j++) {
        Ysr += Yj[j] * nj[j];
    }
    Ysr /= n;
    cout << "Ysr=" << Ysr << endl;

    float X2sr = 0;
    for (int i = 0; i < l; i++) {
        X2sr += Xi[i] * Xi[i] * ni[i];
    }
    X2sr /= n;
    cout << "X2sr=" << X2sr << endl;

    float Y2sr = 0;
    for (int j = 0; j < m; j++) {
        Y2sr += Yj[j] * Yj[j] * nj[j];
    }
    Y2sr /= n;
    cout << "Y2sr=" << Y2sr << endl;

    float S2y;
    S2y = Y2sr - Ysr * Ysr;
    cout << "S2y=" << S2y << endl;

    float S2x;
    S2x = X2sr - Xsr * Xsr;
    cout << "S2x=" << S2x << endl;

    float M;
    M = XYsr - Xsr * Ysr;
    cout << "M=" << M << endl;

    float bxy;
    bxy = M / S2y;
    cout << "Коэффициент регрессии X по Y bxy=" << bxy << endl;//Коэффициент регрессии X по Y

    float byx;
    byx = M / S2x;
    cout << "Коэффициент регрессии Y по X byx=" << byx << endl;//Коэффициент регрессии Y по X

    cout << "Уравнение регрессии  Y по X: Yx=" << byx << "x" << "+" << -1 * byx * Xsr + Ysr << endl;
    cout << "Уравнение регрессии  X по Y: Xy=" << bxy << "y" << "+" << -1 * bxy * Ysr + Xsr << endl;

    float r;
    r = sqrt(byx * bxy);
    cout << "r=" << r << endl;

    float z;
    z = 0.5 * (log((1 + r) / (1 - r)));
    cout << "z=" << z << endl << endl;
    cout << "Введите значение t(1-a) при заданной надежности по таблице значений Лапласа" << endl;
    float ta;
    cin >> ta;
    float Mzl, Mzr;
    Mzl = z - ta / (sqrt(n - 3));
    Mzr = z + ta / (sqrt(n - 3));
    cout << "доверительный интервал для M(z): " << Mzl << "<=M(z)<=" << Mzr << endl << endl;
    float Pl, Pr;
    Pl = tanh(Mzl);
    Pr = tanh(Mzr);
    cout << "доверительный интервал для P: " << Pl << "<=P<=" << Pr << endl;

    cout << "Введите значение t(1-a;n-2) с надежностью 1-a и степенями свободы равным n-2=" << n - 2 << endl;
    float tan;
    cin >> tan;
    float Byxl, Byxr;
    Byxl = byx - tan * sqrt(S2y) * sqrt(1 - r * r) / (sqrt(S2x) * sqrt(n - 2));
    Byxr = byx + tan * sqrt(S2y) * sqrt(1 - r * r) / (sqrt(S2x) * sqrt(n - 2));
    cout << "доверительный интервал для Byx: " << Byxl << "<=Byx<=" << Byxr << endl << endl;
    float Bxyl, Bxyr;
    Bxyl = bxy - tan * sqrt(S2x) * sqrt(1 - r * r) / (sqrt(S2y) * sqrt(n - 2));
    Bxyr = bxy + tan * sqrt(S2x) * sqrt(1 - r * r) / (sqrt(S2y) * sqrt(n - 2));
    cout << "доверительный интервал для Bxy: " << Bxyl << "<=Bxy<=" << Bxyr << endl << endl;

    float DELiy2 = 0;
    for (int i = 0; i < l; i++) {
        DELiy2 += (Ygrsr[i] - Ysr) * (Ygrsr[i] - Ysr) * ni[i] * 1.0;
    }
    DELiy2 /= n;
    float Nyx;
    Nyx = sqrt(DELiy2 / S2y);
    cout << "эмпирическое корреляционное отношение Y по X =Nyx=" << Nyx << endl << endl;

    float* Yxi = new float[l];
    for (int i = 0; i < l; i++) {
        Yxi[i] = Xi[i] * byx - 1 * byx * Xsr + Ysr;
    }

    float DELy2 = 0;
    for (int i = 0; i < l; i++) {
        DELy2 += (Yxi[i] - Ysr) * (Yxi[i] - Ysr) * ni[i] * 1.0;
    }
    DELy2 /= n;
    float Ryx;
    Ryx = sqrt(DELy2 / S2y);
    cout << "индекс корреляции Y по X =Ryx=" << Ryx << endl << endl;

    float FNyx;
    FNyx = Nyx * Nyx * (n - m) / ((1 - Nyx * Nyx) * (m - 1));
    cout << "Fnyx=" << FNyx << endl << endl;

    float FRyx;
    FRyx = Ryx * Ryx * (n - 2) / (1 - Ryx * Ryx);
    cout << "FRyx=" << FRyx << endl << endl;
    //sadasdasdasdasdasda






    float DELix2 = 0;
    for (int j = 0; j < m; j++) {
        DELix2 += (Xgrsr[j] - Xsr) * (Xgrsr[j] - Xsr) * nj[j] * 1.0;
    }
    DELix2 /= n;
    float Nxy;
    Nxy = sqrt(DELix2 / S2x);
    cout << "эмпирическое корреляционное отношение X по Y =Nxy=" << Nxy << endl << endl;

    float* Xyi = new float[l];
    for (int j = 0; j < m; j++) {
        Xyi[j] = Yj[j] * bxy - 1 * bxy * Ysr + Xsr;
    }

    float DELx2 = 0;
    for (int j = 0; j < m; j++) {
        DELx2 += (Xyi[j] - Xsr) * (Xyi[j] - Xsr) * nj[j] * 1.0;
    }
    DELx2 /= n;
    float Rxy;
    Rxy = sqrt(DELx2 / S2x);
    cout << "индекс корреляции X по Y =Rxy=" << Rxy << endl << endl;

    float FNxy;
    FNxy = Nxy * Nxy * (n - l) / ((1 - Nxy * Nxy) * (l - 1));
    cout << "Fnxy=" << FNxy << endl << endl;

    float FRxy;
    FRxy = Rxy * Rxy * (n - 2) / (1 - Rxy * Rxy);
    cout << "FRxy=" << FRxy << endl << endl;

    delete[] Yxi;
    delete[] Xi;
    delete[] Yj;
    delete[] Xgrsr;
    delete[] Ygrsr;
    for (int i = 0; i < l; i++)
    {
        delete[] mat[i];
    }
    delete[] mat;
}
