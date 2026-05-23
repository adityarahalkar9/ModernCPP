#include "basic_header.h"
#include <iostream>;
#include "climits"     // for integer limits (e.g, INT_MAX)
#include <cfloat>;	   // for floating-point limits (e.g, FLT_MAX)
#include <iomanip>;	   // for output

void DataTypes(){

	/*
		 => Data Types
			1. Primitive Data Types:
				-> Integer (int)
				-> Character (char)
				-> Boolean (bool)
				-> Floating Point (float)
				-> Double Floating Point (double)
				-> Void (void)

			2. Derived Data Types:
				-> Function
				-> Array
				-> Pointer
				-> Reference

			3. Abstract or User-Defined Data Types:
				-> Class
				-> Structure
				-> Union
				-> Enumeration
				-> Typedef defined Datatype

		=> Size of Data Types
		   __________________________________________________________________
		   |    TYPE    |  TYPICAL BIT WIDTH  |        TYPICAL RANGE        |
		   |____________|_____________________|_____________________________|
		   |            |                     |                             |
		   | -> char    |  1 byte             |         -127 to 127         |
		   | -> int     |  4 bytes            |  -2147483648 to 2147483647  |
		   | -> float   |  4 bytes            |   -3.4�10^38 to 3.4�10^38   |
		   | -> double  |  8 bytes            |  -1.7�10^308 to 1.7�10^308  |
		   |____________|_____________________|_____________________________|

		=> 1 byte = 8 bits
		   - int type has 32 bits out of which 1 bit is reserved for sign of integer, so
			 the int data type can store upto the value of 2^31
		   - For using all 32 bits declare variable as unsigned int.

		=> Data type modifiers

		   __________________________________________________________________________________________________
		   | DATA TYPE                 |    SIZE IN BYTES    |          RANGE                               |
		   |___________________________|_____________________|______________________________________________|
		   |                           |                     |                                              |
		   | -> short int              |      2 byte         |        -32,768 to 32,767                     |
		   | -> unsigned short int     |      2 bytes        |              0 to 65535                      |
		   | -> unsigned int           |      4 bytes        |              0 to 4,294,967,295              |
		   | -> int                    |      4 bytes        | -2,147,483,648 to 2,147,483,647              |
		   | -> long int               |      4 bytes        | -2,147,483,648 to 2,147,483,647              |
		   | -> unsigned long int      |      4 bytes        |              0 to 4,294,967,295              |
		   | -> long long int          |      8 bytes        |        -(2^63) to (2^63)-1                   |
		   | -> unsigned long long int |      8 bytes        |              0 to 18,446,744,073,709,551,615 |
		   | -> signed char            |      1 bytes        |           -128 to 127                        |
		   | -> float                  |      4 bytes        |     -3.4�10^38 to 3.4�10^38                  |
		   | -> double                 |      8 bytes        |    -1.7�10^308 to 1.7�10^308                 |
		   | -> long double            |     12 bytes        |   -1.1�10^4932 to 1.1�10^4932                |
		   | -> wchar_t                |   2 or 4 bytes      |           1 wide character                   |
		   |___________________________|_____________________|______________________________________________|

	*/


	// ====================
	// 1. Primitive Data Types
	// ====================

	// Boolean: true or false
	bool flag{true};
	std::cout << "bool: " << flag << "(prints 1 for true, 0 for false" << std::endl;
	std::cout << "Size of bool: " << sizeof(bool) << " byte(s)" << std::endl;
	std::cout << std::endl;

	// Character: Single character
	char ch = 'A';
	std::cout << "char: " << ch << std::endl;
	std::cout << "Size of char: " << sizeof(char) << " byte(s)" << std::endl;
	std::cout << std::endl;

	// Signed & Unsigned char range: -128 to 127
	std::cout << "Signed char range: " << SCHAR_MIN << " to " << SCHAR_MAX << std::endl;
	std::cout << "Unsigned char range: 0 to " << UCHAR_MAX << std::endl;
	std::cout << std::endl;

	// Integer 
	int i{35};
	std::cout << "int: " << i << std::endl;
	std::cout << "Size of int: " << sizeof(int) << " bytes" << std::endl;
	std::cout << "Range of int: " << INT_MIN << " to " << INT_MAX << std::endl;
	std::cout << std::endl;

	// Short int
	short s_int{32767};    // Maximum value for signed short
	std::cout << "short int: " << s_int << std::endl;
	std::cout << "Size of short: " << sizeof(short) << " bytes " << std::endl;
	std::cout << "Range of short: " << SHRT_MIN << " to " << SHRT_MAX << std::endl;
	std::cout << std::endl;

	// Unsigned int
	unsigned u_int{40000003U};    // U suffix for unsigned literal
	std::cout << "unsigned int: " << u_int << std::endl;
	std::cout << "Size of unsigned int: " << sizeof(unsigned) << " bytes" << std::endl;
	std::cout << "Range of unsigned: 0 to " << UINT_MAX << std::endl;
	std::cout << std::endl;

	// Long int 
	long l{214567352L};	// L suffix for long	
	std::cout << "Long int: " << l << std::endl;
	std::cout << "Size of long: " << sizeof(long) << " bytes" << std::endl;
	std::cout << "Range of long: " << LONG_MIN << " to " << LONG_MAX << std::endl;
	std::cout << std::endl;


	// Long long int
	long long ll{3243276352485682465LL};	// LL suffix for long long
	std::cout << "long long int: " << ll << std::endl;
	std::cout << "Size of long long: " << sizeof(long long) << " bytes" << std::endl;
	std::cout << "Range of long long: " << LLONG_MIN << " to " << LLONG_MAX << std::endl;
	std::cout << std::endl;

	// Floating point: float
	float f{3.1415f};    // f suffix for float
	std::cout << "float: " << f << std::endl;
	std::cout << "Size of float: " << sizeof(float) << " bytes" << std::endl;
	std::cout << "Range of float: " << FLT_MIN << " to " << FLT_MAX << std::endl;
	std::cout << std::endl;

	// Double
	double d{3.14159323456793};
	std::cout << "double: " << d << std::endl;
	std::cout << "Size of double: " << sizeof(double) << " bytes" << std::endl;
	std::cout << "Range of double: " << DBL_MIN << " to " << DBL_MAX << std::endl;
	std::cout << std::endl;

	// Long Double 
	long double ld{3.1456364943992L};
	std::cout << "long double: " << ld << std::endl;
	std::cout << "Size of long double: " << sizeof(long double) << " bytes" << std::endl;
	std::cout << "Range of long double: " << LDBL_MIN << " to " << LDBL_MAX << std::endl;
	std::cout << std::endl;

	// Wide character (wchar_t)
	wchar_t wch = L'Ω';  // wide character literal
	std::cout << "wchar_t character: " << static_cast<char>(wch) << " (if convertible)\n";
	std::cout << "Size of wchar_t: " << sizeof(wchar_t) << " bytes\n\n";
	std::cout << std::endl;



	// ====================
	// 2. Derived Data Types (brief examples)
	// ====================

	// Array
	int arr[5] = {1, 2, 3, 4, 5};
	std::cout << "Array: ";
	for(int i = 0; i < 5; ++i)
		std::cout << arr[i] << " ";
	std::cout << "\nSize of array: " << sizeof(arr) << " bytes\n\n";

	// Pointer
	int* ptr = &i;
	std::cout << "Pointer to int: points to address " << ptr << ", value = " << *ptr << "\n";
	std::cout << "Size of pointer: " << sizeof(ptr) << " bytes (depends on architecture)\n\n";

	// Reference (alias)
	int& ref = i;
	std::cout << "Reference to int: ref = " << ref << " (same as i)\n";

	// Function (pointer example)
	// We'll just show a simple function pointer
	auto printHello = []() { std::cout << "Hello from function pointer!\n"; };
	std::cout << "\nFunction pointer example: ";
	printHello();



	// ====================
	// 3. User-Defined Data Types (brief examples)
	// ====================

	// Structure
	struct Point {
		int x;
		int y;
	};
	Point p = {10, 20};
	std::cout << "\nStructure Point: (" << p.x << ", " << p.y << ")\n";

	// Enumeration
	enum Color { RED, GREEN, BLUE };
	Color c = GREEN;
	std::cout << "Enumeration Color: value = " << c << " (0=RED,1=GREEN,2=BLUE)\n";

	// Typedef (or using)
	typedef unsigned long ulong;
	ulong bigNumber = 123456789UL;
	std::cout << "Typedef unsigned long as ulong: " << bigNumber << "\n";

	// Class (simple)
	class Rectangle {
		int width, height;
	public:
		Rectangle(int w, int h) : width(w), height(h) {}
		int area() const { return width * height; }
	};
	Rectangle rect(5, 7);
	std::cout << "Class Rectangle area: " << rect.area() << "\n";
}