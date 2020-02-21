// =================================
// CS 433 Programming assignment 1
// Author: Cameron Collver and Nick Dujakovich
// Date: 9/18/2019
// Course: CS433 (Operating Systems)
// Description : Program to implement a priority ready queue of processes
// =================================

// Remember to add comments to your code
// ...

#pragma once

#include "PCB.h"

//Assumes a ReadyQueue of 20 entries to match PCBTable, as per instructions
class ReadyQueue {
private:
	static const int MAX = 20;
	PCB* readyqueue[MAX] = {};
	int count;

	//PURPOSE: After adding PCB to end of ready Queue, move it to correct priority in the queue
	void trickleUp();

	// PURPOSE: retrieves position of lowest priority child (left or right of parent)
	// PARAM: index position of a particular parent PCB in ready queue
	int getSmallerChild(int parent);

	// PURPOSE: retrieves position of the parent PCB 
	// PARAM: index position of a particular child PCB in the ready queue
	int getParent(int child);

	//PURPOSE: determine if index x is even (helps with calculating position)
	bool isEven(int x);

	//PURPOSE: after removing highest priority PCB, we must bring the next highest priority
	//	to the front.
	void sortQueue();

public:
	// TODO: Implement the required functions. Add necessary member fields and functions
	// You may use different parameter signatures from the example below

	// PURPOSE: add the PCB of a process into the ready queue.
	// PARAM: PCB from PCBTable to be inserted
	void insertProc(PCB* pcbPtr);

	// PURPOSE: remove and return the PCB with the highest priority from the queue
	PCB* removeHighestProc();

	// PURPOSE: Returns the number of elements in the queue.
	int size();

	// PURPOSE: Prints the queue contents to standard output.
	void displayQueue();

	ReadyQueue() { count = 0; };

};