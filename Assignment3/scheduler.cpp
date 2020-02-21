// =================================
// CS 433 Programming assignment 3
// Author: Cameron Collver and Nick Dujakovich
// Date: 10/30/2019
// Course: CS433 (Operating Systems)
// Description : Program to simulate CPU scheduling algorithms
// =================================

#include "scheduler.h"
#include "random.h"
#include <ctime>
#include <iostream>

RandomNumGen r((long)time(0)); // seed random number generation by time

// PURPOSE: Constructor, creates processes and populate PCBTable
// PARAM: nproc - Number of processes simulated
// PARAM: p_event_queue - Pointer to the Event queue of simulation
Scheduler::Scheduler(int nproc, EventQueue* p_event_queue)
{
	p_EQ = p_event_queue;
	CPUidle = true; // CPU is initially idle
	for(unsigned int i = 0; i < (unsigned int)nproc; i++)
	{
		PCB* pcb = new PCB(i); // create new process with id corresponding to nprocs
		pcb->arrival = r.ranInt(1000, 300000); // set arrival time to somewhere between 1s and 5 mins
		pcb->totalburst = r.ranInt(1000, 60000); // set totalburst time from 1s to 1 min
		pcb->avgburst = r.ranInt(5, 100); // set initial average burst time from 5ms to 100ms
		pcb->remainingburst = pcb->totalburst;
		procs.add_process(pcb); // add process to PCBTable
	}
	// Added implementation to create nproc processes with random distribution as described in the assignment
	// Those processes should be stored in the PCBTable
}

// PURPOSE: a wrapper function for retrieving a PCB in the PCBTable.
//		Since the PCBTable is protected in the Scheduler class, we needed a way
//		to access PCB's from main to add the Process_Arrival events.
// PARAM: PID of process
PCB* Scheduler::find_process(unsigned int id)
{
	return procs.retrieve_PCB(id);
}

// PURPOSE: prints stats associated with each process and an overall summary
// PARAM: current_time - time system ended on, used for stat calculations
void Scheduler::print_stats(int current_time)
{
	int avg_turnaround = 0, avg_waiting = 0, total_cpu = 0, num_finished  = 0;
	std::cout << "current time = " << current_time << std::endl;
	for(unsigned int i = 0; i < (unsigned int)procs.table_size(); i++)
	{
		PCB* pcb = procs.retrieve_PCB(i);
		// if there are still processes in the ready queue, remove them to complete any needed computations
		if (pcb->state == ProcState::READY)
			pop_ready_queue(current_time);
		std::cout << "Process: " << pcb->id << std::endl;
		std::cout << "arrival time: " << pcb->arrival << " ms" << std::endl;
		// if PCB has no indicated finish time, then that information shouldnt be printed
		if(pcb->finishtime > 0)
		{
			std::cout << "finish time: " << pcb->finishtime << " ms" << std::endl;
			std::cout << "turnaround time: " << pcb->get_turnaround() << " ms" << std::endl;
			avg_turnaround += pcb->get_turnaround();
			num_finished++;
		}
		std::cout << "CPU time: " << pcb->totalburst << " ms" << std::endl;
		total_cpu += pcb->get_time_in_CPU();
		std::cout << "IO time: " << pcb->totalio << " ms" << std::endl;
		std::cout << "Ready Queue Waiting time: " << pcb->rq_waittime << " ms" << std::endl;
		avg_waiting += pcb->rq_waittime;
	}
	float util = 100 * total_cpu / (float) current_time;
	float throughput = 1000 * num_finished / (float) current_time;
	std::cout << "=================================================================" << std::endl;
	std::cout << "CPU utilization: " << util << "%" << std::endl;
	std::cout << "Throughput: " << throughput << " jobs/s" << std:: endl;
	// if no processes finished, then there is no turnaround to compute
	if (num_finished == 0)
		std::cout << "Average turnaround time: " << 0 << std::endl;
	else
		std:: cout << "Average turnaround time: " << avg_turnaround / num_finished << " ms" << std::endl;
	std::cout << "Average waiting time: " << avg_waiting / procs.table_size() <<  " ms" << std::endl;
}

