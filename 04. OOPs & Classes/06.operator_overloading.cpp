#include <iostream>
#include <numbers>
#include <vector>
#include <algorithm>
#include "class_header.h"

/*
	Which operators can be overloaded? Almost all, EXCEPT:
	-> . (member access)
	-> .* (pointer‑to‑member access)
	-> :: (scope resolution)
	-> ?: (ternary conditional)
	-> sizeof, typeid, alignof
	-> casting operators (e.g., static_cast) – these are special and not overloaded

	Rules:
	-> You cannot change the precedence or associativity of an operator.
	-> You cannot change the arity (number of operands) of an operator.
	-> You cannot invent new operators (e.g., ** for exponentiation).
	-> At least one operand must be a user‑defined type (to avoid changing built‑in behavior).
	-> Some operators must be overloaded as member functions: =, [], (), ->, and type
	   conversion operators.
	-> Most binary operators can be overloaded either as member functions or as non‑member
	   functions (often friends).
*/


// Basic Operator overloading example
struct Vector2{
	float X, Y;
	Vector2(float x, float y) : X(x), Y(y) {}
	Vector2 Add(const Vector2& other) const{
		return Vector2(X + other.X, Y + other.Y);
	}
	Vector2 Multiply(const Vector2& other) const{
		return Vector2(X * other.X, Y * other.Y);
	}
	Vector2 operator+(const Vector2& other){
		return Add(other);
	}
	Vector2 operator*(const Vector2& other){
		return Multiply(other);
	}
};
std::ostream& operator<<(std::ostream& stream, const Vector2& other){
	stream << other.X << ", " << other.Y;
	return stream;
}
int BasicOverloading(){
	Vector2 position(4.0f, 4.0f);
	Vector2 speed(0.5f, 1.5f);
	Vector2 powerup(1.1f, 1.1f);

	Vector2 result = position + speed * powerup;
	std::cout << result << std::endl;

	return 0;
}



// ======================>>
/*
	This class demonstrates arithmetic, compound assignment, unary, increment/decrement,
	subscript, function call, comparison, and stream operators
*/
class Complex{
private:
	double m_real{};
	double m_imag{};
public:
	// Constructor with default arguments – can be called with zero, one, or two arguments.
	Complex(double real = 0.0, double imag = 0.0) : m_real{real}, m_imag{imag}{}

	// Accessors(getters) – useful for non‑member functions that need the values.
	double real() const{
		return m_real;
	}
	double imag() const{
		return m_imag;
	}

	// MEMBER OPERATOR OVERLOADS

	// Binary addition (member version)
	// Returns a new Complex object. Marked const because it does not modify *this.
	Complex operator+(const Complex& rhs) const{
		return Complex{m_real + rhs.m_real, m_imag + rhs.m_imag};
	}
	// Binary subtraction (member version)
	Complex operator-(const Complex& rhs) const{
		return Complex{m_real - rhs.m_real, m_imag - rhs.m_imag};
	}
	// Binary multiplication (member version)
	Complex operator*(const Complex& rhs) const{
		return Complex{
			m_real * rhs.m_real - m_imag * rhs.m_imag,
			m_real * rhs.m_imag + m_imag * rhs.m_real
		};
	}
	// Binary division(member version)
	Complex operator/(const Complex& rhs) const{
		double denominator = rhs.m_real * rhs.m_real + rhs.m_imag * rhs.m_imag;
		return Complex{
			(m_real * rhs.m_real + m_imag * rhs.m_imag) / denominator,
			(m_imag * rhs.m_real - m_real * rhs.m_imag) / denominator
		};
	}

	// Compound addition (+=)
	// Modifies *this, so returns a reference to allow chaining (e.g., a += b += c).
	Complex& operator+=(const Complex& rhs){
		m_real += rhs.m_real;
		m_imag += rhs.m_imag;
		return *this;	// return reference to the modified object
	}
	// Compound subtraction (-+)
	Complex& operator-=(const Complex& rhs){
		m_real -= rhs.m_real;
		m_imag -= rhs.m_imag;
		return *this;
	}
	// Compound multiplication (*=)
	Complex& operator*=(const Complex& rhs){
		// Reuse the multiplication operator to avoid code duplication.
		*this = *this * rhs;
		return *this;
	}
	// Compound division (/=)
	Complex& operator/=(const Complex& rhs) {
		*this = *this / rhs;
		return *this;
	}

