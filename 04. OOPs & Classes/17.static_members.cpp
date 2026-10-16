import std;
#include "class_header.h"

// Static data members
class Counter {
public:
    // Static data member: shared by ALL objects
    static int object_count;
    static inline int inline_count = 0;  // C++17: inline static

    // Regular member: each object has its own
    int instance_id;

    Counter() : instance_id(++object_count) {
        ++inline_count;
        std::cout << "  Counter #" << instance_id << " created" << std::endl;
    }

    ~Counter() {
        --inline_count;
    }

    static int get_count() {
        return object_count;
    }
};

// Definition of static member (pre-C++17)
int Counter::object_count = 0;

void StaticDataMembers() {
    std::cout << "=== Static Data Members ===" << std::endl;

    std::cout << "Before creating: count = " << Counter::get_count() << std::endl;

    {
        Counter c1;
        Counter c2;
        Counter c3;

        std::cout << "\nAfter creating 3: count = " << Counter::get_count() << std::endl;
        std::cout << "Inline count: " << Counter::inline_count << std::endl;
    }

    std::cout << "After scope: inline count = " << Counter::inline_count << std::endl;
}

// Static member functions
class MathUtility {
public:
    // Static member function: no 'this' pointer
    static double square(double x) {
        return x * x;
    }

    static double cube(double x) {
        return x * x * x;
    }

    // Can access static members
    static int call_count;
    static void track_call() {
        ++call_count;
    }

    // CANNOT access non-static members
    // static void bad_function() {
    //     non_static_member = 42;  // ERROR: no this pointer
    // }

private:
    double non_static_member = 0;  // Instance member
};

int MathUtility::call_count = 0;

void StaticMemberFunctions() {
    std::cout << "\n=== Static Member Functions ===" << std::endl;

    // Call without object instance
    std::cout << "square(5) = " << MathUtility::square(5) << std::endl;
    std::cout << "cube(3) = " << MathUtility::cube(3) << std::endl;

    // Can also call through instance (but not recommended)
    MathUtility mu;
    std::cout << "Through instance: " << mu.square(4) << std::endl;

    std::cout << "\nStatic function characteristics:" << std::endl;
    std::cout << "  - No 'this' pointer" << std::endl;
    std::cout << "  - Can access static members" << std::endl;
    std::cout << "  - CANNOT access non-static members" << std::endl;
    std::cout << "  - Cannot be virtual" << std::endl;
}

// Singleton pattern with static members
class Singleton {
private:
    Singleton() {
        std::cout << "  Singleton constructed" << std::endl;
    }

public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static Singleton& instance() {
        static Singleton the_instance;  // Thread-safe in C++11
        return the_instance;
    }

    void do_something() {
        std::cout << "  Singleton doing something" << std::endl;
    }
};

void SingletonPattern() {
    std::cout << "\n=== Singleton Pattern ===" << std::endl;

    Singleton::instance().do_something();
    Singleton::instance().do_something();

    // Both calls return same instance
    std::cout << "\nSame instance: "
        << (&Singleton::instance() == &Singleton::instance()) << std::endl;
}

// Static constants
class Config {
public:
    static constexpr int MAX_USERS = 1000;
    static constexpr double PI = 3.14159265358979;
    static constexpr const char* VERSION = "1.0.0";

    // C++17: inline variables (no out-of-class definition needed)
    static inline const std::string APP_NAME = "MyApp";
};

void StaticConstants() {
    std::cout << "\n=== Static Constants ===" << std::endl;

    std::cout << "MAX_USERS: " << Config::MAX_USERS << std::endl;
    std::cout << "PI: " << Config::PI << std::endl;
    std::cout << "VERSION: " << Config::VERSION << std::endl;
    std::cout << "APP_NAME: " << Config::APP_NAME << std::endl;
}

// Static members in templates
template <typename T>
class TemplateCounter {
public:
    static int count;

    TemplateCounter() { ++count; }
    ~TemplateCounter() { --count; }

    static int get_count() { return count; }
};

template <typename T>
int TemplateCounter<T>::count = 0;

void StaticInTemplates() {
    std::cout << "\n=== Static in Templates ===" << std::endl;

    std::cout << "Each instantiation gets separate static:" << std::endl;

    TemplateCounter<int> ti1, ti2;
    TemplateCounter<double> td1;

    std::cout << "int instances: " << TemplateCounter<int>::get_count() << std::endl;
    std::cout << "double instances: " << TemplateCounter<double>::get_count() << std::endl;

    std::cout << "\nEach template instantiation has own statics" << std::endl;
}

// Thread-safe static (C++11 magic statics)
class ThreadSafeInit {
public:
    ThreadSafeInit() {
        std::cout << "  ThreadSafeInit constructed (guaranteed once)" << std::endl;
    }

    static ThreadSafeInit& get() {
        static ThreadSafeInit instance;  // Thread-safe initialization
        return instance;
    }
};

void ThreadSafeStatics() {
    std::cout << "\n=== Thread-Safe Statics ===" << std::endl;

    std::cout << "C++11 guarantees:" << std::endl;
    std::cout << "  Static local variables initialized once" << std::endl;
    std::cout << "  Even if accessed from multiple threads" << std::endl;
    std::cout << "  Other threads wait for initialization" << std::endl;

    ThreadSafeInit::get();
    ThreadSafeInit::get();  // Returns existing, doesn't reconstruct
}

// Static member for tracking
class Tracker {
public:
    static std::atomic<int> active_objects;

    Tracker() {
        ++active_objects;
        std::cout << "  +1: " << active_objects.load() << " active" << std::endl;
    }

    ~Tracker() {
        --active_objects;
        std::cout << "  -1: " << active_objects.load() << " active" << std::endl;
    }
};

std::atomic<int> Tracker::active_objects{0};

void TrackingWithStatic() {
    std::cout << "\n=== Tracking with Statics ===" << std::endl;

    std::cout << "Creating objects:" << std::endl;
    Tracker t1;
    Tracker t2;
    Tracker t3;

    std::cout << "\nDestroying (scope end):" << std::endl;
}