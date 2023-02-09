#define SIZE 100

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

class file_open_error
{
    char* filename;
public:
    file_open_error(char* name) {
        this->filename = name;
    }
    char* GetName() { return filename; }
};

class file_read_error
{
    char* filename;
    streampos pos;
public:
    file_read_error(char* name, streampos pos) {
        this->filename = name;
        this->pos = pos;
    }
    char* GetName() { return filename; }
    streampos GetPos() { return pos; }
};


template <class T>
class List
{
    template <class T>
    class Node
    {
    public:
        T data;
        Node* pNext;
        Node* pPrev;
        Node(T data = T(), Node* pNext = nullptr, Node* pPrev = nullptr) {
            this->data = data;
            this->pNext = pNext;
            this->pPrev = pPrev;
        }
    };
    Node<T>* head;
    Node<T>* tail;
    int Size;
public:
    List()
    {
        Size = 0;
        head = nullptr;
        tail = nullptr;
    }
    ~List()
    {
        while (Size)
        {
            pop_front();
        }
    }
    int GetSize() { return Size; }
    void push_back(T data);
    void push_front(T data);
    void insert(T data);
    void pop_front();
    void pop_back();
    T& operator[](const int index);
    void delete_copy();
    void search_lstr(char* lowstr);
};

template<class T>
void List<T>::push_back(T data)
{
    if (tail == nullptr) {
        Size++;
        head = tail = new Node<T>(data);
        return;
    }
    Node<T>* current = new Node<T>(data, nullptr, tail);
    tail->pNext = current;
    tail = current;
    Size++;
}

template<class T>
void List<T>::push_front(T data)
{
    if (head == nullptr) {
        Size++;
        head = tail = new Node<T>(data);
        return;
    }
    Node<T>* current = new Node<T>(data, head);
    head->pPrev = current;
    head = current;
    Size++;
}

template<class T>
void List<T>::insert(T data)
{
    if (head == nullptr) {
        head = tail = new Node<T>(data);
        Size++;
        return;
    }

    if ((strlen(data) == strlen(head->data) && strcmp(data, head->data) < 0) || (strlen(data) < strlen(head->data)))
    {
        push_front(data);
        return;
    }
    Node<T>* current = head;
    int f = 0;
    int fa = 0;
    while (current->pNext != nullptr)
    {
        f = 1;
        while (current != nullptr)
        {
            if ((strlen(data) == strlen(current->data) && strcmp(data, current->data) > 0) || (strlen(data) > strlen(current->data)))
            {
                fa = 1;
                current = current->pNext;
            }
            else break;
        }
        if (!fa) current = current->pNext;
        else break;
    }
    if (!f)
    {
        push_back(data);
        return;
    }
    if (current == nullptr)
    {
        push_back(data);
        return;
    }
    Node<T>* new_elem = new Node<T>(data, current, current->pPrev);
    current->pPrev->pNext = new_elem;
    current->pPrev = new_elem;
    Size++;
}

template<class T>
void List<T>::pop_front()
{
    if (head->pNext == nullptr) {
        delete head;
        Size--;
        return;
    }
    Node<T>* ptr = this->head;
    head = head->pNext;
    head->pPrev = nullptr;
    delete ptr;
    Size--;
}

template<class T>
void List<T>::pop_back()
{
    if (tail->pPrev == nullptr) {
        delete head;
        Size--;
        return;
    }
    Node<T>* ptr = this->tail;
    tail = tail->pPrev;
    tail->pNext = nullptr;
    delete ptr;
    Size--;
}

template<class T>
T& List<T>::operator[](const int index)
{

    if (index < (this->GetSize() / 2)) {
        int counter = 0;
        Node<T>* current = this->head;
        while (current != nullptr)
        {
            if (index == counter) return current->data;
            counter++;
            current = current->pNext;
        }
    }

    else {
        int counter = this->GetSize() - 1;
        Node<T>* current = this->tail;
        while (current != nullptr)
        {
            if (index == counter) return current->data;
            counter--;
            current = current->pPrev;
        }
    }
}

template<class T>
void List<T>::delete_copy()
{
    Node<T>* current = head;
    while (current->pNext != nullptr)
    {
        if (strcmp(current->pNext->data, current->data)) {
            current = current->pNext;
        }
        else
        {
            current = current->pNext;
            //if (current->pNext == nullptr && strcmp(current->data, current->pPrev->data)
            while (current != nullptr)
            {
                if (strcmp(current->data, current->pPrev->data))
                {
                    break;
                }
                else
                {
                    Node<T>* del_el = current;
                    if (current->pNext == nullptr)
                    {
                        pop_back();
                        return;
                    }
                    current->pPrev->pNext = current->pNext;
                    current->pNext->pPrev = current->pPrev;
                    Size--;
                    if (current->pNext == nullptr) {
                        delete del_el;
                        break;
                    }
                    current = current->pNext;
                    delete del_el;
                }
            }
        }
    }
}

template<class T>
void List<T>::search_lstr(char* lowstr)
{
    Node<T>* current = head;
    while (current != nullptr) {
        char* str = current->data;
        int i = 0;
        while (str[i] != '\0') {
            if (str[i] == lowstr[0]) break;
                i++;
        }
        int j = 0;
        while (str[i] != '\0') {
            if (str[i] != lowstr[j]) break;
            i++;
            j++;
        }
        if (j  == strlen(lowstr)) {
            cout << current->data << " ";
        }
        current = current->pNext;
    }
}

template <class T>
ostream& operator<<(ostream& os, List<T>& ob)
{
    for (int i = 0; i < ob.GetSize(); i++)
    {
        os << ob[i] << " ";
    }
    return os;
}

template <class T>
istream& operator>>(istream& is, List<T>& ob)
{
    while (!is.eof())
    {
        char tmp[SIZE];
        is >> tmp;
        if (is.fail()) throw file_read_error(_strdup("input.txt"), is.tellg());
        ob.insert(_strdup(tmp));
    }
    is.clear();
    is.seekg(0);
    return is;
}

int main()
{
    setlocale(LC_ALL, "ru");
    char* path = _strdup("input.txt");
    List<char*> list;
    ifstream file(path);
    try
    {
        if (!file.is_open()) throw file_open_error(path);
        file >> list;
        file.close();
        cout << "List, состоящий из фамилий: " << endl;
        cout << list << "\n\n";
        list.delete_copy();
        cout << "List без повторяющихся фамилий:" << endl;
        cout << list << endl;
        cout << "Введите подстроку:" << endl;
        char string[SIZE];
        cin >> string;
        cout << "Слова с подстрокой " << _strdup(string) << ":\n";
        list.search_lstr(_strdup(string));
    }
    catch (file_open_error ex)
    {
        cout << "Ошибка открытия файла " << ex.GetName();
        return 1;
    }
    catch (file_read_error ex)
    {
        cout << "Ошибка чтения из файла " << ex.GetName() << " на позиции " << ex.GetPos();
        return 2;
    }
    return 0;
}