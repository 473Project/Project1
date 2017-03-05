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

	//add the thread to the ready buf if it has arrived
	if (tempThread.arriving_time == timer)
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

		//Readybuf needs to be sorted by shortest time remaining
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

		// sort readyBuf by priority
		sort(readyBuf.begin(), readyBuf.end(), sortByPriority());

		//flag to continue preemption
		bool preempt = true;

		// while there still may be threads to preempt
		while (!readyBuf.empty() && preempt) {

			// find lowest priority thread in CPUs
			// note: lowest priority is the priority with the highest value
			// note: if same priority, thread with later arrival time (higher value) has lower priority
			int lowestPriority = CPUs[0]->priority;
			int arrivalTime = CPUs[0]->arriving_time;
			int lp_index = 0;
			bool hasFreeCPU = false;	// check if there is a free CPU

			// check if first CPU block is empty
			if (CPUs[lp_index] == NULL) {
				hasFreeCPU = true;
			}

			// find lowest priority
			for (int i = 1; (i < num_cpu) && !hasFreeCPU; i++) {
				if (CPUs[i] == NULL) {		// if CPU block is empty, the empty block is the lowest priority
					hasFreeCPU = true;
					lp_index = i;
				}
				else if (CPUs[i]->priority > lowestPriority) {	// if CPU block has a thread, check if lower priority
					lowestPriority = CPUs[i]->priority;
					arrivalTime = CPUs[i]->arriving_time;
					lp_index = i;
				}
				else if (CPUs[i]->priority == lowestPriority) {	// else check if same priority 
					if (CPUs[i]->arriving_time > arrivalTime) {	// and later arrival time has lower priority
						lowestPriority = CPUs[i]->priority;
						arrivalTime = CPUs[i]->arriving_time;
						lp_index = i;
					}
				}
				else {
					// lowest priority thread is unchanged
				}
			}

			// check if there is a free CPU or if the highest priority thread in readyBuf has a higher priority than
			// the lowest priority thread in CPUs
			if (hasFreeCPU || (readyBuf.front().priority < lowestPriority)) {

				if (CPUs[lp_index] != NULL) {	// swap threads if CPU block not empty
					
					// get thread from CPU
					ThreadDescriptorBlock *temp = CPUs[lp_index];
					// preempt thread on CPU
					CPUs[lp_index] = &(readyBuf.front());
					// remove thread from readyBuf
					readyBuf.erase(readyBuf.begin());
					// insert thread from CPU
					readyBuf.push_back(*temp);
					
					// sort readyBuf again by priority because new thread inserted
					sort(readyBuf.begin(), readyBuf.end(), sortByPriority());
				}
				else {

					// insert thread into free CPU
					CPUs[lp_index] = &(readyBuf.front());
				}
				
			}
			else {
				// highest priority threads have been scheduled
				// no more preemption
				preempt = false;
			}

		}

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