	// Assignment operator (=)
	/*
		The compiler generates a default version that does memberwise copy,
		but we show an explicit one with self‑assignment check.
	*/
	Complex& operator=(const Complex& rhs){
		// Guard against self assignment
		if(this != &rhs){
			m_real = rhs.m_real;
			m_imag = rhs.m_imag;
		}
		return *this;
	}
	// Unary minus
	Complex operator-() const {
		return Complex{-m_real, -m_imag};
	}
	// Unary plus (rarely used, but possible)
	Complex operator+() const {
		return *this;   // just return a copy
	}

	// Prefix increment (++c)
	// Returns a reference to the modified object.
	Complex& operator++() {
		++m_real;
		return *this;
	}
	// Postfix increment (c++)
	// The dummy int parameter distinguishes postfix from prefix.
	// Returns a copy of the object before increment.
	Complex operator++(int) {
		Complex temp{*this};   // save current state
		++m_real;              // increment
		return temp;           // return old state
	}
	// Prefix decrement (--c)
	Complex& operator--() {
		--m_real;
		return *this;
	}
	// Postfix decrement (c--)
	Complex operator--(int) {
		Complex temp{*this};
		--m_real;
		return temp;
	}

	// Subscript operator [] – allows array‑like access to real/imag parts.
	// Non‑const version: returns a reference so the caller can modify the component.
	double& operator[](size_t index) {
		if(index == 0)      return m_real;
		else if(index == 1) return m_imag;
		else throw std::out_of_range{"Complex index must be 0 or 1"};
	}

	// Const version for read‑only access when the object is const.
	const double& operator[](size_t index) const {
		if(index == 0)      return m_real;
		else if(index == 1) return m_imag;
		else throw std::out_of_range{"Complex index must be 0 or 1"};
	}

	// Function call operator () – makes the object callable like a function.
	// Version with no arguments: returns the magnitude (absolute value).
	double operator()() const {
		return std::sqrt(m_real * m_real + m_imag * m_imag);
	}

	// Overloaded () that takes a bool: returns the phase angle.
	// If degrees == true, returns degrees; otherwise radians.
	double operator()(bool degrees) const {
		double pi = std::numbers::pi;
		double radians = std::atan2(m_imag, m_real);
		if(degrees) {
			return radians * 180.0 / pi;   // M_PI may need _USE_MATH_DEFINES on Windows
		}
		else {
			return radians;
		}
	}

	// FRIEND NON‑MEMBER OPERATORS
	// Friends have access to private members.

