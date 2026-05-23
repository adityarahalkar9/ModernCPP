#include <iostream>
#include "basic_header.h"

/*
	Arrays in C++ are a fundamental data structure used to store multiple items of
	the same type in a contiguous block of memory.
*/
int Array(){
	int arr[5];						// Uninitialised array
	int balance[10]{};				// Zero initialised array of size 10
	int example[5] = {1,2,3,4,5};	// Initialised array
	int ID[] = {1,2,3,4,5};			// Size inferred from initialiser
	int Partial[3] = {1,2};			// Partial array

	std::cout << "Balance array is declared with 10 elements as zero" << std::endl;
	for(int i = 0; i < std::size(balance); ++i){
		std::cout << balance[i] << " ";
	}
	std::cout << "\n" << std::endl;

	std::cout << "Example array is inititalized with 5 elements and assigned values" << std::endl;
	for(int i = 0; i < std::size(example); ++i) {
		std::cout << example[i] << " ";
	}
	std::cout << "\n" << std::endl;

	std::cout << "ID Array is initialised with values without declaring its size" << std::endl;
	for(int i = 0; i < std::size(ID); ++i) {
		std::cout << ID[i] << " ";
	}
	std::cout << "\n" << std::endl;

	std::cout << "Partial array is given 3 size with 2 elements" << std::endl;
	for(int i = 0; i < std::size(Partial); ++i) {
		std::cout << Partial[i] << " ";
	}
	std::cout << "\n" << std::endl;

	return 0;
}



//	2-Dimensional arrays
int TwoDimensionalArrays(){
	// Declare and inititialise a 2d array (matrix[rows][columns])
	int matrix[3][4] = {
		{1,2,3,4},
		{5,6,7,8},
		{9,10,11,12}
	};

	// Accessing and printing a speific element of an array
	std::cout << "matrix[1][2] is: " << matrix[1][2] << std::endl;	// Index starts from 0(Remember)

	// Printing all elements of the matrix
	for(int i = 0; i < 3; ++i){
		for(int j = 0; j < 4; ++j){
			std::cout << "matrix [" << i << "][" << j << "] = " << matrix[i][j] << std::endl;
		}
	}

	// Modify an element
	matrix[2][3] = 99;
	std::cout << "After change, matrix[2][3] = " << matrix[2][3] << std::endl;

	// Traverse and print all elements row‑by‑row (cache‑friendly)
	for(int i = 0; i < 3; ++i){
		for(int j = 0; j < 4; ++j){
			std::cout << matrix[i][j] << "\t";
		}
		std::cout << "\n";	//Simply add a newline after the inner loop completes, so each row ends cleanly :
	}

	return 0;
}



// 3-Dimensional arrays
int ThreeDimensionalArray(){
	// Declare and initialise a 3D array: depth=2, rows=3, columns=2
	int cube[2][3][2] = {
		{ {1,  2}, {3,  4}, {5,  6} },
		{ {7,  8}, {9, 10}, {11, 12} }
	};

	// Traversing cube
	std::cout << "Traversing cube using int indices:\n";
	for(int d = 0; d < 2; ++d) {					// loop over depth
		std::cout << "Layer " << d << ":\n";
		for(int r = 0; r < 3; ++r) {				// loop over rows
			for(int c = 0; c < 2; ++c) {			// loop over columns
				std::cout << cube[d][r][c] << "\t";
			}
			std::cout << "\n";				// newline after each row
		}
		std::cout << "\n";					// extra newline between layers
	}

	// Compute total bytes and total elements
	std::size_t total_bytes = sizeof(cube);
	std::size_t total_elems = total_bytes / sizeof(cube[0][0][0]);
	std::cout << "Cube stores " << total_elems
		<< " elements using " << total_bytes
		<< " bytes.\n";

	return 0;
}



int Arrays(){
	// Array();
	// TwoDimensionalArrays();
	// ThreeDimensionalArray();

	return 0;
}