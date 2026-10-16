import std;
#include "templates_header.h"

// Tag dispatch for iterator categories
template <typename Iterator>
void AdvanceImpl(Iterator& it, int n, std::input_iterator_tag) {
    std::cout << "  Input iterator: advancing one at a time" << std::endl;
    for(int i = 0; i < n; ++i) {
        ++it;
    }
}

template <typename Iterator>
void AdvanceImpl(Iterator& it, int n, std::random_access_iterator_tag) {
    std::cout << "  Random access: jumping directly" << std::endl;
    it += n;
}

template <typename Iterator>
void MyAdvance(Iterator& it, int n) {
    // Dispatch based on iterator category
    AdvanceImpl(it, n,
                typename std::iterator_traits<Iterator>::iterator_category());
}

void IteratorTagDispatch() {
    std::cout << "=== Tag Dispatch ===" << std::endl;

    std::cout << "Select implementation based on type traits:" << std::endl;
    std::cout << "  Uses empty tag classes" << std::endl;

    std::vector<int> vec = {1, 2, 3, 4, 5};

    auto it = vec.begin();
    MyAdvance(it, 2);  // Random access
    std::cout << "  After advance: " << *it << std::endl;
}

// Tag dispatch for integral vs floating
struct IntegralTag {};
struct FloatingTag {};

template <typename T>
struct TagFor {
    using type = std::conditional_t<std::is_integral_v<T>, IntegralTag, FloatingTag>;
};

void ProcessImpl(int value, IntegralTag) {
    std::cout << "  Processing integral: " << value << " (bit ops possible)" << std::endl;
}

void ProcessImpl(double value, FloatingTag) {
    std::cout << "  Processing floating: " << value << " (math ops)" << std::endl;
}

template <typename T>
void Process(T value) {
    ProcessImpl(value, typename TagFor<T>::type{});
}

void NumericTagDispatch() {
    std::cout << "\n=== Numeric Tag Dispatch ===" << std::endl;

    Process(42);
    Process(3.14);
}

// Tag dispatch for container optimization
struct ContiguousTag {};
struct NonContiguousTag {};

template <typename Container>
struct ContainerTag {
    static constexpr bool is_contiguous =
        std::is_same_v<Container, std::vector<typename Container::value_type>> ||
        std::is_same_v<Container, std::array<typename Container::value_type,
        0>>;

    using type = std::conditional_t<is_contiguous, ContiguousTag, NonContiguousTag>;
};

template <typename Container>
void OptimizeAccess(const Container& c, ContiguousTag) {
    std::cout << "  Contiguous: can use memcpy/pointer arithmetic" << std::endl;
}

template <typename Container>
void OptimizeAccess(const Container& c, NonContiguousTag) {
    std::cout << "  Non-contiguous: must iterate" << std::endl;
}

template <typename Container>
void Process(const Container& c) {
    OptimizeAccess(c, typename ContainerTag<Container>::type{});
}

void ContainerTagDispatch() {
    std::cout << "\n=== Container Tag Dispatch ===" << std::endl;

    std::vector<int> vec = {1, 2, 3};
    Process(vec);  // Contiguous

    // std::list<int> lst = {1, 2, 3};
    // Process(lst);  // Non-contiguous
}

// Custom tags for algorithm selection
struct FastTag {};
struct AccurateTag {};

template <typename T>
T Compute(T x, FastTag) {
    // Fast approximation
    return x * 0.5f;
}

template <typename T>
T Compute(T x, AccurateTag) {
    // Precise calculation
    return x / 2.0;
}

template <bool NeedAccuracy>
auto Calculate(double value) {
    using Tag = std::conditional_t<NeedAccuracy, AccurateTag, FastTag>;
    return Compute(value, Tag{});
}

void AlgorithmTagDispatch() {
    std::cout << "\n=== Algorithm Selection ===" << std::endl;

    auto fast = Calculate<false>(10.0);
    auto accurate = Calculate<true>(10.0);

    std::cout << "  Fast: " << fast << std::endl;
    std::cout << "  Accurate: " << accurate << std::endl;
}