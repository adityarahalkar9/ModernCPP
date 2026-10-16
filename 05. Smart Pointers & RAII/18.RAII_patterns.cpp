import std;
#include "smart_pointers_header.h"

// RAII = Resource Acquisition Is Initialization
class RAIIResource {
public:
    explicit RAIIResource(std::string name) : name_(std::move(name)) {
        std::cout << "  [ACQUIRE] " << name_ << std::endl;
    }

    ~RAIIResource() {
        std::cout << "  [RELEASE] " << name_ << std::endl;
    }

    void use() const {
        std::cout << "  [USE] " << name_ << std::endl;
    }

private:
    std::string name_;
};

void RAII Basics() {
    std::cout << "=== RAII Basics ===" << std::endl;

    std::cout << "RAII principle:" << std::endl;
    std::cout << "  1. Acquire resource in constructor" << std::endl;
    std::cout << "  2. Release in destructor" << std::endl;
    std::cout << "  3. Tie resource lifetime to object lifetime" << std::endl;

    std::cout << "\nDemonstration:" << std::endl;
    {
        RAIIResource res("demo_resource");
        res.use();
    }  // Automatically released

    std::cout << "\nEven with early return:" << std::endl;
    auto example = [](bool early) {
        RAIIResource res("early_return");
        if(early) {
            std::cout << "  Returning early..." << std::endl;
            return;
        }
        res.use();
        };

    example(true);
    example(false);
}

// RAII with exceptions
void RAIIWithExceptions() {
    std::cout << "\n=== RAII with Exceptions ===" << std::endl;

    auto risky = [](bool should_throw) {
        RAIIResource res("exception_test");

        if(should_throw) {
            throw std::runtime_error("Test exception");
        }

        std::cout << "  No exception" << std::endl;
        };

    std::cout << "With exception:" << std::endl;
    try {
        risky(true);
    }
    catch(const std::exception& e) {
        std::cout << "  Caught: " << e.what() << std::endl;
        std::cout << "  (Resource still released)" << std::endl;
    }
}

// Classic RAII examples
class FileGuard {
    std::fstream file_;
    std::string filename_;

public:
    explicit FileGuard(const std::string& filename)
        : filename_(filename) {
        file_.open(filename);
        if(!file_.is_open()) {
            throw std::runtime_error("Cannot open: " + filename);
        }
        std::cout << "  [FILE OPEN] " << filename_ << std::endl;
    }

    ~FileGuard() {
        if(file_.is_open()) {
            file_.close();
            std::cout << "  [FILE CLOSE] " << filename_ << std::endl;
        }
    }

    void write(const std::string& data) {
        file_ << data << std::endl;
    }

    // Non-copyable (resource is unique)
    FileGuard(const FileGuard&) = delete;
    FileGuard& operator=(const FileGuard&) = delete;
};

void RAIIFileExample() {
    std::cout << "\n=== RAII File Example ===" << std::endl;

    try {
        FileGuard file("raii_test.txt");
        file.write("Hello from RAII!");
        file.write("Second line");
        std::cout << "  File written successfully" << std::endl;
    }
    catch(const std::exception& e) {
        std::cout << "  Error: " << e.what() << std::endl;
    }
    // File automatically closed
}

// Mutex lock guard
void RAIIMutexExample() {
    std::cout << "\n=== RAII Mutex Example ===" << std::endl;

    std::mutex mtx;

    {
        std::cout << "  Acquiring lock..." << std::endl;
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "  [CRITICAL SECTION]" << std::endl;
        // Lock automatically released
    }

    std::cout << "  Lock released" << std::endl;
}

// Custom RAII wrapper
template <typename T, typename Deleter>
class ScopedResource {
public:
    explicit ScopedResource(T* ptr, Deleter deleter)
        : ptr_(ptr), deleter_(deleter) {
        std::cout << "  Acquired resource" << std::endl;
    }

    ~ScopedResource() {
        if(ptr_) {
            deleter_(ptr_);
            std::cout << "  Released resource" << std::endl;
        }
    }

    // Non-copyable
    ScopedResource(const ScopedResource&) = delete;
    ScopedResource& operator=(const ScopedResource&) = delete;

    // Movable
    ScopedResource(ScopedResource&& other) noexcept
        : ptr_(other.ptr_), deleter_(std::move(other.deleter_)) {
        other.ptr_ = nullptr;
    }

    T* get() const { return ptr_; }
    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }

private:
    T* ptr_;
    Deleter deleter_;
};

void CustomRAIIWrapper() {
    std::cout << "\n=== Custom RAII Wrapper ===" std::endl;

    auto custom_delete = [](int* p) {
        std::cout << "    Custom deleting: " << *p << std::endl;
        delete p;
        };

    {
        ScopedResource<int, decltype(custom_delete)> res(
            new int(42), custom_delete);
        std::cout << "  Value: " << *res << std::endl;
    }
    // Automatically released
}

// RAII design principles
void RAIIDesignPrinciples() {
    std::cout << "\n=== RAII Design Principles ===" << std::endl;

    std::cout << "1. Every resource should have a class:" << std::endl;
    std::cout << "   - Memory → unique_ptr, shared_ptr" << std::endl;
    std::cout << "   - Files → fstream, FileGuard" << std::endl;
    std::cout << "   - Locks → lock_guard, unique_lock" << std::endl;
    std::cout << "   - Sockets → custom RAII class" << std::endl;
    std::cout << std::endl;

    std::cout << "2. Constructor acquires, destructor releases" << std::endl;
    std::cout << std::endl;

    std::cout << "3. Non-copyable if resource is unique" << std::endl;
    std::cout << "   Or implement deep copy for value semantics" << std::endl;
    std::cout << std::endl;

    std::cout << "4. Exception safe by default" << std::endl;
    std::cout << "   If constructor throws, destructor not called" << std::endl;
    std::cout << "   Already-acquired resources are released" << std::endl;
}