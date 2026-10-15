#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
#include "basic_header.h"

/*
// C++23: Literal suffix 'Z'/'z' for std::size_t
void Size_tLiterals(){
    // Before C++23: size_t literals required casts or constants
    size_t old_way = static_cast<size_t>(42);
    size_t old_way2 = 42ULL;  // Might not be same as size_t on all platforms

    // C++23: Direct size_t literals
    auto size_val = 42uz;  // unsigned size_t literal
    auto size_val2 = 42UZ;  // Same, uppercase

    static_assert(std::is_same_v<decltype(size_val), size_t>);
    static_assert(std::is_same_v<decltype(size_val2), size_t>);

    std::cout << "42uz type: " << typeid(size_val).name() << std::endl;
    std::cout << "42uz value: " << size_val << std::endl;

    // Comparison with other literal types
    auto int_val = 42;      // int
    auto unsigned_val = 42u;  // unsigned int
    auto long_val = 42l;    // long
    auto long_long = 42ll;  // long long

    std::cout << "Types of literals:" << std::endl;
    std::cout << "42: " << typeid(int_val).name() << std::endl;
    std::cout << "42u: " << typeid(unsigned_val).name() << std::endl;
    std::cout << "42uz: " << typeid(size_val).name() << " (size_t)" << std::endl;
}



// Practical uses of size_t literals
void Size_tUseCases(){
    // Vector sizing
    std::vector<int> vec(10uz);  // Exactly size_t argument

    // Loop counters
    for(auto i = 0uz; i < vec.size(); ++i){
        vec[i] = static_cast<int>(i * 2);
    }

    // No warning about signed/unsigned comparison
    // (because i is size_t and vec.size() returns size_t)

    std::cout << "Vector contents: ";
    for(const auto& elem : vec){
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // String indexing
    std::string str = "Hello";
    auto pos = 1uz;
    std::cout << "Character at position " << pos << ": " << str[pos] << std::endl;

    // Array sizing
    auto array_size = 5uz;
    auto* arr = new int[array_size];
    for(auto i = 0uz; i < array_size; ++i) {
        arr[i] = static_cast<int>(i);
    }
    delete[] arr;
}



// Avoiding signed/unsigned comparison warnings
void SignedUnsignedIssues(){
    std::vector<int> vec = {1, 2, 3, 4, 5};

    // Potential warning: comparison between signed and unsigned
    for(int i = 0; i < vec.size(); ++i) {  // Warning on some compilers
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;

    // Fix 1: Use size_t loop variable
    for(size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;

    // Fix 2: Use auto with size_t literal (C++23)
    for(auto i = 0uz; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;

    // Fix 3: Use range-based for
    for(const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}



// sizeof and size_t relationship
void SizeofRelationship(){
    // sizeof returns size_t
    static_assert(std::is_same_v<decltype(sizeof(int)), size_t>);

    std::cout << "sizeof(int): " << sizeof(int) << " bytes" << std::endl;
    std::cout << "sizeof(double): " << sizeof(double) << " bytes" << std::endl;
    std::cout << "sizeof(size_t): " << sizeof(size_t) << " bytes" << std::endl;

    // On 64-bit systems: size_t is typically 8 bytes (64 bits)
    // On 32-bit systems: size_t is typically 4 bytes (32 bits)

    // Using sizeof with size_t literals
    auto num_elements = 100uz;
    auto total_bytes = num_elements * sizeof(int);
    std::cout << "Total bytes for " << num_elements << " ints: " << total_bytes << std::endl;
}
*/