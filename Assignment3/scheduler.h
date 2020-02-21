// =================================
// CS 433 Programming assignment 3
// Author: Cameron Collver and Nick Dujakovich
// Date: 10/30/2019
// Course: CS433 (Operating Systems)
// Description : Program to simulate CPU scheduling algorithms
// =================================

#pragma once
#include "event.h"
#include "PCB.h"

// Base Scheduler Class
class Scheduler {
public:
    // PURPOSE: Constructor, creates processes and populate PCBTable
    // PARAM: nproc - Number of processes simulated
    // PARAM: p_event_queue - Pointer to the Event queue of simulation
    Scheduler(int nproc, EventQueue* p_event_queue);

    // Destructor
    virtual ~Scheduler() {};

    // PURPOSE: a wrapper function for retrieving a PCB in the PCBTable.
    //		Since the PCBTable is protected in the Scheduler class, we needed a way
    //		to access PCB's from main to add the Process_Arrival events.
    // PARAM: PID of process
    PCB* find_process(unsigned int id);

    // PURPOSE: prints stats associated with each process and an overall summary
    // PARAM: current_time - time system ended on, used for stat calculations
    void print_stats(int current_time);

    // PURPOSE: Event handling function. It calls actual handling function according to the event type
    // PARAM: e - Event to be handled
    void handle_the_event(const Event& e);

	// PURPOSE: Check if the scheduler should dispatch a ready process to run
	//      A pure virtual function to be overridden in the subclass of specific scheduler.
    // PARAM: current_time - Used for generating completion times for CPU completion events
	virtual void schedule(int current_time) = 0;

	// PURPOSE: Add a process to ready queue.
    //      A pure virtual function to be overridden in the subclass of specific scheduler.
	// PARAM: pcb - PCB to add to ready queue
    // PARAM: current_time - Used to mark arrival time to readyqueue
	virtual void add_to_ready_queue(PCB* pcb, int current_time) = 0;

	// PURPOSE: Remove next process from ready queue according to the scheduling algorithm
	//      A pure virtual function to be overridden in the subclass of specific scheduler.
    // PARAM: current_time - Used to mark departure from readyqueue
	virtual PCB* pop_ready_queue(int current_time) = 0;

protected:
    // PURPOSE: Event handling function for process arrival event
    // PARAM: e - Process_Arrival event to be handled 
    void handle_proc_arrival(const Event& e);

    // PURPOSE: Event handling function for CPU burst completion event
    // PARAM: e - CPU_Completion event to be handled 
    void handle_cpu_completion(const Event& e);

	// PURPOSE: Event handling function for IO completion event
	// PARAM: e - IO_Completion event to be handled
	void handle_io_completion(const Event& e);

    // PURPOSE: Event handling for Timer_Expiration event
    //      Unused for now since we haven't implemented any preemptive algorithms
    // PARAM: e - Timer_Expiration event to be handled
	void handle_timer_expiration(const Event& e);

    // determines if CPU is occupied
    bool CPUidle;

	// Table of all processes 
	PCBTable procs;

	// Pointer to the event queue
	EventQueue* p_EQ;
};

