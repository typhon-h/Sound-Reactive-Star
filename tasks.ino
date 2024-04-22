/** @file tasks.ino
 * @author Leonardo Bolstad
 * @date 04 Oct 2023
 * @brief code and variables regarding tasks
 */
#include "microphone.h"
#include "ir.h"
#include "led.h"

// Task Scheduler
#include <TaskScheduler.h>
#include <TaskSchedulerDeclarations.h>

// Initialise tasks
#define FREQ_TO_PERIOD(FREQUENCY) (1000 * TASK_MILLISECOND) / FREQUENCY // Macro that turns
// frequency to period so all values below are in times / sec
#define IR_TASK_FREQUENCY 100
#define LED_TASK_FREQUENCY 16
#define MICROPHONE_SAMPLE_TASK_FREQUENCY 8

Task ir_task(FREQ_TO_PERIOD(IR_TASK_FREQUENCY), TASK_FOREVER, &ir_poll);
Task led_task(FREQ_TO_PERIOD(LED_TASK_FREQUENCY), TASK_FOREVER, &led_update);
Task microphone_sample_task(FREQ_TO_PERIOD(MICROPHONE_SAMPLE_TASK_FREQUENCY), TASK_FOREVER, &microphone_sample);