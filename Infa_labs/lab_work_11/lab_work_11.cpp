#include <iostream>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <time.h>

using namespace std;

template <class T>
vector<T> operator+(const vector<T>& v1, const vector<T>& v2)
{
    vector<T> result;
    for (int i = 0; i < v1.size(); i++) {
        result.push_back(v1[i]);
    }
    for (int i = 0; i < v2.size(); i++) {
        result.push_back(v2[i]);
    }
    return result;
}

// сортировка массивов
template<class MyType>
void MySwap(MyType& v1, MyType& v2)
{
    MyType v3 = v1;
    v1 = v2;
    v2 = v3;
}

template <class ArrayType, class LessFunctionType>
int FindMinimumIndex(ArrayType& data_array, int beginIndex, int endIndex, LessFunctionType LessFunction)
{
    int minimumIndex = beginIndex;
    for (int element_number = beginIndex + 1; element_number <= endIndex; ++element_number) {
        if (LessFunction(data_array[element_number], data_array[minimumIndex]))
        {
            minimumIndex = element_number;
        }
    }
    return minimumIndex;
}

template<class ArrayType, class LessFunctionType>
void SelectionSort(ArrayType& data_array, int beginIndex, int endIndex, LessFunctionType LessFunction)
{
    for (int element_number = beginIndex; element_number < endIndex; ++element_number)
    {
        int minimumIndex = FindMinimumIndex(data_array, element_number, endIndex, LessFunction);
        MySwap(data_array[minimumIndex], data_array[element_number]);
    }
}

const char DefaultVehicleName[] = "Untyped vehicle";
const char DefaultCoachName[] = "Default Coach";
const char DefaultAutomobileName[] = "Default Automobile";
const char DefaultPlaneName[] = "Default Aeroplane";
const char DefaultAutopovozkaName[] = "Default Autopovozka";

const double DefaultVehicleSpeed = -1.;
const double DefaultCoachSpeed = 10.;
const double DefaultAutomobileSpeed = 100.;
const double DefaultPlaneSpeed = 500.;
const double DefaultAutopovozkaSpeed = 60.;

const double DefaultVehiclePrice = 200;
const double DefaultCoachPrice = 80;
const double DefaultAutomobilePrice = 200;
const double DefaultPlanePrice = 500;
const double DefaultAutopovozkaPrice = 120;

const double DefaultTimeToBoot = 0.01;

class Computer {
public:
    Computer() : baseTimeToBoot(DefaultTimeToBoot) {}

    double GetTimeToBoot() {
        return baseTimeToBoot;
    }

protected:
    double baseTimeToBoot;
};

class Vehicle {
public:

    Vehicle() : totalDistance(0), totalTime(0), baseSpeed(DefaultVehicleSpeed) {
        SetName(DefaultVehicleName);
        count++;
        num = count;
    }

    Vehicle(const char inNameCString[] = DefaultVehicleName, double inBaseSpeed = DefaultVehicleSpeed,
        double inBasePrice = DefaultVehiclePrice) : totalDistance(0), totalTime(0) {
        baseSpeed = inBaseSpeed;
        SetName(inNameCString);
        count++;
        num = count;
    }

    virtual ~Vehicle() {}

    //Метод получения информации об имени ТС
    const char* GetName() const {
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

    double GetNumber() const {
        return num;
    }

    //Абстрактный метод совершения поездки
    //Возвращает true, если поезка удачна и false, если случилась авария.
    virtual bool MakeTrip(double distanceOfTrip) = 0;

    //Абстрактный метод получения информации о текущей скорости
    virtual double GetSpeed() const {
        return baseSpeed;
    }

    bool CompareDefault(Vehicle* lhs, Vehicle* rhs) {
        return *lhs < *rhs;
    }

    //Оператор сравнения по умолчанию
    bool operator< (Vehicle& rhs) const {
        return (GetSpeed() < rhs.GetSpeed());
    }

    //Константа - общее максимальное количество символов в имени ТС
    static const int MAX_NAME_LENGTH = 50;

    //Скорость нового транспортного средства
    double baseSpeed;

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

    int num;
    int count;
};

class Plane : public Vehicle, private Computer
{
public:
    Plane(const char inNameCString[] = DefaultPlaneName, double inBaseSpeed = DefaultPlaneSpeed,
        int inBasePrice = DefaultPlanePrice) : Vehicle(inNameCString, inBaseSpeed, inBasePrice)
    {
        Repair();
    }

