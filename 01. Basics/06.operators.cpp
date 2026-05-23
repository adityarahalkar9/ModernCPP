#include <iostream>
#include <iomanip>	  // for std::boolalpha
#include <cmath>	  // for std::pow
#include <bitset>	  // for std::bitset
#include "basic_header.h"

/*
	An operator is a symbol that tells the compiler to perform a specific mathematical,
	logical, or other operation. The values the operator acts upon are called operands.
*/
int BasicInfo(){
	int a{5};
	int b{6};
	int sum = a + b;	// '+' is the operator; 'a' and 'b' are operands

	return 0;
}
/*
	Operators can be classified by the number of operands they require:
	-> Unary: one operand (e.g., -x, ++i)
	-> Binary: two operands (e.g., x + y, a = b)
	-> Ternary: three operands (only ?:)
*/



// Precedence and Associativity – How Expressions Are Grouped
/*
	When an expression contains multiple operators, precedence determines the order in
	which operators are grouped with their operands. Higher precedence operators are grouped first.
	If adjacent operators have the same precedence, associativity determines the grouping direction:
	-> Left-to-right (L→R): (a op b) op c
	-> Right-to-left (R→L): a op (b op c)
*/
// Precedence of Multiplication over Addition
void PrecedenceOfMultiplicationOverAddition(){
	int result = 4 + 2 * 3;		// Multiplication has higher precedence than addition.
	// Equivalent to: 4 + (2 * 3) → 10
}
void AssociativityOfSubtraction(){
	int result = 7 - 4 - 1;		// Subtraction is left-to-right associative.
	// Equivalent to: (7 - 4) - 1 → 2
	// Not 7 - (4 - 1) → 4
}



// Operator precedence table
/*
	|===============================================================================
	|  C++ Operator Precedence and Associativity (Simplified Reference)
	|===============================================================================
	|Prec | Assoc | Operators                                      | Description
	|-----|-------|------------------------------------------------|----------------------------
	|  1  | L->R  | ::                                             | Scope resolution
	|  2  | L->R  | () [] . -> ++ -- (postfix) typeid const_cast   | Function call, subscript,
	|	  |       | dynamic_cast reinterpret_cast static_cast      | member access, postfix inc/dec,
	|	  |       |                                                | casts, type identification
	|  3  | R->L  | + - ++ -- (prefix) ! ~ (type) sizeof & *       | Unary plus/minus, prefix inc/dec,
	|	  |       | new new[] delete delete[]                      | logical NOT, bitwise NOT, C-style cast,
	|	  |       |                                                | size-of, address-of, dereference,
	|	  |       |                                                | dynamic memory management
	|  4  | L->R  | .* ->*                                         | Pointer-to-member operators
	|  5  | L->R  | * / %                                          | Multiplication, division, remainder
	|  6  | L->R  | + -                                            | Addition, subtraction
	|  7  | L->R  | << >>                                          | Bitwise shift left/right
	|  8  | L->R  | <=>                                            | Three-way comparison (C++20)
	|  9  | L->R  | < <= > >=                                      | Relational comparisons
	| 10  | L->R  | == !=                                          | Equality and inequality
	| 11  | L->R  | &                                              | Bitwise AND
	| 12  | L->R  | ^                                              | Bitwise XOR
	| 13  | L->R  | |                                              | Bitwise OR
	| 14  | L->R  | &&                                             | Logical AND
	| 15  | L->R  | ||                                             | Logical OR
	| 16  | R->L  | ?: = += -= *= /= %= <<= >>= &= |= ^= throw     | Ternary conditional, assignment,
	|	  |       |                                                | compound assignment, throw
	| 17  | L->R  | ,                                              | Comma operator
	 ===============================================================================
	 Note: Precedence 1 is highest. Operators on the same line share precedence.
*/
/*
	Understanding the Table:
	-> Prec: Lower number means higher precedence.
	-> Assoc: Grouping direction.
	-> Operators: Symbols used in expressions.
	-> Description: Brief explanation of each operator group.
*/



