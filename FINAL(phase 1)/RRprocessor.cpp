#include "RRprocessor.h"
#include"scheduler.h"
RRprocessor::RRprocessor()
{
	run = nullptr;
	TypeName = "RR";
}
RRprocessor::~RRprocessor()
{
}
RRprocessor::RRprocessor(schedular* ptr, int x) :Processor(ptr, "RR")
{
	timeslice = x;
	run = nullptr;
	TypeName = "RR";
}
void RRprocessor::fromRDYtoRUN()
{
	if (!run && !ready.isEmpty())
	{
		ready.dequeue(run);
		Processor::BusyCount++;
		IDLE = false;
	}
}
void RRprocessor::fromblktordy(process* ptr)
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

void RRprocessor::ScheduleAlgo()
{
	if (run && run->get_RR_time() == timeslice)
	{
		process* temp = run;
		(run)->set_RR_time(0);
		run = nullptr;
		ready.enqueue(temp);
		IDLE = true;
		Processor::BusyCount--;
	}
	migration();
	if(!ready.isEmpty())
	{
		process* temp = nullptr;
		if (!run)
		{
			Processor::BusyCount++;
			ready.dequeue(temp);
			IDLE = false;
			run = temp;
			if (run->get_Responsetime() == -1)
				run->set_Response_Time(manager->get_time_steps() - run->get_AT());
		}
	}
	
}

void RRprocessor::fromNEWtoRDY(process* x)
{
	if (x)
	{
		ready.enqueue(x);
		total_pro_time += x->get_CT();
	}
}
int RRprocessor::getRDYcnt() const
{
	return ready.getcount();
}

void RRprocessor::totermination(process* x)
{
	
}

void RRprocessor::toblock()
{
	process* x = run;
	run = nullptr;
	if (x)
	{
		manager->ToBLK(x);
		IDLE = true;
	}
}


void RRprocessor::print() const
{
	ready.print(); // calls queue print
}

void RRprocessor::toready(process*temp)
{
	if (temp)
	{
		if (!run && ready.isEmpty())
		{
			run = temp;
			Processor::BusyCount++;
			IDLE = false;
		}
		else
			ready.enqueue(temp);
		total_pro_time += temp->get_CT() - temp->get_runningtime();
	}
}

process* RRprocessor::migration_dequeue()
{
	process* temp;
	if (ready.peek(temp))
	{
		total_pro_time -= temp->get_CT() - temp->get_runningtime();
		ready.dequeue(temp);
		return temp;
	}
	return nullptr;
}

void RRprocessor::update_pro_time()
{
	if (run)
	{
		dec_pro_time(1);  // decrease total pro time
		run->incrementRuntime(); // increse runtime by 1
		busy_time++;
		(run)->set_RR_time(1 + (run)->get_RR_time()); // increment round robin time by 1 to be used
	}
}

void RRprocessor::remove(int a)
{
}

void RRprocessor::terminate()
{
	if (run && run->get_CT() >= run->get_runningtime())
	{
		if (run->getleft())
			manager->gokillorphan(run->getleft()->get_PID());
		else if(run->getright())
			manager->gokillorphan(run->getright()->get_PID());
		manager->ToTrm(run);
		run = nullptr;
		Processor::BusyCount--;
		IDLE = true;
	}
}

void RRprocessor::migration()
{
	if (!run)
	{
		process* temp = nullptr;
		if (!ready.isEmpty())
		{
			ready.peek(temp);
			while (!ready.isEmpty() && temp->get_CT() - temp->get_runningtime() < manager->get_RTF())
			{
				temp->set_RR_time(0);
				total_pro_time -= temp->get_CT() - temp->get_runningtime();
				manager->migration(2, temp);
				ready.dequeue(temp);
				if (!ready.isEmpty())
					ready.peek(temp);
			}
		}
	}
}
void RRprocessor::gotoblk()
{
	if (run)
	{
		if (run->HasIOIR() && run->get_IOIR() == run->get_runningtime()) // updated instead of get timesteps
		{
			run->incrementcurrentior();
			run->set_RR_time(0); // reset round robin time
			dec_pro_time(run->get_CT() - run->get_runningtime());
			toblock();
			Processor::BusyCount--;
			IDLE = true;
		}
	}
}
