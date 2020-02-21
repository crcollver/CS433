// =================================
// CS 433 Programming assignment 1
// Author: Cameron Collver and Nick Dujakovich
// Date: 9/18/2019
// Course: CS433 (Operating Systems)
// Description : Program to implement a priority ready queue of processes
// =================================

// Remember to add comments to your code

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include "ReadyQueue.h"

int main(int argc, char* argv[]) {
	//Print basic information about the program
	std::cout << "CS 433 Programming assignment 1" << std::endl;
	std::cout << "Author: Cameron Collver and Nick Dujakovich" << std::endl;
	std::cout << "Date: 9/18/2019" << std::endl;
	std::cout << "Course: CS433 (Operating Systems)" << std::endl;
	std::cout << "Description : Program to implement a priority ready queue of processes" << std::endl;
	std::cout << "Assumption : PCBTable is instantiated before the ReadyQueue" << std::endl;
	std::cout << "=================================" << std::endl;

	// Test 1
	PCBTable t; // creates PCBTable with 20 entries, numbered 1-20
	ReadyQueue q1; // creates ReadyQueue with equivalent amount of entries
	std::cout << "********Performing Test 1**********" << std::endl;
	std::cout << "Add processes 5, 1, 8, 11 to q1. Display the content of q1" << std::endl;
	q1.insertProc(t.retrievePCB(5));
	q1.insertProc(t.retrievePCB(1));
	q1.insertProc(t.retrievePCB(8));
	q1.insertProc(t.retrievePCB(11));
	q1.displayQueue();

	std::cout << "remove the process with the highest priority from q1 and display q1." << std::endl;
	q1.removeHighestProc();
	q1.displayQueue();

	std::cout << "remove the process with the highest priority from q1 and display q1." << std::endl;
	q1.removeHighestProc();
	q1.displayQueue();
	std::cout << "Insert processes 3, 7, 2, 12, 9 to q1 and display q1." << std::endl;
	q1.insertProc(t.retrievePCB(3));
	q1.insertProc(t.retrievePCB(7));
	q1.insertProc(t.retrievePCB(2));
	q1.insertProc(t.retrievePCB(12));
	q1.insertProc(t.retrievePCB(9));
	q1.displayQueue();

	// remove all elements from current ReadyQueue
	while (q1.size() != 0)
	{
		q1.removeHighestProc();
		q1.displayQueue();
	}
	
	// Test 2
	std::cout << "*********Performing Test 2***********" << std::endl;
	srand((unsigned) time(NULL)); // seeds random number generation


	// add 10 random processes from PCBTable
	for(int i = 0; i < 10; i++)
	{
		PCB* temp = t.retrievePCB((rand() % 20) + 1); // retrieve a random PCB with PID btwn 1-20

		// checks if we're grabbing a PCB that is already in the readyqueue
		while (temp->state == ProcState::READY)
		{
			temp = t.retrievePCB((rand() % 20) + 1);
		}
		
		temp->setPriority((rand() % 50) + 1);	// sets priority of process to something random (1-50)
		q1.insertProc(temp);	// adds it to the readyqueue

	}

	clock_t start;	// used for timing the loop
	start = clock(); // gets starting time

	for (int i = 0; i < 1000000; i++)
	{
		int probability = (rand() % 2);
		if (probability == 0)
		{
			// as long as the queue is not empty, we can remove the highest priority
			if (q1.size() != 0)
			{
				q1.removeHighestProc();
			}
		}
		else
		{
			// we MUST choose a process to INSERT
			// as long as the queue isn't full, we can add processes
			if (q1.size() < t.procSize())
			{
				PCB* temp = t.retrievePCB((rand() % 20) + 1); // grabs another random process
				// checks if we're grabbing a PCB that is already in the readyqueue
				// if it is already in the readyqueue, move through the PCBTable instead of randomly selecting
				int count = t.procSize();
				while (count > 0 && temp->state == ProcState::READY)
				{
					temp = t.retrievePCB(count);
					count--;
				}
				temp->setPriority((rand() % 50) + 1);
				q1.insertProc(temp);
			}
		}
	}
	start = clock() - start;	// how much time has elapsed since clock started
	//start / CLOCKS_PER_SEC gives total time in seconds
	std::cout << "Total execution time: " << std::fixed << (double) start / CLOCKS_PER_SEC << std::setprecision(6) << " sec" << std::endl;
	q1.displayQueue();
}
