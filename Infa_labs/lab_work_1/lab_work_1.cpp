#include <iostream>
#include <cstring>

using namespace std;

template<class TypeOfElement>
class MyVector {
private:
    int Size;
    int Capacity;
    TypeOfElement* dataPointer;
    void UpCapacity(int newCapacity);
public:
    MyVector() 
    {
        Size = 0;
        Capacity = 0;
        dataPointer = new TypeOfElement[Capacity];
    }
    MyVector(const MyVector& ob)
    {
        delete[] dataPointer;
        dataPointer = new TypeOfElement[ob.Size];
        for (int i = 0; i < ob.GetSize(); i++) {
            this->dataPointer[i] = ob.dataPointer[i];
        }
        this->Capacity = this->Size = ob.Size;
    }
    ~MyVector() { delete[] dataPointer; }

    bool isEmpty() const;
    int GetSize() const { return Size; }
    void GetCapacity() const { return Capacity; }
    void PushBack(TypeOfElement data);
    TypeOfElement& At(const int index);
    TypeOfElement& operator[](const int index);
    MyVector<TypeOfElement> operator+(MyVector<TypeOfElement>& vector);
    void SetSize(const int len);

};

template<class TypeOfElement>
bool MyVector<TypeOfElement>::isEmpty() const
{
    if (!Size) return true;
    return false;
}

template<class TypeOfElement>
void MyVector<TypeOfElement>::UpCapacity(int newCapacity)
{
    if (newCapacity >= Capacity) {
        TypeOfElement* newPointer;
        newPointer = new TypeOfElement[Capacity + 1];
        for (int i = 0; i < Capacity; i++) {
            newPointer[i] = dataPointer[i];
        }
        delete[] dataPointer;
        dataPointer = newPointer;
    }
}

template<class TypeOfElement>
void MyVector<TypeOfElement>::PushBack(TypeOfElement data)
{
    Size++;
    UpCapacity(Size);
    dataPointer[Capacity++] = data;
}

template<class TypeOfElement>
TypeOfElement& MyVector<TypeOfElement>::operator[](const int index)
{
    return dataPointer[index];
}

template<class TypeOfElement>
void MyVector<TypeOfElement>::SetSize(const int len)
{
    if (len > Capacity) {
        TypeOfElement* newPointer = new TypeOfElement[len];
        for (int i = 0; i < Size; i++) {
            newPointer[i] = dataPointer[i];
        }
        delete[] dataPointer;
        dataPointer = newPointer;
        Size = Capacity = len;
    }
}

template<class TypeOfElement>
MyVector<TypeOfElement> MyVector<TypeOfElement>::operator+(MyVector<TypeOfElement>& vector)
{
    MyVector<TypeOfElement> vec;
    int len = this->GetSize() + vector.GetSize();
    vec.SetSize(len);
    int i;
    for ( i = 0; i < this->GetSize(); i++) {
        vec.dataPointer[i] = this->dataPointer[i];
    }
    for (int j = i; j < len; j++) {
        vec.dataPointer[j] = vector.dataPointer[i];
    }
    return vec;
}



class Vehicle {
public:
    //Конструктор ТС по умолчанию
    Vehicle() : totalDistance(0), totalTime(0), baseSpeed(DefaultVehicleSpeed) {
        SetName(DefaultVehicleName);
    }
    //Параметризованый конструтор ТС
    Vehicle(const char inNameCString[], double inBaseSpeed, int inBasePrice = 0) : totalDistance(0),
        totalTime(0), baseSpeed(inBaseSpeed) {
        SetName(inNameCString);
    }
    //Деструктор ТС
    virtual ~Vehicle() {

    }
    //Метод получения информации об имени ТС
    const char* const GetName() const {
        return nameCString;
    }
        //Метод получения информации о скорости при покупке
        double GetBaseSpeed() const {
        return baseSpeed;
    }
    //Метод получения информации о текущем пробеге
    double GetTotalDistance() const {
        return totalDistance;
    }
    //Метод получения информации о текущем времени эксплуатации
    double GetTotalTime() const {
        return totalTime;
    }
    //Абстрактный метод совершения поездки
    //Возвращает на true, если поезка удачна и false, если случилась авария.
    virtual bool MakeTrip(double distance) = 0;
    //Абстрактный метод получения информации о текущей скорости
    virtual double GetSpeed() const = 0;
    //Оператор сравнения по умолчанию
    bool operator< (Vehicle& rhs) const {
        if (GetSpeed() < rhs.GetSpeed()) {
            return true;
        }
        return false;
    }
    //Константа - общее максимальное количество символов в имени ТС
    static const int MAX_NAME_LENGTH = 50;
    
protected:
    //Счетчик пройденного расстояния
    double totalDistance;
    //Счетчик общего времени эксплуатации
    double totalTime;
private:
    //Метод установки имени транспортного средства
    void SetName(const char inNameCString[]) {
        int i = 0;
        for (i = 0; (inNameCString[i] != 0) && (i < MAX_NAME_LENGTH); ++i) {
            nameCString[i] = inNameCString[i];
        }
        nameCString[i] = 0;
    }
    //С - строка для хранения имени
    char nameCString[MAX_NAME_LENGTH];