	// Stream insertion (<<)
	// Must be a non‑member because the left operand is std::ostream, not Complex.
	friend std::ostream& operator<<(std::ostream& os, const Complex& c){
		os << c.m_real;
		if(c.m_imag >= 0.0){
			os << " + " << c.m_imag << "i";
		}
		else{
			os << " - " << -c.m_imag << "i";
		}
		return os;
	}
	// Stream extraction (>>)
	// Reads two doubles separated by whitespace.
	friend std::istream& operator>>(std::istream& is, Complex& c) {
		is >> c.m_real >> c.m_imag;
		return is;
	}
	// Equality comparison (==)
	friend bool operator==(const Complex& lhs, const Complex& rhs) {
		return lhs.m_real == rhs.m_real && lhs.m_imag == rhs.m_imag;
	}
	// Inequality comparison (!=) – implemented in terms of ==
	friend bool operator!=(const Complex& lhs, const Complex& rhs) {
		return !(lhs == rhs);
	}
	// Less‑than comparison (<) – useful for ordering (e.g., in std::set)
	friend bool operator<(const Complex& lhs, const Complex& rhs) {
		// Compare real parts first, then imaginary parts.
		if(lhs.m_real < rhs.m_real) return true;
		if(lhs.m_real > rhs.m_real) return false;
		return lhs.m_imag < rhs.m_imag;
	}
};
// NON‑MEMBER ARITHMETIC OPERATORS (for mixed‑type expressions)
// These allow expressions like 2.5 + complex_obj.
Complex operator+(double lhs, const Complex& rhs) {
	return Complex{lhs} + rhs;   // convert double to Complex, then use member +
}
Complex operator-(double lhs, const Complex& rhs) {
	return Complex{lhs} - rhs;
}
Complex operator*(double lhs, const Complex& rhs) {
	return Complex{lhs} * rhs;
}
Complex operator/(double lhs, const Complex& rhs) {
	return Complex{lhs} / rhs;
}
int ComplexOverload(){
	// Create Complex objects
	Complex c1{3.0, 4.0};
	Complex c2{1.0, -2.0};
	std::cout << "c1 = " << c1 << std::endl;
	std::cout << "c2 = " << c2 << std::endl;

	// Arithmetic
	Complex sum = c1 + c2;
	Complex diff = c1 - c2;
	Complex prod = c1 * c2;
	Complex quot = c1 / c2;
	std::cout << "c1 + c2 = " << sum << std::endl;
	std::cout << "c1 - c2 = " << diff << std::endl;
	std::cout << "c1 * c2 = " << prod << std::endl;
	std::cout << "c1 / c2 = " << quot << std::endl;

	// Mixed‑type expressions (uses non‑member operators)
	Complex mix1 = 2.5 + c1;
	Complex mix2 = c1 * 3.0;
	std::cout << "2.5 + c1 = " << mix1 << std::endl;
	std::cout << "c1 * 3.0 = " << mix2 << std::endl;

	// Compound assignment
	c1 += c2;
	std::cout << "After c1 += c2: " << c1 << std::endl;
	c1 *= 2.0;
	std::cout << "After c1 *= 2.0: " << c1 << std::endl;

	// Unary and increment/decrement
	std::cout << "-c1 = " << -c1 << std::endl;
	Complex c3{5.0, 0.0};
	std::cout << "c3 = " << c3 << std::endl;
	std::cout << "c3++ = " << c3++ << " (now c3 = " << c3 << ")" << std::endl;
	std::cout << "++c3 = " << ++c3 << " (now c3 = " << c3 << ")" << std::endl;

	// Subscript operator
	c1[0] = 100.0;   // modify real part via non‑const []
	c1[1] = 200.0;   // modify imaginary part
	std::cout << "After subscript assignment: " << c1 << std::endl;

	// Function call operator
	std::cout << "Magnitude of c1: " << c1() << std::endl;
	std::cout << "Phase (radians): " << c1(false) << std::endl;
	std::cout << "Phase (degrees): " << c1(true) << std::endl;

	// Comparison
	Complex c4{100.0, 200.0};
	std::cout << std::boolalpha;   // print "true"/"false" instead of 1/0
	std::cout << "c1 == c4? " << (c1 == c4) << std::endl;
	std::cout << "c1 < c2? " << (c1 < c2) << std::endl;

	// Stream extraction (input)
	Complex c5;
	std::cout << "Enter a complex number (real imag): ";
	std::cin >> c5;
	std::cout << "You entered: " << c5 << std::endl;

	return 0;
}



// ======================>>
// Smart Pointer – Overloading *, ->, and bool
// A minimal smart pointer that owns a dynamically allocated object.
template <typename T>
class SmartPtr{
private:
	T* m_ptr{nullptr};	// raw pointer to the managed object
public:
	// Constructor: takes ownership of a raw pointer
	explicit SmartPtr(T* ptr = nullptr) : m_ptr{ptr}{}

	// Destructor: deletes the managed object
	~SmartPtr(){
		delete m_ptr;
	}
	// Disable copy semantics (unique ownership).
	SmartPtr(const SmartPtr&) = delete;
	SmartPtr& operator=(const SmartPtr&) = delete;

	// Enable move semantics.
	SmartPtr(SmartPtr&& other) noexcept : m_ptr{other.m_ptr} {
		other.m_ptr = nullptr;   // source pointer no longer owns the object
	}

	SmartPtr& operator=(SmartPtr&& other) noexcept {
		if(this != &other) {
			delete m_ptr;            // free existing resource
			m_ptr = other.m_ptr;     // transfer ownership
			other.m_ptr = nullptr;   // leave source in a valid state
		}
		return *this;
	}

	// OPERATOR OVERLOADS

	// Dereference operator (*)
	// Returns a reference to the managed object.
	T& operator*() const {
		return *m_ptr;   // dereference the raw pointer
	}

	// Arrow operator (->)
	// Returns the raw pointer so that member access works.
	T* operator->() const {
		return m_ptr;
	}

	// Conversion to bool
	// Allows the smart pointer to be used in conditions like if (sp) ...
	// The 'explicit' keyword prevents implicit conversions to bool in arithmetic.
	explicit operator bool() const {
		return m_ptr != nullptr;
	}

