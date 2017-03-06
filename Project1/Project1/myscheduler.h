#pragma once
/*
CMPSC 473 Project 1
GROUP MEMBERS:
Tyler Abbatico
Kyle Eckenstine
Connor Dougherty
*/


//myschedule.h
/* Students need to define their own data structure to contain
and access objects from 'thread class'. The 'CreateThread()'
function should insert the threads into the student defined
data structure after creating them.
They are allowed to add any additional functionality (only
declaration in this file, define in 'myschedule.cpp')
which they might find helpful.*/

#include "scheduler.h"
#include <vector>
#include <iostream>
#include <algorithm>

//Define your data structure here.

//Typedef to make things easier to read
typedef vector<ThreadDescriptorBlock> threadBuffer;

//comparators for sorting the vector
struct sortByArrival {
	inline bool operator() (const ThreadDescriptorBlock& thread1, const ThreadDescriptorBlock& thread2){
		return (thread1.arriving_time < thread2.arriving_time);
	}

};


struct sortByTimeRemaining {
	inline bool operator() (const ThreadDescriptorBlock& thread1, const ThreadDescriptorBlock& thread2){
		return (thread1.remaining_time < thread2.remaining_time);
	}

};

struct sortByPriority {
	inline bool operator() (const ThreadDescriptorBlock& thread1, const ThreadDescriptorBlock& thread2){
		return (thread1.priority < thread2.priority);
	}

};
class MyScheduler : public Scheduler {
public:
	MyScheduler(Policy p, unsigned int n) : Scheduler(p, n) {}
	bool Dispatch() override; //Function to implement scheduling policy and to keep a check on processed threads
	void CreateThread(int arriving_time, int remaining_time, int priority, int tid) override; //Function to create threads and insert them in student defined data structure

	//Declare additional methods(s) below if needed.
	/*void sortByArrival(vector<ThreadDescriptorBlock> block);
	void sortByPriority(vector<ThreadDescriptorBlock> block);
	void sortByTimeRemaining(vector<ThreadDescriptorBlock> block);*/


private:
	//The two queues we will use. 
	threadBuffer mainThreadBuf;
	threadBuffer readyBuf;

};
