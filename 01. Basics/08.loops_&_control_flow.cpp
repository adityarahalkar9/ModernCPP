#include "basic_header.h"
#include <iostream>

/*
	loops are the workhorses that allow you to execute a block of code repeatedly
	until a specific condition is met.
*/

// 	The for Loop
/*
	This is your go-to when you know exactly how many times you want to run the code.
	It keeps all your control logic (initialization, condition, and increment) in one line.
*/
void ForLoop(){
	for(int i = 0; i < 10; ++i){
		std::cout << i << "\n";
	}
}

// The while loop
/*
	Use this when you don't necessarily know the count, but you know the condition that should
	stop it. It checks the condition before executing the body.
*/
//  If the condition is false at the very beginning, the code inside the while loop will never run.
void WhileLoop(){
	int energy{3};
	while(energy > 0){
		std::cout << "Still running...." << std::endl;
		energy--;
	}
}

// The do-while loop
/*
	This is a variation of the while loop with one key difference: it checks the condition
	after the code runs. This guarantees the block executes at least once.
*/
void DoWhileLoop(){
	int choice;
	do{
		std::cout << "Enter 0 to quit: ";
		std::cin >> choice;
	} while(choice != 0);
}

// Range-based for Loop
/*
	If you are working with a collection (like an array or a vector), this is the cleanest
	way to iterate through every element without worrying about indices or counters.
*/
void RangedLoop(){
	int grades[] = {95,88,72,99};
	for(int n : grades){
		std::cout << "Grade: " << n << std::endl;
	}
}

/*
	continue keyword
	=> Instantly stops the current iteration and jumps to the start of the next one.
	=> Works inside for, while, and do-while.
	=> Useful when you want to skip processing certain elements.
*/
void ContinueStatement(){
	for(int i = 0; i < 5; ++i){
		if(i == 3){
			continue;	// Skips the element when i = 3
		}
		std::cout << i << std::endl;
	}
}

/*
	break keyword
	=> Immediately exits the innermost loop or switch block.
	=> Control goes to the statement right after the loop.
*/
void BreakStatement(){
	for(int i = 0; i < 5; ++i){
		if(i == 3){
			break;  // Exits the loop entirely
		}
		std::cout << i << std::endl;
	}
}



int ControlFlow(){
	// ForLoop();
	// WhileLoop();
	// DoWhileLoop();
	// RangedLoop();
	// ContinueStatement();
	// BreakStatement();

	return 0;
}