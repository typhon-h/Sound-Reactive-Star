/** @file tasks.h
 * @author Leonardo Bolstad
 * @date 04 Oct 2023
 * @brief tasks declerations
 */
#ifndef _TASKS_H
#define _TASKS_H

// Task Scheduler
#include <TaskScheduler.h>
#include <TaskSchedulerDeclarations.h>

extern Task ir_task;
extern Task led_task;
extern Task microphone_task;

#endif