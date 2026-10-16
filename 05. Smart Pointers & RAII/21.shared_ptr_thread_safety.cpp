import std;
#include "smart_pointers_header.h"

class ThreadSafe {
public:
    ThreadSafe() {
        std::cout << "  ThreadSafe created" << std::endl;
    }

    ~ThreadSafe() {
        std::cout << "  ThreadSafe destroyed" << std::endl;
    }

    void increment() {
        // Note: This is NOT thread-safe!
        // Multiple threads can race here
        ++value_;
    }

    int value() const { return value_; }

private:
    int value_ = 0;
};

void SharedPtrAtomicity() {
    std::cout << "=== shared_ptr Thread Safety ===" << std::endl;

    std::cout << "shared_ptr guarantees:" << std::endl;
    std::cout << "  Reference counting IS thread-safe (atomic)" << std::endl;
    std::cout << "  Different threads can copy/destroy shared_ptr" << std::endl;
    std::endl;

    std::cout << "shared_ptr does NOT guarantee:" << std::endl;
    std::cout << "  Thread-safe access to the POINTED-TO OBJECT" << std::endl;
    std::cout << "  You must synchronize access to the object" << std::endl;
}

void SharedPtrAcrossThreads() {
    std::cout << "\n=== shared_ptr Across Threads ===" << std::endl;

    std::cout << "Sharing shared_ptr between threads:" << std::endl;

    auto shared = std::make_shared<ThreadSafe>();

    std::vector<std::thread> threads;

    // Multiple threads copy the shared_ptr
    for(int i = 0; i < 3; ++i) {
        threads.emplace_back([shared, i]() {
            std::cout << "  Thread " << i << " has copy" << std::endl;
            std::cout << "    count: " << shared.use_count() << std::endl;
                             });
    }

    for(auto& t : threads) {
        t.join();
    }

    std::cout << "After threads: count = " << shared.use_count() << std::endl;
}

void RaceConditionDemo() {
    std::cout << "\n=== Race Condition (on object) ===" << std::endl;

    std::cout << "NOT thread-safe on the OBJECT:" << std::endl;

    auto shared = std::make_shared<ThreadSafe>();

    std::vector<std::thread> threads;

    // All threads increment same object (race condition!)
    for(int i = 0; i < 4; ++i) {
        threads.emplace_back([shared]() {
            for(int j = 0; j < 1000; ++j) {
                shared->increment();  // NOT ATOMIC!
            }
                             });
    }

    for(auto& t : threads) {
        t.join();
    }

    std::cout << "Expected: 4000, Got: " << shared->value() << std::endl;
    std::cout << "  (Usually less due to lost updates)" << std::endl;
}

void ProperSynchronization() {
    std::cout << "\n=== Proper Synchronization ===" << std::endl;

    std::cout << "Must synchronize access to the object:" << std::endl;

    class SynchronizedObject {
    public:
        void increment() {
            std::lock_guard<std::mutex> lock(mutex_);
            ++value_;
        }

        int value() const {
            std::lock_guard<std::mutex> lock(mutex_);
            return value_;
        }

    private:
        mutable std::mutex mutex_;
        int value_ = 0;
    };

    auto shared = std::make_shared<SynchronizedObject>();

    std::vector<std::thread> threads;

    for(int i = 0; i < 4; ++i) {
        threads.emplace_back([shared]() {
            for(int j = 0; j < 1000; ++j) {
                shared->increment();
            }
                             });
    }

    for(auto& t : threads) {
        t.join();
    }

    std::cout << "Expected: 4000, Got: " << shared->value() << " (correct!)" << std::endl;
}

void AtomicSharedPtrSafety() {
    std::cout << "\n=== Summary ===" << std::endl;

    std::cout << "Thread-safe aspects of shared_ptr:" << std::endl;
    std::cout << "  ✓ Reference count manipulation (atomic)" << std::endl;
    std::cout << "  ✓ Creating copies from multiple threads" << std::endl;
    std::cout << "  ✓ Destroying copies from multiple threads" << std::endl;
    std::endl;

    std::cout << "NOT thread-safe:" << std::endl;
    std::cout << "  ✗ Accessing the pointed-to object" << std::endl;
    std::cout << "  ✗ Modifying same shared_ptr from multiple threads" << std::endl;
    std::endl;

    std::cout << "Rules:" << std::endl;
    std::cout << "  1. Each thread should have its own shared_ptr copy" << std::endl;
    std::cout << "  2. Synchronize access to the object (mutex)" << std::endl;
    std::cout << "  3. Use atomic operations if available (C++20)" << std::endl;
}