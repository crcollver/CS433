// =================================
// CS 433 Programming assignment 3
// Author: Cameron Collver and Nick Dujakovich
// Date: 10/30/2019
// Course: CS433 (Operating Systems)
// Description : Program to simulate CPU scheduling algorithms
// =================================

#pragma once
#include "scheduler.h"
#include <queue>

class FCFSScheduler : public Scheduler
{
private:
	// standard queue for FCFS
	std::queue<PCB*> readyqueue;
public:
	// PURPOSE: First Come First Served (SJF) Scheduler Constructor
	// PARAM: nproc nproc Number of processes simulated
	// PARAM: p_event_queue p_event_queue Pointer to the Event queue of simulation
	FCFSScheduler(int nproc, EventQueue* p_event_queue)
		: Scheduler(nproc, p_event_queue)
	{
	}

	// PURPOSE: adds a PCB to the readyqueue and marks arrival time into queue
	// PARAM: pcb - PCB to be added 
	// PARAM: current_time - used to mark arrival
	virtual void add_to_ready_queue(PCB* pcb, int current_time) override;

	// PURPOSE: removes first element from readyqueue, marks its departure time
	// PARAM: current_time - used to update departure from queue
	virtual PCB* pop_ready_queue(int current_time) override;

	// PURPOSE: schedules a PCB from readyqueue if CPU is idle and readyqueue is not empty
	// PARAM: current_time - used to determine end of current CPU burst
	virtual void schedule(int current_ttime) override;
};
