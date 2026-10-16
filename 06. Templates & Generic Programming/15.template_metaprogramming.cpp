#include <iostream>
import std;
#include "templates_header.h"

// Compile-time if-then-else
template <bool Condition, typename TrueType, typename FalseType>
struct Conditional {
    using type = TrueType;
};

template <typename TrueType, typename FalseType>
struct Conditional<false, TrueType, FalseType> {
    using type = FalseType;
};

// C++14 alias version
template <bool Condition, typename TrueType, typename FalseType>
using ConditionalT = typename Conditional<Condition, TrueType, FalseType>::type;

void CompileTimeIfElse() {
    std::cout << "=== Compile-Time If-Then-Else ===" << std::endl;

    using Result1 = ConditionalT<true, int, double>;
    using Result2 = ConditionalT<false, int, double>;

    static_assert(std::is_same_v<Result1, int>);
    static_assert(std::is_same_v<Result2, double>);

    std::cout << "Conditional<true, int, double> = int" << std::endl;
    std::cout << "Conditional<false, int, double> = double" << std::endl;
}

// Compile-time list operations
template <typename... Types>
struct TypeList {
    static constexpr size_t size = sizeof...(Types);
};

void TypeLists() {
    std::cout << "\n=== Type Lists ===" << std::endl;

    using MyList = TypeList<int, double, char, std::string>;
    std::cout << "List size: " << MyList::size << std::endl;
}

// Compile-time integer sequences
template <int... Ints>
struct IntSequence {
    static constexpr size_t size = sizeof...(Ints);
};

template <int Start, int... Ints>
struct MakeSequence : MakeSequence<Start - 1, Start - 1, Ints...> {};

template <int... Ints>
struct MakeSequence<0, Ints...> : IntSequence<Ints...> {};

void IntegerSequences() {
    std::cout << "\n=== Integer Sequences ===" << std::endl;

    using Seq = MakeSequence<5>;
    std::cout << "Sequence size: " << Seq::size << std::endl;

    std::cout << "C++14: std::make_integer_sequence" << std::endl;
    using StdSeq = std::make_integer_sequence<int, 5>;
    std::cout << "std sequence size: " << StdSeq::size() << std::endl;
}

// Compile-time arithmetic
template <int A, int B>
struct Add {
    static constexpr int value = A + B;
};

template <int A, int B>
struct Multiply {
    static constexpr int value = A * B;
};

void CompileTimeArithmetic() {
    std::cout << "\n=== Compile-Time Arithmetic ===" << std::endl;

    constexpr auto sum = Add<5, 3>::value;
    constexpr auto product = Multiply<4, 7>::value;

    static_assert(sum == 8);
    static_assert(product == 28);

    std::cout << "Add<5, 3> = " << sum << std::endl;
    std::cout << "Multiply<4, 7> = " << product << std::endl;
}

// Template recursion (pre-C++14 constexpr)
template <int N>
struct Fibonacci {
    static constexpr int value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
};

template <>
struct Fibonacci<0> { static constexpr int value = 0; };

template <>
struct Fibonacci<1> { static constexpr int value = 1; };

void TemplateRecursion() {
    std::cout << "\n=== Template Recursion ===" << std::endl;

    constexpr auto fib10 = Fibonacci<10>::value;
    static_assert(fib10 == 55);

    std::cout << "Fibonacci<10> = " << fib10 << std::endl;
    std::cout << "Fibonacci<20> = " << Fibonacci<20>::value << std::endl;
}

// Compile-time power
template <int Base, int Exponent>
struct Power {
    static constexpr int value = Base * Power<Base, Exponent - 1>::value;
};

template <int Base>
struct Power<Base, 0> {
    static constexpr int value = 1;
};

void CompileTimePower() {
    std::cout << "\n=== Compile-Time Power ===" << std::endl;

    static_assert(Power<2, 10>::value == 1024);
    std::cout << "Power<2, 10> = " << Power<2, 10>::value << std::endl;
    std::cout << "Power<3, 4> = " << Power<3, 4>::value << std::endl;
}

// Modern C++14 constexpr (replaces much of TMP)
constexpr int FibonacciModern(int n) {
    return (n <= 1) ? n : FibonacciModern(n - 1) + FibonacciModern(n - 2);
}

void ModernConstexpr() {
    std::cout << "\n=== Modern constexpr ===" << std::endl;

    std::cout << "C++14 constexpr replaces many TMP patterns:" << std::endl;
    std::cout << "  constexpr int fib(int n) { return ...; }" << std::endl;

    constexpr auto result = FibonacciModern(10);
    static_assert(result == 55);
    std::cout << "Fibonacci(10) = " << result << std::endl;
}