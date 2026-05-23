#include <iostream>
#include <functional>
#include "memory_header.h"

// A pointer is a variable that stores the address of another variable
int Pointer(){
	int num{45};
	int* n_ptr = &num;

	std::cout << "num is: " << num << std::endl;
	std::cout << "&num is: " << &num << std::endl;
	std::cout << "n_ptr is: " << n_ptr << std::endl;
	std::cout << "&n_ptr is: " << &n_ptr << std::endl;

	// Dereferencing the pointer n_ptr
	std::cout << "*n_ptr is: " << *n_ptr << std::endl;

	// &num = n_ptr as n_ptr pointer stores memory address of variable num
	// *n_ptr = num = 45 as the dereferenced pointer *n_ptr holds value of num

	return 0;
}

void PointerToChar(){
	const char* message{"Hello World"};    // Using const is necessary
	std::cout << message << std::endl;

	// If modification is required then
	char message1[]{"Hii"};
	message1[0] = 'B';
	std::cout << message1 << std::endl;
}

int PointerManipulation(){
	int a{90};
	int* a_ptr{&a};

	*a_ptr += 4;
	std::cout << "a is: " << a << std::endl;

	*a_ptr = 150;
	std::cout << "a is: " << a << std::endl;

	return 0;
}

// Null Pointers
void NullPointer(){
	int* n_ptr{nullptr};
	if(n_ptr == nullptr){
		std::cout << "No Target" << std::endl;
	}
}


// Pointer declaration syntax & const-correctness
/*
	These control mutability: What the pointer points to, and the pointer itself.
	-> Const Pointer: Pointer can't be reassigned (e.g., int* const p = &x; � p is constant).
	-> Pointer to Const: Can't modify the pointed-to value (e.g., const int* p = &x; � *p is read-only).
	-> Const Pointer to Const: Both (e.g., const int* const p = &x;).
*/
void ConstPointer(){
	int x{13};
	const int y{15};

	int* p1{&x};			  // can change *p1 and p1
	const int* p2{&y};		  // cannot change *p2 (If you change p2, undefined behavior because you are accessing a memory location that you have not explicitly allocated or initialized)
	int* const p3{&x};        // cannot change p3 (address), but can change *p3
	const int* const p4{&y};  // neither p4 nor *p4 can change

	// p2+=1 will create undefined behaviour
}
void ConstPointer2(){
	int x{10};
	const int y{20};

	const int* p1{&x};     // Pointer to const: can't change *p1, but can reassign p1
	// *p1 = 30;           // Error: can't modify pointed-to value
	p1 = &y;               // OK: reassign pointer

	int* const p2{&x};     // Const pointer: can change *p2, but can't reassign p2
	*p2 = 40;              // OK: modifies x
	// p2 = &y;            // Error: p2 is constant

	const int* const p3{&y};  // Both const: can't change *p3 or reassign p3

	std::cout << "x after modification: " << x << std::endl;  // Outputs: 40
}
/*
	Use for safety in functions (e.g., void func(const int* p) to promise not to modify). Implicit
	conversions: Non-const to const pointer is OK, but not vice versa.
*/


// Pointer Arithmetic
void PointerArithmetic(){
	int arr[]{10,20,30,40,50};
	int* p{arr};	// Points to arr[20]
	std::cout << *(p + 1) << std::endl;    // Prints 20
	++p;			// Now points to arr[1]
	std::cout << *p << std::endl;		   // Prints arr[1] which is 20

	// Beware: going outside bounds -> undefined behaviour
}
void PointerArithmetic2(){
	int arr[5]{10, 20, 30, 40, 50};  // Array of 5 ints with brace initialization
	int* p{arr};                      // Pointer to first element (arr decays to &arr[0])

	std::cout << "First element: " << *p << std::endl;  // Outputs: 10

	p++;  // Increment: moves to next int (4 bytes ahead, assuming sizeof(int)=4)
	std::cout << "Second element: " << *p << std::endl;  // Outputs: 20

	*(p + 2) = 60;  // Pointer arithmetic: access third element ahead, modify it
	std::cout << "Modified array[3]: " << arr[3] << std::endl;  // Outputs: 60 (original 40)

	int* end{arr + 5};  // Pointer to one past the end (common in loops)
	for(int* it{arr}; it != end; ++it) {
		std::cout << *it << " ";  // Outputs: 10 20 30 60 50
	}
	std::cout << std::endl;
}
/*
	Pointer subtraction (e.g., end - arr) gives the number of elements (5 here). Arithmetic on void*
	is illegal. Use with caution to avoid buffer overflows. In C++23, consider std::span for safer
	array views (from  via import std;).
*/