// std::pow
int StdMathFunction(){
	double power = std::pow(2.0, 8.0);
	std::cout << "Power is: " << power << std::endl;

	return 0;
}



// Arithmetic Operators (+, -, *, /, %)
int ArithmeticOperators(){
	int x{10}, y{3};
	std::cout << "x + y = " << x + y << std::endl;	// 13
	std::cout << "x - y = " << x - y << std::endl;	// 7
	std::cout << "x * y = " << x * y << std::endl;	// 30
	std::cout << "x / y = " << x / y << std::endl;	// 3(integer division truncates)
	std::cout << "x % y = " << x % y << std::endl;	// 1(remainder, integer only)

	double a{10.0}, b{3.0};
	std::cout << "a / b = " << a / b << std::endl;	// 3.3333(Floating point division)

	return 0;
}



// Assignment Operators (=, +=, -=, *=, /=, %=)
int AssignmentOperators(){
	int value{5};
	value += 3;   // equivalent to: value = value + 3
	std::cout << value << std::endl;  // 8

	value *= 2;   // value = value * 2
	std::cout << value << std::endl;  // 16

	// Chained assignment (right-to-left associativity)
	int a{}, b{};
	a = b = 10;   // b = 10; then a = b;
	std::cout << a << ' ' << b << '\n';  // 10 10

	return 0;
}



// Increment and Decrement (++, --)
/*
	-> Prefix (++i) increments and returns the new value.
	-> Postfix (i++) returns the old value and then increments.
*/
int IncrementDecrementOperator(){
	int i{5};
	int a{i++};		// a = 5, i becomes 6
	std::cout << "a = " << a << ", i = " << i << std::endl;

	int b{++i};		// i becomes 7, b = 7
	std::cout << "b = " << b << ", i = " << i << std::endl;

	return 0;
}



// Relational and Equality Operators (<, <=, >, >=, ==, !=)
int EqualityOperators(){
	int x{5}, y{7};
	bool result{};

	result = (x == y);	std::cout << std::boolalpha << result << std::endl;		// false
	result = (x != y);  std::cout << result << std::endl;						// true
	result = (x < y);   std::cout << std::noboolalpha << result << std::endl;	// 1
	result = (x >= y);  std::cout << result << std::endl;						// 0

	return 0;
}
/*
	Think of std::cout as a printer. std::boolalpha is like pressing a button on the
	printer that says "Print words instead of numbers."
	If you wanted the second line to print as a number again, you would have to explicitly reset it:

	std::cout << std::boolalpha << true << std::endl;   // Prints: true
	std::cout << std::noboolalpha << true << std::endl; // Prints: 1
*/



// Logical Operators(&&, || , !)
int LogicalOperator(){
	bool a{true}, b{false};
	std::cout << std::boolalpha;
	std::cout << (a && b) << std::endl;		// false
	std::cout << (a || b) << std::endl;		// true
	std::cout << (!a) << std::endl;			// false

	// Short-circuit demonstration
	int x = 0;
	if(x != 0 && (10 / x) > 2) {  // Division by zero never occurs because first condition is false
		std::cout << "This won't print.\n";
	}

	return 0;
}



// Bitwise Operators (&, |, ^, ~, <<, >>)
// Operate on the binary representation of integers.
int BitwiseOperator(){
	unsigned char a{0b0011};	// 3
	unsigned char b{0b0101};	// 5

	std::cout << "a & b = " << std::bitset<4>(a & b) << " (1)" << std::endl;	 // 0001
	std::cout << "a | b = " << std::bitset<4>(a | b) << " (7)" << std::endl;     // 0111
	std::cout << "a ^ b = " << std::bitset<4>(a ^ b) << " (6)" << std::endl;     // 0110
	std::cout << "~a    = " << std::bitset<4>(~a) << " (promoted to int)" << std::endl;

	unsigned char c = 0b0001;  // 1
	std::cout << "c << 1 = " << std::bitset<4>(c << 1) << " (2)" << std::endl;    // 0010
	std::cout << "c >> 1 = " << std::bitset<4>(c >> 1) << " (0)" << std::endl;    // 0000

	return 0;
}