	// Utility to get the raw pointer (useful for interoperability).
	T* get() const {
		return m_ptr;
	}
};
// A simple test class to demonstrate -> and *
struct Test{
	int value{0};
	Test(int v) : value{v}{}
	void show() const{
		std::cout << "Test value = " << value << std::endl;
	}
};
int SmartPointerOverloads(){
	// Create a smart pointer that owns a new Test object.
	SmartPtr<Test> sp{new Test{42}};

	// Use overloaded * and -> to access members.
	(*sp).show();   // explicit dereference
	sp->show();     // arrow operator (more natural)

	// Boolean conversion
	if(sp) {
		std::cout << "sp is not null" << std::endl;
	}

	SmartPtr<Test> sp2;   // default‑constructed, holds nullptr
	if(!sp2) {
		std::cout << "sp2 is null" << std::endl;
	}

	// Move ownership
	SmartPtr<Test> sp3 = std::move(sp);
	std::cout << "After move, sp is " << (sp ? "not null" : "null") << std::endl;
	std::cout << "sp3 is " << (sp3 ? "not null" : "null") << std::endl;

	return 0;
}



// ======================>>
// Functor – Overloading ()
/*
	Function objects (functors) are classes that overload operator(). They can
	be called like functions and are widely used with STL algorithms.
*/
// A functor that multiplies a number by a fixed factor.
class MultiplyBy {
private:
	int m_factor{1};

public:
	// Constructor using brace initialization.
	explicit MultiplyBy(int factor) : m_factor{factor} {}

	// Overloaded function call operator.
	// Takes an integer and returns the product.
	int operator()(int x) const {
		return x * m_factor;
	}
};
// A functor that accumulates a running sum.
class Accumulator{
private:
	int m_sum{0};
public:
	// Overloaded () adds the argument to the sum and returns the new sum.
	int operator()(int x){
		m_sum += x;
		return m_sum;
	}

	// Getter for the current sum.
	int sum() const{
		return m_sum;
	}

	// Reset the accumulator.
	void reset(){
		m_sum = 0;
	}
};
int FunctionOverloading(){
	// Create a functor that multiplies by 3.
	MultiplyBy times3{3};
	std::cout << "3 * 5 = " << times3(5) << std::endl;
	std::cout << "3 * 10 = " << times3(10) << std::endl;

	// Use functor with std::transform to double all elements.
	std::vector<int> numbers{1, 2, 3, 4, 5};
	std::transform(numbers.begin(), numbers.end(), numbers.begin(), MultiplyBy{2});

	std::cout << "After MultiplyBy{2}: ";
	for(int n : numbers) {
		std::cout << n << " ";
	}
	std::cout << std::endl;

	// Use Accumulator functor.
	Accumulator acc;
	for(int n : numbers) {
		std::cout << "Adding " << n << ", running sum = " << acc(n) << std::endl;
	}
	std::cout << "Final sum = " << acc.sum() << std::endl;

	return 0;
}



// ======================>>
// Custom String – Overloading +, +=, ==, <, [], <<, >>
/*
	This is a minimal string class that demonstrates many common operator
	overloads.
*/
class MyString{
private:
	char* m_data{nullptr};
	size_t m_length{0};
public:
	// Default constructor: empty string.
	MyString() {
		m_data = new char[1]{'\0'};
		m_length = 0;
	}
	// Constructor from C‑style string.
	MyString(const char* str) {
		m_length = std::strlen(str);
		m_data = new char[m_length + 1];
		std::copy(str, str + m_length + 1, m_data);
	}
	// Copy constructor.
	MyString(const MyString& other) {
		m_length = other.m_length;
		m_data = new char[m_length + 1];
		std::copy(other.m_data, other.m_data + m_length + 1, m_data);
	}
	// Move constructor.
	MyString(MyString&& other) noexcept
		: m_data{other.m_data}, m_length{other.m_length} {
		other.m_data = nullptr;
		other.m_length = 0;
	}
	// Destructor.
	~MyString() {
		delete[] m_data;
	}
	// Copy assignment operator.
	MyString& operator=(const MyString& other) {
		if(this != &other) {
			delete[] m_data;                       // free old memory
			m_length = other.m_length;
			m_data = new char[m_length + 1];
			std::copy(other.m_data, other.m_data + m_length + 1, m_data);
		}
		return *this;
	}
	// Move assignment operator.
	MyString& operator=(MyString&& other) noexcept {
		if(this != &other) {
			delete[] m_data;
			m_data = other.m_data;
			m_length = other.m_length;
			other.m_data = nullptr;
			other.m_length = 0;
		}
		return *this;
	}

