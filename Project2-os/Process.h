#pragma once

#include "Burst.h"
#include <list>
#include <stdio.h>
#include <iostream>
#define DEFAULT_EXIT_VALUE -1
using namespace std;



class Process
{
public:
	Process(int process_num, int arrival_t, int cpu_bursts)
	{
		process_number = process_num;
		
		cpu_time = 0;
		cpu_process_total = 0;
		io_process_total = 0;
		io_time_remaining = 0;

		arrival_time = arrival_t;
		start_time = -1;
		exit_time = DEFAULT_EXIT_VALUE;

		bursts = cpu_bursts;
	}

	void addBurst(int cpu_time, int io_time)
	{
		Burst burst(cpu_time, io_time);
		burst_queue.push_back(burst);
	}

	int getNumberOfBursts()
	{
		return bursts;
	}

	int getArrivalTime()
	{
		return arrival_time;
	}

	int getIOTimeRemaining()
	{
		return io_time_remaining;
	}


	int getProcessNumber()
	{
		return process_number;
	}

	int getStartTime()
	{
		return start_time;
	}

	void setStartTime(int t)
	{
		start_time = t;
	}

	int getIOProcessTotal()
	{
		return io_process_total;
	}

	void setIOProcessTotal(int t)
	{
		io_process_total = t;
	}

	int getCPUTime()
	{
		return cpu_time;
	}
	void cpuTimeIncrease(int d)
	{
		cpu_time += d;
	}

	void cpuProcessTotalIncrease(int d)
	{
		cpu_process_total += d;
	}

	void setExitTime(int t)
	{
		exit_time = t;
	}

	void decrement()
	{
		io_time_remaining--;
	}

	void setTimings()
	{
		Burst burst = burst_queue.front();
		cpu_time = burst.get_cpu_time();
		io_time_remaining = burst.get_io_time();
		burst_queue.pop_front();
	}

	int getCPUProcessTotal()
	{
		return cpu_process_total;
	}

	int getExitTime()
	{
		return exit_time;
	}

	void print()
	{
		cout << "IO Time remaining: " << io_time_remaining << endl;
		cout << "Process Number: " << process_number << endl;

		cout << "Arrival Time: " << arrival_time << endl;
		cout << "Start Time: " << start_time << endl;
		cout << "Exit Time: " << exit_time << endl;
		cout << "Total CPU time: " << cpu_process_total << endl;
		cout << "Total IO time: " << io_process_total << endl;
		cout << "CPU Bursts: " << bursts << endl;
		for (Burst burst : burst_queue)
		{
			burst.display();
		}
		cout << "\n";
	}

private:
	int process_number;         // represents the process to which this thread belong

	int arrival_time;           // time it arrives in CPUSim 
	int start_time;             // time when it begins execution 
	int io_time_remaining;      // for decrementing in IO queue
	int cpu_time;               // length of the current cpu burst
	int io_process_total;        // total io time done by thread
	int cpu_process_total;       // total cpu time done by thread
	int exit_time;              // time it exits the CPUSim*/
	int bursts;                 // number of cpu-io burst pairs*/
	list<Burst> burst_queue;   // execution stack of the thread
};
