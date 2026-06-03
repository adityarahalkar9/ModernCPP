#include <iostream>
#include "smart_header.h"
#include <stdexcept>		 // for std::runtime_error
#include <memory>			 // for std::unique_ptr, std::make_unique
#include <string>
#include <condition_variable>
#include <fstream>
#include <mutex>
#include <shared_mutex>
#include <thread>

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


// Exception Safety – Destructors Are Called During Stack Unwinding
// A smalltracer class that tells us when it is created
struct Tracer{
	std::string name{};		// stores a name for identification

	// Constructor: takes a string by value, then moves it into the member.
	// std::move cast to rvalue to enable move semantics (efficient transfer).
	Tracer(std::string n) : name{std::move(n)}{
		std::cout << name << "Constructed" << std::endl;
	}
	// Destructor: called when the object is destroyed.
	~Tracer(){
		std::cout << name << "Destroyed" << std::endl;
	}
};
// A Function that may throw
void Might_throw(bool do_throw){
	Tracer a{"A"};		// created, will be destroyed on scope exit
	Tracer b{"B"};		// created, will be destroyed
	if(do_throw){
		throw std::runtime_error{"Something went wrong"};	// exception thrown here
	}
	Tracer c{"C"};		// This line will NOT be executed if exception thrown
} // End of function - normal scope exit destroys a,b,c in reverse order 
int Exception_error(){
	try{
		std::cout << "Calling might_throw(true)" << std::endl;
		Might_throw(true);
	}
	catch(const std::exception& e){
		std::cout << "Caught exception: " << e.what() << std::endl;
	}
	std::cout << "Back in main" << std::endl;

	return 0;
}
/*
	-> c is never constructed because the throw happened before its declaration.
	-> b and a are destroyed in reverse order (B first, then A) even though an exception 
	   interrupted normal flow.
	-> This is RAII in action – no resources are leaked.
*/


// Standard Library RAII Classes
/*
	C++ provides many ready‑made RAII types. Using them is the first choice before 
	writing your own.
*/
// 1. std::unique_ptr – Exclusive Ownership of Dynamic Memory
/*
	-> std::unique_ptr<T> is an RAII class that owns a dynamically allocated object 
	   of type T.
	-> It is move‑only (cannot be copied) because only one unique_ptr should own 
	   the resource at a time.
	-> The destructor calls delete (or a custom deleter) on the owned pointer automatically.
	-> std::make_unique<T>(args...) creates a T and wraps it in a unique_ptr in one 
	   safe step.
	-> explicit – used on constructors to prevent implicit conversions.
	-> std::move – converts an lvalue to an rvalue, enabling move semantics (transfer of ownership).
	-> nullptr – the null pointer literal; better than NULL or 0.
*/
struct Widget{
	int id{};
	// 'explicit' forbids the compiler from using thisconstrcutor for implicit conversions
	// It forces users to write Widget{42} instead of accidentally converting 42 to Widget.
	explicit Widget(int i) : id{i}{
		std::cout << "Widget " << id << " Constructed" << std::endl;
	}
	~Widget(){
		std::cout << "Widget " << id << " Destroyed" << std::endl;
	}
};
int StandardLibraryRAIIClasses(){
	// Create a unique_ptr owning a Widget with id = 100.
	// std::make_unique<Widget>(100) forwards 100 to the Widget constructor.
	auto p1 = std::make_unique<Widget>(100);
	std::cout << "Widget id via p1: " << p1->id << std::endl;
	{
		// p2 is another unique_ptr. We cannot copy p1, but we can move it.
		// std::move(p1) transfers ownership: after this, p1 is empty (holds nullptr).
		auto p2 = std::move(p1);	// p2 now owns the Widget, p1 == nullptr
		if(!p1){
			std::cout << "p1 is now null after move" << std::endl;
		}
		// p2 goes out of scope here, so the Widget is deleted
	}
	// p1 is null, but its destructor is harmless (deleted nullptr is safe)
	std::cout << "End of main" << std::endl;

	return 0;
}
/*
	-> std::unique_ptr automatically deletes the object when the pointer goes out of scope.
	-> Moving ownership with std::move.
	-> make_unique avoids manual new and is exception‑safe.
*/
// 2. std::shared_ptr and std::weak_ptr – Shared Ownership and Cycle Breaking
/*
	-> std::shared_ptr<T> – an RAII class that manages a shared resource via reference 
	   counting. When the last shared_ptr pointing to the resource is destroyed, 
	   the resource is deleted.
	-> std::weak_ptr<T> – a smart pointer that does not increase the reference count. 
	   It is used to observe a shared resource, and to break cyclic references that 
	   would otherwise prevent deletion.
	-> std::enable_shared_from_this – a base class that allows an object to safely 
	   obtain a shared_ptr to itself via shared_from_this().
	-> this – a pointer to the current object.
	-> override – indicates that a virtual function overrides a base class virtual function. Not strictly needed here but good practice.
	-> use_count() – returns the number of shared_ptr instances sharing ownership.
*/
// A node that can point to next and previous nodes.
// To break cycles, the 'prev' pointer is a weak_ptr.
struct Node : std::enable_shared_from_this<Node>{
	int value{};
	std::shared_ptr<Node> next;
	std::weak_ptr<Node> prev;	   // weak_ptr does not own the node, avoids cycle
	
