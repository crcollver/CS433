// =================================
// CS 433 Programming assignment 1
// Author: Cameron Collver and Nick Dujakovich
// Date: 9/18/2019
// Course: CS433 (Operating Systems)
// Description : Program to implement a priority ready queue of processes
// =================================

#include "PCB.h"
#include <cstdlib>

// TODO: Add your implementation here

PCB::PCB() {
	id = 0;
	priority = 0;
	state = ProcState::NEW;
}

PCB::PCB(unsigned int value) {
	this->id = value;
	this->priority = value;
	state = ProcState::NEW;
}

PCB::~PCB() {}

// PURPOSE: sets priority of PCB manually, != to PID
// PARAM: the priority to set PCB to
void PCB::setPriority(unsigned int value) {
	this->priority = value;
}

PCBTable::PCBTable() {
	processCount = 0;
	populateTable();
}

PCBTable::~PCBTable(){}

//PURPOSE: Populate PCBTable with MAX number of NEW processes
void PCBTable::populateTable() {
	for (int i = 0; i < MAX; i++)
	{
		PCB proc(i + 1); // initializes PCB objects from 1 - MAX
		this->table[i] = proc; // places PCB objects at each spot in array
		processCount++;
	}
}

//PURPOSE: retrieves a PCB from the table, and returns a pointer to the PCB
//PARAM: the PID of the process to be retrieved
PCB* PCBTable::retrievePCB(unsigned int i) {
	PCB* temp = &table[i - 1];
	return temp;
}

// PURPOSE: retrieves amount of processes in PCBTable
int PCBTable::procSize() {
	return processCount;
}


