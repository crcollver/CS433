// =================================
// CS 433 Programming Assignment 5
// Author: Cameron Collver and Nick Dujakovich
// Date: 12/06/2019
// Course: CS433 (Operating Systems)
// Description : Program to simulate different page replacement algorithms
// =================================

#include "pagetable.h"

// PURPOSE: construct page table with number of pages in logical memory
// PARAM: number of pages in logical memory
PageTable::PageTable(int size)
{
	this->table_size = size;
	this->allocated_count = 0;
	// initialize a page table using calculated amount of pages
	for (int i = 0; i < size; i++)
	{
		PageEntry *p = new PageEntry;
		table.push_back(p); // add empty page entry to table
	}
}

PageTable::~PageTable() {}

// PURPOSE: resets page table for next algorithm to use,
// that way we're using the same vector without taking up unneeded space
void PageTable::reset_page_table()
{
	for (int i = 0; i < table_size; i++)
	{
		table[i]->dirty = false;
		table[i]->frame_num = 0;
		table[i]->valid = false;
	}
	allocated_count = 0;
}

// PURPOSE: allocate the frame number to the page and set valid bit
// PARAM: pointer to the page entry in the table
void PageTable::allocate_frame(PageEntry *p)
{
	p->frame_num = allocated_count; // allocate space in physical memory
	allocated_count++;
	p->valid = true;
}

// PURPOSE: called in main during page fault to replace current page with vicitim
// PARAM: *P page to allocate, *victim page to remove
void PageTable::replace_page(PageEntry* p, PageEntry* victim)
{
	victim->valid = false; // set the valid bit of the victim page
	p->frame_num = victim->frame_num; // place the page where the victim was in physical memory
	p->valid = true; // set its valid bit to valid
}

// PURPOSE: called during LRU algorithm to retrieve when page was last referenced
// PARAM: *p page to check
double PageTable::check_last_ref(PageEntry* p)
{
	return (double) p->last_referenced;
}

// PURPOSE: update when page was last_referenced, and return page at an address
// PARAM: address of page to be retrieved, calculated in main
PageEntry* PageTable::retrieve_entry(int page)
{
	table[page - 1]->last_referenced = (double) clock() / CLOCKS_PER_SEC; // current page was now accessed at current time
	return table[page - 1];
}

// PURPOSE: get amount of frames allocated in physical memory
int PageTable::get_allocated_frames()
{
	return allocated_count;
}