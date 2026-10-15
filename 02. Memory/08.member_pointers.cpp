#include <iostream>
#include <string>
#include <vector>
#include "memory_header.h"

// Basic member pointer syntax
struct Person {
    std::string name;
    int age;
    double height;

    void Introduce() const {
        std::cout << "Hi, I'm " << name << ", " << age << " years old" << std::endl;
    }

    void Birthday() {
        ++age;
        std::cout << name << " had a birthday!" << std::endl;
    }
};

void DataMemberPointers() {
    std::cout << "=== Data Member Pointers ===" << std::endl;

    Person alice{"Alice", 30, 1.75};
    Person bob{"Bob", 25, 1.80};

    // Syntax: Type Class::*pointer = &Class::member;
    std::string Person::* namePtr = &Person::name;
    int Person::* agePtr = &Person::age;

    // Access: object.*pointer or objectPtr->*pointer
    std::cout << "alice.*namePtr = " << alice.*namePtr << std::endl;
    std::cout << "bob.*agePtr = " << bob.*agePtr << std::endl;

    // Through pointer to object
    Person* alicePtr = &alice;
    std::cout << "alicePtr->*namePtr = " << alicePtr->*namePtr << std::endl;

    // Modify through member pointer
    alice.*agePtr = 31;
    std::cout << "After modification: alice.age = " << alice.age << std::endl;
}

void MemberFunctionPointers() {
    std::cout << "\n=== Member Function Pointers ===" << std::endl;

    Person alice{"Alice", 30, 1.75};

    // Syntax: ReturnType (Class::*pointer)(Params) = &Class::method;
    void (Person:: * introPtr)() const = &Person::Introduce;
    void (Person:: * birthdayPtr)() = &Person::Birthday;

    // Call: (object.*pointer)(args)
    (alice.*introPtr)();
    (alice.*birthdayPtr)();
    (alice.*introPtr)();  // Age is now 31

    // Through object pointer
    Person* p = &alice;
    (p->*introPtr)();
}

// Member pointers in containers
void MemberPointersInContainers() {
    std::cout << "\n=== Member Pointers with Containers ===" << std::endl;

    std::vector<Person> people = {
        {"Alice", 30, 1.75},
        {"Bob", 25, 1.80},
        {"Charlie", 35, 1.70}
    };

    // Sort by different criteria using member pointers
    int Person::* agePtr = &Person::age;
    double Person::* heightPtr = &Person::height;

    std::sort(people.begin(), people.end(),
              [agePtr](const Person& a, const Person& b) {
                  return a.*agePtr < b.*agePtr;
              });

    std::cout << "Sorted by age:" << std::endl;
    for(const auto& p : people) {
        std::cout << "  " << p.name << ": " << p.age << std::endl;
    }

    std::sort(people.begin(), people.end(),
              [heightPtr](const Person& a, const Person& b) {
                  return a.*heightPtr > b.*heightPtr;  // Tallest first
              });

    std::cout << "Sorted by height (descending):" << std::endl;
    for(const auto& p : people) {
        std::cout << "  " << p.name << ": " << p.height << "m" << std::endl;
    }
}

// Generic member access
template <typename T, typename MemberPtr>
auto GetValue(const T& obj, MemberPtr ptr) -> decltype(obj.*ptr) {
    return obj.*ptr;
}

void GenericMemberAccess() {
    std::cout << "\n=== Generic Member Access ===" << std::endl;

    Person alice{"Alice", 30, 1.75};

    auto name = GetValue(alice, &Person::name);
    auto age = GetValue(alice, &Person::age);
    auto height = GetValue(alice, &Person::height);

    std::cout << "Name: " << name << std::endl;
    std::cout << "Age: " << age << std::endl;
    std::cout << "Height: " << height << std::endl;
}

// Practical use: selecting members dynamically
void DynamicMemberSelection() {
    std::cout << "\n=== Dynamic Member Selection ===" << std::endl;

    std::vector<Person> people = {
        {"Alice", 30, 1.75},
        {"Bob", 25, 1.80},
        {"Charlie", 35, 1.70}
    };

    // Choose which member to average
    int Person::* selector = &Person::age;

    double sum = 0;
    for(const auto& p : people) {
        sum += p.*selector;
    }
    std::cout << "Average age: " << sum / people.size() << std::endl;

    // Switch to height
    selector = nullptr;
    double Person::* heightSelector = &Person::height;
    sum = 0;
    for(const auto& p : people) {
        sum += p.*heightSelector;
    }
    std::cout << "Average height: " << sum / people.size() << std::endl;
}

// Member pointers and inheritance
struct Employee : Person {
    double salary;
    std::string department;
};

void MemberPointersWithInheritance() {
    std::cout << "\n=== Member Pointers with Inheritance ===" << std::endl;

    Employee emp{"Dave", 40, 1.85, 75000.0, "Engineering"};

    // Can access base class members
    std::string Person::* namePtr = &Person::name;
    std::cout << "Name via base pointer: " << emp.*namePtr << std::endl;

    // Or derived class members
    double Employee::* salaryPtr = &Employee::salary;
    std::cout << "Salary: " << emp.*salaryPtr << std::endl;

    // Note: Member pointers respect access rules
    // Private members can't have pointers created from outside
}