#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstring>
#include "memory_header.h"

// SBO for strings
class SmallString {
public:
    static constexpr size_t SBO_CAPACITY = 15;  // Chars that fit inline

    SmallString() {
        data_.inline_[0] = '\0';
        is_small_ = true;
    }

    explicit SmallString(const char* str) {
        size_t len = strlen(str);
        if(len <= SBO_CAPACITY) {
            // Small: store inline
            memcpy(data_.inline_, str, len + 1);
            is_small_ = true;
        }
        else {
            // Large: allocate on heap
            data_.heap_.ptr = new char[len + 1];
            memcpy(data_.heap_.ptr, str, len + 1);
            data_.heap_.size = len;
            is_small_ = false;
        }
    }

    ~SmallString() {
        if(!is_small_) {
            delete[] data_.heap_.ptr;
        }
    }

    // Copy constructor
    SmallString(const SmallString& other) {
        if(other.is_small_) {
            memcpy(data_.inline_, other.data_.inline_, SBO_CAPACITY + 1);
            is_small_ = true;
        }
        else {
            size_t len = other.data_.heap_.size;
            data_.heap_.ptr = new char[len + 1];
            memcpy(data_.heap_.ptr, other.data_.heap_.ptr, len + 1);
            data_.heap_.size = len;
            is_small_ = false;
        }
    }

    // Move constructor
    SmallString(SmallString&& other) noexcept {
        if(other.is_small_) {
            memcpy(data_.inline_, other.data_.inline_, SBO_CAPACITY + 1);
            is_small_ = true;
        }
        else {
            data_.heap_ = other.data_.heap_;
            other.is_small_ = true;  // Prevent other from deleting
            other.data_.inline_[0] = '\0';
            is_small_ = false;
        }
    }

    const char* c_str() const {
        return is_small_ ? data_.inline_ : data_.heap_.ptr;
    }

    size_t size() const {
        return is_small_ ? strlen(data_.inline_) : data_.heap_.size;
    }

    bool is_small() const { return is_small_; }

private:
    union Data {
        char inline_[SBO_CAPACITY + 1];  // +1 for null terminator
        struct {
            char* ptr;
            size_t size;
        } heap_;
    };

    Data data_;
    bool is_small_;
};

void SmallStringDemo() {
    std::cout << "=== Small String Optimization ===" << std::endl;

    std::cout << "sizeof(SmallString): " << sizeof(SmallString) << std::endl;

    SmallString small("Hello");
    std::cout << "\nSmall string:" << std::endl;
    std::cout << "  Value: " << small.c_str() << std::endl;
    std::cout << "  Is small: " << small.is_small() << std::endl;
    std::cout << "  No heap allocation!" << std::endl;

    SmallString large("This is a much longer string that exceeds the SBO limit");
    std::cout << "\nLarge string:" << std::endl;
    std::cout << "  Value: " << large.c_str() << std::endl;
    std::cout << "  Is small: " << large.is_small() << std::endl;
    std::cout << "  Allocated on heap" << std::endl;
}

// SBO in std::string (implementation detail)
void StdStringSBO() {
    std::cout << "\n=== std::string SBO ===" << std::endl;

    std::cout << "sizeof(std::string): " << sizeof(std::string) << std::endl;

    // Test with different lengths
    std::string s1 = "Short";
    std::string s2 = "This is exactly fifteen!";
    std::string s3 = "This string is definitely too long for SSO buffer!";

    // Check addresses (if same region as object, likely SBO)
    std::cout << "\ns1 ('Short'):" << std::endl;
    std::cout << "  Object at: " << &s1 << std::endl;
    std::cout << "  Data at:   " << static_cast<const void*>(s1.data()) << std::endl;
    std::cout << "  Same region? " << (reinterpret_cast<uintptr_t>(&s1) / 4096 ==
                                       reinterpret_cast<uintptr_t>(s1.data()) / 4096) << std::endl;

    std::cout << "\ns3 (long):" << std::endl;
    std::cout << "  Object at: " << &s3 << std::endl;
    std::cout << "  Data at:   " << static_cast<const void*>(s3.data()) << std::endl;
}

// SBO for function objects
template <size_t BufferSize>
class SmallFunctionStorage {
public:
    SmallFunctionStorage() = default;

    ~SmallFunctionStorage() {
        if(on_heap_ && heap_ptr_) {
            ::operator delete(heap_ptr_);
        }
    }

    void* allocate(size_t size, size_t alignment) {
        if(size <= BufferSize) {
            return &buffer_;  // Use inline buffer
        }
        // Allocate on heap
        heap_ptr_ = ::operator new(size);
        on_heap_ = true;
        return heap_ptr_;
    }

    bool is_inline() const { return !on_heap_; }

private:
    alignas(std::max_align_t) unsigned char buffer_[BufferSize];
    void* heap_ptr_ = nullptr;
    bool on_heap_ = false;
};

void FunctionStorageDemo() {
    std::cout << "\n=== Function Object Storage ===" << std::endl;

    SmallFunctionStorage<32> storage;

    // Small lambda fits in buffer
    auto* small = storage.allocate(sizeof(int), alignof(int));
    std::cout << "Small allocation is inline: " << storage.is_inline() << std::endl;

    // Large allocation goes to heap
    SmallFunctionStorage<32> storage2;
    auto* large = storage2.allocate(1024, alignof(std::max_align_t));
    std::cout << "Large allocation is inline: " << storage2.is_inline() << std::endl;
}

// Benefits of SBO
void SBOBenefits() {
    std::cout << "\n=== SBO Benefits ===" << std::endl;

    std::cout << "1. No heap allocation for common cases" << std::endl;
    std::cout << "   - Faster creation/destruction" << std::endl;
    std::cout << "   - No fragmentation" << std::endl;

    std::cout << "\n2. Better cache locality" << std::endl;
    std::cout << "   - Data is next to object metadata" << std::endl;

    std::cout << "\n3. Fewer indirections" << std::endl;
    std::cout << "   - Direct access vs pointer chase" << std::endl;

    std::cout << "\n4. Exception safety" << std::endl;
    std::cout << "   - Stack allocation can't fail" << std::endl;

    std::cout << "\nTrade-offs:" << std::endl;
    std::cout << "  - Larger object size (buffer always present)" << std::endl;
    std::cout << "  - More complex code" << std::endl;
    std::cout << "  - memcpy overhead for medium sizes" << std::endl;
}