#include <iostream>
#include "memory_header.h"

/*
	A reference is somewhat safer version of a pointer. A reference works as an alias
	for an existing variable, providing an alternative name for it and allowing you to work
	with the orignal data directly.
	Unlike pointers, references
	=> Cannot be null
	=> Do not require dereferencing
	=> Are not objects themselves: they do not occupy memory like pointers do.
*/
int BasicReferences(){
	int var{70};
	int& r_var = var;

	std::cout << "var is: " << var << std::endl;		 // var is : 70
	std::cout << "&var is: " << &var << std::endl;		 // &var is : 0000001AC2EFF5F4
	std::cout << "r_var is: " << r_var << std::endl;	 // r_var is : 70
	std::cout << "&r_var is: " << &r_var << std::endl;	 // &r_var is : 0000001AC2EFF5F4

	// &var = &r_var showing that both variables share the same memory address.
	// Memory location of variable and it's reference is the same unlike pointers.

	return 0;
}


int ReferenceManipulation(){
	int count{30};
	int& r_count = count;

	// Setting new values to variable count
	count = 50;
	std::cout << "Updated Value of count is: " << count << std::endl;
	std::cout << "Updated Value of reference to the count r_count is: " << r_count << std::endl;
	std::cout << std::endl;

	// Value of variable is also updates if we update value of it's reference
	r_count = 100;
	std::cout << "Updated Value of count is: " << count << std::endl;
	std::cout << "Updated Value of reference to the count r_count is: " << r_count << std::endl;

	return 0;
}



// References cannot be reassigned
int ReferencesNotReassigned(){
	int a{5};
	int b{10};
	int& ref{a};	// Binds to a

	ref = b;	// This does NOT bind to b; it assigns b's value (10) to a
	std::cout << "a: " << a << ", b: " << b << std::endl;	// Output is a: 10, b: 10

	// To rebind you will need a new reference, but that's not reassigning
	return 0;
}



// References to constants(Const references)
/*
	You can create references to constants using const. This prevents modification through the
	reference and allows binding to temporaries(rvalues)
*/
int ConstReferences(){
	const int c{42};
	const int& refConst{c};		// Const reference to const variable

	// refConst = 50;	// Error: Cannot modify through const reference

	int temp{100};
	const int& refToNonConst{temp};		// Ok, but still can't modify
	// refToNonConst = 200;		// Error

	const int& refToTemp{99};	// Binds to a temporary(rvalue) - allowed for const refs
	std::cout << refToTemp << std::endl;	// Outputs: 99

	return 0;
}
/*
	=> Const references ectend the lifetime of temporaries they bind to
	=> Non-const references cannot bind to rvalues or const objects
*/



// References vs Pointers
// References are like safer pointers
int ReferencesVsPointers(){
	int x{10};
	int* ptr{&x};	// Pointer to x
	int& ref{x};	// Reference to x

	*ptr = 20;		// Dereference pointer to change x
	ref = 30;		// No dereference needed for reference

	ptr = nullptr;	// Pointer can be null
	// ref = nullptr;	// Error: References cannot be null

	std::cout << "x is: " << x << std::endl;
	std::cout << "ref is: " << ref << std::endl;
	std::cout << "&x is: " << &x << std::endl;
	std::cout << "&ref is: " << &ref << std::endl;
	std::cout << "ptr is: " << ptr << std::endl;
	std::cout << "*ptr is: " << *ptr << std::endl;
	std::cout << "&ptr is: " << &ptr << std::endl;

	return 0;
}
/*
	Differences:
	=> Syntax: References don't need * or & after declaration.
	=> Safety: References guarantee a valid object; pointers don't.
	=> Use cases: References for aliases; pointers for dynamic memory or nullability.
*/



