import std;
#include "templates_header.h"

// Compile-time recursion: Factorial
template <int N>
struct Factorial2 {
    static constexpr int value = N * Factorial2<N - 1>::value;
};

template <>
struct Factorial2<0> {
    static constexpr int value = 1;
};

void BasicRecursion() {
    std::cout << "=== Template Recursion ===" << std::endl;

    constexpr auto fact5 = Factorial2<5>::value;
    static_assert(fact5 == 120);

    std::cout << "5! = " << Factorial2<5>::value << std::endl;
    std::cout << "10! = " << Factorial2<10>::value << std::endl;
}

// Recursion with multiple parameters
template <int Base, int Exponent>
struct Power2 {
    static constexpr int value = Base * Power2<Base, Exponent - 1>::value;
};

template <int Base>
struct Power2<Base, 0> {
    static constexpr int value = 1;
};

void PowerRecursion() {
    std::cout << "\n=== Power Recursion ===" << std::endl;

    std::cout << "2^10 = " << Power2<2, 10>::value << std::endl;
    std::cout << "3^4 = " << Power2<3, 4>::value << std::endl;
}

// Recursion on types
template <typename T>
struct RemoveAllPointers {
    using type = T;
};

template <typename T>
struct RemoveAllPointers<T*> {
    using type = typename RemoveAllPointers<T>::type;
};

void TypeRecursion() {
    std::cout << "\n=== Type Recursion ===" << std::endl;

    using Result1 = RemoveAllPointers<int***>::type;
    static_assert(std::is_same_v<Result1, int>);

    using Result2 = RemoveAllPointers<double**>::type;
    static_assert(std::is_same_v<Result2, double>);

    std::cout << "Removed all pointers from int***" << std::endl;
}

// Generating sequences
template <int... Ints>
struct Sequence {};

template <int N, int... Ints>
struct GenerateSequence : GenerateSequence<N - 1, N - 1, Ints...> {};

template <int... Ints>
struct GenerateSequence<0, Ints...> : Sequence<Ints...> {};

void SequenceGeneration() {
    std::cout << "\n=== Sequence Generation ===" << std::endl;

    using Seq = GenerateSequence<5>;
    std::cout << "Generated sequence (conceptually): 0 1 2 3 4" << std::endl;
}

// Recursion for compile-time string
template <typename T>
constexpr size_t CountVowels(const T& str) {
    size_t count = 0;
    for(char c : str) {
        if(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            ++count;
        }
    }
    return count;
}

void StringRecursion() {
    std::cout << "\n=== Compile-Time String Processing ===" << std::endl;

    constexpr std::string_view str = "hello world";
    constexpr size_t vowels = CountVowels(str);
    static_assert(vowels == 3);

    std::cout << "Vowels in \"hello world\": " << vowels << std::endl;
}

// Limitations of template recursion
void RecursionLimitations() {
    std::cout << "\n=== Recursion Limitations ===" << std::endl;

    std::cout << "Template recursion issues:" << std::endl;
    std::cout << "  1. Compiler limits (usually 1024 levels)" << std::endl;
    std::cout << "  2. Slow compilation" << std::endl;
    std::cout << "  3. Hard to debug" << std::endl;

    std::cout << "\nC++14 constexpr replaces many patterns:" << std::endl;
    std::cout << "  Loops instead of recursion" << std::endl;
    std::cout << "  Much faster compilation" << std::endl;
    std::cout << "  Easier to understand" << std::endl;
}