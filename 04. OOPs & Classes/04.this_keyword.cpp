#include <iostream>
#include "class_header.h"

/*
	In C++, the this keyword is an implicit pointer available within the
	non-static member functions of a class or struct. It points to the specific
	instance of the object that is currently executing the code.
*/
// Basic usage of this operator
void PrintEntity(BasicThis::Entity* e);
namespace BasicThis{
	class Entity{
	public:
		int x, y;
		Entity(int x, int y){
			this->x = x;
			this->y = y;
			PrintEntity(this);
		}
		int GetX() const{
			return x;
		}
	};
}
void PrintEntity(BasicThis::Entity* e){
	// Print something
}


namespace This{
	class Dog{
	private:
		std::string m_dog_name{};
		std::string m_dog_breed{};
		int* m_dog_age{};
	public:
		void displayInfo();
		void set_dog_name(std::string m_dog_name){
			this->m_dog_name = m_dog_name;
		}
		Dog(){
			m_dog_name = "Tommy";
			m_dog_breed = "German shephard";
			m_dog_age = new int{3};
		};
		Dog(std::string name_param, std::string breed_param, int age_param){
			m_dog_name = name_param;
			m_dog_breed = breed_param;
			m_dog_age = new int{age_param};
			std::cout << m_dog_name << " Constructed at: " << this << std::endl;		// this keyword used
		}

		~Dog(){
			delete m_dog_age;
			std::cout << "Dog destructor called for " << m_dog_name << " at " << this << std::endl;
		}
	};
}
void This::Dog::displayInfo(){
	std::cout << m_dog_name << " " << m_dog_breed << " " << *m_dog_age << "\n";
}
int thisKeyword(){
	This::Dog dog1("Fluff", "sled", 5);
	dog1.displayInfo();

	dog1.set_dog_name("Pumba");
	dog1.displayInfo();

	return 0;
}


namespace ThisEntity{
	class Entity{
	public:
		int x{}, y{};
		Entity(int x, int y){
			this->x = x;
			this->y = y;
		}
		int getX() const{
			return x;
		}
	};
}
int entityThisExample(){
	return 0;
}