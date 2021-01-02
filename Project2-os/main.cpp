#include "CPUScheduler.h" // Include the header file

int main(int argc, char ** argv)
{

	CPUScheduler cpu;
	CPUQueue exit_queue;

    // Call function to read the commands
    commandLineFlags(cpu, argv, argc);
  
	initializeJobQueue(cpu);

	while (cpu.canContinue(exit_queue))
	{
		switch (cpu.mode)
		{
		// Initialize CPU 
		case NEWCPU:
			cpu.setMode(DISPATCHING);
			break;

		// Get the next process
		case DISPATCHING:
			
			cpu.getNextProcess();
			break;

		// Executes a burst or loads in a new one if there is not one executing
		case EXECUTING:
		
			cpu.executeProcess(exit_queue);
			break;
		case PSWITCH:
		case TSWITCH:
			// Check the status of CPU
			cpu.checkStatus();
			break;
		default:
			printf("Error. Program exits");
			exit(0);
		}

		// Move Arriving processess into ready queue 
		cpu.addArrivingIOProcessToReadyQueue();

		// Move any finished IO threads to ready queue*/
		cpu.addFinishedIOProcessToReadyQueue();

		// clock starts
		cpu.advanceClock();
	}

	// Display all CPU info
	cpu.calculateStatistics(exit_queue);

	return 0;
}
