#include "Processor.h"
#include "scheduler.h"
int Processor::BusyCount = 0;
Processor::Processor(schedular* ptr, string bb) : manager(ptr), TypeName(bb)
{
	IDLE = true;
	run = nullptr;
	busy_time = 0;
	total_pro_time = 0;
	current_overheatstps = 0;
	overheated = false;
}
Processor::~Processor()
{
}
Processor::Processor()
{
}
process* Processor::getrun()
{
	return run;
}
void Processor::remouverun()  // if not needed you can merge with getrun
{
	if (run)
	{
		run = nullptr;
		Processor::BusyCount--;
		IDLE = true;
	}
}

bool Processor::isIDLE()
{
	return IDLE;
}

void Processor::updatebusytime()
{
	if(!IDLE)
		busy_time++;
}

int Processor::getbusytime() const
{
	return busy_time;
}

int Processor::get_pro_time()
{
	return total_pro_time;
}

void Processor::inc_pro_time(int val)
{
	total_pro_time += val;
}

void Processor::dec_pro_time(int val)
{
	total_pro_time -= val;
}

int Processor::get_runtime()
{
	if (!run)
		return 0;
	else
		return run->get_CT() - run->get_runningtime();
}

std::string Processor::Get_Type() const
{
	return TypeName;
}

bool Processor::isthere(int a)
{
	return false;
}

void Processor::deletetree(int a)
{
}

void Processor::kill()
{
}

void Processor::checkfork()
{
}
