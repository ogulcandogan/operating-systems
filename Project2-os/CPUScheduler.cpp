#include "CPUScheduler.h" // Include the header file
#include <sstream>
#include <string>
#include <memory>
#include <string.h>
#include <iostream>


using namespace std;



/***************************************************************
 * CPU Data structure
 * 
 * *************************************************************/

CPUScheduler::CPUScheduler()
{
	clock = 0;

	mode = NEWCPU;

	prev_process = -1;
	wait = 0;
	cpu_is_executing = 0;

	detailed = UNSET;
	verbose = UNSET;
	round_robin = UNSET;   
    first_come_first_serve = UNSET;           
	shortest_job_first = UNSET;    
	shortest_remaining = UNSET; 

    // RR with time quantum 10 by default
	time_quantum = 10;

	total_cpu_execution_time = 0;
	process_switch = -1;
	process_switch = -1;

	num_of_processes = -1;
}

void CPUScheduler::addFinishedIOProcessToReadyQueue()
{
	std::shared_ptr<Process> arriving_process = nullptr;

	do
	{
		// Removes a process from IO queue which has no IO time left 
		arriving_process = io_queue.removeIOProcessAtTime(IO_COMPLETED);

		
		if (arriving_process != nullptr)
		{
			if (verbose == SET)
			{
			 // Print  verbose description
			cout << "At time " << clock << ": " << " Process " << arriving_process->getProcessNumber() << " moves from BLOCKED to READY" << std::endl;
			}

			// Take process has been removed from IO queue and add to Ready queue
			addProcess(arriving_process, READY);
		}
		// If empty process, exiting the loop
	} while (arriving_process != nullptr);
}

void CPUScheduler::addArrivingIOProcessToReadyQueue()
{
	std::shared_ptr<Process> arriving_process = nullptr;

	do
	{
		
		arriving_process = job_queue.removeProcessAtTime(clock);

		if (arriving_process != nullptr)
		{
			// Print verbose
			if (verbose == SET)
			{
				cout << "At time " << clock << ": " <<  " Process " << arriving_process->getProcessNumber() << " moves from NEW to READY" << std::endl;
			}

			// Add arriving process to ready queue 
			addProcess(arriving_process, READY);
		}
	} while (arriving_process != nullptr);
}

// Function to add the process
void CPUScheduler::addProcess(std::shared_ptr<Process> process, Destination dest)
{
	
	if (dest == READY)
	{
		ready_queue.addProcess(process);
	}

	if (dest == IO)
	{
		io_queue.addProcess(process);
	}

	if (dest == JOB)
	{
		job_queue.addProcess(process);
	}
}

bool CPUScheduler::canContinue(CPUQueue & exit_queue)
{
	if (num_of_processes != exit_queue.size())
	{
		return true;
	}
	else
	{
		return false;
	}
}


/***************************************************************
 * First Come First Serve Schedule
 * 
 * *************************************************************/
int CPUScheduler::FCFS(CPUQueue & q)
{
	
	if (cpu_is_executing == 0)
	{
		
		// Set timings sets the length of the CPU and IO bursts to be executed right now
		current_process->setTimings();
		// et the CPU wait to the length of the cpu burst
		wait = current_process->getCPUTime(); 

		// we set the start time of the process
		if (current_process->getStartTime() == -1)
		{
             // setting start time to current time
			current_process->setStartTime(clock);
		}

		// Get the remaining of the IO time
		if (current_process->getIOTimeRemaining() >= 0)
		{
			
			current_process->setIOProcessTotal(current_process->getIOTimeRemaining() + current_process->getIOProcessTotal());
		}

		// Print verbose
		if (verbose == SET)
		{
			cout << "At time " << clock << ": " << " Process " << current_process->getProcessNumber() << " moves from READY to RUNNING" << std::endl;
		}

		
		cpu_is_executing = 1;
	}
	// CPU is in the middle of a burst
	else if (cpu_is_executing == 1)
	{
		// decrement the wait
		wait--;
		// increase the total amount of cpu execution time
		total_cpu_execution_time++;
		// increse the total cpu time of the process
		current_process->cpuProcessTotalIncrease(1);

		// Wait is done, eg we can move this thread out and start on a new one
		if (wait == 1)
		{
			// Check the last CPU burst 
		
			if (current_process->getIOTimeRemaining() == -1)
			{
		
				current_process->setExitTime(clock);

			
				q.addProcess(current_process);

				// Print Verbose
				if (verbose == SET)
				{
					cout << "At time " << clock << ": " << " Process " << current_process->getProcessNumber() << " moves from RUNNING to EXIT" << std::endl;
				}
			}
			else
			{
				
				if (verbose == SET)
				{
					cout << "At time " << clock << ": " << " Process " << current_process->getProcessNumber() << " moves from RUNNING to BLOCKED" << std::endl;
				}

				
				addProcess(current_process, IO);
			}

		
			setMode(DISPATCHING);
		
            // CPU is stopping the execution
			cpu_is_executing = 0;
		}
	}
	return 1;
}


