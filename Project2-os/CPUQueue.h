#pragma once

#include "Process.h"
#include <memory>

class CPUQueue
{
public:
	void addProcess(std::shared_ptr<Process> t)
	{
		q.push_back(t);
	}

	std::shared_ptr<Process> removeProcess()
	{
		if (q.empty())
		{
			return nullptr;
		}
		std::shared_ptr<Process> t = q.front();
		q.pop_front();
		return t;
	}

	std::shared_ptr<Process> getHead()
	{
		return q.front();
	}

	std::shared_ptr<Process> removeProcessAtTime(int time)
	{
		for (auto p : q)
		{
			if (p->getArrivalTime() == time)
			{
				q.remove(p);
				return p;
			}
		}
		return nullptr;
	}

	std::shared_ptr<Process> removeIOProcessAtTime(int io_time_finished)
	{
		for (auto p : q)
		{
			if (p->getIOTimeRemaining() == io_time_finished)
			{
				q.remove(p);
				return p;
			}
		}
		return nullptr;
	}

	void decrementAllIO()
	{
		for (auto p : q)
		{
			p->decrement();
		}
	}

	void print()
	{
		for (auto p : q)
		{
			p->print();
		}
	}

	int size()
	{
		return q.size();
	}

public:
	std::list<std::shared_ptr<Process>> q;
};