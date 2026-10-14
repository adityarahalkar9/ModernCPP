#include <iostream>
#include <memory>       // std::unique_ptr, std::shared_ptr
#include <mutex>        // std::mutex, std::lock_guard, std::scoped_lock
#include <fstream>      // std::ifstream, std::ofstream
#include <cstdio>       // std::fopen, std::fclose (C file API)
#include <functional>   // std::function
#include <stdexcept>    // std::runtime_error
#include <string>
#include <vector>

/*
	1. WHAT IS RAII?
		RAII = Resource Acquisition Is Initialization
	   • A resource (memory, file handle, lock, socket, ...) is acquired
	     inside an object’s constructor.
	   • That same resource is released inside the object’s destructor.
	   • The resource’s lifetime is exactly the object’s lifetime.
	   • Because C++ guarantees that stack-allocated objects have their
	     destructors called when they go out of scope (even during stack
	     unwinding from an exception), resources are *never* leaked.
	 In short: Tie resource management to object lifetime.
*/

// The simplest possible RAII clas
class Resource{
public:
	/*
		CONSTRUCTOR: acquires the resource here
		Whenever this constructor runs, we know the resource is held
	*/
	Resource(){
		std::cout << "Acquire" << std::endl;
	}

	/*
		DESTRUCTOR: releases the resource here
		Whenever this runs, we know the resource is freed
	*/
	~Resource(){
		std::cout << "Release" << std::endl;
	}
};
void SimplestRAII(){
	Resource r{};
	std::cout << "in scope" << std::endl;

	// At the closing brace, r runs out of scope and ~Resource runs
}
/*
	The RAII contract - Every RAII contract obeys
	1. Constructor: acquire the resource, or throw - never leave the object half-constructed
	2. Destructor: release the resource never throw
	3. Copy: either forbid, deep-copy 
	4. Move: transfer ownership and reset the source to a safe empty state
	5. No manual release: the user of the type should not have to remember everything
*/


int RAII(){
	SimplestRAII();

	return 0;
}