/***************************************************************
 * Round Robin Schedule
 * 
 * *************************************************************/
int	CPUScheduler::RR(CPUQueue & q)
{
	
	if (cpu_is_executing == 0)
	{
		
		current_process->setTimings();
		//Set the CPU wait to the length of the time quantum
		wait = time_quantum;

		
		if (current_process->getStartTime() == -1)
		{
			current_process->setStartTime(clock); 
		}

	
		if (current_process->getIOTimeRemaining() >= 0)
		{
			
			current_process->setIOProcessTotal(current_process->getIOTimeRemaining() + current_process->getIOProcessTotal());
		}

	
		if (verbose == SET)
		{
			cout << "At time " << clock << ": " << " Process " << current_process->getProcessNumber() << " moves from READY to RUNNING" << std::endl;
		}

		cpu_is_executing = 1;
	}

	else if (cpu_is_executing == 1)
	{
	
		wait--;
	
		current_process->cpuTimeIncrease(-1);
		
		total_cpu_execution_time++;
		
		current_process->cpuProcessTotalIncrease(1);

		//if wait (time slice) is done or burst is done,
	
		if (wait == 1 || current_process->getCPUTime() == 1)
		{
		
			if (current_process->getIOTimeRemaining() == -1)
			{
			
				current_process->setExitTime(clock);

			
				q.addProcess(current_process);

				// Print
				if (verbose == SET)
				{
					cout << "At time " << clock << ": " << " Process " << current_process->getProcessNumber() << " moves from RUNNING to EXIT" << std::endl;
				}
			}
			
			else if (wait == 1 && current_process->getCPUTime() != 1)
			{
				current_process->addBurst(current_process->getCPUTime(), current_process->getIOTimeRemaining());
				
				if (verbose == SET)
				{
					cout << "At time " << clock << ": " << " Process " << current_process->getProcessNumber() << " moves from RUNNING to READY" << std::endl;
				}

				
				addProcess(current_process, READY);
			}
			else
			{
			
				if (verbose == SET)
				{
					cout << "At Time" << clock << ": " << " Process " << current_process->getProcessNumber() << " moves from RUNNING to BLOCKED" << std::endl;
				}


			
				addProcess(current_process, IO);
			}

			
			setMode(DISPATCHING);
		
			cpu_is_executing = 0;
		}
	}
	return 1;
}



/***************************************************************
 * Shortest Job First (SJF)(Non-preemptive)
 * 
 * *************************************************************/

