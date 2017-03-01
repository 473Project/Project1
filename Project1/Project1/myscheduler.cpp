//myschedule.cpp
/*Define all the functions in 'myschedule.h' here.*/
#include "myscheduler.h"

void MyScheduler::CreateThread(int arriving_time, int remaining_time, int priority, int tid) //Thread ID not Process ID
{
	//Function to Create Thread(s) and insert them in the student
	//defined data structure

	ThreadDescriptorBlock tempThread;
	tempThread.arriving_time = arriving_time;
	tempThread.remaining_time = remaining_time;
	tempThread.priority = priority;
	tempThread.tid = tid;
	ThreadBuf.push_back(tempThread);
}

bool MyScheduler::Dispatch()
{
	//Todo: Check and remove finished threads
	//Todo: Check if all the threads are finished; if so, return false
	switch(policy)
	{
		case FCFS:		//First Come First Serve, Tyler
			
			break;
		case STRFwoP:	//Shortest Time Remaining First, without preemption, Tyler

			break;
		case STRFwP:	//Shortest Time Remaining First, with preemption, Kyle

			break;
		case PBS:		//Priority Based Scheduling, with preemption, Connor

			break;
		default :
			cout << "Invalid policy!";
			throw 0;
	}
	return true;
}

////////////////////////////////////////////////////////
//Name: sortByArrival
//
//
//
//
void sortByArrival(vector<ThreadDescriptorBlock> block){


}

void sortByPriority(vector<ThreadDescriptorBlock> block){


}

void sortByTimeRemaining(vector<ThreadDescriptorBlock> block){


}