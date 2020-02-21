#pragma once

// =================================
// CS 433 Programming assignment 1
// Author: Cameron Collver and Nick Dujakovich
// Date: 9/18/2019
// Course: CS433 (Operating Systems)
// Description : Program to implement a priority ready queue of processes
// =================================

// Remember to add comments to your code
// ...

// enum class of process state
// A process (PCB) in ready queue should be in READY state
enum class ProcState {NEW, READY, RUNNING, WAITING, TERMINATED};

/* 
Process control block(PCB) is a data structure representing a process in the system.
A process should have at least an ID and a state(i.e.NEW, READY, RUNNING, WAITING or TERMINATED).
It may also have other attributes, such as scheduling information (e.g. priority)
*/
class PCB {
	// TODO: Add the body of PCB class
public:
	unsigned int id;
	unsigned int priority;
	ProcState state;
	PCB();
	PCB(unsigned int value); // create PCB with specified id and priority
	~PCB();

	// PURPOSE: sets priority of PCB manually, != to PID
	// PARAM: the priority to set PCB to
	void setPriority(unsigned int value);
};

/*
An array(list) of all PCB elements in the system.
Assumes a Table of 20 entries, as per instructions
*/
class PCBTable {
private:	// TODO: Add your implementation here
	static const int MAX = 20;
	PCB table[MAX] = {};
	int processCount;	// how many processes are in the table
public:

	// PURPOSE: Populate PCBTable with MAX number of NEW processes
	void populateTable();

	// PURPOSE: retrieves a PCB from the table, and returns a pointer to the PCB
	// PARAM: the PID of the process to be retrieved
	PCB* retrievePCB(unsigned int i);

	// PURPOSE: retrieves amount of processes in PCBTable
	int procSize();
	PCBTable();
	~PCBTable();
};
