#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_set>
#include <string>
#include <windows.h>

using namespace std;

class Car {
private:
    string model;
    double fuelConsumption[3];
    int maxSpeed;
    int power;

public:
    Car() : model(""), maxSpeed(0), power(0) {
        fuelConsumption[0] = fuelConsumption[1] = fuelConsumption[2] = 0.0;
    }

    Car(string m, double highway, double city, double mixed, int speed, int p)
            : model(m), maxSpeed(speed), power(p) {
        fuelConsumption[0] = highway;
        fuelConsumption[1] = city;
        fuelConsumption[2] = mixed;
    }

    string getModel() const { return model; }
    double getHighwayConsumption() const { return fuelConsumption[0]; }
    double getCityConsumption() const { return fuelConsumption[1]; }
    double getMixedConsumption() const { return fuelConsumption[2]; }
    int getMaxSpeed() const { return maxSpeed; }
    int getPower() const { return power; }

    bool operator<(const Car& other) const {
        return model < other.model;
    }

    bool operator==(const Car& other) const {
        return model == other.model &&
               fuelConsumption[0] == other.fuelConsumption[0] &&
               fuelConsumption[1] == other.fuelConsumption[1] &&
               fuelConsumption[2] == other.fuelConsumption[2] &&
               maxSpeed == other.maxSpeed &&
               power == other.power;
    }

    friend ostream& operator<<(ostream& out, const Car& car);
};

ostream& operator<<(ostream& out, const Car& car) {
    out << "Модель: " << car.model
        << ", Расход (трасса/город/смешанный): " << car.fuelConsumption[0]
        << "/" << car.fuelConsumption[1] << "/" << car.fuelConsumption[2]
        << " л/100км, Макс. скорость: " << car.maxSpeed << " км/ч"
        << ", Мощность: " << car.power << " л.с.";
    return out;
}

struct CarHasher {
    hash<string> shash;
    hash<double> dhash;
    hash<int> ihash;

    size_t operator()(const Car& car) const {
        const size_t coef = 2946901;

        size_t result = coef * coef * shash(car.getModel());
        result = result * coef + dhash(car.getHighwayConsumption());
        result = result * coef + dhash(car.getCityConsumption());
        result = result * coef + dhash(car.getMixedConsumption());
        result = result * coef + ihash(car.getMaxSpeed());
        result = result * coef + ihash(car.getPower());

        return result;
    }
};

vector<Car> readFromFile(const string& filename) {
    vector<Car> cars;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Не удалось открыть файл " << filename << endl;
        return cars;
    }

    string model;
    double highway, city, mixed;
    int speed, power;

    while (file >> model >> highway >> city >> mixed >> speed >> power) {
        cars.push_back(Car(model, highway, city, mixed, speed, power));
    }

    file.close();
    return cars;
}

template<typename Container>
void printContainer(const Container& cont, const string& title) {
    if (cont.empty()) {
        cout << "Контейнер пуст" << endl;
        return;
    }
    for (const auto& item : cont) {
        cout << item << endl;
    }
    cout << "Всего элементов: " << cont.size() << endl;
}

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    vector<Car> cars = readFromFile("input.txt");

    for (const auto& car : cars) {
        cout << car << endl;
    }

    set<Car> carSet;

    for (const auto& car : cars) {
        carSet.insert(car);
    }

    printContainer(carSet, "КОНТЕЙНЕР set");

    unordered_set<Car, CarHasher> carUnorderedSet;

    for (const auto& car : cars) {
        carUnorderedSet.insert(car);
    }

    printContainer(carUnorderedSet, "КОНТЕЙНЕР unordered_set");

    return 0;
}