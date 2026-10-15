#include <iostream>
#include <vector>
#include <string>
#include <memory_resource>
#include <memory>
#include "memory_header.h"

// Basic PMR concepts
void PMRIntroduction() {
    std::cout << "=== PMR Introduction ===" << std::endl;

    std::cout << "PMR = Polymorphic Memory Resources" << std::endl;
    std::cout << "Introduced in C++17 as std::pmr namespace" << std::endl;

    std::cout << "\nKey components:" << std::endl;
    std::cout << "  1. std::pmr::memory_resource (abstract base)" << std::endl;
    std::cout << "  2. std::pmr::polymorphic_allocator<T>" << std::endl;
    std::cout << "  3. Built-in resources:" << std::endl;
    std::cout << "     - new_delete_resource" << std::endl;
    std::cout << "     - null_memory_resource" << std::endl;
    std::cout << "     - synchronized_pool_resource" << std::endl;
    std::cout << "     - unsynchronized_pool_resource" << std::endl;
    std::cout << "     - monotonic_buffer_resource" << std::endl;
}

// Using pmr::vector
void PMRVectorDemo() {
    std::cout << "\n=== pmr::vector ===" << std::endl;

    // Regular std::vector
    std::vector<int> regular_vec = {1, 2, 3, 4, 5};

    // PMR vector (on stack buffer)
    std::byte buffer[1024];
    std::pmr::monotonic_buffer_resource resource(buffer, sizeof(buffer));
    std::pmr::vector<int> pmr_vec(&resource);

    // Use it like a regular vector
    for(int i = 0; i < 10; ++i) {
        pmr_vec.push_back(i * 2);
    }

    std::cout << "Regular vector: ";
    for(int i : regular_vec) std::cout << i << " ";
    std::cout << std::endl;

    std::cout << "PMR vector:     ";
    for(int i : pmr_vec) std::cout << i << " ";
    std::cout << std::endl;

    std::cout << "\nPMR vector allocated on stack (fast!)" << std::endl;
    std::cout << "No heap allocation for small sizes" << std::endl;
}

// monotonic_buffer_resource
void MonotonicBufferDemo() {
    std::cout << "\n=== monotonic_buffer_resource ===" << std::endl;

    // Stack buffer
    std::byte buffer[512];

    std::pmr::monotonic_buffer_resource resource(buffer, sizeof(buffer));

    // Allocate from the resource
    void* mem1 = resource.allocate(64, alignof(std::max_align_t));
    void* mem2 = resource.allocate(128, alignof(std::max_align_t));

    std::cout << "Allocation 1 (64 bytes): " << mem1 << std::endl;
    std::cout << "Allocation 2 (128 bytes): " << mem2 << std::endl;

    std::cout << "Allocations are sequential (bump allocator)" << std::endl;

    // Note: deallocate is a no-op for monotonic resources
    resource.deallocate(mem1, 64);
    std::cout << "deallocate is no-op - memory freed only when resource is destroyed" << std::endl;
}

// unsynchronized_pool_resource
void PoolResourceDemo() {
    std::cout << "\n=== unsynchronized_pool_resource ===" << std::endl;

    std::pmr::pool_options options;
    options.max_blocks_per_chunk = 16;
    options.largest_required_pool_block = 256;

    std::pmr::unsynchronized_pool_resource pool(options);

    // Allocate various sizes
    std::vector<void*> allocations;
    for(size_t size : {32, 64, 128, 256}) {
        void* mem = pool.allocate(size);
        allocations.push_back(mem);
        std::cout << "Allocated " << size << " bytes at " << mem << std::endl;
    }

    // Deallocate (returns to pool, not OS)
    for(size_t i = 0; i < allocations.size(); ++i) {
        pool.deallocate(allocations[i], i == 0 ? 32 :
                        i == 1 ? 64 :
                        i == 2 ? 128 : 256);
    }

    std::cout << "\nPool resource reuses memory internally" << std::endl;
    std::cout << "Good for many same-size allocations" << std::endl;
}

// Nested containers with PMR
void NestedContainersDemo() {
    std::cout << "\n=== Nested PMR Containers ===" << std::endl;

    std::byte buffer[4096];
    std::pmr::monotonic_buffer_resource resource(buffer, sizeof(buffer));

    // Vector of strings, all using same resource
    std::pmr::vector<std::pmr::string> vec(&resource);

    vec.emplace_back("Hello");
    vec.emplace_back("World");
    vec.emplace_back("From PMR");

    std::cout << "Vector contents:" << std::endl;
    for(const auto& str : vec) {
        std::cout << "  " << str << std::endl;
    }

    std::cout << "\nAll allocations came from the stack buffer" << std::endl;
    std::cout << "No heap allocation occurred!" << std::endl;
}

// Custom memory resource
class LoggingResource : public std::pmr::memory_resource {
public:
    explicit LoggingResource(std::pmr::memory_resource* upstream)
        : upstream_(upstream) {
    }

protected:
    void* do_allocate(size_t bytes, size_t alignment) override {
        std::cout << "  [ALLOC] " << bytes << " bytes, alignment "
            << alignment << std::endl;
        return upstream_->allocate(bytes, alignment);
    }

    void do_deallocate(void* p, size_t bytes, size_t alignment) override {
        std::cout << "  [DEALLOC] " << bytes << " bytes at " << p << std::endl;
        upstream_->deallocate(p, bytes, alignment);
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
        return this == &other;
    }

private:
    std::pmr::memory_resource* upstream_;
};

void CustomResourceDemo() {
    std::cout << "\n=== Custom Memory Resource ===" << std::endl;

    LoggingResource logging_resource(std::pmr::new_delete_resource());

    std::cout << "Creating vector with logging resource:" << std::endl;
    std::pmr::vector<int> vec(&logging_resource);

    std::cout << "Pushing elements:" << std::endl;
    for(int i = 0; i < 5; ++i) {
        vec.push_back(i);
    }

    std::cout << "Vector destroyed:" << std::endl;
}

// PMR best practices
void PMRBestPractices() {
    std::cout << "\n=== PMR Best Practices ===" << std::endl;

    std::cout << "1. Use monotonic_buffer_resource for:" << std::endl;
    std::cout << "   - Parse once, use many times" << std::endl;
    std::cout << "   - Per-frame game allocations" << std::endl;
    std::cout << "   - Request-scoped web server allocations" << std::endl;

    std::cout << "\n2. Use pool_resource for:" << std::endl;
    std::cout << "   - Many small same-size objects" << std::endl;
    std::cout << "   - Node-based containers (list, map)" << std::endl;

    std::cout << "\n3. Rules:" << std::endl;
    std::cout << "   - Resource must outlive all allocations" << std::endl;
    std::cout << "   - Don't mix resources for same object" << std::endl;
    std::cout << "   - PMR adds slight overhead (virtual calls)" << std::endl;
    std::cout << "   - Use unsynchronized in single-threaded code" << std::endl;
}