// Passing References to Functions
/*
	References are commonly used to pass arguments to functions without copying (efficient for
	large objects) and to allow modification.
*/
void ModifyByRef(int& ref){	  // Non-const Reference: can modify	
	ref += 10;
}
void PrintByConstRef(const std::string& str){	// Const reference: efficient, no copy, no modify
	std::cout << str << std::endl;
	// str = "Modified";	// Error
}
int ReferenceToFunction(){
	int num{5};
	ModifyByRef(num);	// num is modified
	std::cout << num << std::endl;	// Output: 15

	std::string largestr(1000000, 'a');    // Huge string - "paren-initialization" or constructor-style initialization
	PrintByConstRef(largestr);    // No copy overhead
	PrintByConstRef("Temporary string");	// Binds to rvalues - efficient

	return 0;
}
/*
	Comments:
	=> Passing by value copies the argument; by reference avoids this.
	=> Always use const references for read-only parameters to prevent accidental modification and
	allow rvalue binding.
*/



// Returning References from Functions
// Functions can return references, but be careful not to return references to locals (dangling references).
int& GetMax(int& a, int& b){
	return (a > b) ? a : b;
}
int ReturnReferencesFromFunctions(){
	int x{10}, y{20};
	GetMax(x, y) = 30;	// Modifies y through the returned reference
	std::cout << y << std::endl;	// Output: 30

	/*
		Bad Example : Dangling reference
		int& badFunc(){
			int local{42};
			return local;    // Error: local destroyed after return
		}
	*/
	return 0;
}
/*
	Key Points:
	=> Returning references allows "lvalue" returns (modifiable).
	=> Never return references to locals or temporaries—undefined behavior.
	=> Safe for globals, statics, or parameters.
*/



// References in arrays and pointers
int ReferencesToArrays(){
	int arr[]{1,2,3};
	int(&refaArr)[3]{arr};    // Reference to entire array(note Parentheses)

	refaArr[1] = 20;    // Modify arr
	std::cout << arr[1] << std::endl;	// Output: 20

	int* ptr{&arr[0]};
	int*& refToPtr{ptr};	// Reference to Pointer
	refToPtr++;    // Advances the pointer
	std::cout << *refToPtr << std::endl;	// Output: 20 (after modification)

	/*
		=> Pointer to reference: Not allowed directly, but can use typedef or alias
		=> int& ref = arr[0];
		=> int&* ptrToRef = &ref;  // Error: Pointer to reference is invalid
	*/
	return 0;
}
/*
	Comments:
	=> Array references require (&ref)[size] syntax to distinguish from reference to pointer.
	=> References to pointers allow modifying the pointer itself.
	=> Pointers to references are forbidden because references aren't objects with addresses (though &ref gives
	   the address of the referred object).
*/



// References in Structures
struct MyStruct{
	int& refMember;    // Reference member
	MyStruct(int& val) : refMember{val} {}    // Must initialise in list
	// MyStruct(int& val) { refMember = val; }  // Error: Can't assign, must initialize
};
int StructReferences(){
	int x{10};
	MyStruct s{x};
	s.refMember = 20;	// Modifies x
	std::cout << x << std::endl;	// Outputs 20

	return 0;
}
/*
	Key Points:
	=> Reference members make the class non-assignable (can't rebind).
	=> Useful for wrappers or views without copying.
	=> In C++20, designated initializers can be used for structs, but references still need init lists.
*/



// References to Volatile and Const-Volatile
// References can be volatile (for hardware access or multithreading).
int VolatileReference(){
	volatile int v{10};
	volatile int& volRef{v};	// Volatile reference

	// volRef = 20;    // OK, but treats as volatile (no optimization)
	// cvRef = 20;     // Error: const

	return 0;
}
// Volatile prevents compiler optimizations; useful for memory-mapped I/O.



// Rvalue References (C++11)
// Rvalue references(&&) bind to temporaries(rvalues), enabling move semantics for efficient resource transfer.
class Resource{
public:
	int* data{};
	Resource() : data{new int{42}} {}
	~Resource() {
		delete data;
	}

	// Move constructor using rvalue ref
	Resource(Resource&& other) noexcept : data{other.data}{
		other.data = nullptr;    // Steal resource
	}
};
void TakeByRRef(std::string&& rref){	// Binds only to rvalues
	std::cout << rref << std::endl;
}
int RvalueReferences(){
	Resource src{};
	Resource dest{std::move(src)};    // Move: src's data is stolen

	TakeByRRef("Temporary");	// OK, rvalue

	std::string s{"Hello"};
	// takeByRRef(s);	// Error: s is lvalue
	TakeByRRef(std::move(s));	// OK, cast to rvalue

	return 0;
}
/*
	Key Points:
	=> && binds to rvalues; use std::move to cast lvalues to rvalues.
	=> Enables move constructors/assignments, reducing copies for vectors, strings, etc.
	=> Rvalue refs cannot bind to lvalues without std::move.
	=> In C++20, explicit this parameters can use refs, but not directly relevant here.
*/



