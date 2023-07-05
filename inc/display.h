#ifndef DISPLAY_H
#define DISPLAY_H

#include "gpio.h"

// GPIO 2
#define D1_PIN 6
#define D2_PIN 7
#define D3_PIN 8
#define D4_PIN 9

#define A_PIN 10
#define B_PIN 11
#define C_PIN 12
#define D_PIN 13
#define E_PIN 14
#define F_PIN 15
#define G_PIN 16

typedef enum _display
{
  D1,
  D2,
  D3,
  D4
} Display;

typedef struct _digit
{
  bool A;
  bool B;
  bool C;
  bool D;
  bool E;
  bool F;
  bool G;
} Digit;

typedef enum _button
{
  BTN1,
  BTN2,
  BTN3,
  BTN4,
  BTN_OK
} Button;

void InitDisplay();
void printOnDisplay(Display display, char input);

#endif
