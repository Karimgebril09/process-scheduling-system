#pragma once
#include"Processor.h"
#include"LinkedList.h"
class FCFSprocessor :public Processor
{
	private:
		LinkedList<process*> ready;
		bool last;
		int Tree_Count(process* Root)const;
		int searchid(int) const;
	public:
		FCFSprocessor(schedular* ptr, bool = false);			    ///////////////////////////////////////////////
		FCFSprocessor();									        //
		virtual void fromNEWtoRDY(process* x);						//
		virtual void fromblktordy(process* x);						//
		virtual void fromRDYtoRUN();								//				//   FOR COMMENTS PLS CHECK THE ABSTRACT CLASS
		virtual void ScheduleAlgo();								//	
		int getRDYcnt() const;									    //
		virtual void totermination(process* = nullptr);				//
		virtual void toblock();										//
		virtual void print() const;									///////////////////////////////////////////////
		///////////////////////////////////////
		virtual void toready(process*);
		virtual process* migration_dequeue();
		virtual void update_pro_time();
		void deletetree(process*);
		virtual bool isthere(int);
		void remove(int);
		virtual void deletetree(int);
		virtual void gotoblk();
		virtual void terminate();
		virtual void migration();
		virtual void kill();
		virtual void checkfork();
};