// Reference Collapsing Rules (C++11)
/*
	When templates or auto deduce types, references can "collapse" based on rules:
	=> T& & → T&
	=> T& && → T&
	=> T&& & → T&
	=> T&& && → T&&
*/
template <typename T>
void CheckCollapse(T&& arg) {
	if(std::is_lvalue_reference_v<T>) {  // C++17 _v for brevity
		std::cout << "Lvalue ref" << std::endl;
	}
	else if(std::is_rvalue_reference_v<T>) {
		std::cout << "Rvalue ref" << std::endl;
	}
}
int CollapsingReference(){
	int x{10};
	CheckCollapse(x);          // T = int& (collapses to lvalue ref)
	CheckCollapse(std::move(x));  // T = int (but && makes rvalue ref)

	return 0;
}
/*
	In templates, T&& is a "universal reference" (now called forwarding reference in C++20) that can be lvalue or
	rvalue based on argument.
*/



// Perfect Forwarding (C++11)
// Using std::forward with universal references to preserve lvalue/rvalue nature.
void Inner(int& lref);		// Forward declare inner overloads (or define them) before wrapper
void Inner(int&& rref);

template <typename F>
void Wrapper(F&& arg){
	Inner(std::forward<F>(arg));  // Forwards as lvalue or rvalue
}
void Inner(int& lref){
	std::cout << "Lvalue" << std::endl;
}
void Inner(int&& rref){
	std::cout << "Rvalue" << std::endl;
}
void ForwardReference(){
	int x{10};
	Wrapper(x);          // Calls inner(int&)
	Wrapper(20);         // Calls inner(int&&)
}
/*
	Without std::forward, everything would be forwarded as lvalue. This is used in std::make_unique,
	emplace functions, etc. In C++23, some library functions are refined, but core remains.
*/



// References in Templates
template <typename T>
void Func(T& ref){	  // Deduce T as type, ref is lvalue ref
	std::cout << "Lvalue overload" << std::endl;
}
template <typename T>
void Func(T&& ref){    // Universal ref
	std::cout << "Universal overload" << std::endl;
}
int ReferenceTemplate(){
	int x{10};
	Func(x);	// Calls T& (lvalue)
	Func(20);	// Calls T&& (rvalue)

	return 0;
}
/*
	SFINAE (Substitution Failure Is Not An Error) can disable overloads based on ref types.
	In C++20, concepts can constrain templates to require specific ref types
	(Ex: std::same_as<T, int&>).
*/



// structured Bindings(C++17)
// These use references under the hood to bind to tuple / pair / struct elements.
int StructuredBindings(){
	std::tuple<int, std::string> t{42, "Hello"};
	auto& [num, str]{t};  // References to elements

	num = 100;  // Modifies tuple
	std::cout << std::get<0>(t) << std::endl;  // Output: 100

	const auto& [cnum, cstr]{t};  // Const references

	return 0;
}
/*
	auto& binds references; auto copies; auto&& for forwarding. In C++20, can use with custom
	types via std::tuple_size.
*/



// References to Functions
// You can have references to functions(similar to function pointers but without address).
void Func(){
	std::cout << "Called" << std::endl;
}
int FuncReferences(){
	void(&funcRef)(){ Func };  // Reference to function
	funcRef();  // Calls func, Output: Called

	// Useful in templates or aliases
	return 0;
}



int References(){
	 BasicReferences();
	// ReferenceManipulation();
	// ReferencesNotReassigned();
	// ConstReferences();
	// ReferencesVsPointers();
	// ReferenceToFunction();
	// ReturnReferencesFromFunctions();
	// ReferencesToArrays();
	// StructReferences();
	// VolatileReference();
	// CollapsingReference();
	// ForwardReference();
	// ReferenceTemplate();
	// StructuredBindings();
	// FuncReferences();

	return 0;
}