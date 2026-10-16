import std;
#include "class_header.h"

// Proper polymorphic destruction
class Resource {
public:
    Resource() { std::cout << "  Resource acquired" << std::endl; }
    virtual ~Resource() {
        std::cout << "  Resource released" << std::endl;
    }

    virtual void use() = 0;
};

class FileResource : public Resource {
    std::string filename;

public:
    FileResource(std::string f) : filename(std::move(f)) {
        std::cout << "  FileResource(" << filename << ") constructed" << std::endl;
    }

    ~FileResource() override {
        std::cout << "  FileResource(" << filename << ") destroyed" << std::endl;
    }

    void use() override {
        std::cout << "  Using file: " << filename << std::endl;
    }
};

class NetworkResource : public Resource {
    std::string host;

public:
    NetworkResource(std::string h) : host(std::move(h)) {
        std::cout << "  NetworkResource(" << host << ") constructed" << std::endl;
    }

    ~NetworkResource() override {
        std::cout << "  NetworkResource(" << host << ") destroyed" << std::endl;
    }

    void use() override {
        std::cout << "  Using network: " << host << std::endl;
    }
};

void ProperDestruction() {
    std::cout << "=== Polymorphic Destruction ===" << std::endl;

    std::cout << "Virtual destructor ensures:" << std::endl;
    std::cout << "  Most-derived destructor runs" << std::endl;
    std::cout << "  Resources properly released" << std::endl;

    std::cout << "\nCreating through base pointer:" << std::endl;
    {
        std::unique_ptr<Resource> res = std::make_unique<FileResource>("test.txt");
        res->use();
    }  // Proper destruction

    std::cout << "\nVector of polymorphic objects:" << std::endl;
    {
        std::vector<std::unique_ptr<Resource>> resources;
        resources.push_back(std::make_unique<FileResource>("a.txt"));
        resources.push_back(std::make_unique<NetworkResource>("example.com"));

        for(auto& r : resources) {
            r->use();
        }
    }  // All properly destroyed
}

// Destruction order in inheritance
class Base2 {
public:
    Base2() { std::cout << "  Base2 constructed" << std::endl; }
    virtual ~Base2() { std::cout << "  Base2 destroyed" << std::endl; }
};

class Member {
public:
    Member() { std::cout << "  Member constructed" << std::endl; }
    ~Member() { std::cout << "  Member destroyed" << std::endl; }
};

class Derived3 : public Base2 {
    Member member;

public:
    Derived3() { std::cout << "  Derived3 constructed" << std::endl; }
    ~Derived3() override { std::cout << "  Derived3 destroyed" << std::endl; }
};

void DestructionOrder() {
    std::cout << "\n=== Destruction Order ===" << std::endl;

    std::cout << "Construction: Base → Members → Derived body" << std::endl;
    std::cout << "Destruction: Derived body → Members → Base" << std::endl;

    std::cout << "\nDemonstrating:" << std::endl;
    {
        Derived3 d;
    }
}

// RAII with polymorphism
class RAIIResource {
    std::unique_ptr<Resource> resource;

public:
    explicit RAIIResource(std::unique_ptr<Resource> r)
        : resource(std::move(r)) {
        std::cout << "  RAIIResource constructed" << std::endl;
    }

    ~RAIIResource() {
        std::cout << "  RAIIResource destroyed" << std::endl;
    }

    void use() {
        if(resource) {
            resource->use();
        }
    }
};

void RAIIWithPolymorphism() {
    std::cout << "\n=== RAII with Polymorphism ===" << std::endl;

    std::cout << "Smart pointers handle polymorphic destruction:" << std::endl;

    {
        RAIIResource raii(std::make_unique<FileResource>("doc.pdf"));
        raii.use();
    }  // Automatic cleanup
}

// Exception safety in destruction
class SafeResource {
    std::vector<int> data;

public:
    SafeResource() {
        data.reserve(1000000);  // May throw bad_alloc
        std::cout << "  SafeResource constructed" << std::endl;
    }

    ~SafeResource() {
        std::cout << "  SafeResource destroyed" << std::endl;
    }
};

void ExceptionSafety() {
    std::cout << "\n=== Exception Safety ===" << std::endl;

    std::cout << "If constructor throws:" << std::endl;
    std::cout << "  Destructor NOT called" << std::endl;
    std::cout << "  Already-constructed members destroyed" << std::endl;

    std::cout << "\nIf destructor throws:" << std::endl;
    std::cout << "  DANGEROUS during stack unwinding" << std::endl;
    std::cout << "  Destructors should be noexcept" << std::endl;
}

// Virtual destructor anti-patterns
void AntiPatterns() {
    std::cout << "\n=== Anti-Patterns to Avoid ===" << std::endl;

    std::cout << "1. Virtual destructor on non-polymorphic class:" << std::endl;
    std::cout << "   Wastes space (vtable pointer)" << std::endl;
    std::cout << std::endl;

    std::cout << "2. Non-virtual destructor on polymorphic class:" << std::endl;
    std::cout << "   Resource leaks" << std::endl;
    std::cout << std::endl;

    std::cout << "3. Calling virtual functions in destructor:" << std::endl;
    std::cout << "   Calls base version, not derived" << std::endl;
    std::cout << std::endl;

    std::cout << "4. Pure virtual call in constructor/destructor:" << std::endl;
    std::cout << "   Object not fully constructed yet" << std::endl;
}