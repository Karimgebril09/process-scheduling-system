#include "scheduler.h"
#include"ProgramInterface.h"
#include<iomanip>
int process::forkprob = 0; // for forking
int schedular::starting_id = 0; // for children ids
// FOR OUTPUT FILE REQUIREMENTS
int schedular::Migration_Count_RTF = 0; 
int schedular::Migration_Count_MaxW = 0;
int schedular::Killed_Count = 0;
int schedular::Forked_Count = 0;
int schedular::WORKST_Count = 0;
void schedular::loadfrominput()
{
	fin.open(file+".txt");				 //opens the file
	if (!(fin.is_open()))				// Check if Exist
	{
		std::cout << "File Doesnt Exsit";
		return;
	}
	fin >> fcfscount >> sjfcount >> RRcount >> timeslice >> RTF >> MAXw >> STL >> Forkprob >> processescnt;	// Main Data 
	schedular::starting_id = processescnt;
	process::forkprob = Forkprob;
	for (int i = 0; i < processescnt; i++)
	{
		process* newprocess = new process; // create a process
		NEW.enqueue(newprocess);
		int reading;
		fin >> reading;
		newprocess->set_AT(reading); //  arrival time
		fin >> reading;
		newprocess->set_PID(reading); // ID
		fin >> reading;
		newprocess->set_CT(reading);// ct time
		int n;
		fin >> n;
		char ignore;
		newprocess->set_requestsnum(n); // number of block request
		for (int j = 0; j < n; j++)
		{
			(newprocess)->incrementcurrentior(); // update current number of request
			(newprocess)->incrementcurrentiod();
			fin >> ignore;
			fin >> reading;
			newprocess->set_IOIR(reading);
			fin >> ignore;
			fin >> reading;
			newprocess->set_IOID(reading);
			newprocess->set_IODoutput(reading);
			if (n - j == 1)
				fin>>ignore;
			else
			{
				fin >> ignore;
				fin >> ignore;
			}
		}
		newprocess->resetio(); // reset counter of io to 0.
	}
	int reading;
	while (!fin.eof()) // read the signall kills
	{
		fin >> reading;
		sigkilltime.enqueue(reading);
		fin >> reading;
		sigkillid.enqueue(reading);
	}
}
void schedular::Save_Output()
{
	fout.open(filesave + ".txt");  //opens the file of the given name
	fout << "TT\tPID\tAT\tCT\tIO_D\tWT\tRT\tTRT\n";
	long long int total_WT_time = 0;
	long long int total_TRT_time = 0;
	long long int total_RT_time = 0;
	long long int total_RUNTIME_time = 0;
	fout << std::fixed << setprecision(2) << endl;
	for (int i = 0; i < processescnt; i++)
	{
		process* pro;
		TRM.dequeue(pro);
		int TRT = pro->get_TT() - pro->get_AT();
		int WT = TRT - pro->get_CT();
		if (pro->get_CT() > pro->get_runningtime()) // signal kill or orphan
		{
			WT = TRT - pro->get_runningtime();
		}
		total_RUNTIME_time += pro->get_runningtime();
		total_WT_time += WT;
		total_TRT_time += TRT;
		total_RT_time += pro->get_Responsetime();
		fout << pro->get_TT() << "\t";
		fout << pro->get_PID() << "\t";
		fout << pro->get_AT() << "\t";
		fout << pro->get_CT() << "\t";
		fout << pro->get_IOID_Output() << "\t";
		fout << WT << "\t";
		fout << pro->get_Responsetime() << "\t";
		fout << TRT;
		fout << "\n";
		delete pro;
	}
	fout << "Processes: " << processescnt << endl;
	fout << "Avg Wait: " << total_WT_time / float(processescnt) << ",   ";
	fout << "Avg RT: " << total_RT_time / float(processescnt) << ",   ";
	fout << "Avg TRT: " << total_TRT_time / float(processescnt) << endl;
	fout << "Migration%: ";
	fout << "RTF= " << Migration_Count_RTF / float(processescnt) * 100 << "%,     ";
	fout << "MaxW= " << Migration_Count_MaxW / float(processescnt) * 100 << "%" << endl;
	fout << "WorkSteal%: " << WORKST_Count / float(processescnt) * 100 << "%" << endl;
	fout << "ForkedProcess%: " << Forked_Count / float(processescnt) * 100 << "%" << endl;
	fout << "KilledProcess%: " << Killed_Count / float(processescnt) * 100 << "%" << endl << endl;
	fout << "Processors: " << totalprocessorscount << " [" << fcfscount << ", " << sjfcount << ", " << RRcount << "]" << endl;
	fout << "Processors Load" << endl;
	for (int i = 1; i <= totalprocessorscount; i++)
	{
		fout << "p" << i << "=" << float(get_ProcessorPtr(i)->getbusytime()) / total_TRT_time * 100;
		if (i != totalprocessorscount)
			fout << "%,  ";
		else
			fout << "%";
	}
	fout << endl << endl;
	fout << "Processors Utiliz" << endl;
	int total_utiliz_avg = 0;
	for (int i = 1; i <= totalprocessorscount; i++)
	{
		float curr_perc = float(get_ProcessorPtr(i)->getbusytime()) / System_Finishtime * 100;
		fout << "p" << i << "=" << curr_perc;
		total_utiliz_avg += curr_perc;
		if (i != totalprocessorscount)
			fout << "%,  ";
		else
			fout << "%";
	}
	fout << endl;
	fout << "avg utiliz= " << total_utiliz_avg / double(totalprocessorscount) << "%";
	fout << "\nEnd of Output File Successfully" << endl;
	fout.close();
}
schedular::schedular()
{
	file = "block";				// Default File Name
	filesave = "_Output_File_";			//default file name
	loadfrominput();					//load 
	totalprocessorscount = fcfscount + sjfcount + RRcount;
	Allpros = new Processor * [totalprocessorscount];		//Array of Abstract Class pointers Processor
	//CREATION OF  THE PROCESSORS

	for (int i = 0; i < fcfscount - 1; i++) // loop and create fcfs
	{
		Allpros[i] = new FCFSprocessor(this);
	}
	Allpros[fcfscount - 1] = new FCFSprocessor(this, true); // set last processor to last to know if the kill is not found in any fcfs
	for (int i = fcfscount; i < fcfscount + sjfcount; i++)// loop and create sjf
	{
		Allpros[i] = new SJFprocessor(this);
	}
	for (int i = fcfscount + sjfcount; i < totalprocessorscount; i++) // loop and create RR
	{
		Allpros[i] = new RRprocessor(this, timeslice);
	}
	timesteps = 0;			// starting timestep
	currentprocessor = 0;				// index of the starting processor
	first_to_blk = true;
}

