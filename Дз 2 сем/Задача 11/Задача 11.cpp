#include <iostream>
#include <fstream>

using namespace std;

struct queue
{
    char s;
    queue* pNext;
};

void push_back(queue** begin, queue** end, char s); //добавляет новый элемент в конец очереди
void start_queue(queue** begin, queue** end); //вытаскивает первый элемент и удаляет его
void print_queue(queue* begin); //печатает все элементы очереди

int main()
{
    setlocale(LC_ALL, "ru");
    queue* begin, *end;
    begin = end = NULL;

    /*
    В функции main мы проводим чтение файла и запись его символов в очередь. По условию записываем цифры и кириллицу.
    Так как очередь по определению может принимать символы только в конец, то сначала идём по цифрам, а потом по буквам. 
    То есть 2 прохода текстового файла. В конце печатаем содержимое списка (как процесс обслуживания очереди (т.е с её головы), и каждый раз получаем 
    следующий по очереди элемент).
    */

    ifstream file("input.txt");
    while (!file.eof())
    {
        char c;
        file.get(c);
        if (c >= '0' && c <= '9') push_back(&begin, &end, c);
    }

    file.clear();
    file.seekg(0);

    while (!file.eof())
    {
        char c;
        file.get(c);
        if (c >= 'а' && c <= 'я') push_back(&begin, &end, c);
    }

    print_queue(begin);
    return 0;
}

void push_back(queue** begin, queue** end, char s)
{
    //создаём элемент структуры и заносим все начальные данные
    queue* new_el;
    new_el = new queue;
    new_el->s = s;
    new_el->pNext = NULL;

    //если нет ещё ни одного элемента в очереди, то вносим первый элемент 
    if (*begin == NULL) {
        *begin = *end = new_el;
        return;
    }
    //меняем указатель на след элемент текущего последнего элемента на новый последний и обновляем указатель end
    (**end).pNext = new_el;
    (*end) = new_el;
}

void start_queue(queue** begin, queue** end)
{
        //создаём временный объект
        queue* ptr;
        ptr = *begin;
        if (*begin == NULL)
        {
            cout << "Очередь пуста";
            return;
        }
        cout << "Элемент на очереди: " << ptr->s << endl;
        //Передвигается начало очереди:
        (*begin) = (**begin).pNext;
        //Освобождается место удаляемого элемента:
        delete ptr;
}

void print_queue(queue* begin)
{
    //идём по всем элементам очереди с начала и печатаем их в консоль
    queue* ptr = begin;
    int counter = 50;
    while (ptr != NULL)
    {
        if (counter)
        {
            cout << (*ptr).s << " ";
            ptr = (*ptr).pNext;
            counter--;
        }
        else 
        {
            cout << "\n" << (*ptr).s << " ";
            ptr = (*ptr).pNext;
            counter = 50;
        }
    }
}

