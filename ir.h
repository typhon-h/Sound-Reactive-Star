#ifndef IR_H
#define IR_H
#include <IRremote.hpp>

#define IR_RECEIVE_PIN 11 // TODO: set pin

// Active state of the system
static bool power_on = true;

// Initializes the IR receiver
void ir_setup(void);

// Polls IR receiver for incoming commands
void ir_poll(void);

// Executes IR command
void ir_run_command(int command);

#endif
