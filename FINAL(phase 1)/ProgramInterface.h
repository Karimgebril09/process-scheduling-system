#pragma once
#include"scheduler.h"
#include<iostream>
#include<Windows.h>
using namespace std;
enum displaymode
{
	Interactive=1,
	step_by_step,
	silent
};
class ProgramInterface
{
private:
	bool isstart;
	displaymode Dmode;
	schedular* psched;											// To call Lists prints and 
public:
	ProgramInterface(schedular* s);								// to intialize the above ptr
	void readmode();
	bool updateinterface(bool isfinished);										/// the actual print
	HANDLE h;
};