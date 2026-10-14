import std;
#include "basic_header.h"

// Basic I/O streams
void BasicIO(){
    // Output
    std::cout << "Standard output" << std::endl;
    std::cerr << "Standard error" << std::endl;
    std::clog << "Standard log" << std::endl;

    // Chaining output
    int age = 25;
    std::string name = "Alice";
    std::cout << "Name: " << name << ", Age: " << age << std::endl;
}


// Formatted output with manipulators
void FormattedOutput(){
    // std::endl vs '\n'
    std::cout << "Using std::endl (flushes):" << std::endl;
    std::cout << "Using '\\n' (may not flush):\n";

    // std::setw - set field width
    std::cout << "|" << std::setw(10) << "right" << "|" << std::endl;
    std::cout << "|" << std::setw(10) << "left" << "|" << std::endl;

    // std::setfill - set fill character
    std::cout << "|" << std::setfill('*') << std::setw(10) << "text" << "|" << std::endl;

    // Precision for floating point
    double pi = 3.14159265358979;
    std::cout << "Default: " << pi << std::endl;
    std::cout << "Precision 2: " << std::setprecision(2) << pi << std::endl;
    std::cout << "Precision 10: " << std::setprecision(10) << pi << std::endl;
    std::cout << "Scientific: " << std::scientific << pi << std::endl;
    std::cout << "Fixed: " << std::fixed << pi << std::endl;

    // Boolean output
    bool flag = true;
    std::cout << "Default bool: " << flag << std::endl;
    std::cout << "Alpha bool: " << std::boolalpha << flag << std::endl;
    std::cout << "Numeric bool: " << std::noboolalpha << flag << std::endl;

    // Number base
    int number = 255;
    std::cout << "Decimal: " << std::dec << number << std::endl;
    std::cout << "Hexadecimal: " << std::hex << number << std::endl;
    std::cout << "Octal: " << std::oct << number << std::endl;
    std::cout << "Show base: " << std::showbase << std::hex << number << std::endl;
    std::cout << "Uppercase: " << std::uppercase << number << std::endl;
}


// Input handling
void InputHandling(){
    // Note: This is interactive, showing patterns
    // Reading a line
    std::string line;
    std::cout << "Enter a line: ";
    std::getline(std::cin, line);
    std::cout << "You entered: " << line << std::endl;

    // Reading multiple values
    int a, b;
    std::cout << "Enter two integers: ";
    std::cin >> a >> b;
    std::cout << "Sum: " << (a + b) << std::endl;

    // Checking input state
    if(std::cin.good()){
        std::cout << "Input stream is good" << std::endl;
    }

    // Clearing error state
    if(std::cin.fail()){
        std::cin.clear();  // Clear error flags
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}


// String streams
void StringStreams(){
    // ostringstream - output to string
    std::ostringstream oss;
    oss << "Hello, " << "World! " << 42;
    std::string result = oss.str();
    std::cout << "ostringstream: " << result << std::endl;

    // istringstream - input from string
    std::istringstream iss("42 3.14 hello");
    int i;
    double d;
    std::string s;
    iss >> i >> d >> s;
    std::cout << "Parsed: i=" << i << ", d=" << d << ", s=" << s << std::endl;

    // stringstream - both input and output
    std::stringstream ss;
    ss << "Insert: " << 100;
    std::string extracted;
    ss >> extracted;    // "Insert:"
    int value;
    ss >> value;        // 100
    std::cout << "Extracted: " << extracted << " " << value << std::endl;

    // Using stringstream for type conversion
    auto toString = [](auto value){
            std::stringstream ss;
            ss << value;
            return ss.str();
        };

    std::cout << "Converted 42: " << toString(42) << std::endl;
    std::cout << "Converted 3.14: " << toString(3.14) << std::endl;
}


// File I/O
void FileIO(){
    // Writing to a file
    {
        std::ofstream outFile("example.txt");
        if(outFile.is_open()) {
            outFile << "Hello, File!" << std::endl;
            outFile << "Line 2" << std::endl;
            outFile << "Line 3" << std::endl;
        }
    }

    // Reading from a file
    {
        std::ifstream inFile("example.txt");
        if(inFile.is_open()) {
            std::string line;
            while(std::getline(inFile, line)) {
                std::cout << "Read: " << line << std::endl;
            }
        }
    }

    // Appending to a file
    {
        std::ofstream appendFile("example.txt", std::ios::app);
        if(appendFile.is_open()) {
            appendFile << "Appended line" << std::endl;
        }
    }

    // Reading entire file at once
    {
        std::ifstream inFile("example.txt");
        std::string content((std::istreambuf_iterator<char>(inFile)),
                            std::istreambuf_iterator<char>());
        std::cout << "File content:\n" << content << std::endl;
    }

    // Binary file I/O
    {
        std::ofstream binFile("data.bin", std::ios::binary);
        int numbers[] = {1, 2, 3, 4, 5};
        binFile.write(reinterpret_cast<const char*>(numbers), sizeof(numbers));
    }

    {
        std::ifstream binFile("data.bin", std::ios::binary);
        int numbers[5];
        binFile.read(reinterpret_cast<char*>(numbers), sizeof(numbers));
        std::cout << "Binary read: ";
        for(int n : numbers) std::cout << n << " ";
        std::cout << std::endl;
    }
}


// std::format
void FormatDemo(){
    // std::format - Python-like string formatting
#ifdef __cpp_lib_format
#include <format>
    std::string formatted = std::format("Hello, {}! You are {} years old.", "Alice", 25);
    std::cout << "std::format: " << formatted << std::endl;

    std::string precise = std::format("Pi: {:.2f}", 3.14159);
    std::cout << "Formatted pi: " << precise << std::endl;

    std::string numbered = std::format("{0} {1} {0}", "abc", "def");
    std::cout << "Numbered args: " << numbered << std::endl;
#else
    std::cout << "std::format not available (need C++20 with library support)" << std::endl;
#endif
}

// std::print and std::println
void PrintDemo(){
    // C++23 introduces std::print and std::println in <print>
    // These combine formatted output with direct console writing

#ifdef __cpp_lib_print
#include <print>
// std::print("Hello, {}!\n", "World");  // C++23
// std::println("This is a line");       // C++23
    std::cout << "std::print available (C++23)" << std::endl;
#else
    std::cout << "std::print not available (need C++23)" << std::endl;
#endif
}


// Stream states and error handling
void StreamStates(){
    std::istringstream iss("42 abc");
    int num{};
    iss >> num;
    std::cout << "Read integer: " << num << std::endl;
    std::cout << "Stream good: " << iss.good() << std::endl;

    std::string word{};
    iss >> word;
    std::cout << "Read word: " << word << std::endl;

    // After failed read
    int another{};
    iss >> another;  // Will fail (end of stream)
    std::cout << "Stream fail: " << iss.fail() << std::endl;
    std::cout << "Stream eof: " << iss.eof() << std::endl;
}


// Custom output operator
struct Point{
    int x{}, y{};
};
std::ostream& operator<<(std::ostream& os, const Point& p) {
    return os << "(" << p.x << ", " << p.y << ")";
}
void CustomOutputOperator(){
    Point p{3, 4};
    std::cout << "Point: " << p << std::endl;

    // Works with stringstreams too
    std::ostringstream oss{};
    oss << p;
    std::cout << "From stringstream: " << oss.str() << std::endl;
}