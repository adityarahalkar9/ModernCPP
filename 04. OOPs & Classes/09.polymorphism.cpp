#include <iostream>
#include "class_header.h"

/*
	Polymorphism
	The word means "many forms". In C++, it allows a single interface (function name, operator,
	or base pointer) to behave differently depending on the type of object it operates on. C++
	has two orthogonal axes of polymorphism:
	-> Static (Compile-time)  --->  Resolved at Compile time  --->  Mechanism (Overloading, Templates)\
	-> Dynamic (Runtime)      --->  Resolved at Runtime       --->  Mechanism (Virtual functions)
*/



// ============>
// 1. Compile-Time Polymorphism
// 1a: Function Overloading
namespace fOverloading{
	// Same name, different parameter types.
	// Compiler picks the correct version at compile time via overload resolution.
	void Print(int x){
		std::cout << "int: " << x << std::endl;
	}
	void Print(double x){
		std::cout << "double: " << x << std::endl;
	}
	void Print(const char* s){
		std::cout << "string: " << s << std::endl;
	}
}
int PfunctionOverloading(){
	fOverloading::Print(42);	   // Calls print(int)
	fOverloading::Print(3.14);	   // Calls print(double)
	fOverloading::Print("hello");  // Calls print(const char*)

	return 0;
}

// 1b: Operator Overloading
namespace OpOverload{
	struct Vec2{
		float x{};
		float y{};

		// Member operator: left operand is *this.
		// Compiler rewrites  a + b  as  a.operator+(b).
		Vec2 operator+(const Vec2& rhs) const{
			return Vec2{x + rhs.x, y + rhs.y};
		}

		// Friend free function: needed so std::ostream is the left operand.
		// std::ostream cannot be *this (it's not our class to modify).
		friend std::ostream& operator<<(std::ostream& os, const Vec2& v){
			return os << '(' << v.x << ", " << v.y << ')';
		}
	};
}
int OperatorOverload(){
	OpOverload::Vec2 a{1.0f, 2.0f};
	OpOverload::Vec2 b{3.0f, 4.0f};
	std::cout << (a + b) << std::endl;	// prints (4,6)

	return 0;
}

// 1c: Template (Parametric) Polymorphism
namespace Templates{
	/*
		Compiler stamps out a separate concrete function for every T used.
		This is called "template instantiation".
		Fully resolved at compile time — zero runtime overhead.
	*/
	template<typename T>
	T maxOf(T a, T b){
		return (a > b) ? a : b;
	}
}
int TemplatePolymorphism(){
	std::cout << Templates::maxOf(3, 7) << std::endl;		// T = int
	std::cout << Templates::maxOf(2.5, 1.1) << std::endl;	// T = double
	std::cout << Templates::maxOf('a', 'z') << std::endl;	// T = char

	return 0;
}



// ============>
// 2. Runtime polymorphism
// 2a: Without virtual (Static dispatch)
namespace NoVirtual{
	struct Animal{
		/*
			NOT virtual — call is resolved at compile time using the
			*static* type of the pointer, not the actual runtime type.
			This is called "early binding" or "static dispatch".
		*/
		void speak() const{
			std::cout << "...\n";
		}
	};
	struct Dog : Animal{
		/*
			This "hides" Animal::speak but does NOT override it polymorphically.
			Only called when the static type is Dog, never through Animal*.
		*/
		void speak() const{
			std::cout << "Woof!\n";
		}
	};
}
int StaticDispatch(){
	NoVirtual::Dog d;
	NoVirtual::Animal* ptr{&d};    // base pointer pointing at a Dog object

	ptr->speak();
	/*
		Prints "..."  NOT "Woof!"
		Compiler sees Animal* so it hardwires Animal::speak at compile time.
		The Dog object's speak() is completely ignored.
	*/
	d.speak();    // prints "Woof!" — static type IS Dog here, so it works

	return 0;
}

// 2b: With virtual (Dynamic dispatch)
namespace WithVirtual{
	struct Animal{
		/*
			'virtual' — do NOT resolve at compile time.
			At runtime, consult the vtable and call whichever override
			the actual (dynamic) type of the object has.
		*/
		virtual void speak() const{
			std::cout << "....\n";
		}
		// Virtual destructor — mandatory when deleting through a base pointer.
		virtual ~Animal() = default;
	};
	struct Dog : Animal{
		/*
			'override' — compiler verifies this actually overrides a virtual.
			Mismatched signature → compile error instead of a silent new function.
		*/
		void speak() const override{
			std::cout << "Woof!\n";
		}
	};
	struct Cat : Animal{
		void speak() const override{
			std::cout << "Meow!\n";
		}
	};
	struct Bird : Animal{
		void speak() const override{
			std::cout << "Tweet!\n";
		}
	};
}
int DynamicDispatch(){
	WithVirtual::Dog d;
	WithVirtual::Cat c;
	WithVirtual::Bird b;

	// Array of base pointers — each pointing at a different derived object.
	WithVirtual::Animal* animals[]{&d, &c, &b};

	for(WithVirtual::Animal* a : animals){
		a->speak();    // // runtime vtable dispatch — correct type called every time
	}
	// Output:
	// Woof!
	// Meow!
	// Tweet!

	return 0;
}