// Pointer decay (Pointers and Arrays)
/*
	Arrays and pointers are closely related: An array name decays to a pointer to its first element.
	=> Multidimensional Arrays: Pointer to array of arrays.
	=> Pointer to Array: int (*p)[5] points to an array of 5 ints.
*/
void PointerDecay(){
	int arr[]{11,12,13,14,15};
	int* p{arr};
	std::cout << *p << std::endl;
	std::cout << sizeof(arr) << std::endl;
	std::cout << sizeof(p) << std::endl;
}
void PointerDecay2(){
	int arr[3][2]{{1,2}, {3,4}, {5,6}};     // 2D Array with brace initialisation
	int* p(&arr[0][0]);					    // Pointer to first element

	for(int i{0}; i < 6; ++i){
		std::cout << *(p + i) << " ";	    // Outputs: 1 2 3 4 5 6 (linear traversal)
	}
	std::cout << std::endl;

	int(*rowPtr)[2]{ arr };					 // Pointer to array of 2 ints (points to first row)
	std::cout << (*rowPtr)[1] << std::endl;  // Outputs: 2 (arr[0][1])
}
/*
	Arrays are fixed-size; pointers can point to them but not resize them. Use std::array
	(from <array>) for a safer fixed-size wrapper with brace init.
*/


// References vs pointers
/*
	References are aliases to variables, like "constant pointers" that must be initialized and can't
	be null or reassigned.
	=> Syntax: int& ref = x; (no * for dereference; use like variable). In C++23, brace init isn't directly
	   applicable, but refs can bind to braced temps.
	=> Differences: References can't be null, no arithmetic, auto-dereferenced.
*/
/*
	void increment(int& ref){	// Pass by reference: modifies original
		ref++;
	}
	int referencesVsPointers(){
		int x{10};
		int& ref{x};    // Reference to x (brace init not needed, but consistent style)
		ref = 20;       // Changes x

		increment(x);   // x becomes 21

		// int& nullRef{nullptr};     // Error: references can't be null

		std::cout << x << std::endl;  // Outputs: 21

		return 0;
	}
*/
/*
	Pointers can be reassigned/null; references can't. Use references for simplicity when null isn't needed.
	References can be to const: const int& cref{x}; .
*/


// Pointers in functions
/*
	Pointers allow pass-by-reference semantics for efficiency or modification.
	=> Pass by Pointer: To modify arguments or avoid copying large objects.
	=> Return Pointers: Return dynamic memory or addresses (careful with locals!).
*/
void Swap(int* a, int* b){    // Pass pointers to modify originals
	int temp{*a};
	*a = *b;
	*b = temp;
}
int* CreateInt(){    // Bad: returns pointer to local (dangling after return)
	int local{42};
	return &local;    // Undefined behavior!
}
int PointerArgument(){
	int x{10}, y{20};
	Swap(&x, &y);    // Pass addresses
	std::cout << "x: " << x << ", y: " << y << std::endl;    // Outputs: x:20, y:10

	// int* dangling{createInt()};    // Avoid: points to destroyed local

	return 0;
}
/*
	Never return pointers to locals (stack-allocated; destroyed on return). Use dynamic memory
	instead. In functions, consider std::span for pointer+size pairs.
*/


// Pointer to Pointer
// Pointers can point to other pointers, forming chains (e.g., int**).
// Usage: For 2D dynamic arrays or modifying pointers in functions.
void PointerToPointer(){
	int a{20};
	int* a_ptr{&a};
	int** b_ptr{&a_ptr};

	std::cout << "a is: " << a << std::endl;
	std::cout << "a_ptr is: " << a_ptr << std::endl;
	std::cout << "b_ptr is: " << b_ptr << std::endl;
}
/*
	Common in argc/argv or dynamic matrices. Depth can lead to complexity. For dynamic 2D,
	prefer std::vector<std::vector<int>>.
*/