void schedular::NEWtoRDY()    ////////////   modified
{
	if (NEW.isEmpty())
		return;
	process* x;
	NEW.peek(x);
	while (x->get_AT() == timesteps) // loop on all processors with arrival time same as current timestep
	{
		currentprocessor = get_shortest_pro(0, totalprocessorscount); // get shortest processor
		NEW.dequeue(x);
		Allpros[currentprocessor]->fromNEWtoRDY(x);
		if (!NEW.peek(x))
			return;
	}
}

void schedular::ToBLK(process* pros)
{
	if (pros) // not equal null
		BLK.enqueue(pros); // add pros to queue.
}

void schedular::ToTrm(process* pros)
{
	if (pros) // not equal null
	{
		pros->set_TT(timesteps); // set termination time
		if (pros->get_Responsetime() == -1)
			pros->set_Response_Time(timesteps-pros->get_AT());
		TRM.enqueue(pros);
	}
}

void schedular::Updateblock()  //  only increase the io time and return it if finished
{
	if (!BLK.isEmpty())
	{
		process* tempprocess;
		BLK.peek(tempprocess);
		if (first_to_blk) // block was empty so don't update duration because it has just been inserted
			first_to_blk = false;
		else
			tempprocess->inc_IO_duration(1); // increment block time
		if (tempprocess->get_IO_duration() == tempprocess->get_IOID())  // if finished
		{
			BLK.dequeue(tempprocess); // remove from the block queue
			tempprocess->incrementcurrentiod();
			tempprocess->resetiodurtion(); // reset duration to zero for upcoming block
			int curr_processor = get_shortest_pro(0, totalprocessorscount);   //gets the processor
			Allpros[curr_processor]->fromblktordy(tempprocess); // insert to shortest ready
		}
	}
	else
		first_to_blk = true;
}