	// OPERATOR OVERLOADS

	// Concatenation (+)
	MyString operator+(const MyString& rhs) const {
		MyString result;
		result.m_length = m_length + rhs.m_length;
		result.m_data = new char[result.m_length + 1];
		std::copy(m_data, m_data + m_length, result.m_data);
		std::copy(rhs.m_data, rhs.m_data + rhs.m_length + 1, result.m_data + m_length);
		return result;
	}
	// Compound concatenation (+=)
	MyString& operator+=(const MyString& rhs) {
		*this = *this + rhs;   // reuse +
		return *this;
	}
	// Equality comparison (==)
	bool operator==(const MyString& rhs) const {
		return m_length == rhs.m_length && std::strcmp(m_data, rhs.m_data) == 0;
	}
	// Inequality comparison (!=)
	bool operator!=(const MyString& rhs) const {
		return !(*this == rhs);
	}
	// Less‑than comparison (<) – useful for ordering
	bool operator<(const MyString& rhs) const {
		return std::strcmp(m_data, rhs.m_data) < 0;
	}
	// Subscript operator [] (non‑const) – allows modification of characters.
	char& operator[](size_t index) {
		return m_data[index];
	}
	// Subscript operator [] (const) – read‑only access.
	const char& operator[](size_t index) const {
		return m_data[index];
	}
	// Get the length.
	size_t length() const { return m_length; }
	// Friend stream insertion (<<)
	friend std::ostream& operator<<(std::ostream& os, const MyString& s) {
		return os << s.m_data;
	}

	// Friend stream extraction (>>)
	// Reads a word (whitespace‑delimited) into the string.
	friend std::istream& operator>>(std::istream& is, MyString& s) {
		char buffer[1024]{};
		is >> buffer;
		s = MyString{buffer};   // use constructor and move assignment
		return is;
	}
};
int StringOverload(){
	MyString s1{"Hello"};
	MyString s2{" World"};

	MyString s3 = s1 + s2;
	std::cout << "Concatenated: " << s3 << std::endl;

	s1 += MyString{", C++"};
	std::cout << "After += : " << s1 << std::endl;

	s1[7] = 'c';   // modify character using non‑const []
	std::cout << "After subscript assignment: " << s1 << std::endl;

	std::cout << std::boolalpha;
	std::cout << "s1 == s3? " << (s1 == s3) << std::endl;
	std::cout << "s1 < s2? " << (s1 < s2) << std::endl;

	MyString input;
	std::cout << "Enter a word: ";
	std::cin >> input;
	std::cout << "You entered: " << input << std::endl;

	return 0;
}



// ======================>>
// Vector3D – Overloading Arithmetic and Cross Product
/*
	Demonstrates overloading binary operators, compound assignment, subscript, function
	call (norm), and using % for cross product.
*/
class Vector3D{
private:
	double m_x{0.0};
	double m_y{0.0};
	double m_z{0.0};
public:
	Vector3D(double x = 0.0, double y = 0.0, double z = 0.0) : m_x{x}, m_y{y}, m_z{z}{}

	// MEMBER OPERATORS

