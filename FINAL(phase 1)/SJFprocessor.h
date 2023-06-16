#pragma once
#include "Processor.h"
#include"SJFqueue.h"
class SJFprocessor :public Processor
{
private:
	SFJqueue<process*> ready;
public:
	SJFprocessor(schedular* ptr);                       ///////////////////////////////////////////////
	SJFprocessor();										//
	virtual void fromRUNtoRDY();								//
	virtual void fromRDYtoRUN();								//
	virtual void fromblktordy(process* ptr);					//
	virtual void ScheduleAlgo();								//
	virtual void fromNEWtoRDY(process*);						//
	virtual int getRDYcnt() const;									//
	virtual void totermination(process* = nullptr);								//
	virtual void toblock();										//
	virtual void print() const;									/////////////////////////////////////////////////////
	////////////////////////////////////
	virtual void toready(process*);
	virtual process* migration_dequeue();
	////////////////////////////////////
	// ziad
	virtual void update_pro_time();
	virtual void remove(int);
	virtual void gotoblk();
	virtual void terminate();
	virtual void migration();
};

