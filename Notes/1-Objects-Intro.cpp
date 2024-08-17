#include <iostream>
#include <string>

class Animal {
	public:
		std::string name;
		std::string sound;

		Animal(std::string name, std::string sound) : name(name), sound(sound) {}
		void Sound() const{
			std::cout << name << "makes " << sound << std::endl;
	}
};

class Dog: public Animal{
	public:
		Dog(std::string name) : Animal(name, "woof") {}
};

class Cat: public Animal{
	public:
		Cat(std::string name) : Animal(name, "meow") {}
};

Animal lion("Alex", "roar");
Animal dog("Rex", "woof");

Dog myDog("Tony");
Cat myCat("Schrödinger");

int main() {
	lion.Sound();
	dog.Sound();

	myDog.Sound();
	myCat.Sound();

	std::cout << "My dog's name is " << myDog.name << std::endl;

	return 0;
}
