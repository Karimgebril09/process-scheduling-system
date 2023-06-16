#pragma once
class Processor;
#include<iostream>
class process 
{
private:
	int io_requestsnum;
	int* IOIR;
	int* IOID;
	int  IO_duration;           // current duration in block
	int current_IOR;			// index of current IOIR
	int current_IOD;
	bool HasIORID;				// has IODIOR request or not
	int round_robin_time;       // time for RR scheduling
	process* left_child;    // by convention it is the first child
	process* right_child;
	process* parent;
	bool is_child;  // true for children
	int waitingtime; // for output file and migration MAXW
	int AT;
	int Response_Time; // time before being picked to run
	int PID;    // process id
	int CT;     
	int runtime; // how many timesteps it has been running
	int TT;
	bool left;  // left or right child
	bool dont_fork; // if forked twice then no more
	int IOD_sum;
public:
	static int forkprob; // for comparisong in forking
	process();
	process(int, int, int,bool, bool,process*); // this is for the child
	~process();
	//////////////////////////////GETTERS//////////////////////////
	int get_IOIR() const;
	int get_IOID() const;
	int get_TT() const;
	int get_IO_duration() const;
	int get_AT() const;
	int get_PID() const;
	int get_CT() const;
	int get_runningtime() const;
	int get_RR_time() const;
	int get_Responsetime()const;
	int getwaittime() const;
	process* getleft() const; // get left child
	process* getright() const; // get left child
	process* getparent() const;
	int get_IOID_Output() const;

	bool HasIOIR()const;
	bool HasIOID() const;
	bool isleft() const;
	bool ischild() const;
	/*			SETTERS					*/
	void set_requestsnum(int n);
	void set_RR_time(int x);
	void set_current_IOIDIR(int);
	void set_attach_to(Processor* p);
	void set_Response_Time(int Response);
	void set_IOID(int io);
	void set_IOIR(int ir);
	void setleft(process*);
	void setright(process*);
	void set_TT(int inc);
	void set_CT(int inc);
	void set_AT(int inc);
	void set_PID(int inc);
	void set_IODoutput(int);
	void setparent(process*);
	/*			IO_operations			*/	
	void inc_IO_duration(int inc);
	void resetio();
	void incrementcurrentior(); // update requests array
	void incrementcurrentiod(); // update duration 
	void incrementcurrentio();
	void resetiodurtion();
	void incrementRuntime();
	//operator overloading
	bool operator < (process& x);
	bool operator > (process& x);
	friend std::ostream& operator<<(std::ostream& os, const process& ptr);
	/*            FORKING              */
	void set_dontfork(bool);
	bool get_dontfork() const; 
	bool ifcanfork() const; // generate fork probability
	void setwaittime(int);	
};
