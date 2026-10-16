import std;
#include "templates_header.h"

// Thread policy
struct SingleThreaded {
    void lock() {}
    void unlock() {}
};

struct MultiThreaded {
    std::mutex mtx_;

    void lock() { mtx_.lock(); }
    void unlock() { mtx_.unlock(); }
};

// Storage policy
template <typename T>
struct InlineStorage {
    T data_;

    T& get() { return data_; }
    const T& get() const { return data_; }
    void set(const T& v) { data_ = v; }
};

template <typename T>
struct HeapStorage {
    std::unique_ptr<T> data_ = std::make_unique<T>();

    T& get() { return *data_; }
    const T& get() const { return *data_; }
    void set(const T& v) { *data_ = v; }
};

// Checking policy
struct NoCheck {
    template <typename T>
    static void check(const T&) {}
};

struct EnforceRange {
    template <typename T>
    static void check(const T& value) {
        if(value < 0) {
            throw std::invalid_argument("Negative value");
        }
    }
};

// Policy-based class
template <typename T,
    typename StoragePolicy = InlineStorage<T>,
    typename CheckingPolicy = NoCheck,
    typename ThreadingPolicy = SingleThreaded>
class SmartValue {
    StoragePolicy storage_;
    ThreadingPolicy threading_;

public:
    SmartValue() = default;

    explicit SmartValue(T initial) {
        storage_.set(initial);
    }

    T get() const {
        threading_.lock();
        T result = storage_.get();
        threading_.unlock();
        return result;
    }

    void set(T value) {
        CheckingPolicy::check(value);
        threading_.lock();
        storage_.set(value);
        threading_.unlock();
    }
};

void PolicyBasedDesign() {
    std::cout << "=== Policy-Based Design ===" << std::endl;

    std::cout << "Compose behavior from policies:" << std::endl;
    std::cout << "  template <typename T," << std::endl;
    std::cout << "            typename StoragePolicy," << std::endl;
    std::cout << "            typename CheckingPolicy," << std::endl;
    std::cout << "            typename ThreadingPolicy>" << std::endl;

    // Simple: inline storage, no checking, single-threaded
    SmartValue<int> simple(42);
    std::cout << "\nSimple: " << simple.get() << std::endl;

    // Checked: with range enforcement
    SmartValue<int, InlineStorage<int>, EnforceRange> checked(10);
    try {
        checked.set(-5);  // Will throw
    }
    catch(const std::exception& e) {
        std::cout << "  Caught: " << e.what() << std::endl;
    }

    // Heap: heap storage
    SmartValue<double, HeapStorage<double>> heaped(3.14);
    std::cout << "Heap stored: " << heaped.get() << std::endl;
}

// Policy with template template parameters
template <typename T>
struct FastAllocator {
    using value_type = T;

    T* allocate(size_t n) {
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, size_t) noexcept {
        ::operator delete(p);
    }
};

template <typename T>
struct SafeAllocator {
    using value_type = T;

    T* allocate(size_t n) {
        std::cout << "  [ALLOC] " << n * sizeof(T) << " bytes" << std::endl;
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, size_t n) noexcept {
        std::cout << "  [DEALLOC] " << n * sizeof(T) << " bytes" << std::endl;
        ::operator delete(p);
    }
};

template <typename T, template <typename> class Alloc = FastAllocator>
class PolicyContainer {
    Alloc<T> alloc_;

public:
    ~PolicyContainer() {
        if(data_) {
            alloc_.deallocate(data_, size_);
        }
    }

    void push_back(const T& value) {
        if(size_ >= capacity_) {
            grow();
        }
        data_[size_++] = value;
    }

    T& operator[](size_t i) { return data_[i]; }
    size_t size() const { return size_; }

private:
    T* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;

    void grow() {
        size_t new_cap = capacity_ ? capacity_ * 2 : 1;
        T* new_data = alloc_.allocate(new_cap);
        for(size_t i = 0; i < size_; ++i) {
            new_data[i] = std::move(data_[i]);
        }
        if(data_) {
            alloc_.deallocate(data_, capacity_);
        }
        data_ = new_data;
        capacity_ = new_cap;
    }
};

void TemplateTemplatePolicy() {
    std::cout << "\n=== Template Template Policies ===" << std::endl;

    std::cout << "Default (fast):" << std::endl;
    PolicyContainer<int> fast;
    fast.push_back(1);
    fast.push_back(2);

    std::cout << "\nWith logging allocator:" << std::endl;
    PolicyContainer<int, SafeAllocator> safe;
    safe.push_back(10);
    safe.push_back(20);
}

// Policy design benefits
void PolicyBenefits() {
    std::cout << "\n=== Policy Design Benefits ===" << std::endl;

    std::cout << "1. Compile-time composition" << std::endl;
    std::cout << "   Zero runtime overhead" << std::endl;
    std::cout << std::endl;

    std::cout << "2. Orthogonal concerns" << std::endl;
    std::cout << "   Each policy handles one aspect" << std::endl;
    std::cout << std::endl;

    std::cout << "3. Easy extension" << std::endl;
    std::cout << "   Add new policies without changing existing code" << std::endl;
    std::cout << std::endl;

    std::cout << "4. Used in:" << std::endl;
    std::cout << "   - std::allocator" << std::endl;
    std::cout << "   - Loki library" << std::endl;
    std::cout << "   - Boost libraries" << std::endl;
}