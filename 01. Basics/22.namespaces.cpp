import std;
#include "basic_header.h"


// Basic namespace
namespace Math{
    const double PI = 3.14159265358979;
    int add(int a, int b){
        return a + b;
    }
    int multiply(int a, int b){
        return a * b;
    }
    namespace Advanced{
        int power(int base, int exp){
            int result = 1;
            for(int i = 0; i < exp; ++i){
                result *= base;
            }
            return result;
        }
    }
}
void BasicNamespace(){
    std::cout << "Math::PI = " << Math::PI << std::endl;
    std::cout << "Math::add(3, 4) = " << Math::add(3, 4) << std::endl;
    std::cout << "Math::multiply(5, 6) = " << Math::multiply(5, 6) << std::endl;
    std::cout << "Math::Advanced::power(2, 8) = " << Math::Advanced::power(2, 8) << std::endl;
}



// Using declarations and directives
void UsingDeclarations(){
    // Using declaration: introduces a single name
    using Math::add;
    std::cout << "add(10, 20) = " << add(10, 20) << std::endl;  // No Math:: prefix needed

    // Using directive: introduces entire namespace
    // using namespace Math;  // Generally discouraged in headers
    // std::cout << "multiply(7, 8) = " << multiply(7, 8) << std::endl;
}



// Namespace aliases
namespace MA = Math::Advanced;
void NamespaceAliases(){
    std::cout << "MA::power(3, 4) = " << MA::power(3, 4) << std::endl;

    // Useful for long namespace names
    namespace fs = std::filesystem; 
    // fs::path p = "/some/path";
}



// Anonymous (unnamed) namespaces
// Creates internal linkage (like static)
namespace{
    int internal_counter = 0;
    void InternalFunction(){
        ++internal_counter;
        std::cout << "Internal function called, counter: "
            << internal_counter << std::endl;
    }
}
void AnonymousNamespace(){
    InternalFunction();
    InternalFunction();
    InternalFunction();

    // Note: entities in anonymous namespace are only visible in this translation unit
}



// Inline namespaces
namespace Version1{
    void Function(){
        std::cout << "Version 1 function" << std::endl;
    }
}
inline namespace Version2{
    void Function(){
        std::cout << "Version 2 function (default)" << std::endl;
    }
}
void InlineNamespace(){
    // Without qualification, the inline namespace version is used
    Function();  // Calls Version2::function

    // Can still explicitly qualify
    Version1::Function();  // Calls Version1::function
    Version2::Function();  // Calls Version2::function
}



// Nested namespaces
namespace Outer::Inner::Deepest{
    void DeepFunction(){
        std::cout << "Called from Outer::Inner::Deepest" << std::endl;
    }
}
void NestedNamespaceSyntax(){
    Outer::Inner::Deepest::DeepFunction();
}



// Argument-Dependent Lookup (ADL) / Koenig Lookup
namespace MyNamespace{
    struct MyType {};
    void PrintMyType(const MyType& t){
        std::cout << "Printing MyType from MyNamespace" << std::endl;
    }
}
void ADLDemo(){
    MyNamespace::MyType obj;

    // Without ADL, we'd need:
    // MyNamespace::printMyType(obj);

    // With ADL, the compiler looks in the namespace of the argument's type
    PrintMyType(obj);  // ADL finds MyNamespace::printMyType
}



// Namespace for avoiding name collisions
namespace Library1{
    class String {
    public:
        void Print(){ 
            std::cout << "Library1::String" << std::endl; 
        }
    };
}
namespace Library2{
    class String{
    public:
        void Print(){ 
            std::cout << "Library2::String" << std::endl; 
        }
    };
}
void NameCollisionResolution(){
    Library1::String s1;
    Library2::String s2;

    s1.Print();
    s2.Print();

    // Can also use using declarations to disambiguate
    using Library1::String;
    String s3;  // This is Library1::String
    s3.Print();
}



// std namespace and avoiding "using namespace std"
void StdNamespaceBestPractices(){
    // BAD practice (especially in headers):
    // using namespace std;

    // GOOD practice: explicit qualification
    std::cout << "Explicit std:: qualification" << std::endl;
    std::string s = "explicit";
    std::cout << s << std::endl;

    // Or selective using declarations
    using std::cout;
    using std::endl;
    cout << "Using selective declarations" << endl;
}