	// Binary addition
	Vector3D operator+(const Vector3D& rhs) const{
		return Vector3D(m_x + rhs.m_x, m_y + rhs.m_y, m_z + rhs.m_z);
	}
	// Binary subtraction
	Vector3D operator-(const Vector3D& rhs) const{
		return Vector3D{m_x - rhs.m_x, m_y - rhs.m_y, m_z - rhs.m_z};
	}
	// Scalar multiplication (vector * scalar)
	Vector3D operator*(double scalar) const{
		return Vector3D{m_x * scalar, m_y * scalar, m_z * scalar};
	}
	// Scalar division (vector / scalar)
	Vector3D operator/(double scalar) const{
		return Vector3D{m_x / scalar, m_y / scalar, m_z / scalar};
	}
	// Dot product – using * between two vectors.
	double operator*(const Vector3D& rhs) const{
		return m_x * rhs.m_x + m_y * rhs.m_y + m_z * rhs.m_z;
	}
	// Cross product – using % (a less common operator, but legal).
	Vector3D operator%(const Vector3D& rhs) const{
		return Vector3D{
			m_y * rhs.m_z - m_z * rhs.m_y,
			m_z * rhs.m_x - m_x * rhs.m_z,
			m_x * rhs.m_y - m_y * rhs.m_x
		};
	}
	// Compound addition (+=)
	Vector3D& operator+=(const Vector3D& rhs){
		m_x += rhs.m_x;
		m_y += rhs.m_y;
		m_z += rhs.m_z;
		return *this;
	}
	// Compound subtraction (-=)
	Vector3D& operator-=(const Vector3D& rhs){
		m_x -= rhs.m_x;
		m_y -= rhs.m_y;
		m_z -= rhs.m_z;
		return *this;
	}
	// Compound scalar multiplication (*=)
	Vector3D& operator*=(double scalar){
		m_x *= scalar;
		m_y *= scalar;
		m_z *= scalar;
		return *this;
	}
	// Unary minus (negation)
	Vector3D operator-() const{
		return Vector3D{-m_x, -m_y, -m_z};
	}
	// Subscript operator (non‑const) – index 0,1,2 for x,y,z.
	double& operator[](int index){
		if(index == 0)      return m_x;
		else if(index == 1) return m_y;
		else                 return m_z;
	}
	const double& operator[](int index) const{
		if(index == 0)      return m_x;
		else if(index == 1) return m_y;
		else                 return m_z;
	}
	// Function call operator – returns the magnitude (norm).
	double operator()() const{
		return std::sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
	}
	// Equality comparison
	bool operator==(const Vector3D& rhs) const{
		return m_x == rhs.m_x && m_y == rhs.m_y && m_z == rhs.m_z;
	}
	bool operator!=(const Vector3D& rhs) const{
		return !(*this == rhs);
	}

	// Stream output
	friend std::ostream& operator<<(std::ostream& os, const Vector3D& v){
		os << "(" << v.m_x << ", " << v.m_y << ", " << v.m_z << ")";
		return os;
	}
};
// Non‑member scalar * vector (for expressions like 2.5 * v)
Vector3D operator*(double scalar, const Vector3D& v) {
	return v * scalar;   // reuse member operator*
}
int VectorOverload(){
	Vector3D v1{1.0, 2.0, 3.0};
	Vector3D v2{4.0, 5.0, 6.0};

	std::cout << "v1 = " << v1 << std::endl;
	std::cout << "v2 = " << v2 << std::endl;

	std::cout << "v1 + v2 = " << v1 + v2 << std::endl;
	std::cout << "v1 - v2 = " << v1 - v2 << std::endl;
	std::cout << "v1 * 2.5 = " << v1 * 2.5 << std::endl;
	std::cout << "3.0 * v2 = " << 3.0 * v2 << std::endl;
	std::cout << "v1 · v2 (dot) = " << v1 * v2 << std::endl;
	std::cout << "v1 × v2 (cross) = " << v1 % v2 << std::endl;

	v1 += v2;
	std::cout << "After v1 += v2: " << v1 << std::endl;

	std::cout << "Norm of v1: " << v1() << std::endl;

	v1[0] = 100.0;   // modify x component
	std::cout << "After v1[0] = 100: " << v1 << std::endl;

	return 0;
}



// ======================>>
// Matrix – Overloading (), +, *, and <<
/*
	This class shows how to overload operator() for element access, arithmetic operators
	for matrix operations, and stream insertion.
*/
class Matrix{
private:
	std::vector<std::vector<double>> m_data{};
	size_t m_rows{0};
	size_t m_cols{0};
public:
	// Constructor: creates a rows × cols matrix, optionally initializing all elements.
	Matrix(size_t rows, size_t cols, double init = 0.0) : m_data(rows, std::vector<double>(cols, init)), m_rows{rows}, m_cols{cols}{}

	// OPERATOR OVERLOADS

	// Access element at (row, col) – non‑const version (allows modification).
	double& operator()(size_t row, size_t col){
		return m_data.at(row).at(col);	// at() performs bounds checking
	}
	// Const version for read‑only access.
	const double& operator()(size_t row, size_t col) const{
		return m_data.at(row).at(col);
	}

