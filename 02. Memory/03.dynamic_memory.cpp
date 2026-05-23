#include <iostream>
#include <array>
#include "memory_header.h"

/*
	Dynamic memory allocation is the process of allocating memory at the runtime of a program.
	This memory is allocated in the heap of the program instead of stack memory.
	It is useful when:
	=> You are not sure about the size of the array you need
	=> Implementing data structures such as linked lists, trees etc
	=> In complex programs that requires efficient memory management

	- The new operator
	  The new operator is used to allocate memory dynamically. It returns a pointer
	  to the allocated memory.
	  Syntax:
		pointer = new data_type;	// For a single variable
		pointer = new data_type;	// For array
	- The delete operator
	  The delete operator is used to deallocate memory that was previously used by new
	  Syntax:
		delete pointer;      // For single object
		delete[] pointer;    // For arrays of object

	- Memory is a situation where memory allocated for a particular task remains allocated even
	  after it is no longer needed. Morever, if the address to the memory is lost, then it will
	  remain allocated till the program runs. Hence use delete appropriately and use smart pointers
	  whenever possible

	- Dangling pointers are created when memory pointed by the pointer is accessed after it is
	  deallocated leading to undefined behaviour like crashes, garbage. Hence initialise pointer with
	  nullptr and assign nullptr again after dealloacted
*/

void BasicDynamicAllocation(){
	int* p_num = new int;
	*p_num = 34;
	std::cout << "*p_num is: " << *p_num << "\n";

	delete p_num;	  // Delete after use
	p_num = nullptr;  // Set it to null for safety

	// Can reuse pointer. (Valid code)
	int num{89};
	p_num = new int(num);

	// Delete after use
	delete p_num;
	// delete p_num  {Don't delete memory twice}
	p_num = nullptr;

}

void SingleVariableExampleOne(){
	int* p{};		// Pointer initialisation to null
	p = new int;    // Requesting memory for variable using new keyword
	if(!p){
		std::cout << "Allocation failed" << std::endl;
	}
	else{
		// Store value at allocated address
		*p = 25;
		std::cout << "Value of p is: " << *p << std::endl;
	}
	delete p;		// Freeing the allocated memory
	p = nullptr;	// Avoid dangling pointer
}


void SingleVariableExampleTwo(){
	// Declaring variable ptr, requesting memory for it and storing value 56
	int* ptr = new int{56};
	std::cout << "Value of ptr is: " << *ptr << std::endl;
	delete ptr;		// Freeing allocated memory
	ptr = nullptr;	// Avoid dangling pointer
}


void SingleVariableExampleThree(){
	// Declaring variable ptr, requesting memory and initialising to null
	int* ptr = new int{};
	std::cout << "Value of ptr is: " << *ptr << std::endl;
	delete ptr;		// Freeing allocated memory
	ptr = nullptr;	// Avoid dangling pointer
}


// Memory Leak: When we lose access to memory that is dynamically allocated
void MemoryLeak(){
	/*
		====> 1st scenario
		int* p_number{new int{67}};    // Pointes to some address, lets call it address1

		//Should delete and reset here
		delete p_number;
		p_number = nullptr;

		int number{55};     // Lives at address2
		p_number = &number;	// Now p_number points to address 2, but address1 is still in use by
							   our program. But our program has lost access to that memory location
							   Memory has been leaked

		====> 2nd scenario
		int function(){
			{
				int* p_number2{new int{57}};
			}
			// p_number2 itself is a pointer nested in a scope	When the scope ends, p_number2
			   will die but the new int dynamic memory is lost forever. Memory leak has now happened.

			return 0;
		}
	*/
}


void ArrayDynamicMemory(){
	// Request a block of memory of size n
	int n{5};
	int* arr = new int[n];
	if(!arr){
		std::cout << "Memory allocation failed" << std::endl;
	}
	else{
		for(int i = 0; i < n; ++i){
			arr[i] = i + 1;
		}
		std::cout << "Values in the block are" << std::endl;
		for(int i = 0; i < n; ++i){
			std::cout << "arr[" << i << "] = " << arr[i] << " " << std::endl;
		}
	}
	delete[] arr;	// Freeing allocated memory
	arr = nullptr;	// Avoid dangling pointer
}

// Using new and delete with single objects
class MyClass{
public:
	MyClass(){
		std::cout << "Constructor called" << std::endl;
	}
	~MyClass(){
		std::cout << "Destructor called" << std::endl;
	}
};
void SingleObject(){
	// Allocating memory for single object of type MyClass
	MyClass* obj = new MyClass();

	// Deleting the object to call destructor
	delete obj;
}


int DanglingPointer(){
	int* ptr = new int{20};		// Allocating memory and assigning 20
	std::cout << "Value before delete: " << *ptr << std::endl;

	delete ptr;					// Deleting the allocated memory
	ptr = nullptr;				// Setting pointer to nullptr to avoid dangling pointer

	// Now ptr is nullptr, so any dereferencing will be avoided
	if(ptr == nullptr){
		std::cout << "Pointer is null and safe to use " << std::endl;
	}

	return 0;
}



int Dynamic(){
	// SingleVariableExampleOne();
	// SingleVariableExampleTwo();
	// SingleVariableExampleThree();
	// ArrayDynamicMemory();
	// SingleObject();
	// DanglingPointer();
	// BasicDynamicAllocation();

	return 0;
}