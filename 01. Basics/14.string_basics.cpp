import std;

// Basic std::string operations
void StringBasics(){
    // Construction
    std::string s1 = "Hello";
    std::string s2("World");
    std::string s3(5, 'A');             // "AAAAA"
    std::string s4(s1);                 // copy
    std::string s5(s1 + " " + s2);      // concatenation

    std::cout << "s1: " << s1 << std::endl;
    std::cout << "s2: " << s2 << std::endl;
    std::cout << "s3: " << s3 << std::endl;
    std::cout << "s5: " << s5 << std::endl;

    // Size and capacity
    std::cout << "s1 length: " << s1.length() << std::endl;
    std::cout << "s1 size: " << s1.size() << std::endl;         // same as length()
    std::cout << "s1 capacity: " << s1.capacity() << std::endl;
    std::cout << "s1 empty: " << std::boolalpha << s1.empty() << std::endl;
}


// Element access
void StringElementAccess() {
    std::string s = "Hello, World!";

    std::cout << "s[0]: " << s[0] << std::endl;
    std::cout << "s.at(7): " << s.at(7) << std::endl;
    std::cout << "s.front(): " << s.front() << std::endl;
    std::cout << "s.back(): " << s.back() << std::endl;
    std::cout << "s.c_str(): " << s.c_str() << std::endl;

    // Bounds checking with .at()
    try{
        std::cout << s.at(100) << std::endl;    // throws std::out_of_range
    }
    catch(const std::out_of_range& e) {
        std::cout << "Out of range: " << e.what() << std::endl;
    }
}


// Modifying strings
void StringModification(){
    std::string s = "Hello";

    // Append
    s += " World";
    s.append("!");
    s.push_back('~');

    std::cout << "After append: " << s << std::endl;

    // Insert
    std::string t = "World";
    t.insert(0, "Hello ");
    std::cout << "After insert: " << t << std::endl;

    // Erase
    std::string u = "Hello, World!";
    u.erase(5, 2);          // Remove ", " starting at position 5
    std::cout << "After erase: " << u << std::endl;

    // Replace
    std::string v = "Hello, World!";
    v.replace(0, 5, "Goodbye");
    std::cout << "After replace: " << v << std::endl;

    // Substring
    std::string w = "Hello, World!";
    std::string sub = w.substr(0, 5);  // "Hello"
    std::cout << "Substring: " << sub << std::endl;
}


/*
    // String search operations
    void StringSearch(){
        std::string s = "Hello, World! Hello C++!";

        // Find
        size_t pos = s.find("Hello");
        std::cout << "First 'Hello' at position: " << pos << std::endl;

        pos = s.find("Hello", 10);  // Search from position 10
        std::cout << "Second 'Hello' at position: " << pos << std::endl;

        // rfind (search from end)
        pos = s.rfind("Hello");
        std::cout << "Last 'Hello' at position: " << pos << std::endl;

        // find_first_of
        pos = s.find_first_of("lo");
        std::cout << "First 'l' or 'o' at position: " << pos << std::endl;

        // find_last_of
        pos = s.find_last_of("lo");
        std::cout << "Last 'l' or 'o' at position: " << pos << std::endl;

        // C++20: starts_with / ends_with
        std::cout << "starts_with('Hello'): " << std::boolalpha << s.starts_with("Hello") << std::endl;
        std::cout << "ends_with('C++!'): " << s.ends_with("C++!") << std::endl;

        // C++23: contains
        std::cout << "contains('World'): " << s.contains("World") << std::endl;
        std::cout << "contains('xyz'): " << s.contains("xyz") << std::endl;
    }
*/


/*
    // String comparison
    void StringComparison(){
        std::string s1 = "apple";
        std::string s2 = "banana";
        std::string s3 = "apple";

        std::cout << "s1 == s3: " << std::boolalpha << (s1 == s3) << std::endl;
        std::cout << "s1 < s2: " << (s1 < s2) << std::endl;
        std::cout << "s1 compare s2: " << s1.compare(s2) << " (negative means s1 < s2)" << std::endl;

        // Case-insensitive comparison (manual implementation)
        auto to_lower = [](std::string str) {
            std::transform(str.begin(), str.end(), str.begin(), ::tolower);
            return str;
            };

        std::string upper = "APPLE";
        std::cout << "Case-insensitive equal: "
            << (to_lower(s1) == to_lower(upper)) << std::endl;
    }
*/