// Void pointers
/*
	void* is a generic pointer that can point to any type (no type info).
	Usage: For type-agnostic code (e.g., malloc in C). Cast to specific type before use.
	Limitations: No arithmetic or dereferencing without casting.
*/
int VoidPointers(){
	int x{10};
	void* vp{&x};  // OK: void* can hold any address

	// std::cout << *vp << std::endl;  // Error: can't dereference void*
	int* ip{static_cast<int*>(vp)};  // Cast back
	std::cout << *ip << std::endl;   // Outputs: 10

	return 0;
}
// Use static_cast for safe casts. Avoid if possible; prefer templates for type safety.

// Pointer to Whole Array
void PointerToWholeArray(){
	int arr[]{10, 20, 30};
	int (*pArr)[3] = &arr;    // pointer to array of 3 ints
	std::cout << (*pArr)[1] << std::endl;	// prints 20
}


/*
	Pointers to functions for callbacks or dynamic dispatch.
	=> Declaration: return_type (*ptr)(args);
	=> Usage: Store and invoke functions.
*/
// Function pointers
int Addition(int a, int b){
	return a + b;
}
void FunctionPointers(){
	// pointer to function taking (int, int) -> int
	int (*op)(int, int) = &Addition;
	std::cout << op(2, 3) << "\n";      // prints 5
	std::cout << (*op)(4, 5) << "\n";   // prints 9
}
// Use std::function (from <functional>) for flexibility with lambdas.


struct Foo{
	int value{0};
	void show(int x){
		std::cout << "Value: " << value << " arg: " << x << std::endl;
	}
};
int PointerToMember(){
	// pointer to member data
	int Foo::* dataPtr = &Foo::value;
	Foo f{7};
	std::cout << f.*dataPtr << "\n";  // prints 7

	// pointer to member function
	void (Foo:: * fnPtr)(int) = &Foo::show;
	(f.*fnPtr)(10);                   // call on object
	Foo* pf{&f};
	(pf->*fnPtr)(20);                 // call through pointer

	return 0;
}


// Lambda Captures and Pointers
/*
	Lambdas can capture variables by reference (like pointers) or value.
	=> Capture by Reference: [&] captures all by ref (pointer-like).
	=> This Pointer: In classes, capture this for member access.
*/
int LambdaPointers(){
	int x{10};

	auto lambda{[&x]() { x++; }};  // Capture by ref: modifies x (x's address captured)
	lambda();
	std::cout << x << std::endl;   // Outputs: 11

	std::function<void()> func{lambda};  // Store in std::function (uses function pointer internally)

	return 0;
}
/*
	Capture by ref can dangle if lambda outlives variable. Use [=] for value copy. In C++23,
	lambdas support more in coroutines.
*/


// Pointers to member
/*
	Pointers to class members (data or functions) for meta-programming.
	=> Data Member Pointer: type Class::*ptr;
	=> Member Function Pointer: return_type (Class::*ptr)(args);
	=> Usage: With objects via obj.*ptr or pointers via ptrObj->*ptr.
*/
class MyClass{
public:
	int value{50};
	void print() { std::cout << "Hello" << std::endl; }
};
int PointerToMember2(){
	int MyClass::* dataPtr{&MyClass::value};  // Pointer to data member

	MyClass obj{};
	obj.*dataPtr = 60;  // Access via .*
	std::cout << obj.value << std::endl;  // Outputs: 60

	void (MyClass:: * funcPtr)(){ &MyClass::print };  // Pointer to member function
	(obj.*funcPtr)();  // Outputs: Hello

	MyClass* pObj{&obj};
	(pObj->*funcPtr)();  // Via ->*

	return 0;
}
// Useful in templates or callbacks. Virtual functions work polymorphically.


// Volatile pointers
/*
	volatile qualifiers prevent compiler optimizations for pointers to hardware-mapped memory or multi-threaded access.
	=> Usage: volatile int* p; or int* volatile p; (volatile pointer itself).
*/
int VolatilePointer(){
	volatile int x{10};  // Volatile variable
	volatile int* vp{&x};  // Pointer to volatile

	*vp = 20;  // Forces read/write without optimization

	return 0;
}
// Rarely needed; use atomics (<atomic>) for threads.


