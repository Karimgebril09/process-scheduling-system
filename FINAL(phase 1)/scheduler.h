#pragma once
#include<iostream>
#include<ctime>
#include<fstream>//////////////////////////////
#include<cstdlib>
#include"Processor.h"
#include"FCFSprocessor.h"
#include"SJFprocessor.h"
#include"RRprocessor.h"
class ProgramInterface;
class schedular
{
private:

	int timesteps;				// current timestep
	int currentprocessor;		// index of the current Processor in the array


	Processor** Allpros;		// the array of abstract class Processor
	int fcfscount;				// fcfs Processor count
	int sjfcount;				//sjf Processor count
	int RRcount;				//rr Processor count
	int totalprocessorscount;	//sum
	int processescnt;			// Total Process count in the system
	int timeslice;
	int RTF;
	int	MAXw;
	int STL;
	int Forkprob;
	bool first_to_blk;

	LinkedQueue<process*> NEW;			////////
	LinkedQueue<process*> BLK;			// LISTS 
	LinkedQueue<process*> TRM;			/////////


	LinkedQueue<int> sigkilltime;  //sigkill time at which is removed
	LinkedQueue<int> sigkillid;    //the id of processes to be killed


	ifstream fin;						//for reading the input file
	string file;						// default fileloading name and could be changed later on
	ofstream fout;						// saving files
	string filesave;

	static int starting_id;
	static int Forked_Count;
	static int WORKST_Count;
	static int Migration_Count_RTF;
	static int Migration_Count_MaxW ;

	int System_Finishtime;
	/* UTILITY FUNCTIONS */
	void loadfrominput();								// Function Which load the data from the input file
	void Save_Output();									// Function Which save the data into the output file
	int shortestready() const;
	int longestready() const;
public:
	schedular();

	static int Killed_Count;
	
	/////////////////////
	bool ifcankill();
	int getkillid();
	void NEWtoRDY();	     //(modified)
	////////////////////  
														////////////////////
	bool timestep();									/// MAIN FUNCTION WHICH INCREMENT TIMESTEP AND OPERATE THE SYSTEM
														/////////////

	void ToBLK(process* pros);							// function inserting process to the blk.
	void ToTrm(process* pros);							// function inserting process to the termination
	//////////////////////////////////////////

	void Updateblock();									// for Phase 2 for IOIR(modified)

	//////////////////////////////////////////


	Processor* get_ProcessorPtr(int i)const;			// Given an index it returns this Processor
	int get_TotalPRos_count()const;						// Total Processor count for UI Purpose
	void printblk() const;								// Calls the BLK Print
	void printtrm() const;								// Calls the TRM print
	int getcurrentblkcount() const;						// Return the number of process in the BLK
	int getcurrenttrmcount() const;						// Return the number of process in the TRM

	///////////////////////////
	int get_shortest_pro(int start,int end,bool = false);
	int get_longest_pro(bool = false);
	int get_time_steps();
	int get_RTF();
	void migration(int,process*);
	void worksteal();
	//////////////////////////
	/// ziad 
	int getMaxw() const;
	void updatekilllist();
	// forking
	process* initiatefork(process*,int,bool);
	void gokillorphan(int);
	void Gotoblk();
	~schedular();
};