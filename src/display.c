#include "display.h"
#include "uart.h"

void InitDisplay()
{
  gpioInitModule(GPIO2);

  gpioPinMuxSetup(GPIO2, D1_PIN);
  gpioPinMuxSetup(GPIO2, D2_PIN);
  gpioPinMuxSetup(GPIO2, D3_PIN);
  gpioPinMuxSetup(GPIO3, D4_PIN);

  gpioSetDirection(GPIO2, D1_PIN, OUTPUT);
  gpioSetDirection(GPIO2, D2_PIN, OUTPUT);
  gpioSetDirection(GPIO2, D3_PIN, OUTPUT);
  gpioSetDirection(GPIO3, D4_PIN, OUTPUT);

  gpioPinMuxSetup(GPIO2, A_PIN);
  gpioPinMuxSetup(GPIO2, B_PIN);
  gpioPinMuxSetup(GPIO2, C_PIN);
  gpioPinMuxSetup(GPIO2, D_PIN);
  gpioPinMuxSetup(GPIO2, E_PIN);
  gpioPinMuxSetup(GPIO2, F_PIN);
  gpioPinMuxSetup(GPIO2, G_PIN);

  gpioSetDirection(GPIO2, A_PIN, OUTPUT);
  gpioSetDirection(GPIO2, B_PIN, OUTPUT);
  gpioSetDirection(GPIO2, C_PIN, OUTPUT);
  gpioSetDirection(GPIO2, D_PIN, OUTPUT);
  gpioSetDirection(GPIO2, E_PIN, OUTPUT);
  gpioSetDirection(GPIO2, F_PIN, OUTPUT);
  gpioSetDirection(GPIO2, G_PIN, OUTPUT);
}

Digit getDisplayCode(char input)
{
  Digit digit;
  digit.A = 0;
  digit.B = 0;
  digit.C = 0;
  digit.D = 0;
  digit.E = 0;
  digit.F = 0;
  digit.G = 0;

  switch (input)
  {
  case 'X':
    digit.F = 1;
    digit.G = 1;
    digit.E = 1;
    digit.C = 1;
    digit.B = 1;
    break;
  case '-':
    digit.G = 1;
    break;
  case '0':
    digit.A = 1;
    digit.B = 1;
    digit.C = 1;
    digit.D = 1;
    digit.E = 1;
    digit.F = 1;
    break;
  case '1':
    digit.B = 1;
    digit.C = 1;
    break;
  case '2':
    digit.A = 1;
    digit.B = 1;
    digit.D = 1;
    digit.E = 1;
    digit.G = 1;
    break;
  case '3':
    digit.A = 1;
    digit.B = 1;
    digit.C = 1;
    digit.D = 1;
    digit.G = 1;
    break;
  case '4':
    digit.B = 1;
    digit.C = 1;
    digit.F = 1;
    digit.G = 1;
    break;
  case '5':
    digit.A = 1;
    digit.C = 1;
    digit.D = 1;
    digit.F = 1;
    digit.G = 1;
    break;
  case '6':
    digit.A = 1;
    digit.C = 1;
    digit.D = 1;
    digit.E = 1;
    digit.F = 1;
    digit.G = 1;
    break;
  case '7':
    digit.A = 1;
    digit.B = 1;
    digit.C = 1;
    break;
  case '8':
    digit.A = 1;
    digit.B = 1;
    digit.C = 1;
    digit.D = 1;
    digit.E = 1;
    digit.F = 1;
    digit.G = 1;
    break;
  case '9':
    digit.A = 1;
    digit.B = 1;
    digit.C = 1;
    digit.D = 1;
    digit.F = 1;
    digit.G = 1;
    break;
  }

  return digit;
}

void printOnDisplay(Display display, char input)
{
  Digit code = getDisplayCode(input);

  pinLevel d1_enabled = display != D1 ? HIGH : LOW;
  pinLevel d2_enabled = display != D2 ? HIGH : LOW;
  pinLevel d3_enabled = display != D3 ? HIGH : LOW;
  pinLevel d4_enabled = display != D4 ? HIGH : LOW;

  gpioSetPinValue(GPIO2, D1_PIN, d1_enabled);
  gpioSetPinValue(GPIO2, D2_PIN, d2_enabled);
  gpioSetPinValue(GPIO2, D3_PIN, d3_enabled);
  gpioSetPinValue(GPIO2, D4_PIN, d4_enabled);

  pinLevel a_enabled = code.A ? HIGH : LOW;
  pinLevel b_enabled = code.B ? HIGH : LOW;
  pinLevel c_enabled = code.C ? HIGH : LOW;
  pinLevel d_enabled = code.D ? HIGH : LOW;
  pinLevel e_enabled = code.E ? HIGH : LOW;
  pinLevel f_enabled = code.F ? HIGH : LOW;
  pinLevel g_enabled = code.G ? HIGH : LOW;

  gpioSetPinValue(GPIO2, A_PIN, a_enabled);
  gpioSetPinValue(GPIO2, B_PIN, b_enabled);
  gpioSetPinValue(GPIO2, C_PIN, c_enabled);
  gpioSetPinValue(GPIO2, D_PIN, d_enabled);
  gpioSetPinValue(GPIO2, E_PIN, e_enabled);
  gpioSetPinValue(GPIO2, F_PIN, f_enabled);
  gpioSetPinValue(GPIO2, G_PIN, g_enabled);
}