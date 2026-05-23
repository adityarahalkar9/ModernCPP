#include <iostream>
#include <vector>
#include <algorithm>
#include "memory_header.h"

/*
	std::vector is a dynamic array provided by the C++ standard library. It handles memory management
	for you, grows dynamically and offers rich interface for element access and modification.
	When to use std::vector:
	  -> When you need dynamic resizing
	  -> When handling large datasets when stack memory is insufficient
	  -> When passing arrays between functions without worrying about size constraints

*/


// Vector basics
int VectorDeclarationAndInitialisation(){
	// No initialisation i.e. empty vector
	std::vector<int> empty_vector{};

	// Zero initialisation i.e explicitly listing zero
	std::vector<int> zero_initialised{0,0,0,0,0};

	// Regular initialisation with values
	std::vector<int> numbers{1,2,3,4,5};
	std::vector<std::string> words{"Hello", "World", "C++23"};

	// Printing words array
	std::cout << "Declaration & Initialisation: " << std::endl;
	for(const std::string& word : words){
		std::cout << word << " ";
	}
	std::cout << "\n" << std::endl;

	// Printing numbers array
	std::cout << "Numbers are" << std::endl;
	for(const int num : numbers){
		std::cout << num << " ";
	}
	std::cout << "\n" << std::endl;

	// Printing zero_inititialised array
	std::cout << "Zero initiialised (Vector with 5 zeros)" << std::endl;
	for(const int num : zero_initialised){
		std::cout << num << " ";
	}
	std::cout << "\n" << std::endl;

	return 0;
}



/*
	Vector modification i.e. adding elements using push_back() and emplace_back()
		- push_back() function adds a copy of existing object to the vector.
		- emplace_back() function directly constructs the object at the end of vector.

	Both functions add elements at the end of the vector.
		=> Use push_back() when you have an existing object that you want to add to the vector.
		=> Use emplace_back() when you want to construct a new object directly into the vector especially,
		   if the object construction is expensive or it has no default constructor.
		- emplace_back() can offer performance advantages by constructing the object directly within the
		  vector's storage
*/
int VectorModification(){
	std::vector<int> numbers{1,2,3};

	std::cout << "Initial Vector" << std::endl;
	for(const int num : numbers){
		std::cout << num << " ";
	}
	std::cout << "\n" << std::endl;

	numbers.push_back(4);
	numbers.emplace_back(5);

	std::cout << "Vector after modification" << std::endl;
	for(const int num : numbers){
		std::cout << num << " ";
	}
	std::cout << "\n" << std::endl;

	return 0;
}



// Removing elements using pop_back() and erase()
int VectorRemoval(){
	std::vector<int> numbers{10,20,30,40,50,60,70};

	// Removing last element
	numbers.pop_back();

	std::cout << "numbers array after using pop_back() is " << std::endl;
	for(const int num : numbers){
		std::cout << num << " ";
	}
	std::cout << "\n" << std::endl;

	// Using erase()
	numbers.erase(numbers.begin());

	std::cout << "numbers array after using erase() is " << std::endl;
	for(const int num : numbers){
		std::cout << num << " ";
	}
	std::cout << "\n" << std::endl;

	// Erasing 2nd number
	std::cout << "using erase() for removing 2nd number " << std::endl;
	numbers.erase(numbers.begin() + 1);
	for(const int num : numbers){
		std::cout << num << " ";
	}
	std::cout << "\n" << std::endl;

	return 0;
}



// Accessing vector elements
int VectorAccess(){
	std::vector<int> numbers{100,200,300,400,500};
	std::cout << "Element access" << std::endl;
	std::cout << "First element is: " << numbers.front() << std::endl;		// Using numbers.front()
	std::cout << "Second element: " << numbers[1] << std::endl;
	std::cout << "Third element: " << numbers[2] << std::endl;
	std::cout << "Last element is: " << numbers.back() << std::endl;		// Using numbers.back()

	return 0;
}


// Reserving space and using shrink_to_fit
int VectorCapacityManagement(){
	// Initialise an empty vector
	std::vector<int> numbers{};
	std::cout << "Capacity management: " << "\n" << std::endl;

	// Display initial capacity of vector
	std::cout << "Initial capacity: " << numbers.capacity() << std::endl;

	// Reserve space for at least 100 elements
	numbers.reserve(100);
	std::cout << "Capacity after reserve(100): " << numbers.capacity() << std::endl;

	// Add 50 elements to the vector
	for(int i = 0; i < 50; ++i){
		numbers.push_back(i);
	}
	std::cout << "Vector after adding 50 elements is: " << std::endl;
	for(int num : numbers){
		std::cout << num << " ";
	}
	std::cout << std::endl;

	// Display the current size of the vector
	std::cout << "Size after rusing push_back(): " << numbers.size() << std::endl;

	// Display the capacity of the vector
	std::cout << "Capacity after using push_back(): " << numbers.capacity() << std::endl;

	// Reduce the capacity to fit the current size
	numbers.shrink_to_fit();
	std::cout << "Capacity after using shrink_to_fit() is: " << numbers.capacity() << std::endl;

	return 0;

	/*
		This fucntions shows us workings of
		- capacity()
		- reserve()
		- push_back()
		- size()
		- shrink_to_fit()
	*/
}



// Vector iteration and standard algorithm sorting using std::sort
int VectorSort(){
	std::vector<int> numbers{1,5,4,6,9,8};
	std::cout << "Initial order of array " << std::endl;
	for(int num : numbers){
		std::cout << num << " ";
	}
	std::cout << std::endl;

	// Using std::sort
	std::sort(numbers.begin(), numbers.end());
	std::cout << "Order of elements after using std::sort() " << std::endl;
	for(int num : numbers){
		std::cout << num << " ";
	}

	return 0;
}



// Vector matrix
int VectorMatrix(){
	int rows{}, cols{};
	std::cout << "Enter number of rows: ";
	std::cin >> rows;
	std::cout << "Enter number of columns: ";
	std::cin >> cols;
	std::cout << std::endl;

	// Create a matrix using comventional constructor syntax
	std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols, 0));

	// Input matrix elements from user
	for(int i = 0; i < rows; ++i){
		for(int j = 0; j < cols; ++j){
			std::cout << "Enter element [" << i + 1 << "][" << j + 1 << "]: ";
			std::cin >> matrix[i][j];
		}
	}

	// Output the matrix
	std::cout << "\nThe matrix is: \n";
	for(int i = 0; i < rows; ++i){
		for(int j = 0; j < cols; ++j){
			std::cout << matrix[i][j] << " ";
		}
		std::cout << "\n";
	}

	return 0;
}

int Vector(){
	// VectorDeclarationAndInitialisation();
	// VectorModification();
	// VectorRemoval();
	// VectorAccess();
	// VectorCapacityManagement();
	// VectorSort();
	// VectorMatrix();

	return 0;
}