// ============>
// 3. The vtable & vptr - Memory cost demo
/*
	A class with at least one virtual function gets a hidden vptr member.
	The vptr points to the class's vtable — a static array of function pointers.

	Layout of a Poly object on x86-64:

	   ┌──────────────┐
	   │  vptr (8 B)  │ ──────► Poly's vtable
	   ├──────────────┤          ┌────────────────────┐
	   │  x    (4 B)  │          │ [0] → Poly::f()    │
	   ├──────────────┤          └────────────────────┘
	   │  y    (4 B)  │
	   └──────────────┘
	   Total: 16 bytes (vs 8 bytes without vptr)

	 What a virtual call actually compiles to (pseudocode):
	   auto fn = object->__vptr[SLOT_INDEX];
	   fn(object);
	 Cost: 1 extra memory load + 1 indirect branch (~1-3 ns on modern CPUs).
*/
namespace VTableDemo{
	struct Plain{
		int x{};
		int y{};
		// No virtual — no vptr — sizeof = 8
	};
	struct Poly{
		virtual void f(){}    // Adds hidden vptr member
		int x{};
		int y{};
		// sizeof = 16 (8 for vptr + 4 for x + 4 for y)
	};
}
int Vtable(){
	std::cout << "sizeof(Plain) = " << sizeof(VTableDemo::Plain) << " bytes (no vptr)" << std::endl;
	std::cout << "sizeof(Poly) = " << sizeof(VTableDemo::Poly) << " bytes (vptr added = +"
		<< sizeof(VTableDemo::Poly) - sizeof(VTableDemo::Plain) << " bytes per object)\n";

	return 0;
}



// ============>
// 4. override Specifier
namespace OverrideDemo{
	struct Base{
		virtual void process(int x) const;
		virtual void render();
		virtual ~Base() = default;
	};
	// Definitions outside the struct body
	void Base::process(int x) const{
		std::cout << "Base::process(" << x << ")" << std::endl;
	}
	void Base::render(){
		std::cout << "Base::render" << std::endl;
	}
	struct Derived : Base{
		/*
			override' tells the compiler to verify:
			  1. Base has a virtual named 'process'.
			  2. The full signature (params + const) matches exactly.
			  If either check fails → COMPILE ERROR (not a silent new function).
		*/
		void process(int x) const override{
			std::cout << "Derived::process(" << x << ")" << std::endl;
		}
		void render() override{
			std::cout << "Derived::render" << std::endl;
		}
		/*
			 ── What happens WITHOUT override: ──────────────────────────────────
			 void process(int x){ }
			   → Missing 'const' → silently creates a brand new function.
			   → Base::process() still fires through Base* — very hard to debug.

			 ── What happens WITH override + typo: ──────────────────────────────
			 void rendar() override{ }
			   → Compile error: 'render' does not override any virtual function.
		*/
	};
}
int OverrideKeyword(){
	OverrideDemo::Derived d;
	OverrideDemo::Base* bp{&d};    // Base pointer to derived object

	bp->process(42);    // Derived::process(42)  — correct dispatch
	bp->render();       // Derived::render       — correct dispatch

	return 0;
}



// ============>
// 5. final specifier
namespace FinalDemo{
	struct Shape{
		virtual double area() const = 0;
		virtual ~Shape() = default;
	};
	struct Circle : Shape{
		double r{};
		explicit Circle(double radius) : r{radius}{}
		/*
			'final' on a virtual function — no further subclass may override it.
			 Also lets the compiler de-virtualise calls through Circle* (optimisation).
		*/
		double area() const override final{
			return 3.14159 * r * r;
		}
	};
	/*
		❌ Compile error — Circle::area is final:
		struct Ellipse : Circle{
			double area() const override{
				return 0;
			}  // error
		};
	*/

	/*
		'final' on a class — the class itself cannot be used as a base.
		Useful for: singletons, leaf types, preventing unintended inheritance.
	*/
	struct ImmutableString final{
		explicit ImmutableString(std::string s) : data_{std::move(s)}{}
		const std::string& get() const{
			return data_;
		}
	private:
		std::string data_{};
	};
	/*
		❌ Compile error — ImmutableString is final:
		struct MyString : ImmutableString {};  // error: cannot derive from final class
	*/
}
int FinalKeywordDemo(){
	FinalDemo::Circle c{7.0};
	std::cout << "Circle area = " << c.area() << std::endl;

	FinalDemo::ImmutableString s{"Hello, final"};
	std::cout << "ImmutableString = " << s.get() << std::endl;

	return 0;
}



int Polymorphism(){

	// PfunctionOverloading();
	// OperatorOverload();
	// TemplatePolymorphism();

	// StaticDispatch();
	// DynamicDispatch();

	// Vtable();

	// OverrideKeyword();

	FinalKeywordDemo();

	return 0;
}