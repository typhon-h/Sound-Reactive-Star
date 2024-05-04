/** @file Interactive-lights.ino
 * @author Leonardo Bolstad
 * @date 04 Oct 2023
 * @brief Main File
 */
// Include necessary header files
#include "microphone.h" // Microphone-related functions and variables
#include "ir.h"         // Infrared (IR) module functions and variables
#include "led.h"        // LED-related functions and variables
#include "tasks.h"      // Task-related functions and variables

// Task Scheduler
#include <TaskScheduler.h>
#include <TaskSchedulerDeclarations.h>
Scheduler schedule;

void setup()
{
  delay(3000);
  // Serial.begin(115200); // Initialize serial communication

  // // Set up the LED
  led_setup();

  // // Set up the IR
  ir_setup();

  // Set up the microphone
  microphone_setup();

  // Set up the scheduler
  schedule.init();
  schedule.addTask(ir_task);
  schedule.addTask(led_task);
  schedule.addTask(microphone_sample_task);

  led_task.enable();
  microphone_sample_task.enable();

  ir_task.enable();
}

void loop()
{
  schedule.execute(); // Enable task scheduler
}
