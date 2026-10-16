import std;
#include "templates_header.h"

// Friend function of template class
template <typename T>
class Secret {
    friend void Reveal<T>(Secret<T>&);  // Friend declaration

public:
    Secret(T value) : value_(value) {}

private:
    T value_;
};

// Friend definition
template <typename T>
void Reveal(Secret<T>& s) {
    std::cout << "  Secret value: " << s.value_ << std::endl;
}

void FriendFunctions() {
    std::cout << "=== Template Friends ===" << std::endl;

    Secret<int> secret(42);
    Reveal(secret);
}

// Friend class
template <typename T>
class Container5 {
    template <typename U>
    friend class Accessor;  // All instantiations are friends

public:
    void add(T value) { data_.push_back(value); }

private:
    std::vector<T> data_;
};

template <typename U>
class Accessor {
public:
    template <typename T>
    static size_t GetSize(const Container5<T>& c) {
        return c.data_.size();  // Access private member
    }
};

void FriendClasses() {
    std::cout << "\n=== Friend Classes ===" << std::endl;

    Container5<int> container;
    container.add(1);
    container.add(2);
    container.add(3);

    size_t size = Accessor<int>::GetSize(container);
    std::cout << "  Size via friend: " << size << std::endl;
}

// Befriending specific instantiation
template <typename T>
class Number3 {
    // Only Number3<int> is friend of Number3<double>
    friend class Number3<int>;

public:
    Number3(T value) : value_(value) {}

private:
    T value_;
};

void SpecificInstantiation() {
    std::cout << "\n=== Specific Friend Instantiations ===" << std::endl;

    std::cout << "Can befriend specific instantiation:" << std::endl;
    std::cout << "  friend class Number3<int>;" << std::endl;
    std::cout << "  (inside Number3<double>)" << std::endl;

    Number3<double> d(3.14);
    Number3<int> i(42);
}

// Operator overloading with friends
template <typename T>
class Vector3 {
    T x_, y_, z_;

public:
    Vector3(T x, T y, T z) : x_(x), y_(y), z_(z) {}

    // Friend operator for output
    friend std::ostream& operator<<(std::ostream& os, const Vector3& v) {
        return os << "(" << v.x_ << ", " << v.y_ << ", " << v.z_ << ")";
    }

    // Friend operator for dot product
    friend T dot(const Vector3& a, const Vector3& b) {
        return a.x_ * b.x_ + a.y_ * b.y_ + a.z_ * b.z_;
    }
};

void FriendOperators() {
    std::cout << "\n=== Friend Operators ===" << std::endl;

    Vector3<double> v1(1.0, 2.0, 3.0);
    Vector3<double> v2(4.0, 5.0, 6.0);

    std::cout << "  v1 = " << v1 << std::endl;
    std::cout << "  v2 = " << v2 << std::endl;
    std::cout << "  dot = " << dot(v1, v2) << std::endl;
}

// Hidden friends idiom
template <typename T>
class Hidden {
public:
    Hidden(T value) : value_(value) {}

    // Hidden friend: only found via ADL
    friend bool operator==(const Hidden& a, const Hidden& b) {
        return a.value_ == b.value_;
    }

    friend std::ostream& operator<<(std::ostream& os, const Hidden& h) {
        return os << "Hidden(" << h.value_ << ")";
    }

private:
    T value_;
};

void HiddenFriends() {
    std::cout << "\n=== Hidden Friends Idiom ===" << std::endl;

    std::cout << "Benefits:" << std::endl;
    std::cout << "  - Found only via ADL" << std::endl;
    std::cout << "  - Encapsulation" << std::endl;
    std::cout << "  - Prevents accidental conversions" << std::endl;

    Hidden<int> h1(42);
    Hidden<int> h2(42);

    std::cout << "  h1 = " << h1 << std::endl;
    std::cout << "  h1 == h2: " << (h1 == h2) << std::endl;
}