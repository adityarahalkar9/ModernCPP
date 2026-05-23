#include <iostream>;
#include "basic_header.h"

/*
	Variable is a name given to a memory location. It is the basic unit of storage in a
	program. The value stored in a variable can be accessed or changed during program execution.
	=> In C++, there are different types of variables (defined with different keywords)
		-> int - stores integers (whole numbers), without decimals, such as 123 or -123
		-> double - stores floating point numbers, with decimals, such as 19.99 or -19.99
		-> char - stores single characters, such as 'a' or 'B'. Char values are surrounded by
		   single quotes
		-> string - stores text, such as "Hello World". String values are surrounded by double quotes
		-> bool - stores values with two states: true or false
*/

void VariableDemo(){
	int age{23};
	std::cout << "Age is: " << age << std::endl;

	float f{32.456f};
	std::cout << "Value of f is: " << f << std::endl;

	double d{2.456789};
	std::cout << "Value of d is: " << d << std::endl;

	char value{'A'};
	std::cout << "Value of char is: " << value << std::endl;

	bool is_done{true};
	std::cout << "Value of is_done is: " << is_done << std::endl;
}