import std;
#include "structs_header.h"

// Basic comparison operators
struct Point {
    int x, y;

    // Manual comparison operators
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    bool operator<(const Point& other) const {
        if(x != other.x) return x < other.x;
        return y < other.y;
    }

    bool operator>(const Point& other) const {
        return other < *this;
    }

    bool operator<=(const Point& other) const {
        return !(other < *this);
    }

    bool operator>=(const Point& other) const {
        return !(*this < other);
    }
};

void ManualComparisonOperators() {
    std::cout << "=== Manual Comparison Operators ===" << std::endl;

    Point p1{1, 2};
    Point p2{1, 2};
    Point p3{3, 4};

    std::cout << "p1 == p2: " << (p1 == p2) << std::endl;
    std::cout << "p1 != p3: " << (p1 != p3) << std::endl;
    std::cout << "p1 < p3:  " << (p1 < p3) << std::endl;
    std::cout << "p1 <= p2: " << (p1 <= p2) << std::endl;
    std::cout << "p1 > p2:  " << (p1 > p2) << std::endl;
}

// C++20: Defaulted comparison operators
struct ModernPoint {
    int x, y;

    // C++20: Defaulted <=> generates all comparison operators
    auto operator<=>(const ModernPoint&) const = default;
};

void DefaultedComparison() {
    std::cout << "\n=== C++20 Defaulted Comparisons ===" << std::endl;

    ModernPoint p1{1, 2};
    ModernPoint p2{1, 2};
    ModernPoint p3{3, 4};

    // All operators available from single line!
    std::cout << "p1 == p2: " << (p1 == p2) << std::endl;
    std::cout << "p1 != p3: " << (p1 != p3) << std::endl;
    std::cout << "p1 < p3:  " << (p1 < p3) << std::endl;
    std::cout << "p1 <= p2: " << (p1 <= p2) << std::endl;

    // Can use <=> directly
    auto result = p1 <=> p3;
    std::cout << "\np1 <=> p3: ";
    if(result < 0) std::cout << "less";
    else if(result > 0) std::cout << "greater";
    else std::cout << "equal";
    std::cout << std::endl;
}

// Custom three-way comparison
struct Version {
    int major, minor, patch;

    // Custom spaceship operator
    std::strong_ordering operator<=>(const Version& other) const {
        if(major != other.major)
            return major <=> other.major;
        if(minor != other.minor)
            return minor <=> other.minor;
        return patch <=> other.patch;
    }

    // Need explicit == for defaulted operators
    bool operator==(const Version& other) const {
        return major == other.major &&
            minor == other.minor &&
            patch == other.patch;
    }
};

void CustomThreeWayComparison() {
    std::cout << "\n=== Custom <=> Operator ===" << std::endl;

    Version v1{1, 0, 0};
    Version v2{1, 1, 0};
    Version v3{1, 0, 0};

    std::cout << "v1 == v3: " << (v1 == v3) << std::endl;
    std::cout << "v1 < v2:  " << (v1 < v2) << std::endl;

    auto result = v1 <=> v2;
    std::cout << "v1 <=> v2: " << (result < 0 ? "less" : "not less") << std::endl;
}

// Comparison with different categories
struct FloatPoint {
    double x, y;

    auto operator<=>(const FloatPoint&) const = default;
};

void ComparisonCategories() {
    std::cout << "\n=== Comparison Categories ===" << std::endl;

    FloatPoint fp1{1.0, 2.0};
    FloatPoint fp2{1.0, 2.0};
    FloatPoint fp3{std::numeric_limits<double>::quiet_NaN(), 0.0};

    std::cout << "fp1 == fp2: " << (fp1 == fp2) << std::endl;

    auto normal_result = fp1 <=> fp2;
    auto nan_result = fp1 <=> fp3;

    std::cout << "Normal comparison: ";
    if(normal_result == std::partial_ordering::equivalent)
        std::cout << "equivalent" << std::endl;

    std::cout << "NaN comparison: ";
    if(nan_result == std::partial_ordering::unordered)
        std::cout << "unordered" << std::endl;

    std::cout << "\nComparison categories:" << std::endl;
    std::cout << "  std::strong_ordering:   int, pointers" << std::endl;
    std::cout << "  std::weak_ordering:     case-insensitive strings" << std::endl;
    std::cout << "  std::partial_ordering:  floating point (has NaN)" << std::endl;
}

// Using comparisons in containers
struct ComparablePerson {
    std::string name;
    int age;

    auto operator<=>(const ComparablePerson&) const = default;
};

void ComparisonInContainers() {
    std::cout << "\n=== Comparisons in Containers ===" << std::endl;

    std::set<ComparablePerson> people = {
        {"Alice", 30},
        {"Bob", 25},
        {"Charlie", 35}
    };

    std::cout << "Set contents (sorted by name):" << std::endl;
    for(const auto& p : people) {
        std::cout << "  " << p.name << ": " << p.age << std::endl;
    }

    // Map with struct keys
    std::map<ComparablePerson, std::string> notes = {
        {{"Alice", 30}, "Engineer"},
        {{"Bob", 25}, "Designer"}
    };

    std::cout << "\nMap contents:" << std::endl;
    for(const auto& [person, note] : notes) {
        std::cout << "  " << person.name << " -> " << note << std::endl;
    }

    ComparablePerson search{"Alice", 30};
    if(auto it = people.find(search); it != people.end()) {
        std::cout << "\nFound Alice in set" << std::endl;
    }
}

// Sorting with custom comparison
struct Student {
    std::string name;
    double gpa;

    auto operator<=>(const Student&) const = default;
};

void SortingWithComparison() {
    std::cout << "\n=== Sorting with Comparisons ===" << std::endl;

    std::vector<Student> students = {
        {"Alice", 3.8},
        {"Bob", 3.5},
        {"Charlie", 3.9},
        {"Dave", 3.2}
    };

    // Sort by default (all members, in order)
    std::sort(students.begin(), students.end());

    std::cout << "Sorted by name (then GPA):" << std::endl;
    for(const auto& s : students) {
        std::cout << "  " << s.name << ": " << s.gpa << std::endl;
    }

    // Custom sorting (by GPA)
    std::sort(students.begin(), students.end(),
              [](const Student& a, const Student& b) {
                  return a.gpa > b.gpa;
              });

    std::cout << "\nSorted by GPA (descending):" << std::endl;
    for(const auto& s : students) {
        std::cout << "  " << s.name << ": " << s.gpa << std::endl;
    }
}