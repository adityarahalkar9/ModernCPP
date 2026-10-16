import std;
#include "class_header.h"

// Singleton Pattern
class Singleton2 {
private:
    Singleton2() {
        std::cout << "  Singleton2 constructed" << std::endl;
    }

public:
    Singleton2(const Singleton2&) = delete;
    Singleton2& operator=(const Singleton2&) = delete;

    static Singleton2& instance() {
        static Singleton2 the_instance;
        return the_instance;
    }

    void do_something() {
        std::cout << "  Singleton2 doing something" << std::endl;
    }
};

void SingletonPattern() {
    std::cout << "=== Singleton Pattern ===" << std::endl;

    Singleton2::instance().do_something();

    // Both calls return same instance
    std::cout << "  Same instance: "
        << (&Singleton2::instance() == &Singleton2::instance()) << std::endl;
}

// Builder Pattern
class Pizza {
    std::string size;
    std::vector<std::string> toppings;
    bool extra_cheese = false;

public:
    class Builder;

    void describe() const {
        std::cout << "  Pizza: " << size << ", toppings: ";
        for(const auto& t : toppings) std::cout << t << " ";
        if(extra_cheese) std::cout << "(extra cheese)";
        std::cout << std::endl;
    }
};

class Pizza::Builder {
    Pizza pizza;

public:
    Builder& set_size(const std::string& s) {
        pizza.size = s;
        return *this;
    }

    Builder& add_topping(const std::string& t) {
        pizza.toppings.push_back(t);
        return *this;
    }

    Builder& extra_cheese(bool e = true) {
        pizza.extra_cheese = e;
        return *this;
    }

    Pizza build() {
        return std::move(pizza);
    }
};

void BuilderPattern() {
    std::cout << "\n=== Builder Pattern ===" << std::endl;

    std::cout << "Step-by-step construction:" << std::endl;

    Pizza pizza = Pizza::Builder()
        .set_size("Large")
        .add_topping("Mushrooms")
        .add_topping("Peppers")
        .extra_cheese()
        .build();

    pizza.describe();
}

// Adapter Pattern
class EuropeanSocket {
public:
    virtual ~EuropeanSocket() = default;
    virtual void plug_in() {
        std::cout << "  Plugged into European socket" << std::endl;
    }
};

class AmericanDevice {
public:
    void connect() {
        std::cout << "  American device connected" << std::endl;
    }
};

class SocketAdapter : public EuropeanSocket {
    AmericanDevice device;

public:
    void plug_in() override {
        std::cout << "  Adapter converting..." << std::endl;
        device.connect();
    }
};

void AdapterPattern() {
    std::cout << "\n=== Adapter Pattern ===" << std::endl;

    std::cout << "Make incompatible interfaces work together:" << std::endl;

    EuropeanSocket* socket = new SocketAdapter();
    socket->plug_in();
    delete socket;
}

// Facade Pattern
class CPU {
public:
    void start() { std::cout << "  CPU started" << std::endl; }
};

class Memory2 {
public:
    void load() { std::cout << "  Memory loaded" << std::endl; }
};

class Disk {
public:
    void read() { std::cout << "  Disk read" << std::endl; }
};

class ComputerFacade {
    CPU cpu;
    Memory2 memory;
    Disk disk;

public:
    void start_computer() {
        std::cout << "  Starting computer:" << std::endl;
        cpu.start();
        memory.load();
        disk.read();
        std::cout << "  Computer started" << std::endl;
    }
};

void FacadePattern() {
    std::cout << "\n=== Facade Pattern ===" << std::endl;

    std::cout << "Simplified interface to complex subsystem:" << std::endl;

    ComputerFacade computer;
    computer.start_computer();
}

// Command Pattern
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
};

class LightOnCommand : public Command {
public:
    void execute() override {
        std::cout << "  Light is ON" << std::endl;
    }
};

class LightOffCommand : public Command {
public:
    void execute() override {
        std::cout << "  Light is OFF" << std::endl;
    }
};

class RemoteControl {
    std::unique_ptr<Command> command;

public:
    void set_command(std::unique_ptr<Command> c) {
        command = std::move(c);
    }

    void press_button() {
        if(command) {
            command->execute();
        }
    }
};

void CommandPattern() {
    std::cout << "\n=== Command Pattern ===" << std::endl;

    std::cout << "Encapsulate request as object:" << std::endl;

    RemoteControl remote;
    remote.set_command(std::make_unique<LightOnCommand>());
    remote.press_button();

    remote.set_command(std::make_unique<LightOffCommand>());
    remote.press_button();
}

// State Pattern
class State {
public:
    virtual ~State() = default;
    virtual void handle() = 0;
};

class RedState : public State {
public:
    void handle() override {
        std::cout << "  Red light - STOP" << std::endl;
    }
};

class GreenState : public State {
public:
    void handle() override {
        std::cout << "  Green light - GO" << std::endl;
    }
};

class TrafficLight {
    std::unique_ptr<State> state;

public:
    TrafficLight() : state(std::make_unique<RedState>()) {}

    void set_state(std::unique_ptr<State> s) {
        state = std::move(s);
    }

    void request() {
        state->handle();
    }
};

void StatePattern() {
    std::cout << "\n=== State Pattern ===" << std::endl;

    std::cout << "Behavior changes with internal state:" << std::endl;

    TrafficLight light;
    light.request();  // Red

    light.set_state(std::make_unique<GreenState>());
    light.request();  // Green
}