// std::string_view
void StringViewBasics(){
    // Non-owning view of a string
    std::string str = "Hello, World!";
    std::string_view sv = str;

    std::cout << "string_view: " << sv << std::endl;
    std::cout << "Length: " << sv.length() << std::endl;

    // Substring (no allocation!)
    std::string_view sub = sv.substr(0, 5);
    std::cout << "Substring view: " << sub << std::endl;

    // string_view from string literal
    std::string_view literal = "Direct literal";
    std::cout << "Literal view: " << literal << std::endl;

    // C++20: starts_with / ends_with for string_view
    std::cout << "sv starts with 'Hello': " << sv.starts_with("Hello") << std::endl;
    std::cout << "sv ends with '!': " << sv.ends_with("!") << std::endl;

    // Warning: string_view doesn't own data - dangling danger
    // std::string_view dangling = std::string("temporary");  // BAD!
}


// String conversion functions
void StringConversions(){
    // String to number
    std::string numStr = "42";
    int num = std::stoi(numStr);
    std::cout << "stoi: " << num << std::endl;

    std::string dblStr = "3.14159";
    double dbl = std::stod(dblStr);
    std::cout << "stod: " << dbl << std::endl;

    std::string longStr = "123456789012345";
    long long ll = std::stoll(longStr);
    std::cout << "stoll: " << ll << std::endl;

    // Number to string
    int i = 100;
    std::string iStr = std::to_string(i);
    std::cout << "to_string(int): " << iStr << std::endl;

    double d = 3.14159;
    std::string dStr = std::to_string(d);
    std::cout << "to_string(double): " << dStr << std::endl;

    // C++17: std::to_chars and std::from_chars (faster, locale-independent)
    char buffer[32];
    auto [ptr, ec] = std::to_chars(buffer, buffer + sizeof(buffer), 42);
    std::cout << "to_chars result: " << std::string(buffer, ptr) << std::endl;

    int parsed;
    auto [ptr2, ec2] = std::from_chars("123abc", "123abc" + 6, parsed);
    std::cout << "from_chars parsed: " << parsed << std::endl;
}


// Raw string literals
void RawStringLiterals(){
    // Regular string with escapes
    std::string escaped = "C:\\Users\\name\\file.txt";
    std::cout << "Escaped: " << escaped << std::endl;

    // Raw string literal (no escapes needed)
    std::string raw = R"(C:\Users\name\file.txt)";
    std::cout << "Raw: " << raw << std::endl;

    // Multiline raw string
    std::string multiline = R"(Line 1 Line 2 Line 3)";
    std::cout << "Multiline: " << multiline << std::endl;

    // Raw string with custom delimiter (in case content contains ")")
    std::string custom = R"++(This string contains )" and more)++";
    std::cout << "Custom delimiter: " << custom << std::endl;
}


// char8_t for UTF-8
void Char8_t(){
    // C++20 introduced char8_t as a distinct type for UTF-8 code units
    char8_t u8_char = u8'A';

    // u8 string literals have type const char8_t[]
    const char8_t* u8_str = u8"Hello";

    // Note: char8_t is distinct from char
    static_assert(!std::is_same_v<char8_t, char>);
    static_assert(sizeof(char8_t) == 1);

    // Converting u8string to string (if needed for output)
    std::u8string u8string_var = u8"Hello, World!";
    std::string regular(reinterpret_cast<const char*>(u8string_var.data()),
                        u8string_var.size());
    std::cout << "u8string converted: " << regular << std::endl;

    // Unicode escape sequences (C++23: \u{...} delimited)
    std::string unicode = "\u0041\u0042\u0043";  // "ABC"
    std::cout << "Unicode escapes: " << unicode << std::endl;

    // C++23: Named universal character escapes
    // std::string cat = "\N{CAT FACE}";  // 🐱 (if compiler supports)
}