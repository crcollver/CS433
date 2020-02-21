// =================================
// CS 433 Programming assignment 3
// Author: Cameron Collver and Nick Dujakovich
// Date: 10/30/2019
// Course: CS433 (Operating Systems)
// Description : Program to simulate CPU scheduling algorithms
// =================================

#pragma once
#include <vector>

// enum class of process state
// A process (PCB) in ready queue should be in READY state
enum class ProcState {NEW, READY, RUNNING, WAITING, TERMINATED};

/* 
Process control block(PCB) is a data structure representing a process in the system.
A process should have at least an ID and a state(i.e.NEW, READY, RUNNING, WAITING or TERMINATED).
It may also have other attributes, such as scheduling information (e.g. priority)
*/
class PCB {
public:
	int arrival; // randomly determined between 5 min time span
	int rq_arrival; // time stamp for when process arrives in readyqueue
	int rq_leave; // time stamp for when process leaves readyqueue
	int totalburst; // determined at process creation (1 second - 1 min)
	int remainingburst; // whenever process completes a burst, new time is calculated
	int avgburst; // determined at creation time (5ms - 100ms), used for nextburst
	int nextburst; // determined when process is moved into ready queue
	int ioburst; // generated as each process completes I/O burst (30ms - 100ms)
	int totalio; // adds up total time spent in IO burst
	int rq_waittime; // time process spent waiting in ready queue
	int finishtime; // final time when process finishes
	unsigned int id; // process id, assigned at creation
	unsigned int priority; // we are using total burst times as a form of priority
	ProcState state; // what state process is in

	// PURPOSE: calculates turnaround based on arrival and finish time
	int get_turnaround();

	// PURPOSE: calculates time each process spent in CPU
	int get_time_in_CPU();

	// PURPOSE: Constructor for PCB
	// PARAM: id - used for PID of PCB
	PCB(unsigned int id);
	~PCB();
};

// PURPOSE: Compare based on the next shortest CPU burst
// PARAM: p1 and p2 are PCBs in readyqueue that need to be sorted by next burst time
class PCBCompare {
public:
	bool operator() (const PCB* p1, const PCB* p2)
	{
		return (p1->nextburst > p2->nextburst);
	}
};

/*
An array(list) of all PCB elements in the system.
*/
class PCBTable {
private:
	std::vector<PCB*> table;
public:
	// PURPOSE: adds a process to the process table
	// PARAM: pointer to the PCB to add
	void add_process(PCB* p);

	// PURPOSE: retrieves a PCB from the table, and returns a pointer to the PCB
	// PARAM: the PID of the process to be retrieved
	PCB* retrieve_PCB(unsigned int id);

	// PURPOSE: returns amount of processes in PCBTable
	int table_size();
};