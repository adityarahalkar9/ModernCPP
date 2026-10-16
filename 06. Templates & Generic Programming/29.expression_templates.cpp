import std;
#include "templates_header.h"

// Basic vector class (naive)
class Vector4 {
    std::vector<double> data_;

public:
    Vector4(std::initializer_list<double> init) : data_(init) {}
    explicit Vector4(size_t n) : data_(n) {}

    size_t size() const { return data_.size(); }
    double& operator[](size_t i) { return data_[i]; }
    const double& operator[](size_t i) const { return data_[i]; }

    Vector4 operator+(const Vector4& other) const {
        Vector4 result(size());
        for(size_t i = 0; i < size(); ++i) {
            result[i] = (*this)[i] + other[i];
        }
        return result;  // Temporary vector created
    }
};

void NaiveApproach() {
    std::cout << "=== Naive Vector Operations ===" << std::endl;

    Vector4 a = {1.0, 2.0, 3.0};
    Vector4 b = {4.0, 5.0, 6.0};
    Vector4 c = {7.0, 8.0, 9.0};

    Vector4 result = a + b + c;  // Creates temporaries!
    std::cout << "  result[0] = " << result[0] << std::endl;

    std::cout << "\nProblem: Creates temporary vectors" << std::endl;
    std::cout << "  a + b → temp1" << std::endl;
    std::cout << "  temp1 + c → result" << std::endl;
}

// Expression templates approach
template <typename E>
class VecExpression {
public:
    double operator[](size_t i) const {
        return static_cast<const E&>(*this)[i];
    }
    size_t size() const {
        return static_cast<const E&>(*this).size();
    }
};

// Vector wrapper
class Vec : public VecExpression<Vec> {
    std::vector<double> data_;

public:
    Vec(std::initializer_list<double> init) : data_(init) {}
    explicit Vec(size_t n) : data_(n) {}

    // Construct from any expression
    template <typename E>
    Vec(const VecExpression<E>& expr) : data_(expr.size()) {
        for(size_t i = 0; i < expr.size(); ++i) {
            data_[i] = expr[i];
        }
    }

    double operator[](size_t i) const { return data_[i]; }
    double& operator[](size_t i) { return data_[i]; }
    size_t size() const { return data_.size(); }
};

// Sum expression
template <typename E1, typename E2>
class VecSum : public VecExpression<VecSum<E1, E2>> {
    const E1& lhs_;
    const E2& rhs_;

public:
    VecSum(const E1& l, const E2& r) : lhs_(l), rhs_(r) {}

    double operator[](size_t i) const {
        return lhs_[i] + rhs_[i];
    }

    size_t size() const { return lhs_.size(); }
};

// Operator for expressions
template <typename E1, typename E2>
VecSum<E1, E2> operator+(const VecExpression<E1>& l,
                         const VecExpression<E2>& r) {
    return VecSum<E1, E2>(static_cast<const E1&>(l),
                          static_cast<const E2&>(r));
}

void ExpressionTemplateApproach() {
    std::cout << "\n=== Expression Templates ===" << std::endl;

    Vec a = {1.0, 2.0, 3.0};
    Vec b = {4.0, 5.0, 6.0};
    Vec c = {7.0, 8.0, 9.0};

    // No temporaries created!
    Vec result = a + b + c;

    std::cout << "  result[0] = " << result[0] << std::endl;
    std::cout << "\nExpression templates:" << std::endl;
    std::cout << "  Build expression tree at compile time" << std::endl;
    std::cout << "  Evaluate lazily" << std::endl;
    std::cout << "  No temporary vectors!" << std::endl;
}

// Performance comparison
void PerformanceComparison() {
    std::cout << "\n=== Performance Note ===" << std::endl;

    std::cout << "Benefits:" << std::endl;
    std::cout << "  - Eliminates temporaries" << std::endl;
    std::cout << "  - Better cache locality" << std::endl;
    std::cout << "  - Can auto-vectorize" << std::endl;

    std::cout << "\nUsed in:" << std::endl;
    std::cout << "  - Blitz++" << std::endl;
    std::cout << "  - Eigen" << std::endl;
    std::cout << "  - Boost uBLAS" << std::endl;
}