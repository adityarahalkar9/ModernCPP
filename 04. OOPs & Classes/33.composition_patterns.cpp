import std;
#include "class_header.h"

// Composition over inheritance
// BAD: Inheritance for capability
class BadString : public std::string {  // Inheriting from string
public:
    using std::string::string;

    // Adding functionality
    std::string to_upper() const {
        std::string result = *this;
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }
};

// GOOD: Composition
class GoodString {
    std::string data;  // HAS-A string

public:
    GoodString(std::string s) : data(std::move(s)) {}

    // Delegate to contained object
    size_t size() const { return data.size(); }
    const char* c_str() const { return data.c_str(); }
    const std::string& str() const { return data; }

    // Add functionality
    std::string to_upper() const {
        std::string result = data;
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }
};

void CompositionVsInheritance() {
    std::cout << "=== Composition over Inheritance ===" << std::endl;

    std::cout << "Problem with inheritance:" << std::endl;
    std::cout << "  Tightly coupled to implementation" << std::endl;
    std::cout << "  Breaks if base changes" << std::endl;
    std::cout << "  Can't change at runtime" << std::endl;

    GoodString gs("Hello World");
    std::cout << "\nComposed string: " << gs.c_str() << std::endl;
    std::cout << "Upper: " << gs.to_upper() << std::endl;
    std::cout << "Size: " << gs.size() << std::endl;
}

// Strategy pattern with composition
class SortingStrategy {
public:
    virtual ~SortingStrategy() = default;
    virtual void sort(std::vector<int>& data) const = 0;
    virtual std::string name() const = 0;
};

class QuickSort : public SortingStrategy {
public:
    void sort(std::vector<int>& data) const override {
        std::sort(data.begin(), data.end());
        std::cout << "  QuickSort applied" << std::endl;
    }

    std::string name() const override { return "QuickSort"; }
};

class BubbleSort : public SortingStrategy {
public:
    void sort(std::vector<int>& data) const override {
        for(size_t i = 0; i < data.size(); ++i) {
            for(size_t j = 0; j + 1 < data.size() - i; ++j) {
                if(data[j] > data[j + 1]) {
                    std::swap(data[j], data[j + 1]);
                }
            }
        }
        std::cout << "  BubbleSort applied" << std::endl;
    }

    std::string name() const override { return "BubbleSort"; }
};

class Sorter {
    std::unique_ptr<SortingStrategy> strategy;  // HAS-A strategy

public:
    explicit Sorter(std::unique_ptr<SortingStrategy> s)
        : strategy(std::move(s)) {
    }

    void set_strategy(std::unique_ptr<SortingStrategy> s) {
        strategy = std::move(s);
    }

    void sort(std::vector<int>& data) {
        std::cout << "  Using " << strategy->name() << std::endl;
        strategy->sort(data);
    }
};

void StrategyPattern() {
    std::cout << "\n=== Strategy Pattern (Composition) ===" << std::endl;

    std::cout << "Swap strategies at runtime:" << std::endl;

    Sorter sorter(std::make_unique<QuickSort>());

    std::vector<int> data1 = {5, 2, 8, 1, 9};
    sorter.sort(data1);

    std::cout << "\nChanging strategy:" << std::endl;
    sorter.set_strategy(std::make_unique<BubbleSort>());

    std::vector<int> data2 = {3, 1, 4, 1, 5};
    sorter.sort(data2);
}

// Decorator pattern
class Coffee {
public:
    virtual ~Coffee() = default;
    virtual double cost() const = 0;
    virtual std::string description() const = 0;
};

class SimpleCoffee : public Coffee {
public:
    double cost() const override { return 2.0; }
    std::string description() const override { return "Simple coffee"; }
};

class CoffeeDecorator : public Coffee {
protected:
    std::unique_ptr<Coffee> coffee;  // HAS-A coffee

public:
    explicit CoffeeDecorator(std::unique_ptr<Coffee> c)
        : coffee(std::move(c)) {
    }
};

class MilkDecorator : public CoffeeDecorator {
public:
    using CoffeeDecorator::CoffeeDecorator;

    double cost() const override {
        return coffee->cost() + 0.5;
    }

    std::string description() const override {
        return coffee->description() + " + milk";
    }
};

class SugarDecorator : public CoffeeDecorator {
public:
    using CoffeeDecorator::CoffeeDecorator;

    double cost() const override {
        return coffee->cost() + 0.25;
    }

    std::string description() const override {
        return coffee->description() + " + sugar";
    }
};

void DecoratorPattern() {
    std::cout << "\n=== Decorator Pattern ===" << std::endl;

    std::cout << "Add responsibilities dynamically:" << std::endl;

    auto coffee = std::make_unique<SimpleCoffee>();
    std::cout << "Base: " << coffee->description()
        << " $" << coffee->cost() << std::endl;

    auto milk_coffee = std::make_unique<MilkDecorator>(std::move(coffee));
    std::cout << "With milk: " << milk_coffee->description()
        << " $" << milk_coffee->cost() << std::endl;

    auto sweet_milk_coffee = std::make_unique<SugarDecorator>(std::move(milk_coffee));
    std::cout << "With sugar: " << sweet_milk_coffee->description()
        << " $" << sweet_milk_coffee->cost() << std::endl;
}

// Observer pattern
class Observer {
public:
    virtual ~Observer() = default;
    virtual void on_event(const std::string& event) = 0;
};

class Subject {
    std::vector<Observer*> observers;

public:
    void attach(Observer* obs) {
        observers.push_back(obs);
    }

    void detach(Observer* obs) {
        observers.erase(
            std::remove(observers.begin(), observers.end(), obs),
            observers.end()
        );
    }

    void notify(const std::string& event) {
        for(Observer* obs : observers) {
            obs->on_event(event);
        }
    }
};

class LoggingObserver : public Observer {
public:
    void on_event(const std::string& event) override {
        std::cout << "  [Log] " << event << std::endl;
    }
};

class EmailObserver : public Observer {
public:
    void on_event(const std::string& event) override {
        std::cout << "  [Email] Sending notification: " << event << std::endl;
    }
};

void ObserverPattern() {
    std::cout << "\n=== Observer Pattern ===" << std::endl;

    std::cout << "One-to-many dependency:" << std::endl;

    Subject subject;
    LoggingObserver logger;
    EmailObserver emailer;

    subject.attach(&logger);
    subject.attach(&emailer);

    subject.notify("user_registered");
    subject.notify("payment_received");
}