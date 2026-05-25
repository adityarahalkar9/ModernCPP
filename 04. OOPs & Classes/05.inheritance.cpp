#include <iostream>
#include "class_header.h"

/*
	Inheritance is a fundamental concept in OOP (Object Oriented Programming). It is the
	mechanism by which one class is allowed to inherit the features (fields and methods)
	of another class. Inheritance means creating new classes based on existing ones. A
	class that inherits from another class can reuse the methods and fields of that class.

	When a class inherits another class, it gets all the accessible members of the parent class,
	and the child class can also redefine (override) or add new functionality to them.
*/
class Animal{		// Animal is a Base class
public:
	void Sound(){
		std::cout << "Animal makes a sound" << std::endl;
	}
};
class Dog : public Animal{		// Dog is a derived/sub class
public:
	void Sound(){
		std::cout << "Dog barks" << std::endl;
	}
};
class Cat : public Animal{
public:
	void sound(){
		std::cout << "Cat meows" << std::endl;
	}
};
class Cow : public Animal{
public:
	void sound(){
		std::cout << "Cow moos" << std::endl;
	}
};
void BasicInheritance(){
	Dog d;
	d.Sound();

	Cat c;
	c.Sound();
}



int Inheritance(){
	BasicInheritance();

	return 0;
}