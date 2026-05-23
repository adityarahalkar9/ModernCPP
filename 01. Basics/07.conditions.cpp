#include <iostream>
#include "basic_header.h"

void IfElse(){
	int age{26};
	if(age < 18){
		std::cout << "Not and adult" << std::endl;
	}
	else if(age >= 18){
		std::cout << "Adult" << std::endl;
	}
	else{
		std::cout << "Invalid Input" << std::endl;
	}
}

void SwitchStatement(){
	int day = 4;	// Assume 1 = Mon, 2 = Tue, ..., 7 = Sun

	switch(day){
		case 1:
			std::cout << "Monday" << std::endl;
			break;
		case 2:
			std::cout << "Tuesday" << std::endl;
			break;
		case 3:
			std::cout << "Wednesday" << std::endl;
			break;
		case 4:
			std::cout << "Thursday" << std::endl;
			break;
		case 5:
			std::cout << "Friday" << std::endl;
			break;
		case 6:
			std::cout << "Saturday" << std::endl;
			break;
		case 7:
			std::cout << "Sunday" << std::endl;
			break;
	}
}



int Conditions(){
	// IfElse();
	// SwitchStatement();

	return 0;
}