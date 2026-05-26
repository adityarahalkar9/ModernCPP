#include <iostream>
#include <string>

// Pure virtual functions
namespace Interface {
    class Entity{
    public:
        virtual std::string GetName() const = 0;
        virtual ~Entity() = default;
    };
    class Player : public Entity{
    private:
        std::string m_Name;

    public:
        Player(const std::string& name) : m_Name(name) {}
        std::string GetName() const override {
            return m_Name;
        }
    };

} // namespace Interface

void PrintName(const Interface::Entity* entity){
    std::cout << entity->GetName() << std::endl;
}

int Interfaces(){
    Interface::Player* p = new Interface::Player("Alex");

    PrintName(p);

    delete p; // prevent memory leak

    return 0;
}