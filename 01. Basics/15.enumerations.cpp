import std;
#include "basic_header.h"

// Unscoped enum (C-style)
enum Color{
    RED,      // 0
    GREEN,    // 1
    BLUE      // 2
};
void UnscopedEnum(){
    Color c = RED;
    std::cout << "Color: " << c << std::endl;
    std::cout << "RED = " << RED << std::endl;
    std::cout << "GREEN = " << GREEN << std::endl;
    std::cout << "BLUE = " << BLUE << std::endl;

    // Implicit conversion to int
    int value = c;
    std::cout << "As int: " << value << std::endl;
}


/*
    // Unscoped enum with explicit underlying type
    enum SmallEnum : uint8_t{
        SE_A = 0,
        SE_B = 1
    };
    void EnumWithBaseType(){
        std::cout << "sizeof(SmallEnum): " << sizeof(SmallEnum) << std::endl;
        SmallEnum e = SE_A;
        std::cout << "SE_A = " << static_cast<int>(e) << std::endl;
    }
*/


// Scoped enum (enum class) - C++11
enum class ScopedColor{
    Red,
    Green,
    Blue
};
void ScopedEnum(){
    ScopedColor c = ScopedColor::Red;

    // Must use scope resolution
    std::cout << "ScopedColor::Red = " << static_cast<int>(c) << std::endl;

    // No implicit conversion to int
    // int x = c;  // Error!
    int y = static_cast<int>(c);  // Must cast explicitly
    std::cout << "Explicit conversion: " << y << std::endl;
}


// Scoped enum with custom underlying type
enum class Priority : int {
    Lowest = 0,
    Low = 1,
    Medium = 5,
    High = 10,
    Highest = 100
};
void ScopedEnumWithBase(){
    Priority p = Priority::High;
    std::cout << "Priority::High = " << static_cast<int>(p) << std::endl;

    // Type traits for enums
    static_assert(std::is_enum_v<Priority>);
    static_assert(std::is_enum_v<Color>);

    std::cout << "Both are enums" << std::endl;
}


// Using enums for type-safe flags
enum class FileOpenMode{
    None = 0,
    Read = 1,
    Write = 2,
    Binary = 4,
    Append = 8,
    Truncate = 16
};
// Operator overloading for enum flags
constexpr FileOpenMode operator|(FileOpenMode a, FileOpenMode b) {
    return static_cast<FileOpenMode>(
        static_cast<int>(a) | static_cast<int>(b)
        );
}
constexpr bool operator&(FileOpenMode a, FileOpenMode b) {
    return (static_cast<int>(a) & static_cast<int>(b)) != 0;
}
void EnumFlags(){
    auto mode = FileOpenMode::Read | FileOpenMode::Write | FileOpenMode::Binary;

    if(mode & FileOpenMode::Read) {
        std::cout << "File opened for reading" << std::endl;
    }
    if(mode & FileOpenMode::Write) {
        std::cout << "File opened for writing" << std::endl;
    }
    if(mode & FileOpenMode::Binary) {
        std::cout << "File opened in binary mode" << std::endl;
    }
}


/*
    // std::to_underlying
    void ToUnderlying() {
        Color c = GREEN;
        auto underlying = std::to_underlying(c);  // C++23
        std::cout << "std::to_underlying(GREEN) = " << underlying << std::endl;

        ScopedColor sc = ScopedColor::Blue;
        auto sc_underlying = std::to_underlying(sc);
        std::cout << "std::to_underlying(ScopedColor::Blue) = " << sc_underlying << std::endl;

        // Same as static_cast but more explicit about intent
        static_assert(std::is_same_v<decltype(std::to_underlying(c)), int>);
    }
    // C++23: std::is_scoped_enum type trait
    void IsScopedEnum() {
        static_assert(std::is_scoped_enum_v<ScopedColor>);
        static_assert(!std::is_scoped_enum_v<Color>);

        std::cout << "ScopedColor is a scoped enum" << std::endl;
        std::cout << "Color is NOT a scoped enum" << std::endl;
    }
*/


// Enum in switch statements
void EnumInSwitch(){
    auto getColorName = [](ScopedColor c) -> std::string {
        switch(c){
            case ScopedColor::Red:   return "Red";
            case ScopedColor::Green: return "Green";
            case ScopedColor::Blue:  return "Blue";
            default: return "Unknown";
            }
        };
    std::cout << "Color name: " << getColorName(ScopedColor::Green) << std::endl;
}


// Forward declaration of enums
enum class ForwardEnum : int;  // Forward declaration with base type
void FunctionTakingForwardEnum(ForwardEnum e);
enum class ForwardEnum : int{
    Value1,
    Value2
};
void ForwardDeclaredEnum() {
    ForwardEnum e = ForwardEnum::Value1;
    std::cout << "Forward declared enum value: "
        << static_cast<int>(e) << std::endl;
}
void FunctionTakingForwardEnum(ForwardEnum e) {
    std::cout << "Received forward enum: " << static_cast<int>(e) << std::endl;
}


// Iterating over enum values (technique)
enum Weekday {
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday,
    WeekdayCount  // sentinel
};
void EnumIteration() {
    for(int i = 0; i < WeekdayCount; ++i) {
        Weekday day = static_cast<Weekday>(i);
        switch(day) {
            case Monday:    std::cout << "Monday "; break;
            case Tuesday:   std::cout << "Tuesday "; break;
            case Wednesday: std::cout << "Wednesday "; break;
            case Thursday:  std::cout << "Thursday "; break;
            case Friday:    std::cout << "Friday "; break;
            case Saturday:  std::cout << "Saturday "; break;
            case Sunday:    std::cout << "Sunday "; break;
            default: break;
        }
    }
    std::cout << std::endl;
}