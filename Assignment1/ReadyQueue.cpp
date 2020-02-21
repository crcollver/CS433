#include <iostream>
#include "ReadyQueue.h"

// =================================
// CS 433 Programming assignment 1
// Author: Cameron Collver and Nick Dujakovich
// Date: 9/18/2019
// Course: CS433 (Operating Systems)
// Description : Program to implement a priority ready queue of processes
// =================================

// TODO: Add your implementation of ReadyQueue here

// PURPOSE: add the PCB of a process into the ready queue.
// PARAM: PCB from PCBTable to be inserted
void ReadyQueue::insertProc(PCB* pcbPtr) {
	readyqueue[count] = pcbPtr;
	pcbPtr->state = ProcState::READY;
	count++;
	trickleUp();
}

// PURPOSE: remove and return the PCB with the highest priority from the queue
PCB* ReadyQueue::removeHighestProc() {
	readyqueue[0]->state = ProcState::RUNNING;
	PCB* temp = readyqueue[0];
	sortQueue();
	return temp;
}

// PURPOSE: Returns the number of elements in the queue.
int ReadyQueue::size() {
	return count;
}

// PURPOSE: Prints the queue contents to standard output.
void ReadyQueue::displayQueue() {
	std::cout << "Display processes in ReadyQueue: " << std::endl;
	for (int i = 0; i < count; i++)
	{
		std::cout << "Proc ID = " << readyqueue[i]->id << ", priority = " << 
			readyqueue[i]->priority << std::endl;
	}
}

//PURPOSE: after removing highest priority PCB, we must bring the next highest priority
//	to the front.
void ReadyQueue::sortQueue() {
	readyqueue[0] = readyqueue[count - 1];  // since we have executed highest priority, fill it with last element
	count--;	// decrease the size of our queue

	int current = 0; // start at the root index
	int smaller = getSmallerChild(current); // get the index of the smallest child

	while (smaller != -1) // while x has not fallen off the tree
	{
		if (readyqueue[smaller]->priority < readyqueue[current]->priority) // if the child is smaller than element at current PCB
		{
			std::swap(readyqueue[smaller], readyqueue[current]);
			current = smaller; // change index of PCB we are moving to where it was swapped
			smaller = getSmallerChild(current); // get the new child index of current PCB
		}
		else
			break; // otherwise, the current PCB is in the correct place
	}
}


//PURPOSE: After adding PCB to end of ready Queue, move it to correct priority in the queue
void ReadyQueue::trickleUp() {
	int justadded = count - 1; // gives index of PCB that was just added
	int parent = getParent(justadded); // gets parent of newly added PCB

	// eventually get to 0 once we hit the root
	while (justadded > 0)
	{
		// if the parent's priority is larger than the priority of the PCB just added
		if (readyqueue[parent]->priority > readyqueue[justadded]->priority)
		{
			std::swap(readyqueue[parent], readyqueue[justadded]);
			justadded = parent; // index of PCB changes to what the parent's index was
			parent = getParent(justadded); // get the new parent
		}
		else
			break; // otherwise the PCB is in the correct place in queue
	}
}

// PURPOSE: retrieves position of the parent PCB 
// PARAM: index position of a particular child PCB in the ready queue
int ReadyQueue::getParent(int child) {
	if (isEven(child)) // if the child is in an even position
		return (child / 2) - 1;
	else // else if it is in an odd position
		return (int)child / 2;
}

//PURPOSE: determine if index x is even (helps with calculating position)
bool ReadyQueue::isEven(int x) {
	if ((x % 2) == 0)
		return true;
	else
		return false;
}

// PURPOSE: retrieves position of lowest priority child (left or right of parent)
// PARAM: index position of a particular parent PCB in ready queue
int ReadyQueue::getSmallerChild(int parent) {
	// if we're at the end of the tree
	if ((2 * parent + 1 > count) || (2 * parent + 2 > count)) 
		return -1;	//indicate that we are at the end of the tree

	// if the left child's priority is smaller
	else if (readyqueue[2 * parent + 1]->priority < readyqueue[2 * parent + 2]->priority) 
		return 2 * parent + 1; // return the index left child

	// else we know that the right child's priority is smaller
	else 
		return 2 * parent + 2; // return the index of the right child
}