Processor* schedular::get_ProcessorPtr(int i)const
{
	return (Allpros[i - 1]);
}
int schedular::get_TotalPRos_count() const
{
	return totalprocessorscount;
}
bool schedular::timestep()
{
	if (System_Finishtime != 0) // if finished
	{
		Save_Output(); // create output file
		return true;
	}
	timesteps++;
	for (int i = 0; i < totalprocessorscount; i++)
		Allpros[i]->update_pro_time(); // update processors times
	NEWtoRDY(); // see if any process requires inserion
	// handle block
	Gotoblk();
	Updateblock();
	for (int i = 0; i < totalprocessorscount; i++)
	{
		Allpros[i]->ScheduleAlgo();             // call the maestro function for every processor
		Allpros[i]->checkfork();				// check if forking will happen
		Allpros[i]->terminate();				// check if function to terminate
		Allpros[i]->kill();						// check for signall kills
	}
	worksteal();
	if (TRM.getcount() >= processescnt)			//END OF THE PROGRAM
		System_Finishtime = timesteps;
	return false;
}

void schedular::printblk() const
{
	BLK.print();
}

void schedular::printtrm() const
{
	TRM.print();
}

int schedular::getcurrentblkcount() const
{
	return BLK.getcount();
}

int schedular::getcurrenttrmcount() const
{
	return TRM.getcount();
}

int schedular::get_shortest_pro(int start, int end, bool ready)
{
	if (ready)
		return schedular::shortestready(); // special one for ready only
	else
	{
		int j = start;
		int minindex = j;
		int min = Allpros[j]->get_pro_time();
		for (int i = j + 1; i < end; i++) // loop and select the minimum processor time
		{
			if (Allpros[i]->get_pro_time() < min && Allpros[i]->get_pro_time() >= 0)
			{
				minindex = i; // update index
				min = Allpros[minindex]->get_pro_time(); // update min value
			}
		}
		return minindex;
	}
}

int schedular::get_longest_pro(bool ready)
{
	if (ready)
		return schedular::longestready();
	int j = 0;
	if (j < totalprocessorscount)
	{
		int maxindex = j;
		int max = Allpros[maxindex]->get_pro_time();
		for (int i = j + 1; i < totalprocessorscount; i++) // loop and select the minimum processor time
		{
			if (Allpros[i]->get_pro_time() > max && Allpros[i]->get_pro_time() >= 0)
			{
				maxindex = i; // update index
				max = Allpros[i]->get_pro_time(); // update max value
			}
		}
		return maxindex;
	}
	else
		return -1;
}

int schedular::get_time_steps()
{
	return timesteps;
}


int schedular::get_RTF()
{
	return RTF;
}

void schedular::migration(int type,process* temp) // insert igrating process to the required
{
	int start = 0, end = 0;
	switch (type)
	{
	case 2: // means RTF migration
		start = fcfscount;
		end = fcfscount+sjfcount;
		Migration_Count_RTF++;
		break;
	case 3:
		start = fcfscount + sjfcount;
		end = fcfscount + sjfcount+RRcount;
		Migration_Count_MaxW++;
		break;
	}
	int index = get_shortest_pro(start, end); // get shortest pro in the required interval
	Allpros[index]->toready(temp); // insert to the ready
}