    virtual bool MakeTrip(double distanceOfTrip) {
        double timeOfTrip = distanceOfTrip / GetSpeed() + GetTimeToBoot();
        if (timeSinceLastRepair + timeOfTrip > MAX_FLY_TIME) {
            return false;
        }
        timeSinceLastRepair += timeOfTrip;
        totalDistance += distanceOfTrip;
        totalTime += timeOfTrip;
        return true;
    }

    void ComputerUpdate(double newTimeToBoot) {
        baseTimeToBoot = newTimeToBoot;
    }

    virtual double GetSpeed() const {
        return GetBaseSpeed();
    }

    double GetTimeSinceLastRepair() const {
        return timeSinceLastRepair;
    }

    void Repair() {
        timeSinceLastRepair = 0;
    }

    static const int MAX_FLY_TIME = 100;

private:
    double timeSinceLastRepair;
};

class Coach : public virtual Vehicle {
public:
    Coach(const char inNameCString[] = DefaultCoachName, double inBaseSpeed = DefaultCoachSpeed,
        int inBasePrice = DefaultCoachPrice) : Vehicle(inNameCString, inBaseSpeed, inBasePrice) {}

    virtual bool MakeTrip(double distance)
    {
        if (totalDistance + distance > MAX_DISTANCE) {
            return false;
        }
        double timeOfTrip = distance / GetSpeed();
        totalDistance += distance;
        totalTime += timeOfTrip;
        return true;
    }

    virtual double GetSpeed() const {
        return GetBaseSpeed() * exp(-totalTime / 500.);
    }

    static const int MAX_DISTANCE = 3000;
};

class Automobile : public virtual Vehicle {
public:
    Automobile(const char inNameCString[] = DefaultAutomobileName, double inBaseSpeed = DefaultAutomobileSpeed,
        int inBasePrice = DefaultAutomobilePrice) : Vehicle(inNameCString, inBaseSpeed, inBasePrice) {}

    virtual double GetSpeed() const {
        return GetBaseSpeed() * exp(-totalDistance / 500.);
    }

    virtual bool MakeTrip(double distance)
    {
        double timeOfTrip = distance / GetSpeed();
        totalDistance += distance;
        totalTime += timeOfTrip;
        return true;
    }
};

class Autopovozka : public Automobile, public Coach
{
public:
    Autopovozka(const char inNameCString[] = DefaultAutopovozkaName, double inBaseSpeed = DefaultAutopovozkaSpeed, int inBasePrice = DefaultAutopovozkaPrice) : Vehicle(inNameCString, inBaseSpeed, inBasePrice) {}

    virtual bool MakeTrip(double distance)
    {
        if (totalDistance + distance > MAX_DISTANCE) {
            return false;
        }
        double timeOfTrip = distance / GetSpeed();
        totalDistance += distance;
        totalTime += timeOfTrip;
        return true;
    }

    virtual double GetSpeed() const {
        return GetBaseSpeed() * exp(-totalTime / 500.);
    }

