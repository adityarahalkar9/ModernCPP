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


/*
	Types of Inheritance in C++
	1. Single Inheritance
	2. Multiple Inheritance
	3. Multilevel Inheritance
	4. Hierarchical Inheritance
	5. Hybrid inheritance

*/



// SINGLE INHERITANCE
/*
	In single inheritance, a sub-class is derived from only one super class. It inherits
	the properties and behavior of a single-parent class. Sometimes, it is also known as
	simple inheritance.
*/
class Vehicle{		// Base class
protected:
	std::string brand{};
	int year{};
public:
	Vehicle(std::string b, int y) : brand(b), year(y){}
	void DisplayInfo(){
		std::cout << "Brand is: " << brand << ", year " << year << std::endl;
	}
};
class Car : public Vehicle{
private:
	std::string model{};
	int doors{};
public:
	Car(std::string cb, int cy, std::string m, int d) : Vehicle(cb, cy), model(m), doors(d){}
	void DisplayCarInfo(){
		DisplayInfo();
		std::cout << "Model: " << model << " , Doors: " << doors << std::endl;
	}
};
int SingleInheritance(){
	Car myCar("Toyota", 2023, "Camry", 4);
	myCar.DisplayCarInfo();

	return 0;
}
/*
	- Vehicle is the base class with attributes brand and year, and a method displayInfo().
	- Car is the derived class that inherits from Vehicle using public inheritance.
	- Car adds its own attributes (model, doors) and a method displayCarInfo() that uses the base
	  class's displayInfo() method.
	- The protected access specifier allows the derived class to access the base class's members.
	In singleInheritance(), a Car object is created and its details are displayed.
*/



// MULTIPLE INHERITANCE
/*
	In Multiple inheritance, one class can have more than one superclass and inherit features
	from all parent classes.
*/
// Base class 1
class LandVehicle{
protected:
	std::string brand{};
	int max_land_speed{};
public:
	LandVehicle(std::string b, int speed) : brand(b), max_land_speed(speed){}
	void DisplayLandInfo(){
		std::cout << "Brand: " << brand << ", Max Land speed: " << max_land_speed << std::endl;
	}
};
// Base class 2
class WaterVehicle{
protected:
	std::string model{};
	int max_water_speed{};
public:
	WaterVehicle(std::string m, int speed) : model(m), max_water_speed(speed){}
	void DisplayWaterInfo(){
		std::cout << "Model: " << model << ", Max water speed: " << max_water_speed << std::endl;
	}
};
// Derived class from both LandVehicle and WaterVehicle
class AmphibiousVehicle : public WaterVehicle, public LandVehicle{
private:
	std::string vehicle_name{};
public:
	AmphibiousVehicle(std::string b, int landSpeed, std::string m, int waterSpeed, std::string name) :
		LandVehicle(b, landSpeed), WaterVehicle(m, waterSpeed), vehicle_name(name){
	}
	void DisplayInfo(){
		std::cout << "Amphibious Vehicle Display" << std::endl;
		DisplayLandInfo();
		DisplayWaterInfo();
	}
};
int MultipleInheritance(){
	AmphibiousVehicle amp("KSI Technologies", 135, "A12", 45, "Galvatron");
	amp.DisplayInfo();

	return 0;
}



// MULTILEVEL INHERITANCE
/*
	 A class inherits from a class that itself inherits from another class.
	 The chain: Base -> Intermediate -> Derived. Each derived class gets all
	 members of its parent(s) plus its own
*/
// Base class Person
namespace multilevel{
	class Person{
	protected:
		// Protected members can be accessed by derived classes
		std::string name{};
		int age{};
	public:
		Person(std::string n, int a) : name(n), age(a){
			std::cout << "[Person] created: " << name << ", age: " << age << std::endl;
		}
		void Introduce() const{
			std::cout << "Hello, I'm " << name << " and I'm " << age << " years old" << std::endl;
		}
	};
	// derived class 1
	class Student : public Person{
	protected:
		std::string school{};
		int grade_level{};
	public:
		Student(std::string n1, int a1, std::string sch, int grade) : Person{n1, a1}, school{sch}, grade_level{grade}{
			std::cout << "[Student] Created: School = " << school << ", Grade " << grade_level << std::endl;;
		}
		void Study() const{
			std::cout << name << " is studying at " << school << " (Grade " << grade_level << ")." << std::endl;
		}
	};
	// Derived class 2
	class GraduateStudent : public Student{
	private:
		std::string thesis_topic{};
		std::string advisor{};
	public:
		GraduateStudent(std::string n, int a, std::string sch, int grade, std::string topic, std::string adv) :
			Student{n, a, sch, grade}, thesis_topic{topic}, advisor{adv}{
			std::cout << "[GraduateStudent] Created: Thesis = \"" << thesis_topic << "\", Advisor = " << advisor << std::endl;
		}
		void Research() const{
			std::cout << name << " is researching \"" << thesis_topic << "\" under advisor " << advisor << std::endl;
		}
		// Show everything accumulated from all three levels
		void DisplayFullProfile() const {
			std::cout << "\n========== FULL PROFILE ==========\n";
			std::cout << "Name:        " << name << "  (from Person)\n";
			std::cout << "Age:         " << age << "  (from Person)\n";
			std::cout << "School:      " << school << "  (from Student)\n";
			std::cout << "Grade Level: " << grade_level << "  (from Student)\n";
			std::cout << "Thesis:      " << thesis_topic << "  (from GraduateStudent)\n";
			std::cout << "Advisor:     " << advisor << "  (from GraduateStudent)\n";
			std::cout << "===================================\n";
		}
	};
}
int MultilevelInheritance(){
	multilevel::GraduateStudent grad{
		"Alice Smith",          // name
		24,                     // age
		"MIT",                  // school
		5,                      // grade level (PhD student)
		"Quantum Algorithms",   // thesis topic
		"Dr. Robert Chen"       // advisor
	};
	std::cout << "\n--- Calling methods from different levels ---\n";
	grad.Introduce();			// From Person
	grad.Study();				// From Student
	grad.Research();			// From GraduateStudent
	grad.DisplayFullProfile();	// Combines all levels

	return 0;
}



int Inheritance(){
	// BasicInheritance();
	// SingleInheritance();
	// MultipleInheritance();
	// MultilevelInheritance();

	return 0;
}