// PURPOSE: Event handling function. It calls actual handling function according to the event type
// PARAM: e - Event to be handled
void Scheduler::handle_the_event(const Event& e)
{
	// Call different handling function according to the event type
	switch (e.type)
	{
	case Event::Process_Arrival:
		handle_proc_arrival(e);
		break;
	case Event::CPU_Burst_Completion:
		handle_cpu_completion(e);
		break;
	case Event::IO_Burst_Completion:
		handle_io_completion(e);
		break;
	case Event::Timer_Expiration:
		handle_timer_expiration(e);
		break;
	}
}

// PURPOSE: Event handling function for process arrival event
// PARAM: e - Process_Arrival event to be handled 
void Scheduler::handle_proc_arrival(const Event& e)
{
	unsigned int proc = e.procID; // get process ID associated with event
	int current_time = e.time; // get current time of simulation
	PCB* p = procs.retrieve_PCB(proc); // find that process in the PCBTable
	p->state = ProcState::READY; // change process state to ready
	p->nextburst = r.CPUBurstRandom(p->avgburst); // compute next CPU burst time based on avg
	add_to_ready_queue(p, current_time); // add the process to the readyqueue
	schedule(current_time); // if CPU is idle, then next process in readyqueue will be scheduled
}

// PURPOSE: Event handling function for CPU burst completion event
// PARAM: e - CPU_Completion event to be handled 
void Scheduler::handle_cpu_completion(const Event& e)
{
	unsigned int proc = e.procID; // get process ID associated with event
	int current_time = e.time; // get current time of simulation
	PCB* p = procs.retrieve_PCB(proc); // find that process in the PCBTable 
	p->remainingburst -= p->nextburst; // calculate remaining burst
	//if the process has completed its total duration
	if(p->remainingburst <= 0)
	{
		p->state = ProcState::TERMINATED;
		p->finishtime = e.time;
	}
	// else CPU is done, we are generating an IO event
	else
	{
		p->ioburst = r.ranInt(30, 100); // generate random IO burst from 30ms to 100ms
		p->state = ProcState::WAITING;
		// create new IO event and assign time to when IO burst will complete
		Event* ioevent = new Event(Event::IO_Burst_Completion, current_time + p->ioburst, p->id);
		p_EQ->push(*ioevent); // add IO event to event queue
	}
	CPUidle = true; // CPU has completed burst so it is now idle
	schedule(current_time); // if CPU is idle, then next process in readyqueue will be scheduled
}

// PURPOSE: Event handling function for IO completion event
// PARAM: e - IO_Completion event to be handled
void Scheduler::handle_io_completion(const Event& e)
{
	unsigned int proc = e.procID; // get process ID associated with event
	int current_time = e.time; // get current time of simulation
	PCB* p = procs.retrieve_PCB(proc); // find that process in PCBTable
	p->totalio += p->ioburst; // keep track of total time spent in IO
	if (p->remainingburst - p->nextburst < 0)
		p->nextburst = p->remainingburst;
	else
		p->nextburst = r.CPUBurstRandom(p->avgburst); // find next CPU burst
	p->state = ProcState::READY; // set state to READY
	add_to_ready_queue(p, current_time); // add process to ready queue
	schedule(current_time); // if CPU is idle, then next process in readyqueue will be scheduled
}

// PURPOSE: Event handling for Timer_Expiration event
//      Unused for now since we haven't implemented any preemptive algorithms
// PARAM: e - Timer_Expiration event to be handled
void Scheduler::handle_timer_expiration(const Event& e)
{
	// TODO: Add implementation for when we do any preemptive algorithms (RR, etc.)
}
