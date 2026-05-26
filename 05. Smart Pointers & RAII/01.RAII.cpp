#include <iostream>
#include "smart_header.h"

/*
	RAII stands for Resource Acquisition Is Initialisation. It is a 
	programming idiom where:
	=> Resource – anything that needs to be acquired and later released, such as 
				  memory, file handles, mutex locks, network sockets, database 
				  connections, etc.
	=> Acquisition – obtaining the resource, typically in an object’s constructor.
	=> Initialisation – the constructor both creates the object and acquires the resource, 
						making the object immediately usable.
	=> The destructor of the object automatically releases the resource.

	Because C++ guarantees that destructors of stack‑allocated (automatic) objects are 
	called when the object goes out of scope (even if an exception is thrown), we 
	get deterministic, leak‑free resource management. No garbage collector, no manual 
	close() calls.
*/

// 1. Basic RAII – Automatic Resource Cleanup
/*
	A class that acquires a resource in its constructor and releases it in 
	its destructor is an RAII class. Using such a class guarantees that the 
	resource is freed when the object leaves its scope, no matter how the scope is exited.
*/
namespace basicRaii{
	class Resource{
	public:
		// Constructor acquires the resource (here just prints)
		Resource(){
			std::cout << "Resource Acquired" << std::endl;
		}
		// Destructor: releases the resource (prints)
		// '~ClassName' is the destructor syntax. It is called automatically.
		~Resource(){
			std::cout << "Resource released" << std::endl;
		}
	};
}
void BasicRAII(){
	std::cout << "Entering outer block\n";
	{
		// It constructs an object r of type Resource using the default constructor.
		basicRaii::Resource r{};
		std::cout << "Using resource inside block\n";
	} // Here the scope ends. The destructor of 'r' runs automatically.
	std::cout << "After block, resource already freed\n";
}
/*
	-> The resource is acquired as soon as r exists.
	-> The resource is released exactly when r goes out of scope – we never need to 
	   call a cleanup function.
	-> Even if we return, break, continue, goto, or an exception happens inside the block,
	   the destructor will still run (except for program termination via std::exit or abort.
*/



int RAII(){
	BasicRAII();

	return 0;
}