// Ternary Conditional Operator(?:)
// A compact alternative to simple if-else statements.
int TernaryControlOperator(){
	int score{85};
	std::string grade = (score >= 60) ? std::string("p") : std::string("f");
	std::cout << "Grade: " << grade << std::endl;

	int x{10}, y{20};
	int max = (x > y) ? x : y;
	std::cout << "Max: " << max << std::endl;

	return 0;
}



// Comma Operator (,)
/*
	Evaluates each expression from left to right and returns the value of the rightmost
	expression. Rarely used outside for loops.
*/
int CommaOperator(){
	int a{}, b{};
	a = (b = 3, b + 2);    // b = 3, then a = 5
	std::cout << a << ' ' << b << std::endl;    // 5 3

	// Typical usage in for loops
	for(int i = 0, j = 10; i < j; ++i, --j){
		std::cout << i << ' ' << j << std::endl;
	}

	return 0;
}



// sizeof Operator
// Returns the size in bytes of a type or expression.
int SizeofOperator(){
	std::cout << "Size of int: " << sizeof(int) << " bytes" << std::endl;
	std::cout << "Size of double: " << sizeof(double) << " bytes" << std::endl;

	int arr[10]{};
	std::cout << "Size of array of 10 ints: " << sizeof(arr) << " bytes" << std::endl;
	return 0;
}



// Member Access Operators (. and ->)
// Used to access members of structures/classes.
struct Person{
	std::string name{};
	int age{};
};
int MemberAccessOperator(){
	Person alice{"Alice", 30};
	Person* ptr = &alice;

	std::cout << alice.name << " is " << alice.age << std::endl;    // dot operator
	std::cout << ptr->name << " is " << ptr->age << std::endl;    // arrow operator

	return 0;
}



// Best Practices: Use Parentheses for Clarity
/*
	Rule of thumb: Parenthesize everything except addition, subtraction, multiplication,
	division, and simple assignment.
*/
int BestPractices(){
	int a{1}, b{2}, c{3}, w{10}, y{5}, z{6};

	// Without parentheses, meaning may be unclear
	bool unclearResult = a && b || c;    // Actually(a && b) || c

	// Clear intent
	bool clearResult = (a && b) || c;

	// Exception: simple arithmetic
	int x = y + z * w;   // acceptable

	// Exception: right-hand side of a single assignment
	int result = a + b + c;   // parentheses not needed

	return 0;
}



// Order of Evaluation – A Critical Distinction
/*
	Precedence and associativity determine grouping and the order of value computation. They do
	not specify the order in which operands are evaluated.
*/
int GetValue(){
	static int count = 0;
	return ++count;
}
int orderOfEvaluation(){
	// Ambiguous: the calls to getValue() can be evaluated in any order.
	std::cout << GetValue() + GetValue() * GetValue() << '\n';

	return 0;
}
// Solution: Force a specific evaluation order by using separate statements.
int solutionOrder(){
	int a = GetValue();  // first
	int b = GetValue();  // second
	int c = GetValue();  // third
	std::cout << a + b * c << '\n';  // unambiguous
	return 0;
}




/*
	Conclusion
	-> Precedence and associativity dictate how expressions are parsed.
	-> Use the reference table to resolve complex expressions.
	-> Parentheses are your friend for readability and safety.
	-> Order of evaluation of operands is often unspecified; avoid side effects in compound expressions.
*/

int Operators(){
	// BasicInfo();
	// PrecedenceOfMultiplicationOverAddition();
	// AssociativityOfSubtraction();
	// ArithmeticOperators();
	// StdMathFunction();
	// AssignmentOperators();
	// IncrementDecrementOperator();
	// EqualityOperators();
	// LogicalOperator();
	// BitwiseOperator();
	// TernaryControlOperator();
	// CommaOperator();
	// SizeofOperator();
	// MemberAccessOperator();
	// BestPractices();
	// OrderOfEvaluation();
	// SolutionOrder();

	return 0;
}