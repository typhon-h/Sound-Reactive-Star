#include "microphone.h"
#include "ir.h"
#include "led.h"

// Task Scheduler
#include <TaskScheduler.h>
#include <TaskSchedulerDeclarations.h>
Scheduler schedule;

// Initialize tasks
#define IR_TASK_FREQUENCY 20
#define CYCLE_TASK_FREQUENCY 50
#define MICROPHONE_TASK_FREQUENCY 100
Task ir_task(IR_TASK_FREQUENCY, TASK_FOREVER, &ir_poll);
Task led_task(CYCLE_TASK_FREQUENCY, TASK_FOREVER, &pulse_effect);
Task microphone_task(MICROPHONE_TASK_FREQUENCY, TASK_FOREVER, &microphone_sample);

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
    }
    // Set up the LED
    led_setup();

    // Set up the IR
    ir_setup();

    // Set up the microphone
    microphone_setup();

    // Set up the scheduler
    schedule.init();
    // schedule.addTask(ir_task); TODO: Temp disabled to reduce processor load
    schedule.addTask(led_task);
    schedule.addTask(microphone_task);

    led_task.enable();
    microphone_task.enable();
}

void loop()
{
    schedule.execute(); // Enable task scheduler
}
