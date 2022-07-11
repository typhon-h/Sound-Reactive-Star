// Task Scheduler
#include <TaskScheduler.h>
#include <TaskSchedulerDeclarations.h>
Scheduler schedule;

void setup()
{
    schedule.init();
}

void loop()
{
    schedule.execute();
}