int CPUScheduler::SJF(CPUQueue & q)
{
	
	if (cpu_is_executing == 0)
	{
		
		// Set timings sets the length of the CPU and IO bursts to be executed right now
		current_process->setTimings();
		// et the CPU wait to the length of the cpu burst
		wait = current_process->getCPUTime(); 

		// we set the start time of the process
		if (current_process->getStartTime() == -1)
		{
             // setting start time to current time
			current_process->setStartTime(clock);
		}

		// Get the remaining of the IO time
		if (current_process->getIOTimeRemaining() >= 0)
		{
			
			current_process->setIOProcessTotal(current_process->getIOTimeRemaining() + current_process->getIOProcessTotal());
		}

		// Print verbose
		if (verbose == SET)
		{
			cout << "At time " << clock << ": " << " Process " << current_process->getProcessNumber() << " moves from READY to RUNNING" << std::endl;
		}

		
		cpu_is_executing = 1;
	}
	// CPU is in the middle of a burst
	else if (cpu_is_executing == 1)
	{
		// decrement the wait
		wait--;
		// increase the total amount of cpu execution time
		total_cpu_execution_time++;
		// increse the total cpu time of the process
		current_process->cpuProcessTotalIncrease(1);

		// Wait is done, eg we can move this thread out and start on a new one
		if (wait == 1)
		{
			// Check the last CPU burst 
		
			if (current_process->getIOTimeRemaining() == -1)
			{
		
				current_process->setExitTime(clock);

			
				q.addProcess(current_process);

				// Print Verbose
				if (verbose == SET)
				{
					cout << "At time " << clock << ": " << " Process " << current_process->getProcessNumber() << " moves from RUNNING to EXIT" << std::endl;
				}
			}
			else
			{
				
				if (verbose == SET)
				{
					cout << "At time " << clock << ": " << " Process " << current_process->getProcessNumber() << " moves from RUNNING to BLOCKED" << std::endl;
				}

				
				addProcess(current_process, IO);
			}

		
			setMode(DISPATCHING);
		
            // CPU is stopping the execution
			cpu_is_executing = 0;
		}
	}
	return 1;
}


/***************************************************************
 * Shortest Remaining Time Next (SRTN)(Preemptive)
 * 
 * *************************************************************/

int CPUScheduler::SRTN(CPUQueue & q)
{
	
	if (cpu_is_executing == 0)
	{
		
		// Set timings sets the length of the CPU and IO bursts to be executed right now
		current_process->setTimings();
		// et the CPU wait to the length of the cpu burst
		wait = current_process->getCPUTime(); 

		// we set the start time of the process
		if (current_process->getStartTime() == -1)
		{
             // setting start time to current time
			current_process->setStartTime(clock);
		}

		// Get the remaining of the IO time
		if (current_process->getIOTimeRemaining() >= 0)
		{
			
			current_process->setIOProcessTotal(current_process->getIOTimeRemaining() + current_process->getIOProcessTotal());
		}

		// Print verbose
		if (verbose == SET)
		{
			cout << "At time " << clock << ": " << " Process " << current_process->getProcessNumber() << " moves from READY to RUNNING" << std::endl;
		}

		
		cpu_is_executing = 1;
	}
	// CPU is in the middle of a burst
	else if (cpu_is_executing == 1)
	{
		// decrement the wait
		wait--;
		// increase the total amount of cpu execution time
		total_cpu_execution_time++;
		// increse the total cpu time of the process
		current_process->cpuProcessTotalIncrease(1);

		// Wait is done, eg we can move this thread out and start on a new one
		if (wait == 1)
		{
			// Check the last CPU burst 
		
			if (current_process->getIOTimeRemaining() == -1)
			{
		
				current_process->setExitTime(clock);

			
				q.addProcess(current_process);

				// Print Verbose
				if (verbose == SET)
				{
					cout << "At time " << clock << ": " << " Process " << current_process->getProcessNumber() << " moves from RUNNING to EXIT" << std::endl;
				}
			}
			else
			{
				
				if (verbose == SET)
				{
					cout << "At time " << clock << ": " << " Process " << current_process->getProcessNumber() << " moves from RUNNING to BLOCKED" << std::endl;
				}

				
				addProcess(current_process, IO);
			}

		
			setMode(DISPATCHING);
		
            // CPU is stopping the execution
			cpu_is_executing = 0;
		}
	}
	return 1;
}


int CPUScheduler::getNumberOfProcesses()
{
	return num_of_processes;
}



