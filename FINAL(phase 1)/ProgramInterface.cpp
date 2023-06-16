#include "ProgramInterface.h"
#include <limits>
ProgramInterface::ProgramInterface(schedular* s) : psched(s), isstart(true)
{
	h = GetStdHandle(STD_OUTPUT_HANDLE);
}

void ProgramInterface::readmode() // defines a mode from the 3 interface modes
{
	int MODE;
	SetConsoleTextAttribute(h, 13); // for coloring the console
	cout << "Please enter the required mode:" << endl;
	cout << "Interactive:1" << endl;
	cout << "Step-by-step:2" << endl;
	cout << "Silent:3" << endl;
	do // loop for valid entry
	{
		cin >> MODE;
		cin.ignore(); // ignore the new line 
		if (MODE > 3 || MODE < 1) // invalid
			cout << "invalid entry please try again\n";
	} while (MODE > 3 || MODE < 1);
	switch (MODE)
	{
	case Interactive: // case 1
		Dmode = Interactive;
		break;
	case step_by_step:
		Dmode = step_by_step;
		break;
	case silent:
		Dmode = silent;
		break;
	}
}

bool ProgramInterface::updateinterface(bool isfinished)
{
	if ( (Dmode == step_by_step || Dmode == Interactive) &&(!isfinished) )
	{
		///////////////////////start of main cout///////////////////
		
		SetConsoleTextAttribute(h, 11);
		cout << "Current Timestep:" << psched->get_time_steps() << endl; 
		SetConsoleTextAttribute(h, 5); // for coloring the console
		cout << "-------------------RDY Processes--------------------------" << endl;

		///////////////////////Start Printing of the Processors RDY count///////////////////
		int tot_count = psched->get_TotalPRos_count();
		for (int i = 1; i <= tot_count; i++) // loop on all processors and print ready
		{
			int rdycnt = 0;
			Processor* ptr = psched->get_ProcessorPtr(i);
			{
				rdycnt = psched->get_ProcessorPtr(i)->getRDYcnt();
				if (ptr->Get_Type() == "SJF")  // color depend on processor type
					SetConsoleTextAttribute(h, 1);
				else if (ptr->Get_Type() == "RR")
						SetConsoleTextAttribute(h, 8);
				cout << "Processor " << i << "[" << ptr->Get_Type() << "] " << rdycnt << " RDY: ";
				ptr->print();
				cout << endl;
			}
		}
		/////////////////END PRINTING OF PROCESSORERS///////////////


		////////////////Start of Blk List//////////////
		SetConsoleTextAttribute(h, 6);
		cout << "------------------------ BLK processes------------------------------------" << endl;
		cout << psched->getcurrentblkcount();
		cout << " BLK: ";
		psched->printblk(); // calls block print from schedular
		cout << endl;

		/////////////////Start Of Run of All Processor///////////////
		SetConsoleTextAttribute(h, 10);
		cout << "------------------------ RUN processes------------------------------------" << endl;
		cout << Processor::BusyCount;
		cout << " RUN: ";
		bool flag = false;
		for (int i = 1; i <= tot_count; i++) // loop on all processors and get run
		{
			Processor* ptr;
			ptr = psched->get_ProcessorPtr(i);
			if (ptr)
			{
				if (ptr->getrun()) // if it has run
				{
					if (flag)
						cout << ", ";
					cout << ptr->getrun()->get_PID() << "(P" << i << ")"; // print id
					flag = true;
				}
			}
		}
		cout << endl;


		/// //////////////////////////////////////////////////////////////////////////////
		SetConsoleTextAttribute(h, 12);
		cout << "------------------------ TRM processes------------------------------------" << endl;
		cout << psched->getcurrenttrmcount();
		cout << " TRM: ";
		psched->printtrm();
		cout << endl;
		if (Dmode == step_by_step)
		{
		 Sleep(1000);
		}
		else
		{
			SetConsoleTextAttribute(h, 3);
			char dummyreading;
			cout << "Press Any Key To Continue To The Next Step!" << endl;
			dummyreading = getchar();
		}
	}
	else if(!isfinished)
	{
		if (isstart)
		{
			SetConsoleTextAttribute(h, 11);
			cout << "Silent Mode....................\tSimulation start...\n";
			isstart = false;
		}
	}
	else if (Dmode == 3) /// silent mode
	{
		SetConsoleTextAttribute(h,11);
		cout << "Simulation ends , Output file created\n"; 
	}
	return isfinished; // true if it is finished
}