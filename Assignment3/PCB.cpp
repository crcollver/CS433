// =================================
// CS 433 Programming assignment 3
// Author: Cameron Collver and Nick Dujakovich
// Date: 10/30/2019
// Course: CS433 (Operating Systems)
// Description : Program to simulate CPU scheduling algorithms
// =================================

#include "PCB.h"

// PURPOSE: Constructor for PCB
// PARAM: id - used for PID of PCB
PCB::PCB(unsigned int id) {
	this->id = id; // sets id relative to how many processes are needed for simulation
    state = ProcState::NEW; // indicate that this is a new process
	arrival = totalburst = remainingburst = 
		avgburst = nextburst = ioburst = 
		totalio = rq_waittime = finishtime = 
		rq_arrival = rq_leave = priority = 0;
}

PCB::~PCB() {}

// PURPOSE: calculates turnaround based on arrival and finish time
int PCB::get_turnaround()
{
    return finishtime - arrival;
}

// PURPOSE: calculates time each process spent in CPU
int PCB::get_time_in_CPU()
{
	return totalburst - remainingburst;
}

// PURPOSE: adds a process to the process table
// PARAM: pointer to the PCB to add
void PCBTable::add_process(PCB* p) {
    table.push_back(p);
}

// PURPOSE: retrieves a PCB from the table, and returns a pointer to the PCB (may not need)
// PARAM: the PID of the process to be retrieved
PCB* PCBTable::retrieve_PCB(unsigned int id) {
    return table.at(id);
}

// PURPOSE: returns amount of processes in PCBTable
int PCBTable::table_size() {
    return table.size();
}

