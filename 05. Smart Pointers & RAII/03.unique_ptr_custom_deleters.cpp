import std;
#include "smart_pointers_header.h"

// Custom deleter functions
void file_closer(FILE* f) {
    if(f) {
        std::cout << "  Closing file" << std::endl;
        fclose(f);
    }
}

void UniquePtrWithFunctionDeleter() {
    std::cout << "=== unique_ptr with Function Deleter ===" << std::endl;

    // Using function pointer as deleter
    std::unique_ptr<FILE, decltype(&file_closer)> file(fopen("test.txt", "w"), file_closer);

    if(file) {
        std::cout << "  File opened successfully" << std::endl;
        fprintf(file.get(), "Hello, custom deleter!");
    }

    // File automatically closed by deleter
}

// Lambda deleter
void UniquePtrWithLambdaDeleter() {
    std::cout << "\n=== unique_ptr with Lambda Deleter ===" << std::endl;

    auto deleter = [](FILE* f) {
        if(f) {
            std::cout << "  Lambda closing file" << std::endl;
            fclose(f);
        }
        };

    std::unique_ptr<FILE, decltype(deleter)> file(fopen("test.txt", "w"), deleter);

    if(file) {
        std::cout << "  File opened with lambda deleter" << std::endl;
    }
}

// Functor deleter with state
class LoggingDeleter {
    int* delete_count_;

public:
    explicit LoggingDeleter(int* counter) : delete_count_(counter) {}

    void operator()(FILE* f) const {
        if(f) {
            ++(*delete_count_);
            std::cout << "  Deletion #" << *delete_count_ << ": closing file" << std::endl;
            fclose(f);
        }
    }
};

void UniquePtrWithFunctorDeleter() {
    std::cout << "\n=== unique_ptr with Functor Deleter ===" std::endl;

    int delete_count = 0;

    {
        std::unique_ptr<FILE, LoggingDeleter> file(fopen("test.txt", "w"), LoggingDeleter(&delete_count));
        std::cout << "  File opened with functor deleter" << std::endl;
    }

    std::cout << "  Total deletions: " << delete_count << std::endl;
}

// Deleter for malloc/free
void UniquePtrWithMallocDeleter() {
    std::cout << "\n=== unique_ptr with C-style Allocation ===" << std::endl;

    // For C-style memory
    auto malloc_deleter = [](void* p) {
        std::cout << "  Freeing malloc'd memory" << std::endl;
        free(p);
        };

    // Note: unique_ptr<T> calls delete, not free
    // So we need custom deleter for malloc'd memory
    std::unique_ptr<void, decltype(malloc_deleter)> memory(malloc(100), malloc_deleter);

    if(memory) {
        std::cout << "  Allocated 100 bytes with malloc" << std::endl;
    }
}

// No-op deleter (for non-owning pointers)
struct NoOpDeleter {
    void operator()(void*) const {
        // Do nothing - we don't own this
        std::cout << "  No-op deleter called (not deleting)" << std::endl;
    }
};

void UniquePtrWithNoOpDeleter() {
    std::cout << "\n=== unique_ptr with No-Op Deleter ===" std::endl;

    int value = 42;

    // Wrap without owning (observer pattern)
    std::unique_ptr<int, NoOpDeleter> observer(&value, NoOpDeleter{});

    std::cout << "  Observer sees: " << *observer << std::endl;
    std::cout << "  (Won't delete stack variable)" << std::endl;
}

// Type definition for common patterns
namespace smart {
    // File pointer that closes automatically
    struct FileCloser {
        void operator()(FILE* f) const {
            if(f) fclose(f);
        }
    };

    using file_ptr = std::unique_ptr<FILE, FileCloser>;

    // Malloc'd pointer
    struct FreeDeleter {
        void operator()(void* p) const {
            free(p);
        }
    };

    using malloc_ptr = std::unique_ptr<void, FreeDeleter>;
}

void UsingTypeAliases() {
    std::cout << "\n=== Smart Pointer Type Aliases ===" << std::endl;

    std::cout << "Cleaner code with type aliases:" << std::endl;

    smart::file_ptr file(fopen("test.txt", "w"));
    if(file) {
        std::cout << "  File managed cleanly" << std::endl;
    }

    smart::malloc_ptr mem(malloc(50));
    if(mem) {
        std::cout << "  Malloc'd memory managed cleanly" << std::endl;
    }
}

// Deleter size considerations
void DeleterSizeOverhead() {
    std::cout << "\n=== Deleter Size Overhead ===" << std::endl;

    // Empty deleter (functor) - no space
    struct EmptyDeleter {
        void operator()(void*) {}
    };

    // Function pointer - takes space
    using func_ptr = void(*)(void*);

    std::cout << "sizeof(unique_ptr with empty functor): "
        << sizeof(std::unique_ptr<void, EmptyDeleter>) << std::endl;
    std::cout << "sizeof(unique_ptr with function ptr): "
        << sizeof(std::unique_ptr<void, func_ptr>) << std::endl;
    std::cout << "sizeof(unique_ptr default): "
        << sizeof(std::unique_ptr<void>) << std::endl;

    std::cout << "\nEmpty functors are FREE (EBO applies)" << std::endl;
    std::cout << "Stateful deleters add size" << std::endl;
}