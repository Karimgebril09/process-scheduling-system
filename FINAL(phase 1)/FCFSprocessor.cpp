#include "FCFSprocessor.h"
#include"scheduler.h"
FCFSprocessor::FCFSprocessor()
{
	TypeName = "FCFS";
	run = nullptr;
}
FCFSprocessor::FCFSprocessor(schedular* ptr, bool end) :Processor(ptr, "FCFS"), last(end) // initialize the base class constructors
{
	TypeName = "FCFS";
	run = nullptr;
}
void FCFSprocessor::fromNEWtoRDY(process* x)
{
	if (x)
	{
		ready.insert(ready.getLength(), x);  // just insert in the ready list
		total_pro_time += x->get_CT();       // increment the processor time
	}
}
// This function restores from blk to ready
void FCFSprocessor::fromblktordy(process* x)
{
	if (!run && ready.isEmpty())    // if no current running process the goes to run
	{
		run = x;
		Processor::BusyCount++;
		IDLE = false;
	}
	else
		ready.insert(ready.getLength(), x); // else go into the rdy 
	total_pro_time += x->get_CT() - x->get_runningtime(); // increment the processor time
}
void FCFSprocessor::fromRDYtoRUN()
{
	if (!run && !ready.isEmpty()) // if no current running process
	{
		run = ready.getEntry(0); // pick the first to run
		ready.remove(0);
		Processor::BusyCount++; // increment busy processors count
		IDLE = false;
	}
}
void FCFSprocessor::ScheduleAlgo()  // MAESTRO 
{ 
	migration();    // calls migration to check if a process exceeded MAXW
	if (!ready.isEmpty() && !run)
	{
		run = ready.getEntry(0);  // new process to run
		ready.remove(0);// remove from the list
		IDLE = false;
		Processor::BusyCount++;
		if (run->get_Responsetime() == -1) // set time at which the first run happens
			run->set_Response_Time(manager->get_time_steps() - run->get_AT());
	}
}
int FCFSprocessor::getRDYcnt() const
{
	return ready.getLength(); //readycnt
}
void FCFSprocessor::totermination(process* x)
{
	if (!x) // not nullptr then means run
	{
		x = run;
		run = nullptr;
		if (x)
		{
			IDLE = true;
			Processor::BusyCount--;
			manager->ToTrm(x); // calls to termination of the schedular
			IDLE = true;
		}
	}
	else
		manager->ToTrm(x);
}
void FCFSprocessor::toblock() // means it requested block
{
	process* x = run;
	run = nullptr;
	if (x)
	{
		manager->ToBLK(x); // calls to block of the schedular
		IDLE = true;
	}

}

void FCFSprocessor::print() const
{
	ready.print();  // normal print function
}

void FCFSprocessor::toready(process* temp) // called by migration
{
	if (temp)
	{
		if (!run && ready.isEmpty())  // if no current running process
		{
			run = temp; // assigns the new run
			Processor::BusyCount++;
			IDLE = false;
		}
		else
			ready.insert(ready.getLength(), temp); // else insert to rdy
		total_pro_time += temp->get_CT() - temp->get_runningtime();
	}
}

process* FCFSprocessor::migration_dequeue() // for work steal
{
	process* temp;
	int index = 0; // to search for the first non-cild
	temp = ready.getEntry(index);
	while (temp && temp->ischild()) // loop untill picks the first non-child
	{
		index++;
		if (index < ready.getLength())
			temp = ready.getEntry(index);
		else
			break;
	}
	if (index < ready.getLength()) // there is a non-child
	{
		if (temp)
		{
			total_pro_time -= temp->get_CT() - temp->get_runningtime(); // decrement pro time
			ready.remove(index); // remove 
		}
		return temp;
	}
	else
		return nullptr; // all children 
}