// Raw Pointers
/*
	Raw Pointers in Modern C++
	=> reinterpret_cast: Unsafe cast between pointer types (e.g., int* to char* for byte access).
	=> const_cast: Remove const (dangerous; avoid).
	=> alignas/alignof: For pointer alignment in low-level code.
	=> Pointer Traits: std::pointer_traits for custom pointers.
*/
int RawPointers(){
	int x{0x12345678};
	char* bytes{reinterpret_cast<char*>(&x)};  // View as bytes

	for(int i{0}; i < sizeof(int); ++i) {
		std::cout << std::hex << static_cast<int>(bytes[i]) << " ";  // Outputs bytes (endian-dependent)
	}
	std::cout << std::endl;

	return 0;
}
// Use casts sparingly; they bypass safety. In C++23, improved support for extended alignments.


// Pointers in Templates and Classes
// Pointers in generics or classes require careful management (e.g., rule of five for ownership).
template <typename T>
T* create(T value){
	return new T{value};
}
class PointerHolder{
public:
	int* ptr{nullptr};
	PointerHolder(int* p) : ptr{p} {}  // Constructor
	~PointerHolder() { delete ptr; }   // Destructor for ownership
	// Rule of five: copy/move constructors/assignments needed if copying
};
int PointerTemplates(){
	auto p{create<int>(42)};
	std::cout << *p << std::endl;  // Outputs: 42
	delete p;

	return 0;
}
// For classes with pointers, implement copy/move to avoid shallow copies. Prefer smart pointers.


// Smart Pointers
/*
	Smart pointers automate memory management to prevent leaks/dangling. From <memory>.
	=> unique_ptr: Exclusive ownership; auto-deletes on scope exit. Can't copy.
	=> shared_ptr: Shared ownership; reference-counted; deletes when count=0.
	=> weak_ptr: Non-owning reference to shared_ptr; doesn't increment count (avoids cycles).
*/
int UniquePtr(){
	std::unique_ptr<int> up{new int{10}};  // Allocates and owns with brace init
	std::cout << *up << std::endl;         // Outputs: 10

	// up = new int{20};  // Error: can't reassign raw pointer; use reset
	up.reset(new int{20});                 // Releases old, takes new
	std::cout << *up << std::endl;         // Outputs: 20

	// Auto-deletes when up goes out of scope
	return 0;
}
int SharedPtr(){
	std::shared_ptr<int> sp1{std::make_shared<int>(30)};  // Preferred: make_shared for efficiency (no brace for make_shared, but value is 30)
	std::shared_ptr<int> sp2{sp1};                        // Copy: ref count=2

	std::cout << *sp1 << " " << sp2.use_count() << std::endl;  // Outputs: 30 2

	sp1.reset();  // sp1 releases; count=1
	std::cout << *sp2 << std::endl;  // Still valid: 30

	return 0;  // Deletes when count=0
}
int WeakPtr(){
	std::shared_ptr<int> sp{std::make_shared<int>(40)};
	std::weak_ptr<int> wp{sp};  // Observes without owning

	if(auto locked{wp.lock()}) {  // Lock: gets shared_ptr if alive (brace init for auto)
		std::cout << *locked << std::endl;  // Outputs: 40
	}
	else {
		std::cout << "Expired" << std::endl;
	}

	sp.reset();  // Releases; now expired
	if(wp.lock()) {
		// Won't enter
	}
	else {
		std::cout << "Expired" << std::endl;
	}

	return 0;
}
/*
	Prefer smart pointers over raw. std::make_unique/make_shared for exception safety (C++14+ make_unique).
	Weak_ptr breaks circular references. In C++23, smart pointers support coroutines better, but basics unchanged.
*/



int Pointers(){
	// Pointer();
	// PointerToChar();
	// PointerManipulation();
	// NullPointer();
	// ConstPointer();
	// ConstPointer2();
	// PointerArithmetic();
	// PointerArithmetic2();
	// PointerDecay();
	// PointerDecay2();
	// PointerArgument();
	// PointerToPointer();
	// VoidPointers();
	// PointerToWholeArray();
	// FunctionPointers();
	// PointerToMember();
	// LambdaPointers();
	// PointerToMember2();
	// VolatilePointer();
	// RawPointers();
	// PointerTemplates();

	// UniquePtr();
	// SharedPtr();
	// WeakPtr();

	return 0;
}