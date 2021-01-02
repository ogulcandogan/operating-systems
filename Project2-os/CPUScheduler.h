#pragma once
#include "CPUQueue.h"
#include <memory>

#define NO_QUANTUM_VALUE -1
#define IO_COMPLETED 0
#define EXIT -99

// Flag type used to represent possible user flag inputs (-d, -v -a) 
typedef enum Flag {
	UNSET = 0,  
	SET = 1
} Flag;

 // objects can be placed in the IO, JOB, or READY queues
typedef enum Destination {
	READY = 0,         
	IO = 1,
	JOB = 2
}Destination;

// CPU can be in one of 5 modes
typedef enum Mode {
	EXECUTING = 0,      
	DISPATCHING = 1,
	NEWCPU = 2,
	PSWITCH = 3,
	TSWITCH = 4
}Mode;


class CPUScheduler
{
public:
	CPUScheduler();

	void addFinishedIOProcessToReadyQueue();

	void addArrivingIOProcessToReadyQueue();

	void addProcess(std::shared_ptr<Process> process, Destination dest);

	bool canContinue(CPUQueue & exit_queue);

	int FCFS(CPUQueue & q);

	int	RR(CPUQueue & q);

    int SJF(CPUQueue & q);

	int	SRTN(CPUQueue & q);



	int getNumberOfProcesses();

	int getNextProcess();

	void advanceClock();

	void calculateStatistics(CPUQueue & q);

	void checkStatus();

	void executeProcess(CPUQueue & q);

	void setMode(Mode mode);

	
	
public:
	Flag verbose;               // Flag -v included in command flags
	Flag detailed;              // Flag if -d included in command flags
	Flag round_robin;           // Flag if -a RR included in command flags
	Flag first_come_first_serve;           // Flag if -a RR included in command flags
	Flag shortest_job_first;    //  Flag if -a SJF included in command flags
	Flag shortest_remaining;    //  Flag if -a SRTN included in command flags

	int clock;                  // the main clock for the CPU
	int cpu_is_executing;       // set to 1 when the CPU is in the middle of a burst, 0 otherwise*/

	int num_of_processes;       // number of processes being worked on by CPU 
	int prev_process;           
	int process_switch;         // time it takes to switch process 

	int time_quantum;           // time quantum for use in RR if included 
	int total_cpu_execution_time;   // incremented for every CPU tick in which it is executing 
	int wait;                   // tells the cpu for how long to wait during context switch/burst execution before changing mode
	Mode mode;                  // current mode of the CPU
	std::shared_ptr<Process> current_process;    
	CPUQueue ready_queue; // CPU ready queue
	CPUQueue io_queue;    // CPU io queue, home of blocked threads
	CPUQueue job_queue;   // All processes read from file are initialized into job queue
};

void stats_default(CPUScheduler & cpu, CPUQueue q);

// Prints the final stats of the CPUScheduler in detailed mode
void stats_detailed(CPUScheduler & cpu, CPUQueue  q);

// Print function
void printDefaultStats(CPUScheduler & cpu, CPUQueue  q, float cpu_util, int time);

float turnaroundTime(CPUScheduler & cpu, CPUQueue  q);

int initializeJobQueue(CPUScheduler & cpu);


int parseProcesses(CPUScheduler & cpu);


int parseThreads(CPUScheduler & cpu, int process_num);


int parseThread(CPUScheduler & cpu, int process_num);


int parseBursts(std::shared_ptr<Process> process);

// Function to read the first line of the input file
int readCPUInfo(CPUScheduler & cpu);


void commandLineFlags(CPUScheduler & cpu, char ** argv, int argc);