void FCFSprocessor::update_pro_time()
{
	if (run)
	{
		busy_time++; 
		dec_pro_time(1); // processor time decreases by one
		run->incrementRuntime(); // increse run time by 1
	}
}
void FCFSprocessor::deletetree(process* parent)
{
	if (!parent)
		return;
	if (!parent->ischild()) // only case when it is achild who finish runtime
		schedular::Killed_Count += Tree_Count(parent) - 1;
	else if (parent->getparent() && (parent->get_CT() == parent->get_runningtime() && parent->get_runningtime() > 0)) // parent alive
		schedular::Killed_Count += Tree_Count(parent) - 1;;
	if (parent->getleft()) // there is a tree
	{
		process* left = parent->getleft();
		left->setparent(nullptr); // free parent
		parent->setleft(nullptr);
		deletetree(left); // kill the tree
	}
	if (parent->getright()) // there is a tree
	{
		process* right = parent->getright();
		right->setparent(nullptr); // free parent
		parent->setright(nullptr);
		deletetree(right); // kill the tree
	}
	if (parent != run)
	{
		int search_id = searchid(parent->get_PID());
		if (search_id != -1) // means it is in the rdy
		{
			dec_pro_time(parent->get_CT() - parent->get_runningtime());
			ready.remove(search_id); // remove it from the list
			totermination(parent);
		}
		else
			manager->gokillorphan(parent->get_PID()); // neither run nor rdy then it is in another processor
	}
	else // means in run
	{
		dec_pro_time(run->get_CT() - run->get_runningtime());
		totermination();
	}
}

bool FCFSprocessor::isthere(int id) // search by id 
{
	if (searchid(id) != -1 || (run && run->get_PID() == id))
		return true;
	else // not run or ready
		return false;
}

void FCFSprocessor::remove(int id)
{
	ready.remove(searchid(id));
}

void FCFSprocessor::deletetree(int id) // called from the schedular 
{
	if (run && run->get_PID() == id)
		deletetree(run); // if in run then terminate
	else
	{
		int pos = searchid(id); // get location in list
		process* temp = ready.getEntry(pos);
		deletetree(temp);
	}
}

int FCFSprocessor::Tree_Count(process* Root) const // count the number of nodes in tree
{
	if (Root == nullptr)
		return 0;
	else
		return 1 + Tree_Count(Root->getleft()) + Tree_Count(Root->getright());
}
void FCFSprocessor::gotoblk()
{
	if (run && run->HasIOIR() && run->get_runningtime() == run->get_IOIR())
	{

		run->incrementcurrentior();
		dec_pro_time(run->get_CT() - run->get_runningtime());
		toblock();
		Processor::BusyCount--;
		IDLE = true;
	}
}

void FCFSprocessor::terminate() // called to terminate run 
{
	if (run && (run && run->get_runningtime() >= run->get_CT())) // if it finished required ct
		deletetree(run);
}

void FCFSprocessor::migration()
{
	if (!run)
	{
		if (!ready.isEmpty())
		{
			process* temp = ready.getEntry(0);
			bool condition = manager->get_time_steps() - temp->get_AT() - temp->get_runningtime() >= manager->getMaxw();
			while (!ready.isEmpty() && !temp->ischild() && condition) // loop on all non-children which exceeded maxw
			{
				total_pro_time -= temp->get_CT() - temp->get_runningtime(); // decrement processor time
				manager->migration(3, temp); // migrate to shortest RR
				ready.remove(0); // remove first element
				if (!ready.isEmpty())
				{
					temp = ready.getEntry(0); // get the next
					condition = manager->get_time_steps() - temp->get_AT() - temp->get_runningtime() >= manager->getMaxw(); // update condition
				}
			}
		}
	}
}

void FCFSprocessor::kill()
{
	while (manager->ifcankill()) // If in the current timestep there is a signal kill
	{
		int id = manager->getkillid();
		if (run && id == run->get_PID())
		{
			deletetree(run); // delete tree decrements pro time
			schedular::Killed_Count++;
			manager->updatekilllist();
			IDLE = true;
		}
		else if (searchid(id) != -1)
		{
			int position = searchid(id);
			process* ptr = ready.getEntry(position);
			schedular::Killed_Count += Tree_Count(ptr);
			deletetree(ptr); // delete tree removes implicitly the ptr
			manager->updatekilllist();
		}
		else
		{
			if (last) // last processor in fcfs type
				manager->updatekilllist(); // maybe the process to be killed has migrated so, remove it from kill_list
			break;
		}
	}
}

void FCFSprocessor::checkfork()
{
	if (run && !run->get_dontfork() && run->ifcanfork() && (!run->getleft() || !run->getright()))
	{
		if (!run->getleft())
		{
			process* temp = run;
			temp->setleft(manager->initiatefork(run, run->get_CT() - run->get_runningtime(), true));
		}
		else
		{
			process* temp = run;
			temp->setright(manager->initiatefork(run, run->get_CT() - run->get_runningtime(), false));
			temp->set_dontfork(true);
		}
	}
}

int FCFSprocessor::searchid(int id) const
{
	int ind = 0;
	while (ind < ready.getLength())
	{
		process* temp = ready.getEntry(ind);
		if (temp->get_PID() == id)
			return ind;
		ind++;
	}
	return -1;
}