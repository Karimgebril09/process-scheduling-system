#pragma once
#include "Processor.h"
#include"LinkedQueue.h"
class RRprocessor : public Processor
{
private:
	int timeslice;
	LinkedQueue<process*> ready;
public:
	RRprocessor(schedular* ptr, int x);						///////////////////////////////////////////////            
	RRprocessor();											//
	~RRprocessor();											//
	virtual void fromRDYtoRUN();							//
	virtual void fromblktordy(process* ptr); 				//
	virtual void fromNEWtoRDY(process*); 					//
	virtual void toready(process*);
	virtual void totermination(process* = nullptr);				//
	virtual void toblock();											//
	virtual void ScheduleAlgo();							// the main function
	virtual int getRDYcnt() const;								//				//
	virtual void print() const;										///////////////////////////////////////////////

	virtual process* migration_dequeue();
	virtual void update_pro_time();
	virtual void remove(int);
	virtual void terminate();
	virtual void migration();
	virtual void gotoblk();
};

