#include <iostream>
#include "class_header.h"

/*
	Destructor is an instance member function that is invoked automatically whenever an object
	is going to be destroyed. Meaning, a destructor is the last function that is going to be
	called before an object is destroyed.
*/
class Test{
public:
	Test(){
		std::cout << "Constructor called" << std::endl;
	}
	~Test(){
		std::cout << "Destructor called" << std::endl;
	}
};
void BasicDestructor(){
	Test t;
}


/*
	If we do not write our own destructor in class, the compiler creates a default
	destructor for us. The default destructor works fine unless we have dynamically
	allocated memory or pointer in class. When a class contains a pointer to memory
	allocated in the class, we should write a destructor to release memory before the
	class instance is destroyed. This must be done to avoid memory leaks.
*/
class SimpleHolder{
private:
	int* ptr{};
public:
	// Constructor
	SimpleHolder(int value){
		ptr = new int(value);	// Allocating int on the heap
		std::cout << "Constructor: ptr points to value = " << *ptr << std::endl;
	}

	// Destructor
	~SimpleHolder(){
		std::cout << "Destructor: deleting ptr which points to value = " << *ptr << std::endl;
		delete ptr;
		ptr = nullptr;
		std::cout << "Destructor called, ptr deleted" << std::endl;
	}
	void show() const{
		std::cout << "Value = " << *ptr << std::endl;
	}
};
void ExplicitDestructor(){
	SimpleHolder sh(99);
	sh.show();
	/*
		when explicitDestrcutor() ends (or `sh` goes out of scope), the destructor ~SimpleHolder()
		is automatically called.
	*/
}


// Using destructors with dynamic memory
class Dog{
private:
	std::string m_dog_name{};
	std::string m_dog_breed{};
	int* m_dog_age{};
public:
	void DisplayInfo();
	void SetDogBreed(std::string m_dog_breed){
		this->m_dog_breed = m_dog_breed;
	}
	void SetDogAge(int age){
		*(this->m_dog_age) = age;
	}
	Dog(){
		m_dog_name = "Tommy";
		m_dog_breed = "German Shephard";
		m_dog_age = new int;
		*m_dog_age = 3;
	}
	Dog(std::string name_param, std::string breed_param, int age_param) : m_dog_name(name_param), m_dog_breed(breed_param), m_dog_age(new int(age_param)){
		std::cout << m_dog_name << " Constructed at: " << this << std::endl;
	}
	~Dog(){
		delete m_dog_age;
		std::cout << "Dog destructor called for " << m_dog_name << " at " << this << std::endl;
	}
};
void Dog::DisplayInfo(){
	std::cout << m_dog_name << " " << m_dog_breed << " " << *m_dog_age << "\n";
}
void PointerDestructor(){
	Dog dog("Tim", "pomerian", 4);
	dog.DisplayInfo();

	// Just practicing using pointers
	Dog* dog1 = new Dog();
	dog1->DisplayInfo();
	delete dog1;

	Dog dog2("Fluffy", "Pitbull", 7);
	dog2.DisplayInfo();

	dog2.SetDogAge(12);
	dog2.SetDogBreed("sled");
	dog2.DisplayInfo();
}



int Destructors(){
	// BasicDestructor();
	// ExplicitDestructor();
	// PointerDestructor();

	return 0;
}

/*
	CHARACTERISTICS OF A DESTRUCTOR
	All the points mentioned below show the characteristics of a destructor:
	- Destructor has the same name as their class name preceded by a tilde (~) symbol.
	- It is not possible to define more than one destructor.
	- The destructor is only one way to destroy the object created by the constructor.
	  Hence, destructor cannot be overloaded.
	- It cannot be declared static or const.
	- Destructor neither requires any argument nor returns any value.
	- It is automatically called when an object goes out of scope.
	- Destructor release memory space occupied by the objects created by the constructor.
	- In destructor, objects are destroyed in the reverse of an object creation.

	When is the destructor called?
	A destructor function is called automatically when the object goes out of scope or is
	deleted. Following are the cases where destructor is called:
	- Destructor is called when the function ends.
	- Destructor is called when the program ends.
	- Destructor is called when a block containing local variables ends.
	- Destructor is called when a delete operator is called.
*/