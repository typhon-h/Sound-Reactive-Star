#include "ir.h"
#include "led.h"

// Task Scheduler
#include <TaskScheduler.h>
#include <TaskSchedulerDeclarations.h>
Scheduler schedule;

// Initialize tasks
#define IR_TASK_FREQUENCY 400
Task ir_task(IR_TASK_FREQUENCY, TASK_FOREVER, &ir_poll);

void setup()
{
    // Set up the LED
    led_setup();

    // Set up the IR
    ir_setup();

    // Set up the scheduler
    schedule.init();
    schedule.addTask(ir_task);

    ir_task.enable();
}

void loop()
{
    schedule.execute(); // Enable task scheduler
}