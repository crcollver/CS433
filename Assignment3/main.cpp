// =================================
// CS 433 Programming assignment 3
// Author: Cameron Collver and Nick Dujakovich
// Date: 10/30/2019
// Course: CS433 (Operating Systems)
// Description : Program to simulate CPU scheduling algorithms
// =================================

#include <iostream>
#include <cstdlib>
#include "event.h"
#include "SJF_scheduler.h"
#include "FCFS_scheduler.h"

int main(int argc, char* argv[]) {
	//Print basic information about the program
	std::cout << "=================================================================" << std::endl;
	std::cout << "CS 433 Programming assignment 3" << std::endl;
	std::cout << "Author: Cameron Collver and Nick Dujakovich" << std::endl;
	std::cout << "Date: 10/30/2019" << std::endl;
	std::cout << "Course: CS433 (Operating Systems)" << std::endl;
	std::cout << "Description : Program to simulate CPU scheduling algorithms" << std::endl;
	std::cout << "=================================================================\n" << std::endl;

	int nproc = 1;		// number of processes to be simulated
	int scheduler_type = 1;
	// This program needs a command-line argument for the number of simulated processes  
	if(argc < 3 )
	{
		std::cerr << "Must specify the number of processes for simulation and type of scheduler" << std::endl;
		std::cerr << "Usage: " << argv[0] << " <np> <scheduler type>" << std::endl;
		std::cerr << "scheduler type 1: FCFS, 2: SJF" << std::endl;
		exit(1);
	} else
	{
		nproc = atoi(argv[1]);
		scheduler_type = atoi(argv[2]);
		if(nproc < 1)  // Do some error checking
		{
			std::cerr << "Must have at least 1 process for the simulation." << std::endl;
			exit(1);
		}
		if(scheduler_type < 1 || scheduler_type > 2)
		{
			std::cerr << "Scheduler type must be 1 (FCFS) or 2 (SJF)." << std::endl;
			exit(1);
		}
	}
	// Create a event queue 
	EventQueue event_que;
	const int QUIT_TIME = 300000;	// 300,000 ms = 5 minutes

	Scheduler *p_scheduler = NULL; // compiler was complaining about uninitalized pointer
	if(scheduler_type == 1) {
		std::cout << "****************Simulate FCFS scheduler****************************" << std::endl;
		p_scheduler = new FCFSScheduler(nproc, &event_que);
	} else if (scheduler_type == 2)
	{
		std::cout << "****************Simulate SJF scheduler****************************" << std::endl;
		p_scheduler = new SJFScheduler(nproc, &event_que);
	}

	// add Process_Arrival events of all processes to the event queue
	for(int i = 0; i < nproc; i++)
	{
		Event* e = new Event(Event::Process_Arrival, 
			p_scheduler->find_process(i)->arrival, 
			p_scheduler->find_process(i)->id);
		event_que.push(*e);
	}
	
	int current_time = 0;
	// Main loop of the simulation
	while(!event_que.empty() && current_time < QUIT_TIME) {
		Event e = event_que.top(); // get the next event
		event_que.pop(); // remove the next event from queue
		current_time = e.time; // set current time to time that next event occurs
		// Handle the event 
		p_scheduler->handle_the_event(e);		
	}
	p_scheduler->print_stats(current_time); // print stats on simulation

	delete p_scheduler;
}
