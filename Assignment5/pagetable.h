// =================================
// CS 433 Programming Assignment 5
// Author: Cameron Collver and Nick Dujakovich
// Date: 12/06/2019
// Course: CS433 (Operating Systems)
// Description : Program to simulate different page replacement algorithms
// =================================

#pragma once

#include <vector>
#include <ctime>

// Remember to add comments to your code

// A page table entry
class PageEntry
{
public:
	// Physical frame number for a given page
	int frame_num = 0;
	// valid bit represents whether a page is in the physical memory
	bool valid = false;
	// dirty bit represents whether a page is changed
	bool dirty = false;
	// the time that entry was last referenced
	double last_referenced;
};


/**
 * \brief A page table is like an array of page entries. The size of the page table should equal to the number of pages in logical memory
 */
class PageTable
{
private:
	std::vector<PageEntry*> table; // vector to hold all page entries
	int table_size; // size of the table used for constructing all initial entries
	int allocated_count; // keep track of how many frames are allocated
public:
	// PURPOSE: allocate the frame number to the page and set valid bit
	// PARAM: pointer to the page entry in the table
	void allocate_frame(PageEntry *p);

	// PURPOSE: resets page table for next algorithm to use,
	// that way we're using the same vector without taking up unneeded space
	void reset_page_table();

	// PURPOSE: called in main during page fault to replace current page with vicitim
	// PARAM: *P page to allocate, *victim page to remove
	void replace_page(PageEntry* p, PageEntry* victim);

	// PURPOSE: called during LRU algorithm to retrieve when page was last referenced
	// PARAM: *p page to check
	double check_last_ref(PageEntry* p);

	// PURPOSE: get amount of frames allocated in physical memory
	int get_allocated_frames();

	// PURPOSE: update when page was last_referenced, and return page at an address
	// PARAM: address of page to be retrieved, calculated in main
	PageEntry* retrieve_entry(int page);

	// PURPOSE: construct page table with number of pages in logical memory
	// PARAM: number of pages in logical memory
	PageTable(int size);
	~PageTable();
};
