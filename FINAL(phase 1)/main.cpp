#include"ProgramInterface.h"
// The Main Function That Increment Timestep and Perform Needed Update
int main()
{
	// Main Driving Ptr and it Calls needed Functions like Loading from input and initialization
	schedular* Project = new schedular;
	// variable to terminate or continue
	bool Finished = false;
	// To Update Interface and Print
	ProgramInterface UI(Project);
	//reads curent mode
	UI.readmode();
	// Untill Termination or when the user ends
	while (!Finished)
	{
		Finished = UI.updateinterface(Project->timestep());
	}
	// System Ended
	delete Project;
	return 0;
}