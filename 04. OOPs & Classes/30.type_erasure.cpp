import std;
#include "class_header.h"

// Type erasure using virtual functions
class AnyDrawable {
    // Type-erased implementation
    struct Concept {
        virtual void draw() const = 0;
        virtual ~Concept() = default;
    };

    template <typename T>
    struct Model : Concept {
        T data;

        explicit Model(T d) : data(std::move(d)) {}

        void draw() const override {
            data.draw();  // Duck typing
        }
    };

    std::shared_ptr<const Concept> ptr_;

public:
    template <typename T>
    AnyDrawable(T obj)
        : ptr_(std::make_shared<Model<T>>(std::move(obj))) {
    }

    void draw() const {
        ptr_->draw();
    }
};

// Types that satisfy the "Drawable" concept
class Circle {
    double radius;

public:
    Circle(double r) : radius(r) {}

    void draw() const {
        std::cout << "  Circle with radius " << radius << std::endl;
    }
};

class Square {
    double side;

public:
    Square(double s) : side(s) {}

    void draw() const {
        std::cout << "  Square with side " << side << std::endl;
    }
};

class Triangle2 {
    double base, height;

public:
    Triangle2(double b, double h) : base(b), height(h) {}

    void draw() const {
        std::cout << "  Triangle " << base << "x" << height << std::endl;
    }
};

void TypeErasureBasics() {
    std::cout << "=== Type Erasure ===" << std::endl;

    std::cout << "Type erasure:" << std::endl;
    std::cout << "  Store different types in one container" << std::endl;
    std::cout << "  No common base class needed" << std::endl;
    std::cout << "  Runtime polymorphism without inheritance" << std::endl;

    std::vector<AnyDrawable> shapes;
    shapes.push_back(Circle(5.0));
    shapes.push_back(Square(4.0));
    shapes.push_back(Triangle2(3.0, 4.0));

    for(const auto& shape : shapes) {
        shape.draw();
    }

    std::cout << "\nNo inheritance hierarchy required!" << std::endl;
    std::cout << "Any type with draw() method works" << std::endl;
}

// std::function as type erasure
void process_values(const std::function<void(int)>& func) {
    for(int i = 1; i <= 5; ++i) {
        func(i);
    }
}

void StdFunctionTypeErasure() {
    std::cout << "\n=== std::function Type Erasure ===" << std::endl;

    std::cout << "std::function erases type of callable:" << std::endl;

    // Lambda
    process_values([](int x) {
        std::cout << "  Lambda: " << x << std::endl;
                   });

    // Function object
    struct Multiplier {
        int factor;
        explicit Multiplier(int f) : factor(f) {}

        void operator()(int x) const {
            std::cout << "  Multiplier: " << x << " * " << factor
                << " = " << x * factor << std::endl;
        }
    };

    process_values(Multiplier(3));

    // Regular function
    process_values([](int x) {
        std::cout << "  Plain: " << x << std::endl;
                   });
}

// Type erasure with multiple operations
class Any Comparable {
    struct Concept {
        virtual bool less_than(const Concept& other) const = 0;
        virtual std::string to_string() const = 0;
        virtual ~Concept() = default;
    };

    template <typename T>
    struct Model : Concept {
        T value;

        explicit Model(T v) : value(std::move(v)) {}

        bool less_than(const Concept& other) const override {
            // Need to type-cast 'other' back to Model<T>
            const auto& other_model = static_cast<const Model<T>&>(other);
            return value < other_model.value;
        }

        std::string to_string() const override {
            return std::to_string(value);
        }
    };

    std::shared_ptr<const Concept> ptr_;

public:
    template <typename T>
    AnyComparable(T v)
        : ptr_(std::make_shared<Model<T>>(std::move(v))) {
    }

    bool operator<(const AnyComparable& other) const {
        return ptr_->less_than(*other.ptr_);
    }

    friend std::ostream& operator<<(std::ostream& os, const AnyComparable& ac) {
        return os << ac.ptr_->to_string();
    }
};

void MultiOperationTypeErasure() {
    std::cout << "\n=== Multi-Operation Type Erasure ===" << std::endl;

    std::cout << "Type erasure with multiple operations:" << std::endl;

    AnyComparable a(42);
    AnyComparable b(100);
    AnyComparable c(7);

    std::cout << "  a = " << a << std::endl;
    std::cout << "  b = " << b << std::endl;
    std::cout << "  c = " << c << std::endl;

    std::cout << "  a < b: " << (a < b) << std::endl;
    std::cout << "  b < c: " << (b < c) << std::endl;
}

// Type erasure limitations
void TypeErasureLimitations() {
    std::cout << "\n=== Type Erasure Limitations ===" << std::endl;

    std::cout << "Costs:" << std::endl;
    std::cout << "  Heap allocation (usually)" << std::endl;
    std::cout << "  Virtual dispatch overhead" << std::endl;
    std::cout << "  Indirection on every access" << std::endl;

    std::cout << "\nLimitations:" << std::endl;
    std::cout << "  Loses static type information" << std::endl;
    std::cout << "  Can't easily get original type back" << std::endl;
    std::cout << "  Comparison requires same underlying type" << std::endl;

    std::cout << "\nWhen to use:" << std::endl;
    std::cout << "  Heterogeneous collections" << std::endl;
    std::cout << "  Plugin architectures" << std::endl;
    std::cout << "  When inheritance is inappropriate" << std::endl;
}