import std;
#include "class_header.h"

// Product hierarchy
class Product {
public:
    virtual ~Product() = default;
    virtual void use() const = 0;
    virtual std::string get_name() const = 0;
};

class ConcreteProductA : public Product {
public:
    void use() const override {
        std::cout << "  Using Product A" << std::endl;
    }

    std::string get_name() const override { return "ProductA"; }
};

class ConcreteProductB : public Product {
public:
    void use() const override {
        std::cout << "  Using Product B" << std::endl;
    }

    std::string get_name() const override { return "ProductB"; }
};

class ConcreteProductC : public Product {
public:
    void use() const override {
        std::cout << "  Using Product C (special)" << std::endl;
    }

    std::string get_name() const override { return "ProductC"; }
};

// Simple factory
class SimpleFactory {
public:
    static std::unique_ptr<Product> create(const std::string& type) {
        if(type == "A") {
            return std::make_unique<ConcreteProductA>();
        }
        else if(type == "B") {
            return std::make_unique<ConcreteProductB>();
        }
        else if(type == "C") {
            return std::make_unique<ConcreteProductC>();
        }
        throw std::invalid_argument("Unknown product type: " + type);
    }
};

void SimpleFactoryDemo() {
    std::cout << "=== Simple Factory ===" << std::endl;

    auto product = SimpleFactory::create("A");
    product->use();

    auto product2 = SimpleFactory::create("B");
    product2->use();

    std::cout << "\nFactory creates objects without exposing creation logic" << std::endl;
}

// Factory Method pattern
class Creator {
public:
    virtual ~Creator() = default;

    // Factory method
    virtual std::unique_ptr<Product> create_product() const = 0;

    // Template method that uses factory
    std::unique_ptr<Product> manufacture() {
        auto product = create_product();
        std::cout << "  Manufactured " << product->get_name() << std::endl;
        return product;
    }
};

class CreatorA : public Creator {
public:
    std::unique_ptr<Product> create_product() const override {
        return std::make_unique<ConcreteProductA>();
    }
};

class CreatorB : public Creator {
public:
    std::unique_ptr<Product> create_product() const override {
        return std::make_unique<ConcreteProductB>();
    }
};

void FactoryMethodDemo() {
    std::cout << "\n=== Factory Method Pattern ===" << std::endl;

    std::cout << "Subclasses decide what to create:" << std::endl;

    CreatorA creator_a;
    CreatorB creator_b;

    auto pa = creator_a.manufacture();
    auto pb = creator_b.manufacture();

    pa->use();
    pb->use();
}

// Abstract Factory pattern
class AbstractFactory {
public:
    virtual ~AbstractFactory() = default;

    virtual std::unique_ptr<Product> create_product() = 0;
    virtual std::unique_ptr<Product> create_premium_product() = 0;
};

class StandardFactory : public AbstractFactory {
public:
    std::unique_ptr<Product> create_product() override {
        return std::make_unique<ConcreteProductA>();
    }

    std::unique_ptr<Product> create_premium_product() override {
        return std::make_unique<ConcreteProductC>();
    }
};

class BudgetFactory : public AbstractFactory {
public:
    std::unique_ptr<Product> create_product() override {
        return std::make_unique<ConcreteProductB>();
    }

    std::unique_ptr<Product> create_premium_product() override {
        return std::make_unique<ConcreteProductB>();
    }
};

void AbstractFactoryDemo() {
    std::cout << "\n=== Abstract Factory Pattern ===" << std::endl;

    std::cout << "Creates families of related objects:" << std::endl;

    std::cout << "Standard factory:" << std::endl;
    StandardFactory standard;
    standard.create_product()->use();
    standard.create_premium_product()->use();

    std::cout << "Budget factory:" << std::endl;
    BudgetFactory budget;
    budget.create_product()->use();
    budget.create_premium_product()->use();
}

// Registered factory (plugin-style)
class RegistryFactory {
    using Creator2 = std::function<std::unique_ptr<Product>()>;
    std::map<std::string, Creator2> creators;

public:
    void register_product(const std::string& name, Creator2 creator) {
        creators[name] = std::move(creator);
        std::cout << "  Registered: " << name << std::endl;
    }

    std::unique_ptr<Product> create(const std::string& name) {
        auto it = creators.find(name);
        if(it != creators.end()) {
            return it->second();
        }
        throw std::invalid_argument("Unknown: " + name);
    }
};

void RegistryFactoryDemo() {
    std::cout << "\n=== Registry Factory ===" << std::endl;

    std::cout << "Register creators at runtime:" << std::endl;

    RegistryFactory factory;
    factory.register_product("A", [] { return std::make_unique<ConcreteProductA>(); });
    factory.register_product("B", [] { return std::make_unique<ConcreteProductB>(); });

    auto product = factory.create("A");
    product->use();

    auto product2 = factory.create("B");
    product2->use();
}

// Modern factory with templates
template <typename T>
class ModernFactory {
public:
    template <typename... Args>
    static std::unique_ptr<T> create(Args&&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
};

class ConfigurableProduct : public Product {
    std::string name;

public:
    explicit ConfigurableProduct(std::string n) : name(std::move(n)) {}

    void use() const override {
        std::cout << "  Using " << name << std::endl;
    }

    std::string get_name() const override { return name; }
};

void ModernFactoryDemo() {
    std::cout << "\n=== Modern Factory ===" std::endl;

    std::cout << "std::make_unique is already a factory:" << std::endl;

    auto product = ModernFactory<ConfigurableProduct>::create("custom");
    product->use();

    std::cout << "\nFor most cases, make_unique/make_shared suffice" << std::endl;
}