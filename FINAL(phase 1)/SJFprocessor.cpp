#include "SJFprocessor.h"
#include"scheduler.h"
SJFprocessor::SJFprocessor(schedular* ptr) :Processor(ptr, "SJF")
{
	run = nullptr;
	TypeName = "SJF";
}

SJFprocessor::SJFprocessor()
{
	TypeName = "SJF";
	run = nullptr;
}

void SJFprocessor::fromRUNtoRDY()
{
}
void SJFprocessor::fromRDYtoRUN()
{
	if (!run && !ready.isEmpty())
	{
		ready.dequeue(run);
		Processor::BusyCount++;
		IDLE = false;
	}
}
void SJFprocessor::fromblktordy(process* ptr)
{
	if (ptr)
	{
		if (!run && ready.isEmpty())
		{
			run = ptr;
			Processor::BusyCount++;
			IDLE = false;
		}
		else
			ready.enqueue(ptr);
		total_pro_time += ptr->get_CT() - ptr->get_runningtime();
	}
}
void SJFprocessor::ScheduleAlgo()
{
	process* temp = nullptr;
	if (!ready.isEmpty())
		ready.peek(temp);
	if (!run)
	{
		if (!ready.isEmpty()) // if there are processes in rdy list
		{
			ready.dequeue(temp);
			run = temp;  // new process to run
			IDLE = false;
			Processor::BusyCount++; // increment busy count
			if (run->get_Responsetime() == -1)
				run->set_Response_Time(manager->get_time_steps() - run->get_AT());
		}
	}
}
void SJFprocessor::fromNEWtoRDY(process* x)
{
	if (x)
	{
		ready.enqueue(x);
		total_pro_time += x->get_CT();
	}
}
int SJFprocessor::getRDYcnt() const
{
	return ready.getcount();
}

void SJFprocessor::totermination(process* x)
{
	if (!x) // not nullptr then means run
	{
		x = run;
		run = nullptr;
		if (x)
		{
			Processor::BusyCount--;
			IDLE = true;
			manager->ToTrm(x);
			IDLE = true;
		}
	}
	else
	{
		manager->ToTrm(x);
	}
}

void SJFprocessor::toblock()
{
	process* x = run;
	run = nullptr;
	if (x)
	{
		manager->ToBLK(x);
		IDLE = true;
	}
}

void SJFprocessor::print() const
{
	ready.print();
}

void SJFprocessor::toready(process* temp)
{
	if (temp)
	{
		if (!run && ready.isEmpty()) // if no running process
		{
			run = temp; // set run to the new pointer
			Processor::BusyCount++;
			IDLE = false;
		}
		else
			ready.enqueue(temp);
		total_pro_time += temp->get_CT() - temp->get_runningtime();
	}
}

process* SJFprocessor::migration_dequeue() // for work steal
{
	process* temp;
	if (ready.peek(temp))
	{
		total_pro_time -= temp->get_CT() - temp->get_runningtime(); // decrement processor time
		ready.dequeue(temp); 
		return temp; // return a process
	}
	return nullptr;
}

void SJFprocessor::update_pro_time()
{
	if (run)
	{
		dec_pro_time(1); // update processor time
		run->incrementRuntime(); // increse runtime by 1
		busy_time++;
	}
}

void SJFprocessor::remove(int a)
{
}

void SJFprocessor::gotoblk()
{
	if (run && run->HasIOIR())
	{
		if (run->get_runningtime() == (run)->get_IOIR()) // if i has block now
		{
			run->incrementcurrentior();
			dec_pro_time(run->get_CT() - run->get_runningtime()); // decrement pro time
			toblock(); // block assigns it to null
			Processor::BusyCount--;
			IDLE = true;
		}
	}
}

void SJFprocessor::terminate()
{
	if (run && (run && run->get_runningtime() >= run->get_CT()))
	{
		// special case: if fcfs migrated to RR then RR migrated to sjf so there may be children
		if (run->getleft() || run->getright())
		{
			if (run->getleft())
				manager->gokillorphan(run->getleft()->get_PID());
			if (run->getright())
				manager->gokillorphan(run->getright()->get_PID());
		}
		totermination(); // move the run to the termination list
	}
}

void SJFprocessor::migration() // no migration from fcfs
{
	return;
}