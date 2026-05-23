#include "basic_header.h"
#include <iostream>;
#include "climits"

/*
    Use size_t When:
    => Representing sizes or counts (e.g., results of sizeof, strlen, container sizes)
    => Looping forward over arrays or buffers
    => Ensuring portability—size_t scales with architecture (32‑bit vs 64‑bit)

    Avoid size_t When:
    => You need negative values (e.g., reverse loops, pointer subtraction), use signed types like int or ptrdiff_t
    => It doesn’t represent memory size (like "number of chairs" or "rectangle area")
    => Decrementing an unsigned type-unsigned wrap-around can cause infinite loops
*/
int Size_t_size_of(){
    // 1. Store sizes of built-in types in size_t variables
    size_t sz_char = sizeof(char);              // 1 byte
    size_t sz_int = sizeof(int);                // usually 4 bytes
    size_t sz_long = sizeof(long);              // often 8 bytes on 64-bit
    size_t sz_float = sizeof(float);            // usually 4 bytes
    size_t sz_double = sizeof(double);          // usually 8 bytes

    std::cout << "char: " << sz_char << " byte(s)\n"
        << "int: " << sz_int << " byte(s)\n"
        << "long: " << sz_long << " byte(s)\n"
        << "float: " << sz_float << " byte(s)\n"
        << "double: " << sz_double << " byte(s)\n\n";

    // 2. sizeof on variables or expressions
    int i = 0;
    long l = 0L;
    std::cout << "sizeof i: " << sizeof i << "\nsizeof (l + 1): " << sizeof(l + 1) << "\n\n";

    // 3. sizeof on arrays: determine total bytes and element count
    int arr[10];
    size_t sz_arr = sizeof(arr);                // total memory footprint
    size_t arr_len = sz_arr / sizeof(arr[0]);   // how many elements
    std::cout << "arr: " << sz_arr << " bytes, length = " << arr_len << "\n\n";

    // 4. sizeof on C-string literal including the null terminator
    char s[] = "hello";
    std::cout << "\"hello\" (with '\\0'): " << sizeof(s) << " byte(s)\n\n";

    // 5. Show number of bits per byte and confirm sizeof(char) = 1
    std::cout << "Bits per byte: " << CHAR_BIT << "\n" << "sizeof(char): " << sizeof(char) << "\n\n";

    // 6. Combine and display totals; show maximum possible size_t value
    size_t total = sz_char + sz_int + sz_long;
    std::cout << "Total size (char+int+long): " << total << ", size_t max = " << SIZE_MAX << "\n\n";

    // 7. Pitfall example: reverse loop using unsigned size_t
    std::cout << "Reverse loop using size_t (requires manual break):\n";
    for(size_t j = arr_len; ; --j) {
        std::cout << j << " ";
        if(j == 0) break;      // prevents wrapping from 0 to max size_t
    }
    std::cout << "\n";

    return 0;
}