    const char* DefaultVehicleName = "Untyped vehicle";
    const double DefaultVehicleSpeed = -1.;
    //Скорость нового транспортного средства
    double baseSpeed;
};


class Coach : public Vehicle 
{
private: 
    const char* DefaultCoachName = "Default Coach";
    const double DefaultCoachSpeed = 10.;
    const double MAX_DISTANCE;
public:
    //конструктор класса Coach
    Coach()  : MAX_DISTANCE(1000) , Vehicle("Default Coach", DefaultCoachSpeed )  {
    }
    Coach(const char inNameCString[], double inBaseSpeed) : MAX_DISTANCE(1000) , Vehicle(inNameCString, inBaseSpeed = DefaultCoachSpeed)  {
    }
    Coach(const char inNameCString[]) : Vehicle(inNameCString, DefaultCoachSpeed),  MAX_DISTANCE(1000) {
    }
    ~Coach() {

    }
    Coach& operator=(const Coach& ob) {
        Coach cmp = ob;
        return cmp;
    }
    bool MakeTrip(double distance);
    double GetSpeed() const;
};

bool Coach::MakeTrip(double distance)
{
    if (distance > MAX_DISTANCE) return false;
    else {
        totalDistance += distance;
        totalTime += distance / GetSpeed();
        return true;
    }
}

double Coach::GetSpeed() const {
    return GetBaseSpeed() * exp(-totalTime / 500.);
}

class Computer {
public:
    Computer() : baseTimeToBoot(DefaultTimeToBoot){}
    double GetTimeToBoot() {
        return baseTimeToBoot;
    }
protected:
    double baseTimeToBoot;
private:
    const double DefaultTimeToBoot = 0.01;
};

class Aeroplane :public Vehicle, private Computer
{
    const char* DefaultAeroplaneName = "Default Aeroplane";
    const double DefaultAeroplaneSpeed = 500.;
    double MAX_FLY_TIME;
    double timeSinceLastRepair = 0;
public:
    Aeroplane() : Vehicle("Default Aeroplane", DefaultAeroplaneSpeed), Computer() { putMaxTime(); }
    Aeroplane (const char inNameCString[]) : Vehicle(inNameCString, DefaultAeroplaneSpeed), Computer() { putMaxTime(); }
    Aeroplane (const char inNameCString[], double inBaseSpeed) : Vehicle(inNameCString, inBaseSpeed), Computer() { putMaxTime(); }
    ~Aeroplane() {

    }
    Aeroplane& operator=(const Aeroplane& ob) {
        Aeroplane cmp = ob;
        return cmp;
    }
    void ComputerUpdate(double newTimeToBoot) {
        baseTimeToBoot = newTimeToBoot;
    }
    void putMaxTime() { MAX_FLY_TIME = 10000.; }
    void changeMaxTime(double newMaxTime) { MAX_FLY_TIME = newMaxTime; }
    bool MakeTrip(double distance) override;
    double GetSpeed() const override;
    void Repair();
    double GetTimeSinceLastRepair() const;
};

double Aeroplane::GetTimeSinceLastRepair() const {
    return timeSinceLastRepair;
}

void Aeroplane::Repair() {
    timeSinceLastRepair = 0;
}
bool Aeroplane::MakeTrip(double distance)
{
    double timeOfTrip = distance / GetSpeed() + GetTimeToBoot();
    if (timeSinceLastRepair + timeOfTrip > MAX_FLY_TIME) {
        return false;
    }
    timeSinceLastRepair += timeOfTrip;
    totalDistance += distance;
    totalTime += timeOfTrip;
    return true;
}

double Aeroplane::GetSpeed() const {
    return GetBaseSpeed();
}
class Automobile :public Vehicle
{
private:
    const char* DefaultAutomobileName = "Default Automobile";
    const double DefaultAutomobileSpeed = 100.;
public:
    //конструктор класса Coach
    Automobile(): Vehicle("Default Automobile", DefaultAutomobileSpeed) {
    }
    Automobile(const char inNameCString[], double inBaseSpeed) :  Vehicle(inNameCString, inBaseSpeed = DefaultAutomobileSpeed) {
    }
    Automobile(const char inNameCString[]) : Vehicle(inNameCString, DefaultAutomobileSpeed) {
    }
    ~Automobile() {

    }
    Automobile& operator=(const Automobile& ob) {
        Automobile cmp = ob;
        return cmp;
    }
    bool MakeTrip(double distance) override;
    double GetSpeed() const override;
};

