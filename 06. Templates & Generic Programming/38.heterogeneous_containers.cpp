import std;
#include "templates_header.h"

// Heterogeneous container using tuple
void TupleBasics() {
    std::cout << "=== Heterogeneous Containers ===" << std::endl;

    std::cout << "std::tuple: fixed-size, compile-time types" << std::endl;

    std::tuple<int, double, std::string> data = {42, 3.14, "hello"};

    std::cout << "  int: " << std::get<0>(data) << std::endl;
    std::cout << "  double: " << std::get<1>(data) << std::endl;
    std::cout << "  string: " << std::get<2>(data) << std::endl;
}

// Iterate over tuple
template <typename Tuple, typename Func, size_t... Is>
void ForEachImpl(Tuple&& t, Func&& f, std::index_sequence<Is...>) {
    ((f(std::get<Is>(t))), ...);
}

template <typename Tuple, typename Func>
void ForEach(Tuple&& t, Func&& f) {
    ForEachImpl(std::forward<Tuple>(t), std::forward<Func>(f),
                std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>{});
}

void TupleIteration() {
    std::cout << "\n=== Tuple Iteration ===" << std::endl;

    auto data = std::make_tuple(42, 3.14, "hello");

    std::cout << "  Iterating tuple: ";
    ForEach(data, [](const auto& value) {
        std::cout << value << " ";
            });
    std::cout << std::endl;
}

// Runtime heterogeneous: std::variant
void VariantBasics() {
    std::cout << "\n=== std::variant ===" << std::endl;

    std::cout << "Runtime type selection from fixed set:" << std::endl;

    std::variant<int, double, std::string> v;

    v = 42;
    std::cout << "  Currently int: " << std::get<int>(v) << std::endl;

    v = 3.14;
    std::cout << "  Now double: " << std::get<double>(v) << std::endl;

    v = "hello";
    std::cout << "  Now string: " << std::get<std::string>(v) << std::endl;
}

// Variant visitation
void VariantVisitation() {
    std::cout << "\n=== Variant Visitation ===" << std::endl;

    std::vector<std::variant<int, double, std::string>> values = {
        42, 3.14, "hello", 100
    };

    for(const auto& val : values) {
        std::visit([](const auto& v) {
            std::cout << "  ";
            if constexpr(std::is_same_v<std::decay_t<decltype(v)>, int>) {
                std::cout << "int: " << v;
            }
            else if constexpr(std::is_same_v<std::decay_t<decltype(v)>, double>) {
                std::cout << "double: " << v;
            }
            else {
                std::cout << "string: " << v;
            }
            std::cout << std::endl;
                   }, val);
    }
}

// Type-safe heterogeneous container
template <typename... Types>
class HeteroContainer {
    std::vector<std::variant<Types...>> data_;

public:
    template <typename T>
    void push(T value) {
        data_.push_back(std::move(value));
    }

    size_t size() const { return data_.size(); }

    template <typename Func>
    void for_each(Func f) {
        for(const auto& item : data_) {
            std::visit(f, item);
        }
    }
};

void CustomHeteroContainer() {
    std::cout << "\n=== Custom Hetero Container ===" << std::endl;

    HeteroContainer<int, double, std::string> container;
    container.push(42);
    container.push(3.14);
    container.push("hello");

    std::cout << "  Size: " << container.size() << std::endl;

    container.for_each([](const auto& value) {
        std::cout << "  Value: " << value << std::endl;
                       });
}

// Compile-time type list operations
template <typename... Types>
struct TypeList {
    static constexpr size_t size = sizeof...(Types);
};

template <typename List>
struct TypeAt;

template <typename Head, typename... Tail>
struct TypeAt<TypeList<Head, Tail...>> {
    using type = Head;
};

template <size_t N, typename List>
struct TypeAtN;

template <typename Head, typename... Tail>
struct TypeAtN<0, TypeList<Head, Tail...>> {
    using type = Head;
};

template <size_t N, typename Head, typename... Tail>
struct TypeAtN<N, TypeList<Head, Tail...>>
    : TypeAtN<N - 1, TypeList<Tail...>> {
};

void CompileTimeTypeLists() {
    std::cout << "\n=== Compile-Time Type Lists ===" << std::endl;

    using MyTypes = TypeList<int, double, std::string>;

    std::cout << "  Number of types: " << MyTypes::size << std::endl;

    using FirstType = typename TypeAt<MyTypes>::type;
    using TypeAt1 = typename TypeAtN<1, MyTypes>::type;

    static_assert(std::is_same_v<FirstType, int>);
    static_assert(std::is_same_v<TypeAt1, double>);

    std::cout << "  First type is int" << std::endl;
    std::cout << "  Type at index 1 is double" << std::endl;
}