	explicit Node(int v) : value{v}{
		std::cout << "Node " << value << " created";
	}
	~Node(){
		std::cout << "Node " << value << " destroyed";
	}
	// A member function that returns a shared_ptr to this node.
	std::shared_ptr<Node> get_shared(){
		// shared_from_this() is inherited from enable_shared_from_this.
		// It works only if the object is already managed by a shared_ptr.
		return shared_from_this();
	}
};
int SharedPtrWeakPtr(){
	// Create two nodes managed by shared_ptr.
	auto n1 = std::make_shared<Node>(1);
	auto n2 = std::make_shared<Node>(2);

	n1->next = n2;		// n1's next points to n2 (shared ownership)
	n2->prev = n1;		// n2's prev observes n1 (weak_ptr, no ownership)

	std::cout << "n1 use count: " << n1.use_count() << std::endl;	// 1 (only n1 owns it)
	std::cout << "n2 use count: " << n2.use_count() << std::endl;   // 2 (n2 and n1->next)

	// Use shared_from_this to get another shared_ptr to n1.
	auto n1_shared = n1->get_shared();
	std::cout << "n1 use count after shared_from_this: " << n1.use_count() << std::endl;	// 2
	// n1_shared goes out of scope, count drops.

	return 0;
}
/*
	Why weak_ptr? If prev were also a shared_ptr, then n1 (via prev) and 
	n2 (via next) would hold strong references to each other. When n1 and n2 
	are destroyed, their internal shared_ptrs would keep the nodes alive → memory leak.
	The weak_ptr breaks the cycle.
*/
// 3.3 File Streams – std::fstream
/*
	-> std::ifstream – input file stream.
	-> std::ofstream – output file stream.
	-> is_open() – checks if the file is open.
	-> operator<< – formatted output.
	-> std::getline – reads a line into a string.
*/
void FileStream(){
	// Write to a file. The file is opened when the ofstrea, object is created.
	{
		std::ofstream outfile{"example.txt"};    // brace init, filenames "example.txt"
		if(!outfile){	// if file couldn't be opened
			std::cerr << "Error opening file for writing" << std::endl;
		}
		outfile << "Hello, RAII file handling";
		// No need to close; the destructor will close the file when outfile goes out of scope.
	}   // outfile destroyed → file closed
	// Now read the file back
	{
		std::ifstream infile{"example.txt"};
		if(!infile){
			std::cerr << "Error opening file for reading\n";
		}
		// Again, file closed automatically when infile is destroyed.
	}
}

// 4. Mutex Locks – std::lock_guard, std::scoped_lock, std::unique_lock, std::shared_lock
/*
	-> std::mutex – a mutual exclusion primitive. lock() acquires it, unlock() releases it.
	-> std::lock_guard<std::mutex> – an RAII wrapper that locks a mutex in its constructor 
	   and unlocks it in the destructor. Non‑copyable, non‑movable.
	-> std::scoped_lock – like lock_guard but can lock multiple mutexes 
	   simultaneously using a deadlock‑avoidance algorithm.
	-> std::unique_lock<std::mutex> – a more flexible RAII lock: it can be deferred, 
	   unlocked and relocked, and transferred (moved). Required for condition variables.
	-> std::shared_lock<std::shared_mutex> – for shared mutexes that allow multiple 
	   concurrent readers.
	-> std::defer_lock – tag used with unique_lock to tell the constructor not to lock 
	   the mutex immediately.
	-> try_lock() – attempts to lock, returns true if successful.
	-> std::adopt_lock – assume the mutex is already locked (used to transfer ownership).
*/
namespace mutexlocks{
	std::mutex mtx1, mtx2;			 // two mutexes
	std::shared_mutex shared_mtx;	 // a mutex that allows shared (read) locks
	int data{};						 // shared data, brace init to 0

	void MutexLocks(){
		// 1. lock_guard – simplest, scoped lock
		{
			// 'std::lock_guard guard{mtx1};' locks mtx1.
			std::lock_guard guard{mtx1};	// mtx1.lock() called here
			++data;
			// destructor of guard calls mtx1.unlock()
		}

		// 2. scoped_lock - locks multiple mutexes atomically
		{
			std::scoped_lock{mtx1, mtx2};	// locks both without deadlock risk
			// safe access using both mutexes
		} // both unlocked

		// 3. unique_lock – defer locking, then possibly lock later
		{
			std::unique_lock u1{mtx1, std::defer_lock};    // does NOT lock yet
			// do some non-critical work...
			u1.lock();      // now manually lock
			// .... critical section
			u1.unlock();	// we can unlcok early
			// destructor will unlock if still locked (but it's already unlocked, safe)
		}

		// 4. shared_lock – multiple readers using a shared_mutex
		{
			std::shared_lock read_lock{shared_mtx};		// acquires shared ownership
			int local = data;	// read safely
			// other threads can also hold shared_locks concurrently
		}   // shared lock released
		// Note: condition_variable (later) requires unique_lock
	}

	// Condition Variable Example
	std::mutex cv_mtx;
	std::condition_variable cv;
	bool ready{false};

	void Waiter(){
		std::unique_lock lock{cv_mtx};		  // lock must be unique_lock
		// wait() unloacks the mutex and sleeps until notified, then re_locks
		cv.wait(lock, []{ return ready; });	  // lamba cheacks predicate
		std::cout << "Notified, ready is true";
	}
	void Signal() {
		{
			std::lock_guard guard{cv_mtx};
			ready = true;
		}
		cv.notify_one();  // wake up one waiter
	}
	int ConditionVariable(){
		std::jthread t1{Waiter};	// jthread - auto-join (next section)
		std::jthread t2{Signal};
		// both threads join automatically

		return 0;
	}
}



int RAII(){
	// BasicRAII();
	// Exception_error();
	// StandardLibraryRAIIClasses();
	// SharedPtrWeakPtr();
	mutexlocks::MutexLocks();

	return 0;
}