bool Automobile::MakeTrip(double distance)
{
        totalDistance += distance;
        totalTime += distance / GetSpeed();
        return true;
}

double Automobile::GetSpeed() const {
    return GetBaseSpeed() * exp(-totalDistance / 500.);
}

void DisplayVehicles(MyVector<Vehicle*> &vehicles)
{
    cout << "Name\t\tSpeed\tDist\tTime" << endl;
    for (int i = 0; i < vehicles.GetSize(); i++) {
        cout << vehicles[i]->GetName() << "\t" << vehicles[i]->GetSpeed() << "\t" << vehicles[i]->GetTotalDistance() << "\t" << vehicles[i]->GetTotalTime() << endl;
    }
}

template <class MyType>
void MySwap(MyType& v1, MyType& v2) {
    MyType v3 = v1;
    v1 = v2;
    v2 = v3;
}

template<class ArrayType, class LessFunctionType>
int FindMinimumIndex(ArrayType& data_array, int beginIndex, int endIndex, LessFunctionType LessFunction) {
    int minimumIndex = beginIndex;
    for (int element_number = beginIndex + 1; element_number <= endIndex; element_number++) {
        if (LessFunction(data_array[element_number], data_array[minimumIndex])) {
            minimumIndex = element_number;
        }
    }
    return minimumIndex;
}

template<class ArrayType, class LessFunctionType>
void SelectionSort(ArrayType& data_array, int beginIndex, int endIndex, LessFunctionType LessFunction) {
    for (int element_number = beginIndex; element_number < endIndex; element_number++) {
        int minimumIndex = FindMinimumIndex(data_array, element_number, endIndex, LessFunction);
        MySwap(data_array[minimumIndex], data_array[element_number]);
    }
}

bool CompareTime(Vehicle* lhs, Vehicle* rhs) {
    return lhs->GetTotalTime() < rhs->GetTotalTime();
}

void CommitRandomTrips(MyVector<Vehicle*> &vehicles)
{
    for (int i = 0; i < vehicles.GetSize(); i++) {
        double randomDistance = double(rand() % 20001) / 10.;
        vehicles[i]->MakeTrip(randomDistance);
    }
}

int main()
{
    setlocale(LC_ALL, "ru");
    srand(0);
    
    //Векторы для хранения экземпляров ТС
    MyVector<Coach> coaches;
    MyVector<Automobile> automobiles;
    MyVector<Aeroplane> aeroplanes;
    //Добавление конкретных объектов
    coaches.PushBack(Coach("Coach 1", 9.));
    coaches.PushBack(Coach("Coach 2", 11.));
    coaches.PushBack(Coach("Coach 3", 10.));
    coaches.PushBack(Coach("Coach 4", 9.5));
    coaches.PushBack(Coach("Coach 5"));
    automobiles.PushBack(Automobile("Automobile 1"));
    automobiles.PushBack(Automobile("Automobile 2", 90.));
    automobiles.PushBack(Automobile("Automobile 3", 120.));
    automobiles.PushBack(Automobile("Automobile 4", 150.));
    aeroplanes.PushBack(Aeroplane("Aeroplane 1", 1030.));
    aeroplanes.PushBack(Aeroplane("Aeroplane 2", 560.));
    aeroplanes.PushBack(Aeroplane("Aeroplane 3", 2200.));
    
    //Векторы указателей на объекты
    MyVector<Vehicle*> coachPointers;
    MyVector<Vehicle*> automobilePointers;
    MyVector<Vehicle*> aeroplanePointers;
    //Инициализация векторов указателей
    for (int i = 0; i < coaches.GetSize(); ++i) {
        coachPointers.PushBack(&coaches[i]);
    }
    for (int i = 0; i < automobiles.GetSize(); ++i) {
        automobilePointers.PushBack(&automobiles[i]);
    }
    for (int i = 0; i < aeroplanes.GetSize(); ++i) {
        aeroplanePointers.PushBack(&aeroplanes[i]);
    }
    MyVector<Vehicle*> vehiclePointers = coachPointers + automobilePointers + aeroplanePointers;
    int n = 5;
    for (int i = 0; i < n; i++) 
    {
        CommitRandomTrips(vehiclePointers);
    }
    SelectionSort(vehiclePointers, 0, vehiclePointers.GetSize(), CompareTime);
    DisplayVehicles(vehiclePointers);
    return 0;
}