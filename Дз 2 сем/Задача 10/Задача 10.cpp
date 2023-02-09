#define SIZE 100

#include <iostream>
#include <fstream>

using namespace std;

typedef struct { //структура со студентом, экзаменом и оценкой за него
    char* surname;
    char* name;
    char* patr;
    char* subject;
    int mark;
}students;

typedef struct { //структура с предметами и средним баллом за них
    char** subjects;
    double* ball;
}session;

void show_st(students* A, int n); //функция для вывода всех студентов 

void sort_st(students* A, int n); //функция для сортировки студентов по возрастанию балла за экзамен

session& generate_list(students* A, int n, int& kol); //функция, заполняющая структуру session

void show_ses(session& A, int kol); //функция для вывода структуры session

int main()
{
    setlocale(LC_ALL, "ru");
    ifstream fin("input.txt"); //список студентов(вызов для чтения)
    ofstream fou("outout.txt"); //выходной файл(для записи)
    int n = 0;
    int kol;
    if (!fin.is_open())
    {
        cout << "Ошибка открытия файла ";
        return 1;
    }
    while (!fin.eof()) //считываем строки для определения количества студентов
    {
        char test[SIZE];
        fin.getline(test, SIZE,'\n');
        n++;
    }
    fin.seekg(0); //ставим позицию в потоке файла на первый элемент
    /*
    Заполнение массива структур инфой из входного файла:
    */

    students* array = new students[n]; 
    for (int i = 0; i < n; i++) {
        char str[4][SIZE];
        fin >> str[0] >>  str[1] >> str[2] >> str[3] >> array[i].mark;
        array[i].surname = _strdup(str[0]);
        array[i].name = _strdup(str[1]);
        array[i].patr = _strdup(str[2]);
        array[i].subject = _strdup(str[3]);
    }

    cout << "Первоначальный список: \n";
    show_st(array, n);

    sort_st(array, n);
    cout << "Отсортированный список: \n";
    show_st(array, n);
    /*
    Вывод списка в выходной файл:
    */
    fou << "СПИСОК СТУДЕНТОВ С РЕЗАМИ ЭКЗОВ: \n\n";
    for (int i = 0; i < n; i++) {
        fou << array[i].surname << " " << array[i].name << " " << array[i].patr << " " << array[i].subject << " " << array[i].mark << endl;
    }

    session ses = generate_list(array, n, kol);
    cout << "Экзаменационная сессия:\n";
    show_ses(ses, kol);

    delete[] array; //для виду удалил память, выделенную под массив структур
    return 0;
}

void show_st(students* A, int n)
{
    for (int i = 0; i < n; i++) {
        cout << A[i].surname << " " << A[i].name << " " << A[i].patr << " " << A[i].subject << " " << A[i].mark << endl;
    }
}

void sort_st(students* A, int n) //обычный алгоритм сортировки методом пузырька, сортируем по возрастанию среднего балла
{
    int f, k = 0;
    do {
        f = 0;
        for(int i = 0; i < n - k - 1; i++)
            if (A[i].mark > A[i + 1].mark) {
                swap(A[i], A[i + 1]);
                f = 1;
            }
        k++;
    }while (f);
}

session& generate_list(students* A, int n, int& kol) //тут подаем ещё ссылку на kol, так как при выводе данных структуры надо знать общее количество предметов
{
    session res; //локальная структура, над которой работаем
    kol = 0;
    res.subjects = new char* [n]; //так как у нас тут матрица символов (массив из предметов), то необходимо выделить память под сами предметы
    res.ball = new double[n]; //выделяем память под оценки
    int* k = new int[n]; //массив, содержащий количество писавших каждый экзамен студентов
    //везде количество символов ставим по максимуму, т.е n, так как все могут писать один экзамен
    for (int i = 0; i < n; i++)
        res.ball[i] = 0;
    for (int i = 0; i < n; i++) //идём в массиве по элементам массива структур students
    {
        int j = 0; //вводим j для прохода по массивам выше, идём до kol, обозначающий текущее количество предметов
        while (j < kol && strcmp(A[i].subject, res.subjects[j])) j++; //либо до той позиции, где совпал предмет
        if (j >= kol) //если мы получили j, равный kol, то записываем новый предмет 
        { 
            res.subjects[kol] = A[i].subject;
            res.ball[kol] = A[i].mark; //в mark сохраняем первую оценку 
            k[kol] = 1; //устанавливаем количество писавших экз студентов = 1
            kol++; //появился новый предмет, увеличиваем счетчик на 1
        }
        else { //иначе...
            k[j]++; //также увеличиваем счётчик писавших студентов
            res.ball[j] += A[i].mark; //в mark пока суммируем все оценки, не переписывая элемент 
        }
    }
    for (int i = 0; i < kol; i++) //делим mark на количество k и получаем средний балл 
    {
        res.ball[i] /= k[i];
    }
    delete[] k; 
    return res;
}

void show_ses(session& A, int kol)
{
    for (int i = 0; i < kol; i++) {
        cout << A.subjects[i] << " " << A.ball[i] << endl;
    }
}