#include <iostream>
#include "class_header.h"

/*
	In C++, constructors are special methods that are automatically called whenever an object of a
	class is created. A constructor is different from normal functions in following ways:
	- A constructor has same name as the class itself
	- Don't have return type
	- Automatically called when an object is created.
	- If we do not specify a constructor, C++ compiler generates a default constructor for us
	  (expects no parameters and has an empty body).
*/
// (Throwing error after compilation)
class Employee{
private:
	int empid{};
	double salary{};
public:
	// This is the Member Initializer List
	Employee() : empid(2334), salary(34800.0){
		std::cout << "Employee created via Initializer List" << std::endl;
		std::cout << "Employee ID: " << empid << " Salary: " << salary << std::endl;
	}
};
void BasicConstructor(){
	Employee e;
}


/*
	Types of Constructors in C++
	Constructors can be classified based on the situations they are being used in.
	There are 4 types of constructors in C++:
	- Default Constructor
	- Parameterized Constructor
	- Copy Constructor
	- Move Constructor
*/
// Default Constructor
class Default{
public:
	void Display(){
		std::cout << "Default Constructor called" << std::endl;
	}
};



int Constructors(){
	BasicConstructor();

	return 0;
}