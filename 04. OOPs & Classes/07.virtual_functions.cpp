#include <iostream>
#include "class_header.h"

namespace VirtualKey{
	class Entity{
	public:
		virtual std::string getName(){
			return "Entity";
		}
	};
	class Player : public Entity{
	private:
		std::string m_Name{};
	public:
		Player(const std::string& name) : m_Name(name){}
		std::string getName() override{
			return m_Name;
		}
	};
}

int VirtualFuction(){
	VirtualKey::Entity* e = new VirtualKey::Entity();
	std::cout << e->getName() << std::endl;

	VirtualKey::Player* p = new VirtualKey::Player("Aditya");
	std::cout << p->getName() << std::endl;

	VirtualKey::Entity* entity = p;
	std::cout << entity->getName() << std::endl;

	return 0;
}