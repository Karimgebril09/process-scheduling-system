#pragma once
class schedular;
#include"process.h"
#include<string>
#include<ctime>

class Processor
{
protected:
	schedular* manager;								// Schedular Ptr to manage transfering of process
	process* run;									//Each Processor have his current running process

	bool IDLE;										// to know his current state wether idle or not
	int busy_time;									// phase 2
	std::string TypeName;							//To be Used in UI for (Cout), each processor have his own string name
	int total_pro_time;

	int current_overheatstps;
	bool overheated;

public:
	static int BusyCount;							//Count of Processors That have running process 
	Processor();
	Processor(schedular* ptr,std::string);						//intialize the manger ptr 
	virtual ~Processor();
	process* getrun();								// return the current running process
	void remouverun();								// remove the run process and make it null 
	bool isIDLE();									// to know his current state wether idle or not
	// time based functions
	void inc_pro_time(int val);
	void dec_pro_time(int val);
	void updatebusytime();
	int getbusytime()const;
	int get_pro_time();
	int get_runtime();
	/*				 Pure virtual functions				*/
	virtual void fromRDYtoRUN() = 0;
	virtual void fromblktordy(process* ptr) = 0; 
	virtual void ScheduleAlgo() = 0;				// The maestro function
	virtual void fromNEWtoRDY(process*) = 0;	    
	virtual void print() const = 0;					// calls ready Queue print
	virtual void totermination(process*)=0;    //modified
	virtual void toblock()=0;
	virtual void toready(process*)=0;
	virtual void update_pro_time() = 0;
	virtual int getRDYcnt() const = 0;					// return the Number of procceses that in its ready list
	virtual process* migration_dequeue() = 0;
	virtual void gotoblk() = 0;
	virtual void terminate() = 0;
	virtual void migration() = 0;
	virtual void remove(int) = 0;
	/*				NON-PURE virtual		*/		
	virtual bool isthere(int);                // recieves id and returns position
	virtual void deletetree(int);             // to be used by fcfs only
	virtual void kill();					 // to be used by fcfs only
	virtual void checkfork();					// to be used by fcfs only
	virtual std::string Get_Type() const;		//To be Used in UI for (Cout), each processor have his own string name
};