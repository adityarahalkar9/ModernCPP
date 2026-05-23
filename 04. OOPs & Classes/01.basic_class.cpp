#include <iostream>
#include "class_header.h"

/*
	A class is a user-defined type that encapsulates data (members) and behavior (functions).
	It's a blueprint for objects. In C++, class and struct are similar; the difference
	is default access: class defaults to private, struct to public. Use class for OOP with
	encapsulation.
*/
class Point{
private:
	int x{};
	int y{};
public:
	void setCordinates(int newX, int newY){
		x = newX;
		y = newY;
	}
	void print() const{		// Const member function: doesn't modify object
		std::cout << "Point is: " << x << ", " << y << std::endl;
	}
};
int BClass(){
	Point p{};
	p.setCordinates(3, 4);
	p.print();

	return 0;
}


// Access modifiers are special keywords in C++, that control the visibility of class members
/*
	- public    : Public specifier makes class members (variables and functions) accessible
				  from anywhere - inside the class, outside the class, or even other files.
	- private   : Private specifier makes class members accessible only inside the class itself
				  (mainly member methods), and is mainly used to hide data from outside so that we can make
				  changes internally later without changing the code of its users. By default, all class
				  members in C++ are private if no specifier is mentioned.
	- protected : The protected specifier makes members accessible inside the class itself and in its
				  derived (child) classes, but not form outside code. It is mainly used in inheritance,
				  allowing child classes to reuse or modify data and functions while still keeping
				  them hidden from the outside world.
*/
class Person{
private:
	int age{};		// Private: only accessible from inside class
public:
	std::string name{};
	void setAge(int a){		// public method: accessible
		age = a;
	}
	int getAge() const{		// public method: accessible
		return age;
	}
protected:
	std::string secretCode{};
};
class Employee : public Person{
public:
	void setSecret(const std::string& code){
		secretCode = code;
	}
	std::string viewSecret() const{
		return secretCode;
	}
};
int AccessModifiers(){
	Person p;
	p.name = "Alice";	     // OK: public member
	// p.age = 30;		     // Error: as age is private -> not accessible here
	p.setAge(30);		     // OK: using public method

	Employee e;
	e.name = "Bob";		     // OK: public
	e.setAge(40);			 // OK: via public method
	e.setSecret("X12345");   // OK: derived class method sets protected member

	return 0;
}



int BasicClass(){
	// BClass();
	// AccessModifiers();

	return 0;
}