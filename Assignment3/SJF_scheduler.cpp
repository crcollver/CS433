// =================================
// CS 433 Programming assignment 3
// Author: Cameron Collver and Nick Dujakovich
// Date: 10/30/2019
// Course: CS433 (Operating Systems)
// Description : Program to simulate CPU scheduling algorithms
// =================================

#include "SJF_scheduler.h"

// PURPOSE: adds a PCB to the readyqueue and marks arrival time into queue
// PARAM: pcb - PCB to be added 
// PARAM: current_time - used to mark arrival
void SJFScheduler::add_to_ready_queue(PCB* pcb, int current_time)
{
	pcb->rq_arrival = current_time; // mark arrival time
	readyqueue.push(pcb); // add pcb to ready queue
}

// PURPOSE: removes first element from readyqueue, marks its departure time
// PARAM: current_time - used to update departure from queue
PCB* SJFScheduler::pop_ready_queue(int current_time)
{
	PCB* pcb = readyqueue.top(); // store first element
	pcb->rq_leave = current_time; // mark departure time
	// calculate wait time spent in readyqueue based on arrival and leave
	pcb->rq_waittime = pcb->rq_waittime + pcb->rq_leave - pcb->rq_arrival;
	readyqueue.pop(); // remove PCB from readyqueue
	return pcb;
}

// PURPOSE: schedules a PCB from readyqueue if CPU is idle and readyqueue is not empty
// PARAM: current_time - used to determine end of current CPU burst
void SJFScheduler::schedule(int current_time)
{
	if (CPUidle && !readyqueue.empty())
	{
		PCB* p = pop_ready_queue(current_time); // must access data from first process
		p->state = ProcState::RUNNING; // set state to RUNNING
		// generate a CPU burst completion event at time when CPU burst will complete
		Event* cpuevent = new Event(Event::CPU_Burst_Completion, current_time + p->nextburst, p->id);
		p_EQ->push(*cpuevent); // add CPU completion event to event queue
		CPUidle = false; // CPU is no longer idle
	}
}
