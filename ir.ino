/** @file ir.ino
 @author Leonardo Bolstad
 @date 04 Oct 2023
 @brief code regarding ir remote commands
*/
#include "ir.h"
#include "led.h"
#include "microphone.h"
#include "tasks.h"

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
void ir_poll() // TODO: Remove LED sim to increase reliability in production
{
  if (IrReceiver.decode())
  {
    IrReceiver.resume();

    if (IrReceiver.decodedIRData.protocol == NEC) // NEC
    {
      // Serial.println(IrReceiver.decodedIRData.command);
      ir_run_command(IrReceiver.decodedIRData.command);
    }
  }
}

/**
 * @brief Processes IR command and executes mapped task
 *
 * @param command command to interpret
 */
void ir_run_command(int command)
{
  if (!power_on && command == 0x03)
  {
    power_on = true;
    led_task.enable();
    microphone_sample_task.enable();
    // Serial.println("Power on");
    return;
  }

  switch (command)
  {
  case 0x00:
    if (mic_sensitivity < 1 - DELTA_SENSITIVITY)
    {
      mic_sensitivity += DELTA_SENSITIVITY;
    }
    else
    {
      mic_sensitivity = 1;
    }
    break;

  case 0x01:
    if (mic_sensitivity > DELTA_SENSITIVITY)
    {
      mic_sensitivity -= DELTA_SENSITIVITY;
    }
    else
    {
      mic_sensitivity = 0;
    }
    break;

  case 0x02: // Power off
    power_on = false;
    led_task.disable();
    microphone_sample_task.disable();
    led_off();
    // Serial.println("Power off");
    break;

  case 0x03: // Power on
    break;   // Do nothing when power on

  case 0x04:
    current_hue = 0;
    break;

  case 0x05:
    current_hue = 85;
    break;

  case 0x06:
    current_hue = 170;
    break;

  case 0x07:
    active_effect = PULSE;
    break;

  case 0x08:
    current_hue = 17;
    break;

  case 0x09:
    current_hue = 102;
    break;

  case 0x0A:
    current_hue = 187;
    break;

  case 0x0B:
    active_effect = INVERSE_PULSE;
    break;

  case 0x0C:
    current_hue = 34;
    break;

  case 0x0D:
    current_hue = 119;
    break;

  case 0x0E:
    current_hue = 204;
    break;

  case 0x0F:
    active_effect = COLOR_PULSE;
    break;

  case 0x10:
    current_hue = 51;
    break;

  case 0x11:
    current_hue = 136;
    break;

  case 0x12:
    current_hue = 221;
    break;

  case 0x13:
    active_effect = BEAT;
    break;

  case 0x14:
    current_hue = 68;
    break;

  case 0x15:
    current_hue = 153;
    break;

  case 0x16:
    current_hue = 238;
    break;

  case 0x17:
    active_effect = SPIRAL;
    break;

  default:
    break;
  }
}