void schedular::worksteal() // longval > 0 && 
{
	if (!(timesteps%STL))
	{
		int longindex = get_longest_pro(true);
		int shortindex = get_shortest_pro(0,totalprocessorscount,true);
		int longval = Allpros[longindex]->get_pro_time() - Allpros[longindex]->get_runtime();
		int shortval = Allpros[shortindex]->get_pro_time() - Allpros[shortindex]->get_runtime();
		bool dont_exit = (longval - shortval) / float(longval) > 0.4;
		while (dont_exit)
		{
			process* temp=Allpros[longindex]->migration_dequeue();
			if (temp)
			{
				WORKST_Count++; // increment work steal count for final statistics
				Allpros[shortindex]->toready(temp); 
				Allpros[shortindex]->fromRDYtoRUN();
				longval = Allpros[longindex]->get_pro_time() - Allpros[longindex]->get_runtime();
				shortval = Allpros[shortindex]->get_pro_time() - Allpros[shortindex]->get_runtime();
			}
			dont_exit = ((longval - shortval) / float(longval) > 0.4) && temp;
		}

	}
}

int schedular::getkillid()
{
	if (sigkillid.isEmpty())
		return -1;
	int id;
	sigkillid.peek(id);
	return id;
}

bool schedular::ifcankill()
{
	if (sigkilltime.isEmpty()) // there are signall kills
		return false;
	int time;
	sigkilltime.peek(time);
	if (time == timesteps) // if it is now
		return true;
	else
		return false;
}


int schedular::getMaxw() const
{
	return MAXw;
}

void schedular::updatekilllist()
{
	int dummy;
	sigkillid.dequeue(dummy);
	sigkilltime.dequeue(dummy);
}

process* schedular::initiatefork(process* parent ,int newct, bool left)
{
	starting_id++; // update id to give
	processescnt++;// increment processes count
	schedular::Forked_Count++;
	process* ptr = new process(timesteps, starting_id, newct,true, left,parent); // instantiate a child
	int index = get_shortest_pro(0, fcfscount); // get shortest fcfs
	Allpros[index]->toready(ptr);
	return ptr;
}
void schedular::gokillorphan(int search_id) // go search for which processor and removes it
{
	for (int i = 0; i < fcfscount; i++)
	{
		int there = Allpros[i]->isthere(search_id); // if found in processor
		if (there)
			Allpros[i]->deletetree(search_id); // kill its tree
	}
}

int schedular::shortestready() const
{
	int j = 0;
	if (j < totalprocessorscount)
	{
		int minindex = j;
		int min = Allpros[minindex]->get_pro_time() - Allpros[minindex]->get_runtime();  // notice it doesn't include runtime
		for (int i = j + 1; i < totalprocessorscount; i++)
		{
			if (Allpros[i]->get_pro_time() - Allpros[i]->get_runtime() < min)
			{
				minindex = i;
				min = Allpros[minindex]->get_pro_time() - Allpros[minindex]->get_runtime();
			}
		}
		return minindex;
	}
	else
		return -1;
}

int schedular::longestready() const // only a special version to search only in ready
{
	int j = 0;
	if (j < totalprocessorscount)
	{
		int maxindex = j;
		int max = Allpros[maxindex]->get_pro_time() - Allpros[maxindex]->get_runtime(); // notice it doesn't include runtime
		for (int i = j + 1; i < totalprocessorscount; i++)
		{
			if (Allpros[i]->get_pro_time() - Allpros[i]->get_runtime() > max)
			{
				maxindex = i;
				max = Allpros[i]->get_pro_time() - Allpros[maxindex]->get_runtime();
			}
		}
		return maxindex;
	}
	else
		return -1;
}
void schedular::Gotoblk()
{
	for (int i = 0; i < totalprocessorscount; i++)
		Allpros[i]->gotoblk();
}
schedular::~schedular() // delete processors created with new
{
	for (int i = 0; i < totalprocessorscount;i++)
		delete Allpros[i];
	delete[]Allpros;
}
