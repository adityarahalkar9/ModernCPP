#include <iostream>
#include <typeinfo>
#include "basic_header.h"

// C++11 auto: deduces type from initializer
void AutoBasic(){	
	auto i = 42;		// int
	auto d = 3.13;		// double
	auto s = "hello";	// const char*
	auto b = true;		// bool
	auto c = 'A';		// char

	std::cout << "auto i: " << typeid(i).name() << " = " << i << std::endl;
	std::cout << "auto d: " << typeid(d).name() << " = " << d << std::endl;
	std::cout << "auto s: " << typeid(s).name() << " = " << s << std::endl;
	std::cout << "auto b: " << typeid(b).name() << " = " << b << std::endl;
	std::cout << "auto c: " << typeid(c).name() << " = " << c << std::endl;
}

// auto with references and const
void AutoWithModifiers(){
	int x{10};
	const int cx{20};
	const int& rx{cx};

	auto a{x};			// int (copy)
	auto b{cx};			// int (const is dropped)
	auto c{rx};			// int (reference is dropped)
	auto& d{x};			// int& (reference preserved)
	const auto& e{x};	// const int&
}


int TypeDeduction(){
	AutoBasic();

	return 0;
}