#include <iostream>
#include <string>
#include "struct_header.h"

// Array structures
struct Student{
	std::string name{};
	int age{};
	double gpa{};
};
int ArrayStructure(){
	Student students[3]{
		{"Aditya", 26, 7.31},
		{"Bob", 27, 6.78},
		{"Charlie, 28, 9.85"}
	};
	for(int i = 0; i < 3; ++i){
		std::cout << "Name: " << students[i].name << ", Age: " << students[i].age
			<< ", gpa: " << students[i].gpa << std::endl;
	}

	return 0;
}
/*
	This code demonstrates how to create an array where each element is a student structure. We declare
	an array of structs named students of size 3 and initialise each element using brace initialisation
	with the name, age and GPA of student. We can iterate through the array and print the information
	for each student.
*/



// Nested strcuture
struct Date{
	int year{};
	int month{};
	int date{};
};
struct Employee{
	std::string name{};
	Date birthDate{};
};
int NestedStructure(){
	Employee emp1{"Adi",{2000, 1, 9}};
	std::cout << "Employee name: " << emp1.name << std::endl;
	std::cout << "Employee birthdate: " << emp1.birthDate.date << "-" << emp1.birthDate.month
		<< "-" << emp1.birthDate.year << std::endl;

	return 0;
}



// Pointers to structures
struct Book{
	std::string title{};
	std::string author{};
};
int PointerToStructure(){
	Book book1{"Hitchhiker's guide", "Douglas adams"};

	// Declaring a pointer to a book structure
	Book* bookptr = &book1;

	// Accessing members using arrow operator
	std::cout << "Book Title: " << bookptr->title << std::endl;
	std::cout << "Book Author: " << bookptr->author << std::endl;

	// Dereferencing the pointer using dot operator
	std::cout << "Book title(dereferenced): " << (*bookptr).title << std::endl;
	std::cout << "Book Author(dereferenced): " << (*bookptr).author << std::endl;

	return 0;
}
/*
	This example shows how to work with pointers to structures.
	Book* bookptr = &book1; declares a pointer bookptr that stores the memory address of the book1
	structure. To access the members of the structure through the pointer, we use the arrow operator(->).
	The bookptr->title is equal to (*bookptr).title, where (*bookptr) dereferences the pointer to get
	the Book structure itself.
*/



// Structures and Classes
// All members in struct are public by default
struct DataStruct{
	int value{};
	void printValue(){
		std::cout << "Value in struct: " << value << std::endl;

	}
};
// All members in classes are private by default
class DataClass{
public:
	int value{};
	void printValue(){
		std::cout << "Value in class: " << value << std::endl;
	}
};
int StructClass(){
	DataStruct ds{100};
	ds.printValue();	// Accessible directly

	DataClass dc{200};
	dc.printValue();	// Accessible as it is in public section

	return 0;
}
/*
	This highlights a key difference in default access specifiers between structs and classes. In struct, members
	are public by default, meaning they can be accessed from anywhere. In class, members are private by default
	so you need to explicitly declare them as public to access from outside the class.
*/



// Bitfields in structures
struct StatusFlags{
	unsigned int error : 1{};	 // 1 bit for error flag initialised to zero
	unsigned int warning : 1{};  // 1 bit warning flag
	unsigned int enabled : 1{};	 // 1 bit for enabled flag
	unsigned int reserved : 5{}; // 5 reserved bits
};
int StructBitfield(){
	StatusFlags flags{};
	flags.error = 1;
	flags.warning = 0;
	flags.enabled = 1;
	std::cout << "Error: " << flags.error << std::endl;
	std::cout << "Warning: " << flags.warning << std::endl;
	std::cout << "Enabled: " << flags.enabled << std::endl;
	std::cout << "Size of StatusFlages: " << sizeof(StatusFlags) << std::endl;

	return 0;
}
/*
	Bitfields allow you to control the number of bits allocated to a member of a structure. This is useful
	for saving memory when you know that a variable will only hold a small range of values. Here error,
	warning and enabled are each allocated only 1 bit, reserved is allocated 5 bits. The total size of StatusFlags
	structure might be smaller than if these were regular int or even bool variables.
*/



int IntermediateStruct(){
	// ArrayStructure();
	// NestedStructure();
	// PointerToStructure();
	// StructClass();
	// StructBitfield();

	return 0;
}