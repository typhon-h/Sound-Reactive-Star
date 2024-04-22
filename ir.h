/** @file ir.h
 * @author Leonardo Bolstad
 * @date 04 Oct 2023
 * @brief variables regarding ir remote commands
 */
#ifndef IR_H
#define IR_H
#define DECODE_NEC
#include <IRremote.hpp>

#define IR_RECEIVE_PIN A1 // TODO: set pin

// Active state of the system
static bool power_on = true;

// Initializes the IR receiver
void ir_setup(void);

// Polls IR receiver for incoming commands
void ir_poll(void);

// Executes IR command
void ir_run_command(int command);

void ir_command(void);

// extern int8_t ir_command;

#endif