int CPUScheduler::getNextProcess()
{
	shared_ptr<Process>  next_process = nullptr;

	// grab next process from ready queue
	next_process = ready_queue.removeProcess();

	if (next_process != nullptr)
	{
		// sets the current process of the cpu to the process pulled from the ready queue
		// this thread will be used once the cpu goes into EXECUTING mode
		current_process = next_process;

		
		if (prev_process == current_process->getProcessNumber())
		{
			
			setMode(TSWITCH);
		}
		else 
		{
			// set the new previous process
			prev_process = current_process->getProcessNumber();
			/*change cpu to process switch mode*/
			setMode(PSWITCH);
		}
	}

	return 1;
}

void CPUScheduler::advanceClock()
{
	clock++;

	io_queue.decrementAllIO();
}

void CPUScheduler::calculateStatistics(CPUQueue & q)
{
	clock--; /*one extra clock tick upon exit, so removing it here*/

			 /*if detailed is set, get detailed statistics*/
	if (detailed == SET)
	{
		stats_detailed(*this, q);
	}
	else
	{
		stats_default(*this, q);
	}

}

void CPUScheduler::checkStatus()
{
	/*decrement the wait*/
	wait--;

	/*if wait is over...*/
	if (wait == 0)
	{
		/*move cpu into executing mode*/
		setMode(EXECUTING);
		wait = 0;
	}
}


void CPUScheduler::executeProcess(CPUQueue & q)
{
    
	if (round_robin == SET)
	{
		RR(q);
	
	}
    else if(first_come_first_serve == SET)
    {
        FCFS(q);
    }
    else if(shortest_job_first == SET)
    {
        SJF(q);
    }
    else if(shortest_remaining == SET)
    {
        SRTN(q);
    }
	else
	{
        
		FCFS(q);
	
	}
}

void CPUScheduler::setMode(Mode mode)
{
	if (mode == TSWITCH)
	{
		
		wait = process_switch;
	}
	else if (mode == PSWITCH)
	{
		
		wait = process_switch;
	}

	
	this->mode = mode;
}


void stats_default(CPUScheduler & cpu, CPUQueue q)
{
	float cpu_util = 0;
	int total_time = cpu.clock;

	// calculate cpu utilization
	cpu_util = ((float)cpu.total_cpu_execution_time / cpu.clock) * 100;

	// pass to print function 
	printDefaultStats(cpu, q, cpu_util, total_time);
	
}


// prints the final stats of the CPUScheduler in detailed mode, threads presented in exit order
void stats_detailed(CPUScheduler & cpu, CPUQueue q)
{
	/*default stats are part of the detailed stats*/
	stats_default(cpu, q);

	

	for (auto p : q.q)
	{
		// print detailed info
		cout << "\n";
		cout << "Process:" << p->getProcessNumber() << endl;
		cout << "arrival time: " <<  p->getArrivalTime() << endl;
		cout << "service time: " <<  p->getCPUProcessTotal() << endl;
		cout << "I/O time: " <<  p->getIOProcessTotal() << endl;
		cout << "turnaround time: "<< p->getExitTime() - p->getArrivalTime()<< endl;
		cout << "finish time: " << p->getExitTime() << endl;
		cout << "\n";
	}
}

// Print function
void printDefaultStats(CPUScheduler & cpu, CPUQueue q, float cpu_util, int time)
{
	
	/*chooses which mode to print out based on what mode the cpu executed in*/
	if (cpu.time_quantum != 10)
	{
		cout << "\nRound Robin with time quantum: " <<  cpu.time_quantum << endl;

		cout << "\nRound Robin\n\n";
	    	// Default Mode: no flags are given 
    	cout << "Total Time required is " << time << " time units\n"; 
		cout << "Average Turnaround Time is " << turnaroundTime(cpu, q) << " time units\n" ;
		cout << "CPU Utilization is  " << cpu_util << "%\n\n";
		
	}
	
	else
	{
		cout << "\nFirst Come First Serve:\n\n";
		
    	cout << "Total Time required is " << time << " time units\n"; 
		cout << "Average Turnaround Time is " << turnaroundTime(cpu, q) << " time units\n" ;
		cout << "CPU Utilization is  " << cpu_util << "%\n\n";


	}

}