	// Matrix addition
	Matrix operator+(const Matrix& rhs) const{
		if(m_rows != rhs.m_rows || m_cols != rhs.m_cols){
			throw std::invalid_argument{"Matrix dimensions must match for addition"};
		}
		Matrix result{m_rows, m_cols};
		for(size_t i = 0; i < m_rows; ++i){
			for(size_t j = 0; j < m_cols; ++j){
				result(i, j) = (*this)(i, j) + rhs(i, j);
			}
		}
		return result;
	}
	// Matrix multiplication (this * rhs)
	Matrix operator*(const Matrix& rhs) const{
		if(m_cols != rhs.m_rows){
			throw std::invalid_argument{"Incompatible dimensions for multiplication"};
		}
		Matrix result{m_rows, rhs.m_cols};
		for(size_t i = 0; i < m_rows; ++i){
			for(size_t j = 0; j < rhs.m_cols; ++j){
				double sum = 0.0;
				for(size_t k = 0; k < m_cols; ++k){
					sum += (*this)(i, k) * rhs(k, j);
				}
				result(i, j) = sum;
			}
		}
		return result;
	}
	// Scalar multiplication (matrix * scalar)
	Matrix operator*(double scalar) const{
		Matrix result{m_rows, m_cols};
		for(size_t i = 0; i < m_rows; ++i){
			for(size_t j = 0; j < m_cols; ++j){
				result(i, j) = (*this)(i, j) * scalar;
			}
		}
		return result;
	}

	// Equality comparison
	bool operator==(const Matrix& rhs) const{
		return m_rows == rhs.m_rows && m_cols == rhs.m_cols && m_data == rhs.m_data;
	}
	// Stream output – prints the matrix row by row.
	friend std::ostream& operator<<(std::ostream& os, const Matrix& m){
		for(size_t i = 0; i < m.m_rows; ++i){
			os << "[ ";
			for(size_t j = 0; j < m.m_cols; ++j) {
				os << m(i, j) << " ";
			}
			os << "]" << std::endl;
		}
		return os;
	}
	// Getters for dimensions.
	size_t rows() const{
		return m_rows;
	}
	size_t cols() const{
		return m_cols;
	}
};
// Non‑member scalar * matrix (for expressions like 2.0 * mat)
Matrix operator*(double scalar, const Matrix& m) {
	return m * scalar;   // reuse member operator*
}
int MatrixOverload(){
	// Create a 2x3 matrix initialized to 1.0, then modify some elements.
	Matrix A{2, 3, 1.0};
	A(0, 1) = 2.0;
	A(1, 2) = 3.0;
	std::cout << "Matrix A:" << std::endl << A;

	// Create a 3x2 matrix initialized to 2.0, then modify.
	Matrix B{3, 2, 2.0};
	B(1, 0) = 0.0;
	std::cout << "Matrix B:" << std::endl << B;

	Matrix C = A * B;
	std::cout << "A * B:" << std::endl << C;

	Matrix D = A * 2.5;
	std::cout << "A * 2.5:" << std::endl << D;

	Matrix E = 0.5 * B;
	std::cout << "0.5 * B:" << std::endl << E;

	return 0;
}



// ======================>>
// Overloading new and delete (Class‑Specific)
// You can customize memory allocation for a specific class.
class Traced{
private:
	int m_id{0};
	static inline int s_counter{0};
public:
	Traced() : m_id{++s_counter}{
		std::cout << "Traced #" << m_id << " constructed" << std::endl;
	}
	~Traced(){
		std::cout << "Traced #" << m_id << " destroyed" << std::endl;
	}
	// Class-specific operator new (single object)
	static void* operator new(size_t size){
		std::cout << "Custom new: allocating " << size << " bytes" << std::endl;
		return std::malloc(size);
	}
	// Class-specific operator delete (single object)
	static void operator delete(void* ptr){
		std::cout << "Custom delete: freeing memory" << std::endl;
		std::free(ptr);
	}
	// Class‑specific operator new[] (array)
	static void* operator new[](size_t size){
		std::cout << "Custom new[]: allocating " << size << " bytes" << std::endl;
		return std::malloc(size);
	}
	// Class‑specific operator delete[] (array)
	static void operator delete[](void* ptr){
		std::cout << "Custom delete[]: freeing memory" << std::endl;
		std::free(ptr);
	}
};
int OverloadingNewDelete(){
	std::cout << "--- Single object ---" << std::endl;
	Traced* p = new Traced;
	delete p;
	std::cout << std::endl << "--- Array of objects ---" << std::endl;
	Traced* arr = new Traced[3];
	delete[] arr;

	return 0;
}



