/** @file ir.ino
 @author Leonardo Bolstad
 @date 04 Oct 2023
 @brief code regarding ir remote commands
*/
#include "ir.h"
#include "led.h"
#include "tasks.h"

#define IR_DEBOUNCE_MS 600

/**
 * @brief Initializes IR receiver
 *
 */
void ir_setup()
{
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);
}

/**
 * @brief Polls IR receiver for incoming command
 *
 */
void ir_poll()
{
  static long lastInput = 0;
  long currentInput = millis();

  if ((currentInput - lastInput) < IR_DEBOUNCE_MS)
  {
    return;
  }

  if (IrReceiver.decode())
  {
    if (IrReceiver.decodedIRData.protocol == NEC) // NEC
    {
      Serial.println(IrReceiver.decodedIRData.command);
      ir_run_command(IrReceiver.decodedIRData.command);
      lastInput = currentInput;
    }
    IrReceiver.resume();
  }
}

/**
 * @brief Processes IR command and executes mapped task
 *
 * @param command command to interpret
 */
void ir_run_command(int command)
{
  // Only register other buttons if power on
  switch (command)
  {
  case 0x00:
    rotate_task.disable();
    break;

  case 0x01:
    rotate_task.enable();
    break;

  case 0x02: // Power
    power_on = false;
    rotate_task.disable();
    led_task.disable();
    microphone_task.disable();
    led_off();
    Serial.println("Power off");
    break;

  case 0x03:
    power_on = true;
    // code disable that disables all three of the tasks
    led_task.enable();
    microphone_task.enable();
    Serial.println("Power on");
    break;

  case 0x04:
    current_hue = 0; // Red
    step_hue = false;
    shuffle = false;

    break;

  case 0x05:
    current_hue = 85; // determines colour in this case Green
    step_hue = false;
    shuffle = false;

    break;

  case 0x06:
    current_hue = 170; // Blue
    step_hue = false;
    shuffle = false;

    break;

  case 0x07:
    current_hue = 10;
    step_hue = true;
    shuffle = false;
    break;

  case 0x08:
    current_hue = 17;
    step_hue = false;
    shuffle = false;

    break;

  case 0x09:
    current_hue = 102;
    step_hue = false;
    shuffle = false;

    break;

  case 0x0A:
    current_hue = 187;
    step_hue = false;
    shuffle = false;

    break;

  case 0x0B:
    shuffle = true;
    break;

  case 0x0C:
    current_hue = 34;
    step_hue = false;
    shuffle = false;

    break;

  case 0x0D:
    current_hue = 119;
    step_hue = false;
    shuffle = false;

    break;

  case 0x0E:
    current_hue = 204;
    step_hue = false;
    shuffle = false;

    break;

  case 0x0F:
    break;

  case 0x10:
    current_hue = 51;
    step_hue = false;
    shuffle = false;

    break;

  case 0x11:
    current_hue = 136;
    step_hue = false;
    shuffle = false;
    break;

  case 0x12:
    current_hue = 221;
    step_hue = false;
    shuffle = false;
    break;

  case 0x13:
    break;

  case 0x14:
    current_hue = 68;
    step_hue = false;
    shuffle = false;

    break;

  case 0x15:
    current_hue = 153;
    step_hue = false;
    shuffle = false;

    break;

  case 0x16:
    current_hue = 238;
    step_hue = false;
    shuffle = false;
    break;

  case 0x17: // unassigned
    break;

  default:
    break;
  }
}