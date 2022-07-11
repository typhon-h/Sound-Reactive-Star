#include "ir.h"

static bool power_on = true;

void ir_setup()
{
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);
}

void ir_poll()
{
  if (IrReceiver.decode())
  {
    if (IrReceiver.decodedIRData.protocol == 8) // NEC
    {
      ir_run_command(IrReceiver.decodedIRData.command);
    }
    IrReceiver.resume();
  }
}

void ir_run_command(int command)
{
  if (!power_on && command == 0x03)
  { // 0x03: Power
    power_on = true;
  }
  else
  { // Only register other buttons if power on
    switch (command)
    {
    case 0x00:
      break;

    case 0x01:
      break;

    case 0x02:
      break;

    case 0x03: // Power
      power_on = false;
      break;

    case 0x04:
      break;

    case 0x05:
      break;

    case 0x06:
      break;

    case 0x07:
      break;

    case 0x08:
      break;

    case 0x09:
      break;

    case 0x0A:
      break;

    case 0x0B:
      break;

    case 0x0C:
      break;

    case 0x0D:
      break;

    case 0x0E:
      break;

    case 0x0F:
      break;

    case 0x10:
      break;

    case 0x11:
      break;

    case 0x12:
      break;

    case 0x13:
      break;

    case 0x14:
      break;

    case 0x15:
      break;

    case 0x16:
      break;

    case 0x17:
      break;

    default:
      break;
    }
  }
}