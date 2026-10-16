import std;
#include "smart_pointers_header.h"

// C++20: atomic<shared_ptr<T>>
// Note: Support varies by compiler

class AtomicDemo {
public:
    AtomicDemo() { std::cout << "  AtomicDemo created" << std::endl; }
    ~AtomicDemo() { std::cout << "  AtomicDemo destroyed" << std::endl; }
};

void AtomicSharedPtrConcept() {
    std::cout << "=== atomic<shared_ptr> (C++20) ===" << std::endl;

    std::cout << "C++20 introduces std::atomic<std::shared_ptr<T>>:" << std::endl;
    std::cout << "  Atomic load/store of shared_ptr" << std::endl;
    std::cout << "  Compare-and-swap operations" << std::endl;
    std::cout << "  Lock-free updates (implementation-dependent)" << std::endl;

    std::cout << "\nBefore C++20:" << std::endl;
    std::cout << "  free functions: atomic_load, atomic_store" << std::endl;
    std::cout << "  std::atomic_load(&shared_ptr)" << std::endl;
    std::cout << "  std::atomic_store(&shared_ptr, new_value)" << std::endl;

    std::cout << "\nWith C++20:" << std::endl;
    std::cout << "  std::atomic<std::shared_ptr<T>> atomic_sp;" << std::endl;
    std::cout << "  atomic_sp.store(new_ptr);" << std::endl;
    std::cout << "  auto current = atomic_sp.load();" << std::endl;
}

// Simulating atomic shared_ptr (pre-C++20)
class AtomicSharedPtrSim {
    std::shared_ptr<AtomicDemo> ptr_;
    std::mutex mutex_;

public:
    std::shared_ptr<AtomicDemo> load() {
        std::lock_guard<std::mutex> lock(mutex_);
        return ptr_;
    }

    void store(std::shared_ptr<AtomicDemo> new_ptr) {
        std::lock_guard<std::mutex> lock(mutex_);
        ptr_ = std::move(new_ptr);
    }

    bool compare_exchange_strong(
        std::shared_ptr<AtomicDemo>& expected,
        std::shared_ptr<AtomicDemo> desired) {
        std::lock_guard<std::mutex> lock(mutex_);
        if(ptr_ == expected) {
            ptr_ = std::move(desired);
            return true;
        }
        expected = ptr_;
        return false;
    }
};

void SimulatedAtomicSharedPtr() {
    std::cout << "\n=== Simulated Atomic shared_ptr ===" << std::endl;

    AtomicSharedPtrSim atomic_sp;
    atomic_sp.store(std::make_shared<AtomicDemo>());

    auto current = atomic_sp.load();
    std::cout << "  Loaded, use_count: " << current.use_count() << std::endl;

    // Update atomically
    atomic_sp.store(std::make_shared<AtomicDemo>());
    std::cout << "  Stored new value" << std::endl;
}

// Producer-consumer with shared_ptr
void ProducerConsumerPattern() {
    std::cout << "\n=== Producer-Consumer Pattern ===" << std::endl;

    AtomicSharedPtrSim channel;

    auto producer = [&channel]() {
        for(int i = 1; i <= 3; ++i) {
            auto item = std::make_shared<AtomicDemo>();
            channel.store(item);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        };

    auto consumer = [&channel]() {
        for(int i = 0; i < 3; ++i) {
            auto item = channel.load();
            if(item) {
                std::cout << "  Consumer got item" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
        }
        };

    std::thread prod(producer);
    std::thread cons(consumer);

    prod.join();
    cons.join();
}

// RCU (Read-Copy-Update) pattern
void RCUPattern() {
    std::cout << "\n=== RCU Pattern ===" << std::endl;

    std::cout << "Read-Copy-Update with shared_ptr:" << std::endl;
    std::cout << "  Readers: get current copy, read freely" << std::endl;
    std::cout << "  Writers: create copy, modify, atomically swap" << std::endl;

    AtomicSharedPtrSim config;

    // Initialize
    config.store(std::make_shared<AtomicDemo>());

    // Reader (can run concurrently)
    auto reader = [&config]() {
        auto snapshot = config.load();  // Atomic snapshot
        std::cout << "  Reader sees stable snapshot" << std::endl;
        // Can read 'snapshot' without locks
        // Even if writer updates, our snapshot is stable
        };

    // Writer
    auto writer = [&config]() {
        auto current = config.load();
        // Create new version
        auto updated = std::make_shared<AtomicDemo>();
        // Atomically replace
        config.store(updated);
        std::cout << "  Writer installed new version" << std::endl;
        // Old version stays alive for existing readers
        };

    std::thread r(reader);
    std::thread w(writer);

    r.join();
    w.join();
}

void AtomicSharedPtrBestPractices() {
    std::cout << "\n=== Best Practices ===" << std::endl;

    std::cout << "When to use atomic<shared_ptr>:" << std::endl;
    std::cout << "  Lock-free data structures" << std::endl;
    std::cout << "  RCU patterns" << std::endl;
    std::cout << "  Snapshot semantics" << std::endl;

    std::cout << "\nWhen to use mutex instead:" << std::endl;
    std::cout << "  Complex multi-step operations" << std::endl;
    std::cout << "  Need to protect multiple variables" << std::endl;
    std::cout << "  Simplicity is more important" << std::endl;

    std::cout << "\nRemember:" << std::endl;
    std::cout << "  atomic<shared_ptr> ≠ atomic access to object" << std::endl;
    std::cout << "  Only the pointer swap is atomic" << std::endl;
}