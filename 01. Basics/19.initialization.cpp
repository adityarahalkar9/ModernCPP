import std;
#include "basic_header.h"

// Different initialization syntaxes
void InitializationSyntaxes(){
    // Copy initialization
    int a = 42;
    double d = 3.14;
    std::string s1 = "hello";

    // Direct initialization
    int b(42);
    double e(3.14);
    std::string s2("hello");

    // List/Brace initialization
    int c{42};
    double f{3.14};
    std::string s3{"hello"};

    std::cout << "a=" << a << ", b=" << b << ", c=" << c << std::endl;
    std::cout << "d=" << d << ", e=" << e << ", f=" << f << std::endl;

    // Brace initialization prevents narrowing
    // int x{3.14};  // Error: narrowing conversion
    // char y{300};  // Error: narrowing conversion
}


// Aggregate initialization
struct Point{
    int x{};
    int y{};
};
struct Rectangle{
    Point top_left{};
    Point bottom_right{};
    std::string label{};
};
void AggregateInitialization(){
    // C-style aggregate initialization
    Point p1 = {1, 2};
    Point p2{3, 4};  // Brace initialization

    std::cout << "p1: (" << p1.x << ", " << p1.y << ")" << std::endl;
    std::cout << "p2: (" << p2.x << ", " << p2.y << ")" << std::endl;

    // Nested aggregate initialization
    Rectangle r1 = {{0, 0}, {10, 10}, "square"};
    Rectangle r2 = {{1, 1}, {5, 5}, "small"};

    std::cout << "r1: " << r1.label << " from ("
        << r1.top_left.x << "," << r1.top_left.y << ") to ("
        << r1.bottom_right.x << "," << r1.bottom_right.y << ")" << std::endl;

    // C++20: Designated initializers
    Point p3{.x = 5, .y = 10};
    Rectangle r3{
        .top_left = {0, 0},
        .bottom_right = {20, 20},
        .label = "designated"
    };

    std::cout << "p3: (" << p3.x << ", " << p3.y << ")" << std::endl;
    std::cout << "r3: " << r3.label << std::endl;
}


// Designated initializers in depth (C++20)
struct Config{
    bool verbose = false;
    int timeout = 30;
    std::string name = "default";
    std::vector<int> values = {};
};
void DesignatedInitializers(){
    // Must be in declaration order
    Config c1{
        .verbose = true,
        .timeout = 60,
        .name = "my_config"
    };

    std::cout << "verbose: " << c1.verbose << std::endl;
    std::cout << "timeout: " << c1.timeout << std::endl;
    std::cout << "name: " << c1.name << std::endl;

    // Can skip fields (they get default values)
    Config c2{
        .verbose = true
        // timeout, name, values use defaults
    };

    std::cout << "c2 timeout (default): " << c2.timeout << std::endl;
    std::cout << "c2 name (default): " << c2.name << std::endl;

    // Cannot reorder: this would be a compile error
    // Config bad{
    //     .name = "test",     // Error: must come after .timeout
    //     .timeout = 100
    // };

    // Mixed designated and non-designated is not allowed
    // Config bad2{
    //     .verbose = true,
    //     60,              // Error: cannot mix
    //     .name = "test"
    // };
}


// Initialization of arrays and containers
void ContainerInitialization(){
    // Arrays
    int arr1[] = {1, 2, 3, 4, 5};
    int arr2[]{1, 2, 3};  // Brace initialization

    // std::array
    std::array<int, 3> arr3 = {1, 2, 3};
    std::array<int, 3> arr4{4, 5, 6};

    // std::vector
    std::vector<int> vec1 = {1, 2, 3, 4, 5};
    std::vector<int> vec2{6, 7, 8};
    std::vector<int> vec3(5, 100);      // 5 elements, each initialized to 100
    std::vector<int> vec4(10);          // 10 elements, value-initialized to 0

    std::cout << "vec1: ";
    for(int i : vec1) { 
        std::cout << i << " ";
    }

    std::cout << std::endl;

    std::cout << "vec3: ";
    for(int i : vec3) { 
        std::cout << i << " "; 
    }
    std::cout << std::endl;

    // std::string initialization
    std::string str1 = "hello";
    std::string str2("world");
    std::string str3{"!"};
    std::string str4(5, 'a');   // "aaaaa"
    std::string str5(str1);     // copy

    std::cout << "str4: " << str4 << std::endl;
}


// Value initialization vs default initialization
struct SimpleStruct{
    int a{};
    double b{};
    std::string c{};
};
class SimpleClass{
public:
    int x{};
    double y{};

    // If no constructor is provided:
    // - SimpleClass obj;        -> x, y are uninitialized
    // - SimpleClass obj{};      -> x=0, y=0.0 (value-initialized)
    // - SimpleClass obj();      -> function declaration! (most vexing parse)
};
void ValueVsDefaultInit(){
    // Value initialization: zero-initializes members
    SimpleStruct s1{};
    std::cout << "Value-initialized: a=" << s1.a
        << ", b=" << s1.b << ", c='" << s1.c << "'" << std::endl;

    // Default initialization: members are uninitialized (for POD)
    // SimpleStruct s2;  // a and b contain garbage!

    // With class types, default constructor is called
    std::string s3{};  // Calls default constructor, s3 = ""
    std::cout << "Default string: '" << s3 << "', empty: " << s3.empty() << std::endl;

    // Value initialization for fundamental types
    int* ptr1 = new int;      // Uninitialized
    int* ptr2 = new int();    // Value-initialized to 0
    int* ptr3 = new int{};    // Value-initialized to 0 (C++11)

    std::cout << "*ptr2 = " << *ptr2 << ", *ptr3 = " << *ptr3 << std::endl;

    delete ptr1;
    delete ptr2;
    delete ptr3;
}


// Copy list initialization vs direct list initialization
void CopyVsDirectListInit(){
    // Copy list initialization (with =)
    auto a = {1, 2, 3};  // std::initializer_list<int>
    std::vector<int> v1 = {1, 2, 3};

    // Direct list initialization (without =)
    std::vector<int> v2{1, 2, 3};
    auto b{42};  // int (C++17: direct list init with single element)

    // Note: auto with braces has special rules
    auto c = {42};       // std::initializer_list<int>
    auto d{42};          // int (direct list init)

    std::cout << "v1 size: " << v1.size() << std::endl;
    std::cout << "v2 size: " << v2.size() << std::endl;
    std::cout << "d = " << d << std::endl;
}


// Parenthesized aggregate initialization
void ParenthesizedInit(){
    // C++20 allows aggregate initialization with parentheses
    Point p1(1, 2);  // C++20: OK, same as {1, 2}

    std::cout << "p1: (" << p1.x << ", " << p1.y << ")" << std::endl;

    // However, designators don't work with parentheses
    // Point p2(.x = 1, .y = 2);  // Error: designated initializers need braces

    // Mixing is possible in nested cases
    Rectangle r1(Point(0, 0), Point(10, 10), "parens");
    std::cout << "r1: " << r1.label << std::endl;
}


// Initialization order and evaluation
void InitializationOrder(){
    // Static initialization order fiasco (for globals)
    // Local static variables are initialized on first use (C++11 thread-safe)

    static int counter = []() {
            std::cout << "Static initialized only once!" << std::endl;
            return 42;
        }();

    std::cout << "Counter: " << counter << std::endl;
    std::cout << "Counter again: " << counter << std::endl;  // No re-initialization
}