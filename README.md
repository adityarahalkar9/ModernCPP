# ModernCPP

A practical, topic-by-topic learning repository for modern C++, spanning **C++17, C++20, C++23, and selected C++26-era features**. The repository is organized as numbered folders, making it useful as a learning path, a reference while studying individual language features, and a place to experiment with small C++ examples.

> **Compiler note:** support for language and library features varies by compiler and version. This is especially important for newer C++26 features, which may require a recent compiler and may not yet be fully implemented.

## What you'll learn

The repository progresses from core language concepts into resource management, generic programming, concurrency, systems programming, and algorithms.

| Folder | Subject | Topics covered |
|---|---|---|
| [01. Basics](./01.%20Basics/) | C++ fundamentals | Core syntax and language building blocks |
| [02. Memory](./02.%20Memory/) | Memory | Memory, addresses, and lifetime fundamentals |
| [03. Structs](./03.%20Structs/) | Structures | Defining and working with structured data |
| [04. OOPs & Classes](./04.%20OOPs%20%26%20Classes/) | Object-oriented programming | Classes and object-oriented design |
| [05. Smart Pointers & RAII](./05.%20Smart%20Pointers%20%26%20RAII/) | Resource management | Smart pointers, ownership, and RAII |
| [06. Templates & Generic Programming](./06.%20Templates%20%26%20Generic%20Programming/) | Generic programming | Templates and reusable type-generic code |
| [07. Lambdas, Closures & Functional Patterns](./07.%20Lambdas%2C%20Closures%20%26%20Functional%20Patterns/) | Lambdas and functional patterns | Lambda expressions, captures, and closures |
| [08. Constexpr, Consteval & Compile time computation](./08.%20Constexpr%2C%20Consteval%20%26%20Compile%20time%20computation/) | Compile-time programming | `constexpr`, `consteval`, and compile-time computation |
| [09. Concurrency](./09.%20Concurrency/) | Concurrency | Coordinating work and shared state |
| [10. Multithreading & Parallelism](./10.%20Multithreading%20%26%20Parallelism/) | Parallel execution | Threads and parallelism concepts |
| [11. Async Programming & Coroutines](./11.%20Async%20Programming%20%26%20Coroutines/) | Asynchronous programming | Async patterns and coroutines |
| [12. Reflection](./12.%20Reflection/) | Reflection | Reflection-related language features and developments |
| [13. Standard Library Mastery (STL)](./13.%20Standard%20Library%20Mastery%28STL%29/) | Standard library | Containers, algorithms, and standard-library facilities |
| [14. Exception handling](./14.%20Exception%20handling/) | Error handling | Exceptions and failure handling |
| [15. Networking IO & Systems Programming](./15.%20Networking%20IO%20%26%20Systems%20Programming/) | Systems programming | Networking, I/O, and system-level concepts |
| [16. Low Level Memory, Object Model & ABI](./16.%20Low%20Level%20Memory%2C%20Object%20Model%20%26%20ABI/) | Low-level C++ | Object lifetime, memory model details, and ABI concepts |
| [17. Data Structures & Algorithms (DSA)](./17.%20Data%20Structures%20%26%20Algorithms%28DSA%29/) | Data structures and algorithms | Core DSA concepts and problem-solving techniques |

Folder descriptions are intended as a navigation guide. Check the examples in each folder for the precise features and standard version demonstrated.

## Getting started

### Requirements

- Git, to clone the repository.
- A C++ compiler such as **Clang**, **GCC**, or **MSVC**.
- A compiler version that supports the language features used by the example you want to build.
- **Visual Studio with `.slnx` support** if you want to use the solution file at the repository root.

### Clone the repository

```bash
git clone https://github.com/adityarahalkar9/ModernCPP.git
cd ModernCPP
```

### Use Visual Studio

1. Open `ModernCPP.slnx` in a compatible version of Visual Studio.
2. Browse the topic folders and select the example or project you want to explore.
3. Select the appropriate configuration and platform.
4. Build the selected project, then run it using the Visual Studio debugger or start command.

If the solution contains several runnable projects, select the intended project as the startup project before running it.

### Compile an individual example from the command line

Many tutorial examples can be compiled as individual source files. The exact file name and required C++ standard depend on the sample.

**Clang or GCC** (example using C++23):

```bash
clang++ -std=c++23 -Wall -Wextra -Wpedantic path/to/example.cpp -o example
./example
```

To use GCC, replace `clang++` with `g++`. On Windows, run the resulting executable as `example.exe` (for example, `./example.exe` in a compatible shell).

**MSVC** from a Developer Command Prompt:

```bat
cl /std:c++latest /EHsc /W4 path\to\example.cpp
```

Use the language mode required by the example and supported by your toolchain. For C++17 or C++20 examples, select that standard explicitly where your compiler supports it. For C++26-era examples, consult your compiler's current options and feature-support documentation; selecting a newer language mode does not guarantee every proposed or newly standardized feature is implemented.

> These commands are templates: replace `path/to/example.cpp` with the actual source file. Examples that rely on other source files or external libraries may need additional build arguments.

## Suggested learning path

1. Start with **Basics**, **Memory**, **Structs**, and **OOPs & Classes** to establish the foundations.
2. Continue with **Smart Pointers & RAII**, **Templates**, **Lambdas**, and **Compile-time computation** to explore modern C++ techniques.
3. Study **Concurrency**, **Multithreading & Parallelism**, and **Async Programming & Coroutines** when you're comfortable with object lifetime and resource management.
4. Explore **Standard Library Mastery**, **Exception handling**, **Networking & Systems Programming**, and **Low-level C++**.
5. Practise applying the concepts through **Data Structures & Algorithms**.

The folders are numbered for convenience; revisit earlier topics whenever a later example depends on them.

## How to work with the examples

- Read the source before running it, and note the C++ standard and compiler requirements for the features it uses.
- Build and run one example at a time so compiler errors and output are easier to understand.
- Experiment by changing inputs, types, and implementation details, then compare the behavior.
- Treat concurrency and low-level examples carefully: understand object lifetimes, synchronization, and ownership before adapting them to production code.
- Verify compiler support rather than assuming that a feature is available just because a compiler accepts a newer language-standard flag.

## Contributing

Contributions that improve clarity, correctness, portability, or learning value are welcome. When adding or updating an example:

- Keep it focused on the concept being demonstrated.
- Use descriptive file names and comments where they improve understanding.
- State the required C++ standard and any compiler-specific limitations when relevant.
- Document external dependencies and build steps if an example needs them.
- Test the example with the stated compiler and standard before submitting a change.

## License

This repository includes an `Apache-2.0` license. See [`LICENSE.txt`](./LICENSE.txt) for the license terms.
