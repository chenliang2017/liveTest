// This is a test for basic objects, such as TaskSchduler, UsageEnvironment and
// so on. It's just for study purpose.

#include <BasicUsageEnvironment.hh>
#include <iostream>

#include "TestConfigure.h"

using namespace std;

TaskScheduler* scheduler;
UsageEnvironment* env;

//#define STDOUT_FILENO fileno(stdout)

#define STDIN_FILENO 0 /* Standard input. */
#define STDOUT_FILENO 1 /* Standard output. */
#define STDERR_FILENO 2 /* Standard error output. */

void taskFunc(void* clientData){
	cout << "taskFunc(\"" << (char*)clientData << "\") called." << endl;
}

void handlerFunc(void* clientData, int mask){
	cout << "handlerFunc(\"" << (char*)clientData << "\", " << mask
		<< ") called." << endl;
	//scheduler->disableBackgroundHandling(STDOUT_FILENO);
}

#ifdef TEST_USAGE_ENVIRONMENT

int main(int argc, char* args[])
{
	TaskScheduler* scheduler = BasicTaskScheduler::createNew();
	UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);

	// IO event test
	//char handlerClientData[] = "IO Event";
	//scheduler->setBackgroundHandling(STDOUT_FILENO, SOCKET_WRITABLE,
	//	(TaskScheduler::BackgroundHandlerProc*)&handlerFunc, handlerClientData);

	// trigger event test
	EventTriggerId id1 = scheduler->createEventTrigger(taskFunc);
	char triggerClientData1[] = "Trigger Event 1";
	EventTriggerId id2 = scheduler->createEventTrigger(taskFunc);
	char triggerClientData2[] = "Trigger Event 2";
	EventTriggerId id3 = scheduler->createEventTrigger(taskFunc);
	char triggerClientData3[] = "Trigger Event 3";
	(*env) << "Setting Event triggers...\n";
	scheduler->triggerEvent(id2, (void*)triggerClientData2);
	scheduler->triggerEvent(id1, (void*)triggerClientData1);
	scheduler->triggerEvent(id3, (void*)triggerClientData3);
	(*env) << "Event triggers has been set.\n";

	// delayed task test
	char delayedTaskClientData1[] = "Delayed Task 1s";
	TaskToken token1 = scheduler->scheduleDelayedTask(1000000,
		taskFunc, delayedTaskClientData1);
	char delayedTaskClientData2[] = "Delayed Task 5s";
	TaskToken token2 = scheduler->scheduleDelayedTask(5000000,
		taskFunc, delayedTaskClientData2);

	// loop
	scheduler->doEventLoop();

	return 0;
}

#endif
