// myschedule.cpp
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

	if (tempThread.arriving_time == 0)
		readyBuf.push_back(tempThread);
	else
		mainThreadBuf.push_back(tempThread);

}

bool MyScheduler::Dispatch()
{
	//Todo: Check and remove finished threads
	//Todo: Check if all the threads are finished; if so, return false

	//sort mainBuf by arrival time
	sort(mainThreadBuf.begin(), mainThreadBuf.end(), sortByArrival());

	//first check if new threads are ready
	//if a thread is ready add it to the ready queue and remove it from the main buffer
	while (!mainThreadBuf.empty() && mainThreadBuf.front().arriving_time <= timer){
		readyBuf.push_back(mainThreadBuf.front());
		mainThreadBuf.erase(mainThreadBuf.begin());
	}

	//Check if there are still unfinished threads
	//If all threads are finished return false
	//otherwise return true
	bool finished = true;

	//first check the working cpus to see if they have work
	for (int i = 0; i < num_cpu; i++)
	{
		if (CPUs[i] != NULL)
			finished = false; //atleast 1 cpu still has work so we are not finished
	}

	//Then check the ready buf
	if (!readyBuf.empty())
		finished = false; //if the ready buffer is not empty we are not finished

	//Lastly check if there are any threads in the main buffer
	if (!mainThreadBuf.empty())
		finished = false; //if the main buffer is not empty we still have threads to work on and are not finished

	if (finished)//if we are finished return false so the scheduler knows we are done dispatching threads.
		return false;

	switch (policy)
	{
	case FCFS:		//First Come First Serve, Tyler
		//Threads should be taken off ready queue in order since they should already be in ordre by arrival time


		break;
	case STRFwoP:	//Shortest Time Remaining First, without preemption, Tyler

		break;
	case STRFwP:	//Shortest Time Remaining First, with preemption, Kyle

		break;
	case PBS:		//Priority Based Scheduling, with preemption, Connor

		break;
	default:
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
/*void sortByArrival(vector<ThreadDescriptorBlock> block){


}

void sortByPriority(vector<ThreadDescriptorBlock> block){


}

void sortByTimeRemaining(vector<ThreadDescriptorBlock> block){


}*/
