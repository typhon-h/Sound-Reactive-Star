#include "ir.h"

// Task Scheduler
#include <TaskScheduler.h>
#include <TaskSchedulerDeclarations.h>
Scheduler schedule;

// Initialize tasks
#define IR_TASK_FREQUENCY 400
Task ir_task(IR_TASK_FREQUENCY, TASK_FOREVER, &ir_poll);

void setup()
{
    ir_setup();
    schedule.init();
    schedule.addTask(ir_task);

    ir_task.enable();
}

void loop()
{
    schedule.execute();
}