#include <iostream>
#include <string>
#include <string_view>
#include "basic_header.h"



// C++20 introduced char8_t as a distinct type for UTF-8 code units
void Char8_tBasics(){
    // char8_t is a distinct type, not just an alias for char
    static_assert(!std::is_same_v<char8_t, unsigned char>);
    static_assert(!std::is_same_v<char8_t, char>);
    static_assert(sizeof(char8_t) == 1);

    // Character literals
    char8_t u8_char = u8'A';
    std::cout << "Size of char8_t: " << sizeof(char8_t) << std::endl;

    // u8 string literals have type const char8_t[]
    const char8_t* u8_str = u8"Hello";
    std::cout << "u8 string literal type: const char8_t*" << std::endl;
}



// u8string and u8string_view (C++20)
void U8String(){
    // std::u8string uses char8_t as character type
    std::u8string u8str = u8"Hello, World!";

    std::cout << "u8string length: " << u8str.length() << std::endl;
    std::cout << "u8string size: " << u8str.size() << std::endl;

    // Note: Can't directly output u8string with std::cout
    // Need to convert to regular string for output
    std::string regular(reinterpret_cast<const char*>(u8str.data()),
                        u8str.size());
    std::cout << "Converted to string: " << regular << std::endl;
}



// Working with UTF-8
void UTF8Handling(){
    // UTF-8 encoded string (regular char)
    std::string utf8 = "Hello, 世界! 🌍";  // Mixed ASCII and UTF-8

    std::cout << "UTF-8 string: " << utf8 << std::endl;
    std::cout << "Byte length: " << utf8.length() << std::endl;
    // Note: byte length != character count for multi-byte characters

    // Converting between char8_t and char strings
    std::u8string u8version(reinterpret_cast<const char8_t*>(utf8.data()), utf8.size());

    // And back
    std::string back(reinterpret_cast<const char*>(u8version.data()), u8version.size());
    std::cout << "Round-trip: " << back << std::endl;
}



// char8_t in function signatures
void ProcessU8String(std::u8string_view sv){
    // Process UTF-8 string
    std::cout << "Processing u8string_view of length: " << sv.length() << std::endl;
}
void U8StringInView(){
    std::u8string str = u8"Test string";
    std::u8string_view view = str;
    ProcessU8String(view);
}



// Unicode escape sequences
void UnicodeEscapes(){
    // Universal character names (pre-C++23)
    std::string unicode1 = "\u0041\u0042\u0043";  // "ABC"
    std::cout << "Universal character names: " << unicode1 << std::endl;

    // Hexadecimal escape sequences
    std::string hex = "\x41\x42\x43";  // "ABC"
    std::cout << "Hex escapes: " << hex << std::endl;

    // Octal escape sequences
    std::string octal = "\101\102\103";  // "ABC"
    std::cout << "Octal escapes: " << octal << std::endl;

    // C++23: Delimited escape sequences
    // std::string delimited = "\x{41}\x{42}\x{43}";  // C++23
    // std::string delimited_unicode = "\u{0041}\u{0042}\u{0043}";  // C++23

    // C++23: Named universal character escapes
    // std::string cat = "\N{CAT FACE}";  // 🐱 (C++23)
    // std::string snowman = "\N{SNOWMAN}";  // ☃ (C++23)
}



// Character type comparison
void CharacterTypesComparison(){
    // Different character types in C++
    char c = 'A';               // 1 byte, implementation-defined signedness
    signed char sc = 'A';       // 1 byte, signed
    unsigned char uc = 'A';     // 1 byte, unsigned
    wchar_t wc = L'A';          // Wide character (platform-dependent size)
    char8_t c8 = u8'A';         // 1 byte, UTF-8 code unit (C++20)
    char16_t c16 = u'A';        // 2 bytes, UTF-16 code unit
    char32_t c32 = U'A';        // 4 bytes, UTF-32 code unit

    std::cout << "sizeof(char): " << sizeof(char) << std::endl;
    std::cout << "sizeof(wchar_t): " << sizeof(wchar_t) << std::endl;
    std::cout << "sizeof(char8_t): " << sizeof(char8_t) << std::endl;
    std::cout << "sizeof(char16_t): " << sizeof(char16_t) << std::endl;
    std::cout << "sizeof(char32_t): " << sizeof(char32_t) << std::endl;

    // All represent 'A' but are different types
    std::cout << "All represent 'A' in different encodings" << std::endl;
}



// Migration considerations for char8_t
void MigrationNotes(){
    // Before C++20: u8 literals were const char[]
    // After C++20: u8 literals are const char8_t[]

    // This can break existing code:
    // const char* str = u8"Hello";  // Error in C++20+

    // Workaround 1: Use regular string literals for standard ASCII/UTF-8 bytes
    const char* ascii_str = "Hello";
    std::cout << "1. Regular literal: " << ascii_str << std::endl;

    // Workaround 2: Convert u8string_view to std::string when needed
    auto u8_to_string = [](const char8_t* u8str) -> std::string {
            std::u8string_view view(u8str);
            return std::string(view.begin(), view.end());
        };

    std::string converted = u8_to_string(u8"Hello");
    std::cout << "2. Converted u8 string: " << converted << std::endl;

    // Workaround 3: Use C++20 templated lambdas to accept both char and char8_t
    auto get_length = []<typename CharT>(const CharT * str) {
        return std::char_traits<CharT>::length(str);
    };

    std::cout << "3. Char length (regular): " << get_length("Hello") << std::endl;
    std::cout << "   Char length (u8): " << get_length(u8"Hello") << std::endl;

    std::cout << "Migration strategies demonstrated successfully!" << std::endl;
}