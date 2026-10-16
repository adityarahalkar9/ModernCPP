import std;
#include "smart_pointers_header.h"

class Container {
public:
    std::string name = "container";
    std::vector<int> data = {1, 2, 3, 4, 5};
    int important_value = 42;
};

void AliasingConstructorBasics() {
    std::cout << "=== shared_ptr Aliasing Constructor ===" << std::endl;

    std::cout << "Aliasing constructor:" << std::endl;
    std::cout << "  shared_ptr<T>(owner_ptr, raw_ptr)" << std::endl;
    std::cout << "  Shares ownership, but points to different object" << std::endl;

    auto owner = std::make_shared<Container>();

    // Aliasing: shares ownership of Container, but points to member
    std::shared_ptr<std::vector<int>> data_ptr(owner, &owner->data);
    std::shared_ptr<int> value_ptr(owner, &owner->important_value);
    std::shared_ptr<std::string> name_ptr(owner, &owner->name);

    std::cout << "\nAll share ownership of Container:" << std::endl;
    std::cout << "  owner use_count: " << owner.use_count() << std::endl;

    std::cout << "\nBut point to different objects:" << std::endl;
    std::cout << "  data_ptr->size(): " << data_ptr->size() << std::endl;
    std::cout << "  *value_ptr: " << *value_ptr << std::endl;
    std::cout << "  *name_ptr: " << *name_ptr << std::endl;
}

void AliasingForMembers() {
    std::cout << "\n=== Aliasing for Members ===" << std::endl;

    std::cout << "Use case: returning member references that extend owner lifetime:" << std::endl;

    auto get_data = [](std::shared_ptr<Container> c) -> std::shared_ptr<std::vector<int>> {
        // Return shared_ptr to member, keeps container alive
        return std::shared_ptr<std::vector<int>>(c, &c->data);
        };

    auto container = std::make_shared<Container>();

    // Get shared_ptr to member
    auto data = get_data(container);
    container.reset();  // Give up our reference

    std::cout << "Container should be alive (data holds reference):" << std::endl;
    std::cout << "  data->size(): " << data->size() << std::endl;
    std::cout << "  data use_count: " << data.use_count() << std::endl;
}

void AliasingWithArrays() {
    std::cout << "\n=== Aliasing with Arrays ===" << std::endl;

    // Simulating a view into an array
    auto owner = std::make_shared<std::vector<int>>(std::vector<int>{10, 20, 30, 40, 50});

    // Create view into middle of array
    std::shared_ptr<int> view(owner, owner->data() + 2);

    std::cout << "View points to 3rd element: " << *view << std::endl;
    *view = 300;
    std::cout << "After modification: (*owner)[2] = " << (*owner)[2] << std::endl;

    std::cout << "\nView keeps array alive even if owner goes away:" << std::endl;
    owner.reset();
    std::cout << "  *view still valid: " << *view << std::endl;
}

void AliasingForPolymorphism() {
    std::cout << "\n=== Aliasing for Polymorphic Members ===" << std::endl;

    class Base {
    public:
        virtual ~Base() = default;
        virtual void method() const {
            std::cout << "  Base::method" << std::endl;
        }
    };

    class Derived : public Base {
    public:
        void method() const override {
            std::cout << "  Derived::method" << std::endl;
        }
    };

    class Holder {
    public:
        std::shared_ptr<Base> member = std::make_shared<Derived>();
    };

    auto holder = std::make_shared<Holder>();

    // Get shared_ptr to holder's member
    // Shares ownership of HOLDER, but points to MEMBER
    std::shared_ptr<Base> member_ptr(holder, holder->member.get());

    std::cout << "Calling through aliasing pointer:" << std::endl;
    member_ptr->method();  // Calls Derived::method

    std::cout << "\nHolder use_count: " << holder.use_count() << std::endl;
    std::cout << "member_ptr keeps HOLDER alive" << std::endl;
}

void AliasingUseCases() {
    std::cout << "\n=== Aliasing Use Cases ===" << std::endl;

    std::cout << "1. Returning references that extend lifetime:" << std::endl;
    std::cout << "   auto& get_member(Widget& w) {" << std::endl;
    std::cout << "       return w.member;" << std::endl;
    std::cout << "   }" << std::endl;
    std::cout << "   // DANGEROUS if w goes away" << std::endl;
    std::cout << std::endl;

    std::cout << "2. With aliasing:" << std::endl;
    std::cout << "   shared_ptr<Member> get_member(shared_ptr<Widget> w) {" << std::endl;
    std::cout << "       return {w, &w->member};" << std::endl;
    std::cout << "   }" << std::endl;
    std::cout << "   // SAFE: w kept alive by returned ptr" << std::endl;
    std::cout << std::endl;

    std::cout << "3. Sub-object views:" << std::endl;
    std::cout << "   View into parts of a larger object" << std::endl;
    std::cout << "   Keep whole object alive while using view" << std::endl;
    std::cout << std::endl;

    std::cout << "4. Implementing std::span-like semantics:" << std::endl;
    std::cout << "   shared ownership with different view" << std::endl;
}