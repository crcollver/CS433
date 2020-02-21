// =================================
// CS 433 Programming Assignment 5
// Author: Cameron Collver and Nick Dujakovich
// Date: 12/06/2019
// Course: CS433 (Operating Systems)
// Description : Program to simulate different page replacement algorithms
// =================================

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <queue>
#include <ctime>
#include <iomanip>
#include <random>
#include "pagetable.h"

// Check if an integer is power of 2
bool isPowerOfTwo(unsigned int x)
{
	/* First x in the below expression is for the case when x is 0 */
	return x && (!(x & (x - 1)));
}

// prints appropriate statistics for each algorithm
void print_statistics(int ref_count, int num_faults, int num_replacements)
{
	std::cout << "Number of references:\t\t" << ref_count << std::endl << "Number of faults:\t\t" << num_faults <<
		std::endl << "Number of page replacements:\t" << num_replacements << std::endl;
}

int main(int argc, char* argv[]) {
	//Print basic information about the program
	std::cout << "=================================================================" << std::endl;
	std::cout << "CS 433 Programming assignment 5" << std::endl;
	std::cout << "Author: Cameron Collver and Nick Dujakovich" << std::endl;
	std::cout << "Date: 12/06/2019" << std::endl;
	std::cout << "Course: CS433 (Operating Systems)" << std::endl;
	std::cout << "Description : Program to simulate different page replacement algorithms" << std::endl;
	std::cout << "=================================================================\n" << std::endl;

	if (argc < 3) {
		// user does not enter enough parameters
		std::cout << "You have entered too few parameters to run the program.  You must enter" << std::endl
			<< "two command-line arguments:" << std::endl
			<< " - page size (in bytes): between 256 and 8192, inclusive" << std::endl
			<< " - physical memory size (in megabytes): between 4 and 64, inclusive" << std::endl;
		exit(1);
	}
	
	// Page size and Physical memory size 
	// Their values should be read from command-line arguments, and always a power of 2
	unsigned int page_size = atoi(argv[1]);
	if(!isPowerOfTwo(page_size))
	{
		std::cout << "You have entered an invalid parameter for page size (bytes)" << std::endl
			<< "  (must be an power of 2 between 256 and 8192, inclusive)." << std::endl;
		return 1;
	}
	unsigned int phys_mem_size = atoi(argv[2]) << 20; // convert from MB to bytes
	if(!isPowerOfTwo(phys_mem_size))
	{
		std::cout << "You have entered an invalid parameter for physical memory size (MB)" << std::endl
			<< "  (must be an even integer between 4 and 64, inclusive)." << std::endl;
		return 1;
	}

	// calculate number of pages and frames;
	int logic_mem_bits = 27;		// 27-bit logical memory (128 MB logical memory assumed by the assignment)
	int phys_mem_bits = (int) std::log2(phys_mem_size);		// Num of bits for physical memory addresses, calculated from physical memory size, e.g. 24 bits for 16 MB memory
	int page_offset_bits = (int) std::log2(page_size);				// Num of bits for page offset, calculated from page size, e.g. 12 bits for 4096 byte page
	// Number of pages in logical memory = 2^(logic_mem_bits - page_bit)
	int num_pages = 1 << (logic_mem_bits - page_offset_bits);
	// Number of free frames in physical memory = 2^(phys_mem_bits - page_offset_bits)
	int num_frames = 1 << (phys_mem_bits - page_offset_bits);

	int logical_address, is_fault, frame, page, num_faults, ref_count, num_replacements;
	PageTable table1(num_pages); // create a page table with calculated number of pages
	PageEntry* victim;
	srand((unsigned int)time(NULL));
	
	std::cout << "Page size = " << page_size << " bytes" << std::endl;
	std::cout << "Physical Memory size = " << phys_mem_size << " bytes" << std::endl;
	std::cout << "Number of pages = " << num_pages << std::endl; 
	std::cout << "Number of physical frames = " << num_frames << std::endl;

	// Test 1: Read and simulate the small list of logical addresses from the input file "small_refs.txt"
	std::cout <<"\n================================Test 1==================================================\n";
	std::fstream small_file("small_refs.txt"); // create a stream to read our file
	ref_count = num_faults = num_replacements = frame = 0;
	while (small_file >> logical_address)
	{
		ref_count++;
		page = (int) logical_address / page_size;
		PageEntry* p = table1.retrieve_entry(page);
		if (!p->valid)
		{
			is_fault = 1;
			num_faults++;
			if (table1.get_allocated_frames() == num_frames)
				// replace the frame with an algorithm here
				// allocated frames in this test will never exceed physical frames
				num_replacements++;
			else 
			{
				table1.allocate_frame(p);
				frame = p->frame_num;
			}
		}
		else
		{
			is_fault = 0;
			frame = p->frame_num;
		}
		std::cout << "Logical address: " << logical_address << ",\tpagenumber: " << page << ",\tframe number = " << frame <<
			"\tis page fault? " << is_fault << std::endl;
	}
	small_file.close();
	print_statistics(ref_count, num_faults, num_replacements);
	table1.reset_page_table();
	
	// Test 2: Read and simulate the large list of logical addresses from the input file "large_refs.txt"
	std::cout <<"\n================================Test 2==================================================\n";
	std::fstream large_file("large_refs.txt"); // open the large_refs.txt file
	std::vector<int> reference_string; // a vector of the reference string contained in the text file
	while (large_file >> logical_address)
		reference_string.push_back(logical_address);
	large_file.close();
	std::cout << "Number of references: " << reference_string.size() << std::endl;


	// BEGIN FIFO replacement algorithm
	// Uses queue STL container to simulate main memory
	std::cout << "****************Simulate FIFO replacement****************************" << std::endl;
	clock_t start;	// used for timing the algorithm
	start = clock(); // gets starting time
	std::queue<PageEntry*> FIFO_memory; // queue container to represent memory
	num_faults = num_replacements = 0; // reset number of faults and replacements

	// read the entire reference string
	for (unsigned int i = 0; i < reference_string.size(); i++)
	{
		logical_address = reference_string[i]; // get logical address from reference string
		page = (int)logical_address / page_size; // determine its page number
		PageEntry* p = table1.retrieve_entry(page); // retrieve page entry from the page table
		// if that page entry is invalid (not in main memory)
		// we must place it in a frame
		if (!p->valid)
		{
			num_faults++;
			// if the amount of frames allocated is equal to the max amount of physical frames
			if (table1.get_allocated_frames() == num_frames)
			{
				PageEntry* victim = FIFO_memory.front(); // choose a victim page based on FIFO
				FIFO_memory.pop(); // remove that page from main memory
				table1.replace_page(p, victim); // replace victim page with new page in main memory
				FIFO_memory.push(p); // place page in queue since it has now been referenced
				num_replacements++;
			}
			// else it is a normal frame allocation
			else
			{
				table1.allocate_frame(p); // allocate a frame to the reference
				FIFO_memory.push(p); // place page in queue since it has now been referenced
			}
		}
	}
	start = clock() - start;	// how much time has elapsed since clock started
	print_statistics(reference_string.size(), num_faults, num_replacements);
	//start / CLOCKS_PER_SEC gives total time in seconds
	std::cout << "Elapsed Time = " << std::fixed << (double)start / CLOCKS_PER_SEC << std::setprecision(6) << " sec" << std::endl;
	table1.reset_page_table();
	// END FIFO replacement algorithm


	std::vector<PageEntry*> main_memory; // vector to simulate main memory for these next two algorithms
	// BEGIN Random replacement algorithm
	// Uses a vector to simulate main memory
	std::cout << "****************Simulate Random replacement****************************" << std::endl;
	start = clock(); // gets starting time
	num_faults = num_replacements = 0; // reset number of faults and replacements

	// read the entire reference string
	for (unsigned int i = 0; i < reference_string.size(); i++)
	{
		logical_address = reference_string[i]; // get logical address from reference string
		page = (int)logical_address / page_size; // determine its page number
		PageEntry* p = table1.retrieve_entry(page); // retrieve page entry from the page table
		// if that page entry is invalid (not in main memory)
		// we must place it in a frame
		if (!p->valid)
		{
			num_faults++;
			// if the amount of frames allocated is equal to the max amount of physical frames
			if (table1.get_allocated_frames() == num_frames)
			{
				int index_removed = rand() % num_frames; // calculates a random victim index
				victim = main_memory[index_removed]; // retrieve that victim from main memory
				table1.replace_page(p, victim); // replace the victim page with new page in main memory
				main_memory[index_removed] = p; // update frame with new page
				num_replacements++;
			}
			// else it is a normal frame allocation
			else
			{
				table1.allocate_frame(p); // allocate a frame to the reference
				main_memory.push_back(p); // place page into vector representing main memory
			}
		}
	}
	start = clock() - start;	// how much time has elapsed since clock started
	print_statistics(reference_string.size(), num_faults, num_replacements);
	//start / CLOCKS_PER_SEC gives total time in seconds
	std::cout << "Elapsed Time = " << std::fixed << (double)start / CLOCKS_PER_SEC << std::setprecision(6) << " sec" << std::endl;
	table1.reset_page_table();
	main_memory.clear();
	// END Random replacement algorithm

	// BEGIN LRU replacement algorithm
	// Uses a vector to simulate main memory
	std::cout << "****************Simulate LRU replacement****************************" << std::endl;
	start = clock(); // gets starting time
	num_faults = num_replacements = 0; // reset number of faults and replacements
	double LRU, current;
	int to_remove;

	// read the entire reference string
	for (unsigned int i = 0; i < reference_string.size(); i++)
	{
		logical_address = reference_string[i]; // get logical address from reference string
		page = (int)logical_address / page_size; // determine its page number
		PageEntry* p = table1.retrieve_entry(page); // retrieve page entry from the page table
		// if that page entry is invalid (not in main memory)
		// we must place it in a frame
		if (!p->valid)
		{
			num_faults++;
			// if the amount of frames allocated is equal to the max amount of physical frames
			if (table1.get_allocated_frames() == num_frames)
			{
				LRU = table1.check_last_ref(main_memory[0]); // get time when first element was last referenced
				to_remove = 0; // used for determining which page to replace in main memory
				// go through each page currently in memory and determine which is LRU
				for (unsigned int i = 1; i < main_memory.size(); i++)
				{
					current = table1.check_last_ref(main_memory[i]);
					if (current < LRU)
					{
						to_remove = i;
						LRU = current;
					}
				}
				victim = main_memory[to_remove]; // retrieve the LRU page
				table1.replace_page(p, victim); // set valid bits appropriately
				main_memory[to_remove] = p; // replace victim with current page
				num_replacements++;
			}
			// else it is a normal frame allocation
			else
			{
				table1.allocate_frame(p); // allocate a frame to the reference
				main_memory.push_back(p); // place page into vector representing main memory
			}
		}
	}
	start = clock() - start;	// how much time has elapsed since clock started
	print_statistics(reference_string.size(), num_faults, num_replacements);
	//start / CLOCKS_PER_SEC gives total time in seconds
	std::cout << "Elapsed Time = " << std::fixed << (double)start / CLOCKS_PER_SEC << std::setprecision(6) << " sec" << std::endl;
	// END LRU replacement algorithm

}
