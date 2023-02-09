#define _CRT_SECURE_NO_WARNINGS
#define SIZE 100

#include <iostream>
#include <fstream>

using namespace std;

class file_read_error 
{
    char* file_name;
    streampos pos;
public:
    file_read_error(char* file_name, streampos pos) {
        file_read_error::file_name = file_name;
        file_read_error::pos = pos;
    }
    char* get_filename() { return file_name; }
    streampos get_pos() { return pos; }
};
class file_open_error 
{
    char* file_name;
public:
    file_open_error(char* file_name) {
        file_open_error::file_name = file_name;
        //strcpy(file_open_error::file_name, file_name);
    }
    char* get_filename() { return file_name; }
};
class file_write_error {
    char* file_name;
    streampos pos;
public:
    file_write_error(char* file_name, streampos pos) {
        file_write_error::file_name = file_name;
        file_write_error::pos = pos;
    }
    char* get_filename() { return file_name; }
    streampos get_pos() { return pos; }
};

void fill_file(ofstream& file);
void file_read(ifstream& file, int& k, char** surnames);
double aver_ball(ifstream& f, int& flag);
void file_write(ofstream& file, char** array, int k);

int main()
{
    setlocale(LC_ALL, "ru");
    char* path = _strdup("input.txt");
    char* path_2 = _strdup("output.txt");
    try 
    {
        ofstream f(path);
        if (f.fail()) throw file_open_error(path);
        fill_file(f);
        f.close();

        ifstream ff(path);
        if (!ff.is_open()) throw file_open_error(path);
        char** surnames = new char*[SIZE];
        int k;
        file_read(ff, k, surnames);
        ff.close();

        ofstream fff(path_2);
        if (fff.fail()) throw file_open_error(path);
        file_write(fff, surnames, k);
        fff.close();

       delete[] surnames;
    }
    catch (file_open_error ex) 
    {
        cout << "Ошибка открытия файла " << ex.get_filename();
        return 1;
    }
    catch (file_read_error ex)
    {
        cout << "Ошибка чтения файла " << ex.get_filename() << " на " << ex.get_pos() << " позиции";
        return 2;
    }
    catch (file_write_error ex)
    {
        cout << "Ошибка записи в файл " << ex.get_filename() << " на " << ex.get_pos() << " позиции";
        return 3;
    }
    catch (bad_alloc xa) {
        cout << "Не удалось выделить динамическую память";
        return 4;
    }
    return 0;
}

void fill_file(ofstream& file)
{
    int p;
    do {
        cout << "Введите 0 для конца ввода ";
        cin >> p;
        if (!p) break;
        cout << "Введите фамилию и количество сданных экзаменов: " << endl;
        char* surname;
        int kol;
        char sur[20];
        cin >> sur >> kol;
        surname = _strdup(sur);
        file << surname << " " << kol << " ";
        if (file.fail()) throw file_write_error(_strdup("input.txt"), file.tellp());
        cout << "Введите оценки: " << endl;
        for (int i = 0; i < kol; i++) {
            int x;
            cin >> x;
            file << x << " ";
            if (file.fail()) throw file_write_error(_strdup("input.txt"), file.tellp());
        }
        file << "\n";
        delete[] surname;
    } while (true);
}

 void file_read(ifstream& file, int& k, char** surnames) 
{
     int n = 0;
     streampos pos = file.tellg();
     double average = 0;
     while (!file.eof()) {
         char fam[20];
         int f;
         file >> fam;
         if (!strcmp(fam, "")) continue;
         if (file.fail()) throw file_read_error(_strdup("input.txt"), file.peek());
         average += aver_ball(file, f);
         n++;
     }
     average /= n;
     file.clear();
     file.seekg(0);

     k = 0;
     for (int i = 0; i < n; i++) {
         char fam[20];
         file >> fam;
         if (file.fail()) throw file_read_error(_strdup("input.txt"), file.peek());
         char* surname = _strdup(fam);
         double aver;
         int flag = 1;
         aver = aver_ball(file, flag);
         if (flag && aver > average)
         {
             surnames[k] = _strdup(surname);
             k++;
         }
         delete[] surname;
     }
}

 double aver_ball(ifstream& f, int& flag) {
     double ball = 0;
     int kol, k = 0;
     f >> kol;
     if (f.fail()) throw file_read_error(_strdup("input.txt"), f.peek());
     int* marks = new int[kol];
     for (int j = 0; j < kol; j++) {
         f >> marks[j];
         if (f.fail()) throw file_read_error(_strdup("input.txt"), f.peek());
         if (marks[j] > 2) {
             ball += marks[j];
             k++;
         }
         else flag = 0;
     }
     delete[] marks;
     return ball/k;
 }

 void file_write(ofstream& file, char** array, int k) 
 {
     for (int i = 0; i < k; i++) {
         file << array[i] << endl;
         if (file.fail()) throw file_write_error(_strdup("output.txt"), file.tellp());
         delete array[i];
     }
 }