float turnaroundTime(CPUScheduler & cpu, CPUQueue q)
{
	float arr_temp = 0;
	float exit_temp = 0;
	float turnaround = 0;
	shared_ptr<Process> temp = nullptr;

	for (int i = 1; i <= cpu.num_of_processes; i++)
	{
		arr_temp = exit_temp = 0;

		for (auto p : q.q)
		{
			if (p->getProcessNumber() == i)
			{
				arr_temp = p->getArrivalTime();
				exit_temp = p->getExitTime();
			}
		}

		turnaround += (exit_temp - arr_temp);
	}

	return turnaround / (cpu.num_of_processes);
}

int initializeJobQueue(CPUScheduler & cpu)
{

   // Rad the first line of the file
	readCPUInfo(cpu); 

    // Read all processes in the file
	parseProcesses(cpu); 

	// Set the number of processes the cpu has, to the size of the job queue
	cpu.num_of_processes = cpu.job_queue.size();


	return 1;
}

// Read the first line of the file
int readCPUInfo(CPUScheduler & cpu)
{
	cin >> cpu.num_of_processes >> cpu.process_switch;

	return 1;
}



// reading all processes
int parseProcesses(CPUScheduler & cpu)
{
	/*for the number of processes in the file...*/
	for (int i = 0; i < cpu.num_of_processes; i++)
	{
		/*parse a process*/
		int process_num = 0;
		int arrival_time = -1;
		int num_of_bursts = 0;
		shared_ptr<Process> new_process = NULL;

		// process, its arrival time, and number of cpu burts*/
		cin.ignore(200, '\n');
		cin >> process_num >> arrival_time >> num_of_bursts;

		
		new_process = make_shared<Process>(process_num, arrival_time, num_of_bursts);

	
		parseBursts(new_process);

		
		cpu.addProcess(new_process, JOB);

	
	}

	return 1;
}


int parseBursts(shared_ptr<Process> process)
{
	int burst_num = 0;
	int cpu_time = 0;
	int io_time = 0;

	if (!process)
	{
		return -1;
	}

	// for x-1 number of bursts in the stack...
	for (int i = 0; i < (process->getNumberOfBursts() - 1); i++)
	{
		// Read the file
		std::cin.ignore(200, '\n');
		std::cin >> burst_num >> cpu_time >> io_time;
		
		process->addBurst(cpu_time, io_time);
	}

	// Read the last line of the process
	cin.ignore(200, '\n');
	cin >> burst_num >> cpu_time;
	
	process->addBurst(cpu_time, -1);

	return 1;
}

// Setting flags 
void commandLineFlags(CPUScheduler & cpu, char ** argv, int argc)
{
	// Make sure user doesn't enter too many arguments
	if (argc > 7)
	{
		cout << "Invalid command line parameters. Exiting.\n";
		exit(0);
	}
   
    // Details flag
	for (int i = 0; i < argc; i++)
	{
	    
		if (strcmp(argv[i], "-d") == 0)
		{
			
			cpu.detailed = SET;
			break;
		}
	}
	// Verbose flag

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-v") == 0)
		{
			cpu.verbose = SET;
			break;
		}
	}

 
  
    // Round Robin 
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-a RR") == 0)
		{
			cpu.round_robin = SET;
			break;
		}
	}

    // First Come First Serve 
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-a FCFS") == 0)
		{
			cpu.first_come_first_serve = SET;
			break;
		}
	}
   
     // Shortest Job First  
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-a SJF") == 0)
		{
			cpu.shortest_job_first = SET;
			break;
		}
	}
   
       // Shortest Remaining Time Next (SRTN)(Preemptive) 
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-a SRTN") == 0)
		{
			cpu.shortest_remaining = SET;
			break;
		}
	}
   
	for (int i = 0; i < argc; i++)
	{
		// Make sure to accept the time quantum input

		if (isdigit(argv[i][0]))
		{
			
			int num = atoi(argv[i]);
			cpu.time_quantum = num;
		
			break;
		}
	}
}
