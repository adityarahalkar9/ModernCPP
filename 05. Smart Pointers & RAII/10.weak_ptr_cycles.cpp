import std;
#include "smart_pointers_header.h"

// The classic shared_ptr cycle problem
class Node {
public:
    explicit Node(std::string name) : name_(std::move(name)) {
        std::cout << "  Node('" << name_ << "') created" << std::endl;
    }

    ~Node() {
        std::cout << "  Node('" << name_ << "') destroyed" << std::endl;
    }

    // BAD: shared_ptr creates cycle
    std::shared_ptr<Node> next_shared;

    // GOOD: weak_ptr breaks cycle
    std::weak_ptr<Node> next_weak;

    const std::string& name() const { return name_; }

private:
    std::string name_;
};

void CycleProblem() {
    std::cout << "=== shared_ptr Cycle Problem ===" << std::endl;

    std::cout << "THE PROBLEM:" << std::endl;
    std::cout << "  A → B (shared_ptr)" << std::endl;
    std::cout << "  B → A (shared_ptr)" << std::endl;
    std::cout << "  A's count can never reach 0" << std::endl;
    std::cout << "  B's count can never reach 0" << std::endl;
    std::cout << "  MEMORY LEAK!" << std::endl;

    std::cout << "\nCreating cycle (will leak):" << std::endl;
    {
        auto a = std::make_shared<Node>("A");
        auto b = std::make_shared<Node>("B");

        a->next_shared = b;  // A owns B
        b->next_shared = a;  // B owns A  → CYCLE!

        std::cout << "  A's count: " << a.use_count() << std::endl;
        std::cout << "  B's count: " << b.use_count() << std::endl;
    }

    std::cout << "Scope ended, but nodes NOT destroyed (leak):" << std::endl;
    // Note: No "destroyed" messages printed!
}

void CycleSolution() {
    std::cout << "\n=== Breaking Cycles with weak_ptr ===" << std::endl;

    std::cout << "THE SOLUTION:" << std::endl;
    std::cout << "  One direction uses shared_ptr" << std::endl;
    std::cout << "  Other direction uses weak_ptr" << std::endl;

    std::cout << "\nCreating acyclic (proper cleanup):" << std::endl;
    {
        auto a = std::make_shared<Node>("A");
        auto b = std::make_shared<Node>("B");

        a->next_shared = b;  // A owns B
        b->next_weak = a;    // B observes A (no ownership)

        std::cout << "  A's count: " << a.use_count() << std::endl;
        std::cout << "  B's count: " << b.use_count() << std::endl;

        // B can still access A
        if(auto a_ref = b->next_weak.lock()) {
            std::cout << "  B sees A: " << a_ref->name() << std::endl;
        }
    }

    std::cout << "Scope ended, nodes properly destroyed" << std::endl;
}

// Parent-child pattern
class Parent;
class Child;

class Parent2 {
public:
    ~Parent2() { std::cout << "  Parent2 destroyed" << std::endl; }

    std::vector<std::shared_ptr<Child>> children;  // Parent owns children
};

class Child2 {
public:
    ~Child2() { std::cout << "  Child2 destroyed" << std::endl; }

    std::weak_ptr<Parent2> parent;  // Child references parent (no ownership)
};

void ParentChildPattern() {
    std::cout << "\n=== Parent-Child Pattern ===" << std::endl;

    std::cout << "Ownership hierarchy:" << std::endl;
    std::cout << "  Parent owns children (shared_ptr)" << std::endl;
    std::cout << "  Children reference parent (weak_ptr)" << std::endl;

    auto parent = std::make_shared<Parent2>();

    // Create children
    for(int i = 0; i < 3; ++i) {
        auto child = std::make_shared<Child2>();
        child->parent = parent;
        parent->children.push_back(child);
    }

    std::cout << "  Parent has " << parent->children.size() << " children" << std::endl;

    // Children can access parent
    if(!parent->children.empty()) {
        if(auto p = parent->children[0]->parent.lock()) {
            std::cout << "  Child accesses parent" << std::endl;
        }
    }
}

// Observer pattern with weak_ptr
class Subject {
public:
    ~Subject() {
        std::cout << "  Subject destroyed" << std::endl;
    }

    void attach(std::weak_ptr<class Observer2> obs) {
        observers.push_back(obs);
    }

    void notify() {
        for(auto& weak_obs : observers) {
            if(auto obs = weak_obs.lock()) {
                obs->update();
            }
        }
    }

private:
    std::vector<std::weak_ptr<Observer2>> observers;
};

class Observer2 {
public:
    ~Observer2() {
        std::cout << "  Observer2 destroyed" << std::endl;
    }

    void update() {
        std::cout << "  Observer2 notified" << std::endl;
    }
};

void ObserverPatternWeak() {
    std::cout << "\n=== Observer Pattern with weak_ptr ===" std::endl;

    auto subject = std::make_shared<Subject>();
    auto observer1 = std::make_shared<Observer2>();
    auto observer2 = std::make_shared<Observer2>();

    subject->attach(observer1);
    subject->attach(observer2);

    std::cout << "Notifying all:" << std::endl;
    subject->notify();

    std::cout << "\nOne observer goes away:" << std::endl;
    observer1.reset();

    std::cout << "Notifying again (only observer2):" << std::endl;
    subject->notify();
}

// Detecting cycles
void CycleDetection() {
    std::cout << "\n=== Detecting Cycles ===" << std::endl;

    std::cout << "Rules to avoid cycles:" << std::endl;
    std::cout << "  1. Define clear ownership hierarchy" << std::endl;
    std::cout << "     (e.g., parent owns child)" << std::endl;
    std::cout << std::endl;
    std::cout << "  2. Non-owning references use weak_ptr" << std::endl;
    std::cout << std::endl;
    std::cout << "  3. Only use shared_ptr for true shared ownership" << std::endl;
    std::cout << std::endl;
    std::cout << "  4. Ask: 'Who is responsible for deleting?'" << std::endl;
    std::cout << std::endl;
    std::cout << "  5. If you can't answer, redesign!" << std::endl;
}