    static const int MAX_DISTANCE = 6000;
};

void CommitRandomTrips(vector<Vehicle*>& vehicles) {
    for (int i = 0; i < vehicles.size(); ++i)
    {
        double randomDistance = double(rand() % 20001) / 10.;
        vehicles[i]->MakeTrip(randomDistance);
    }
}

void DisplayVehicles(const vector<Vehicle*>& vehicles) {
    printf("%20s   \t%10s\t%10s\t%10s\n", "Name", "Speed", "Dist", "Time");
    for (int i = 0; i < vehicles.size(); ++i)
    {
        printf("%20s \t%10.2f\t%10.2f\t%10.2f\n", vehicles[i]->GetName(), vehicles[i]->GetSpeed(),
            vehicles[i]->GetTotalDistance(), vehicles[i]->GetTotalTime());
    }
}

bool CompareSpeed(Vehicle* lhs, Vehicle* rhs)
{
    return lhs->GetSpeed() < rhs->GetSpeed();
}

bool CompareName(Vehicle* lhs, Vehicle* rhs)
{
    return lhs->GetName() < rhs->GetName();
}

bool CompareTime(Vehicle* lhs, Vehicle* rhs)
{
    return lhs->GetTotalTime() < rhs->GetTotalTime();
}

bool CompareDist(Vehicle* lhs, Vehicle* rhs)
{
    return lhs->GetTotalDistance() < rhs->GetTotalDistance();
}

int main() {
    srand(time(0)); // генератор случайных чисел
    setlocale(LC_ALL, "ru");
    //Векторы для хранения экземпляров ТС
    vector <Coach> coaches;
    vector <Automobile> automobiles;
    vector <Plane> aeroplanes;
    vector <Autopovozka> povozki;

    //Добавление конкретных объектов
    coaches.push_back(Coach("Coach 1", 9., 100.));
    coaches.push_back(Coach("Coach 2", 11., 100.));
    coaches.push_back(Coach("Coach 3", 10., 100.));
    coaches.push_back(Coach("Coach 4", 9.5, 100.));
    coaches.push_back(Coach("Coach 5"));

    automobiles.push_back(Automobile("Automobile 1"));
    automobiles.push_back(Automobile("Automobile 2", 90., 270.));
    automobiles.push_back(Automobile("Automobile 3", 120., 290.));
    automobiles.push_back(Automobile("Automobile 4", 150., 300.));

    aeroplanes.push_back(Plane("Aeroplane 1", 1030., 800.));
    aeroplanes.push_back(Plane("Aeroplane 2", 560., 600.));
    aeroplanes.push_back(Plane("Aeroplane 3", 2200., 1000.));

    povozki.push_back(Autopovozka("Autopovozka 1", 40., 150.));
    povozki.push_back(Autopovozka("Autopovozka 2", 45., 160.));
    povozki.push_back(Autopovozka("Autopovozka 3", 50., 180.));
    povozki.push_back(Autopovozka("Autopovozka 4", 70., 200.));

    //Векторы указателей на объекты
    vector<Vehicle*> coachPointers;
    vector<Vehicle*> automobilePointers;
    vector<Vehicle*> aeroplanePointers;
    vector<Vehicle*> autopovozkaPointers;

    //Инициализация векторов указателей
    for (int i = 0; i < coaches.size(); ++i) {
        coachPointers.push_back(&coaches[i]);

    }

    for (int i = 0; i < automobiles.size(); ++i) {
        automobilePointers.push_back(&automobiles[i]);
    }

    for (int i = 0; i < aeroplanes.size(); ++i) {
        aeroplanePointers.push_back(&aeroplanes[i]);
    }

    for (int i = 0; i < povozki.size(); ++i) {
        autopovozkaPointers.push_back(&povozki[i]);
    }

    bool (*Compare) (Vehicle*, Vehicle*);
    Compare = &CompareSpeed;

    vector<Vehicle*> vehiclePointers = coachPointers + automobilePointers + aeroplanePointers + autopovozkaPointers;
    DisplayVehicles(vehiclePointers);
    cout << endl;

    CommitRandomTrips(vehiclePointers);

    cout << "Введите 1 - сортровка по скорости, 2 - сортировка по имени, 3 - сортировка по времени, 4 - сортировка по дистанции: " << endl;
    int x;
    cin >> x;
    switch (x) 
    {
    case 1:
        Compare = &CompareSpeed;
        break;
    case 2:
        Compare = &CompareName;
        break;
    case 3:
        Compare = &CompareTime;
        break;
    case 4:
        Compare = &CompareDist;
        break;
    }
    
    SelectionSort(vehiclePointers, 0, vehiclePointers.size() - 1, Compare);
    DisplayVehicles(vehiclePointers);
    cout << endl;

    return 0;
}
