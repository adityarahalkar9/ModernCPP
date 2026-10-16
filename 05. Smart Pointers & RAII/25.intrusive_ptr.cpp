import std;
#include "smart_pointers_header.h"

// Simulating intrusive_ptr (like Boost)
// Reference count stored in the OBJECT itself
class IntrusiveObject {
    friend void intrusive_ptr_add_ref(IntrusiveObject*);
    friend void intrusive_ptr_release(IntrusiveObject*);

public:
    IntrusiveObject() {
        std::cout << "  IntrusiveObject created" << std::endl;
    }

    ~IntrusiveObject() {
        std::cout << "  IntrusiveObject destroyed" << std::endl;
    }

    int ref_count() const { return ref_count_.load(); }

    void use() const {
        std::cout << "  Using IntrusiveObject (count: "
            << ref_count() << ")" << std::endl;
    }

private:
    std::atomic<int> ref_count_{0};
};

// Required helper functions
void intrusive_ptr_add_ref(IntrusiveObject* p) {
    p->ref_count_.fetch_add(1, std::memory_order_relaxed);
}

void intrusive_ptr_release(IntrusiveObject* p) {
    if(p->ref_count_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete p;
    }
}

// Minimal intrusive_ptr implementation
template <typename T>
class intrusive_ptr {
    T* ptr_ = nullptr;

public:
    intrusive_ptr() = default;

    explicit intrusive_ptr(T* p, bool add_ref = true) : ptr_(p) {
        if(p && add_ref) {
            intrusive_ptr_add_ref(p);
        }
    }

    intrusive_ptr(const intrusive_ptr& other) : ptr_(other.ptr_) {
        if(ptr_) {
            intrusive_ptr_add_ref(ptr_);
        }
    }

    intrusive_ptr(intrusive_ptr&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    intrusive_ptr& operator=(const intrusive_ptr& other) {
        if(this != &other) {
            reset();
            ptr_ = other.ptr_;
            if(ptr_) {
                intrusive_ptr_add_ref(ptr_);
            }
        }
        return *this;
    }

    intrusive_ptr& operator=(intrusive_ptr&& other) noexcept {
        if(this != &other) {
            reset();
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    ~intrusive_ptr() {
        reset();
    }

    void reset() {
        if(ptr_) {
            intrusive_ptr_release(ptr_);
            ptr_ = nullptr;
        }
    }

    T* get() const { return ptr_; }
    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }
    explicit operator bool() const { return ptr_ != nullptr; }

    bool operator==(const intrusive_ptr& other) const {
        return ptr_ == other.ptr_;
    }
};

void IntrusivePtrBasics() {
    std::cout << "=== intrusive_ptr Concept ===" << std::endl;

    std::cout << "intrusive_ptr stores count IN THE OBJECT:" << std::endl;
    std::cout << "  shared_ptr: count in separate control block" << std::endl;
    std::cout << "  intrusive_ptr: count is member of object" << std::endl;

    std::cout << "\nAdvantages:" << std::endl;
    std::cout << "  1. Less memory (no control block)" << std::endl;
    std::cout << "  2. Better cache locality" << std::endl;
    std::cout << "  3. Can get shared_ptr from 'this'" << std::endl;

    std::cout << "\nDisadvantages:" << std::endl;
    std::cout << "  1. Object must implement counting" << std::endl;
    std::cout << "  2. More coupling" << std::endl;
    std::cout << "  3. Not in standard library" << std::endl;
}

void IntrusivePtrUsage() {
    std::cout << "\n=== intrusive_ptr Usage ===" << std::endl;

    std::cout << "Creating intrusive_ptr:" << std::endl;
    intrusive_ptr<IntrusiveObject> ptr1(new IntrusiveObject());

    std::cout << "  Count after creation: " << ptr1->ref_count() << std::endl;

    std::cout << "\nCopying:" << std::endl;
    intrusive_ptr<IntrusiveObject> ptr2 = ptr1;
    std::cout << "  Count after copy: " << ptr1->ref_count() << std::endl;

    std::cout << "\nUsing object:" << std::endl;
    ptr1->use();

    std::cout << "\nReleasing one reference:" << std::endl;
    ptr1.reset();
    std::cout << "  ptr2 still valid, count: " << ptr2->ref_count() << std::endl;
}

void IntrusiveVsShared() {
    std::cout << "\n=== intrusive vs shared ===" << std::endl;

    std::cout << "Memory layout comparison:" << std::endl;
    std::endl;
    std::cout << "shared_ptr:" << std::endl;
    std::cout << "  [Object]           (heap allocation 1)" << std::endl;
    std::cout << "  [Control block]     (heap allocation 2)" << std::endl;
    std::cout << "  [shared_ptr] = [obj ptr] [control ptr]" << std::endl;
    std::endl;
    std::cout << "intrusive_ptr:" << std::endl;
    std::cout << "  [Object + count]    (single allocation)" << std::endl;
    std::cout << "  [intrusive_ptr] = [obj ptr]" << std::endl;

    std::cout << "\nWhen to use intrusive:" << std::endl;
    std::cout << "  Performance critical" << std::endl;
    std::cout << "  Need shared_from_this-like functionality" << std::endl;
    std::cout << "  Interop with systems requiring it" << std::endl;

    std::cout << "\nWhen to use shared_ptr:" << std::endl;
    std::cout << "  Default choice" << std::endl;
    std::cout << "  Can't modify the class" << std::endl;
    std::cout << "  Standard library compatibility" << std::endl;
}