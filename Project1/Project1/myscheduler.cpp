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

	//add the thread to the ready buf if it arrived at 0 (instantly valid)
	if (tempThread.arriving_time == 0)
		readyBuf.push_back(tempThread);
	else
		//otherwise add it to the mainBuffer until it is valid
		mainThreadBuf.push_back(tempThread);
		//sort mainBuf by arrival time each time you add a new thread so they are in order by arrival time
		sort(mainThreadBuf.begin(), mainThreadBuf.end(), sortByArrival());

}

bool MyScheduler::Dispatch()
{
	//Todo: Check and remove finished threads
	//Todo: Check if all the threads are finished; if so, return false

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

		//loop through the cpus
		for (int i = 0; i < num_cpu; i++)
		{
			//Do nothing if readyBuf is empty as there are no more valid threads to schedule
			//If readyBuf is not empty attempt to assign a thread to an open CPU
			if (!readyBuf.empty()){ 
				if (CPUs[i] == NULL){
					//if current cpu is not working on a thread and there is thread(s) in ready buffer assign it to the cpu
					CPUs[i] = &(readyBuf.front());
					//remove that thread from the ready buff
					readyBuf.erase(readyBuf.begin());
				}

			}

		}

		break;
	case STRFwoP:	//Shortest Time Remaining First, without preemption, Tyler

		//Readybuf needs sortred by shortest time remaining
		sort(readyBuf.begin(), readyBuf.end(), sortByTimeRemaining());

		//Similarly to FCFS loop through the CPUs
		for (int i = 0; i < num_cpu; i++){

			//do nothing if readyBuf is empty 
			//if not empty check if thread can be assigned 
			if (!readyBuf.empty()){
				//CPU is free
				if (CPUs[i] == NULL){
					//assign that cpu to the first thread in the readyBUf (shortest time remaining)
					CPUs[i] = &(readyBuf.front());
					//remove that thread from the readyBuf
					readyBuf.erase(readyBuf.begin());
				}
			}
		}

		break;
	case STRFwP:	//Shortest Time Remaining First, with preemption, Kyle

		sort(readyBuf.begin(), readyBuf.end(), sortByTimeRemaining());

		for (int i = 0; i < num_cpu; i++){
			//do nothing if readyBuf is empty 
			//if not empty check if thread can be assigned 
			if (!readyBuf.empty()){
				//CPU is free
				if (CPUs[i] == NULL){
					//assign that cpu to the first thread in the readyBUf (shortest time remaining)
					CPUs[i] = &(readyBuf.front());
					//remove that thread from the readyBuf
					readyBuf.erase(readyBuf.begin());
				}
				else { //check if remaining time of the front of the ready queue is less than the current item in the cpu
					if (CPUs[i]->remaining_time > readyBuf.front().remaining_time){
						readyBuf.push_back(*CPUs[i]);
						CPUs[i] = &(readyBuf.front());
						readyBuf.erase(readyBuf.begin());
						sort(readyBuf.begin(), readyBuf.end(), sortByTimeRemaining());
					}
				}
			}
		}



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
