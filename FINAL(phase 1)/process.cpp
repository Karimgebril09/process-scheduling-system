#include "process.h"
process::process() : parent(nullptr) ,is_child(false), Response_Time(-1), IOD_sum(0), dont_fork(false), round_robin_time(0), io_requestsnum(0), current_IOR(-1), current_IOD(-1), HasIORID(false), IO_duration(0)
{
	srand(time(0)); // for forking probability
	left_child = nullptr;  
	right_child = nullptr;
	runtime = 0;
}
// child constructor 
process::process(int a, int b, int c, bool child,bool lf, process* ptr)
{
	AT = a;   //sets AT with curret=nt time step
	PID = b;
	CT = c;
	runtime = 0;
	left = lf;
	TT = 0;
	waitingtime = 0;
	left_child = nullptr;
	right_child = nullptr;
	parent = ptr;  // has a pointer to its parent
	is_child = true;
	dont_fork = false;
	IOD_sum = 0;
	Response_Time = -1;
	srand(time(0));
}
process::~process()
{
	if (io_requestsnum > 0) // has block operations
	{
		delete[]IOIR;
		delete[]IOID;
	}
}
///////////////GETTERS////////
int process::get_IOIR()const
{
	if (HasIOIR())
		return IOIR[current_IOR];
	else
		return -1;
}
int process::get_IOID()const
{
	if (HasIOID())
		return IOID[current_IOD];
}
int process::get_TT()const
{
	return TT;
}

int process::get_IO_duration() const
{
	return IO_duration;
}
int process::get_RR_time() const
{
	return round_robin_time;
}
void process::incrementcurrentior()
{
	current_IOR++;
}
void process::incrementcurrentiod()
{
	current_IOD++;
}
////////////////////////////// SETTERS///////////////////// BY INCREMNTING WHATEVER ITS GIVEN
void process::set_Response_Time(int Response)
{
	Response_Time = Response;
}


void process::set_requestsnum(int n)
{
	if (n > 0)
	{
		IOID = new int[n];
		IOIR = new int[n];
		io_requestsnum = n;
	}
}

void process::set_current_IOIDIR(int inc)
{
	current_IOR += inc;
}
bool process::ischild() const
{
	return is_child;
}
void process::set_RR_time(int x)
{
	round_robin_time = x;
}
bool process::isleft() const
{
	return left; // if left child or not
}
void process::inc_IO_duration(int inc) // update curent io_duratiob
{
	IO_duration += inc;
}
bool process::HasIOIR() const  // if there is an upcoming block request
{
	if (io_requestsnum > 0 && current_IOR < io_requestsnum && !is_child)
		return true;
	else
		return false;
}
bool process::HasIOID() const
{
	if (io_requestsnum > 0 && current_IOD < io_requestsnum && !is_child)
		return true;
	else
		return false;
}
void process::set_IOID(int io)
{
	IOID[current_IOD] = io;
}
void process::set_IOIR(int ir)
{
	IOIR[current_IOR] = ir;
}
void process::resetio() 
{
	current_IOD = 0;  // reset current iorequest to start of the array
	current_IOR = 0;  // reset current ioduration to start of the array
}
// operators based on runtime
bool process::operator<(process& x) 
{
	return (CT - runtime) < (x.CT - x.runtime);
}
bool process::operator>(process& x)
{
	return (CT - runtime) > (x.CT - x.runtime);
}

void process::setleft(process* ptr) // points to my child 
{
	left_child = ptr;
}
void process::setright(process* ptr)
{
	right_child = ptr;
}
process* process::getleft() const
{
	return left_child;
}
process* process::getright() const
{
	return right_child;
}
bool process::ifcanfork() const
{
	if (rand() %101 <= forkprob && get_runningtime() > 0) // generate fork probabilty
		return true;
	return false;
}
int process::get_runningtime()const
{
	return runtime;
}
int process::get_Responsetime()const
{
	return Response_Time;
}
void process::set_TT(int inc)
{
	TT += inc;
}
void process::set_CT(int inc)
{
	CT += inc;
}
void process::set_AT(int inc)
{
	AT += inc;
}
void process::set_PID(int inc)
{
	PID += inc;
}
void process::incrementcurrentio()
{
	current_IOR++;
}
std::ostream& operator<<(std::ostream& os, const process& ptr)
{
	os << ptr.get_PID();
	return os;
}
void process::incrementRuntime()
{
	runtime++;
}
int process::getwaittime() const
{
	return waitingtime;
}
void process::setwaittime(int a)
{
	waitingtime = a;
}
void process::set_dontfork(bool l)
{
	dont_fork = l;
}
bool process::get_dontfork() const
{
	return dont_fork;
}
int process::get_IOID_Output() const
{
	return IOD_sum;
}
void process::set_IODoutput(int key)
{
	IOD_sum += key;
}
process* process::getparent() const
{
	return parent;
}

void process::setparent(process* pp)
{
	parent = pp;
}

void process::resetiodurtion()
{
	IO_duration = 0;
}

int process::get_AT()const
{
	return AT;
}
int process::get_PID()const
{
	return PID;
}
int process::get_CT()const
{
	return CT;
}