// ======================>>
// Type Conversion Operators
// Allows implicit or explicit conversion of your class to another type.
class Fraction{
private:
	int m_num{0};
	int m_den{1};
public:
	Fraction(int num = 0, int den = 1) : m_num{num}, m_den{den}{
		if(m_den == 0){
			throw std::invalid_argument{"Denominator cannot be zero"};
		}
	}

	// TYPE CONVERSION OPERATORS

	// Implicit conversion to double
	operator double() const{
		return static_cast<double>(m_num) / m_den;
	}
	// Implicit conversion to float
	operator float() const{
		return static_cast<float>(m_num) / m_den;
	}
	// Explicit conversion to int (truncates toward zero)
	explicit operator int() const{
		return m_num / m_den;
	}
	// Stream output for easy printing
	friend std::ostream& operator<<(std::ostream& os, const Fraction& f){
		return os << f.m_num << "/" << f.m_den;
	}
};
int OverloadingTypeConversionOperators(){
	Fraction f{3, 4};
	std::cout << "Fraction: " << f << std::endl;

	double d = f;                 // implicit conversion to double
	float fl = f;                 // implicit conversion to float
	int i = static_cast<int>(f);  // explicit conversion required (cannot be implicit)

	std::cout << "As double: " << d << std::endl;
	std::cout << "As float:  " << fl << std::endl;
	std::cout << "As int (truncated): " << i << std::endl;

	return 0;
}



// ======================>>
// Overloading ->* (Pointer‑to‑Member)
/*
	This is an advanced feature. The example shows a wrapper that allows using ->*
	with member pointers.
*/
namespace overloadPointerMember{
	class Test{
	public:
		int value{42};
		void show() const{
			std::cout << "Value = " << value << std::endl;
		}
	};
	// A wrapper that holds a pointer to T and overloads ->*
	template <typename T>
	class PointerWrapper{
	private:
		T* m_ptr{nullptr};
	public:
		explicit PointerWrapper(T* ptr) : m_ptr{ptr}{}

		// Overload ->* for member data pointers (e.g., &Test::value)
		auto& operator->*(int T::* memberPtr){
			return m_ptr->*memberPtr;
		}

		// Overload ->* for const member function pointers
		// Returns a lambda that captures 'this' and the member pointer.
		template <typename Ret, typename... Args>
		auto operator->*(Ret(T::* memberFunc)(Args...) const) const {
			// The lambda will call the member function on the stored object.
			return [this, memberFunc](Args... args) -> Ret{
				return (m_ptr->*memberFunc)(std::forward<Args>(args)...);
				};
		}
	};
}
int OverloadingPointerToMember(){
	overloadPointerMember::Test obj{100};
	overloadPointerMember::PointerWrapper<overloadPointerMember::Test> pw{&obj};

	// Using ->* with a pointer to data member
	int overloadPointerMember::Test::* pValue = &overloadPointerMember::Test::value;
	std::cout << "pw->*pValue = " << (pw->*pValue) << std::endl;
	pw->*pValue = 200;
	std::cout << "After assignment, obj.value = " << obj.value << std::endl;

	// Using ->* with a pointer to member function
	void (overloadPointerMember::Test:: * pShow)() const = &overloadPointerMember::Test::show;
	(pw->*pShow)();   // Calls obj.show()

	return 0;
}



int OperatorOverloading(){
	// BasicOverloading();
	// ComplexOverload();
	// SmartPointerOverloads();
	// FunctionOverloading();
	// StringOverload();
	// VectorOverload();
	// MatrixOverload();
	// OverloadingNewDelete();
	// OverloadingTypeConversionOperators();
	OverloadingPointerToMember();

	return 0;
}

/*
	Summary of Best Practices

	-> Keep semantics intuitive: Overload operators to do what users
	   expect (e.g., + should add, not subtract).
	-> Maintain symmetry: For binary operators that can accept different types on
	   either side, implement non‑member versions.
	-> Return references where appropriate: Compound assignment (+=) should return
	   *this by reference.
	-> Use const correctly: Mark member operators that don't modify the object as const.
	-> Avoid overloading &&, ||, and , unless you have a very compelling reason; they lose
	   short‑circuit evaluation.
	-> Prefer non‑member friend functions for operators that need access to private data
	   but should work symmetrically (e.g., +, ==, <<).
*/