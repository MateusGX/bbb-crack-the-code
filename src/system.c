#include "bbb_regs.h"
#include "interrupt.h"
#include "gpio.h"
#include "timer.h"
#include "led_animations.h"
#include "system.h"
#include "uart.h"

#define NUM_INTERRUPTS (128u)

InterruptFunction fnRAMVectors[NUM_INTERRUPTS];

static void IntDefaultHandler(void)
{
  /* Go Back. Nothing to be done */
  ;
}

void AddIrq(unsigned char irqNum, InterruptFunction function)
{
  if (irqNum > 0)
  {
    fnRAMVectors[irqNum] = function;
  }
}

void ResetIrq(unsigned char irqNum)
{
  if (irqNum > 0)
  {
    fnRAMVectors[irqNum] = IntDefaultHandler;
  }
}

void IrqInit()
{
  for (int i = 0; i < NUM_INTERRUPTS; i++)
  {
    fnRAMVectors[i] = IntDefaultHandler;
  }
}

void printb(unsigned int num)
{
  for (int i = 32 - 1; i >= 0; i--)
  {
    if ((num >> i) & 1)
      uartPutC(UART0, '1');
    else
      uartPutC(UART0, '0');
  }
}

void IrqGlobalHandler(void)
{
  unsigned int irq_number = HWREG(BBB_INTC_SIR_IRQ) & 0x7f;
  uartPutString(UART0, "IRQ:", 4);
  printb(irq_number);
  uartPutString(UART0, "\r\n", 2);
  fnRAMVectors[irq_number]();
  HWREG(BBB_INTC_CONTROL) = 0x1;
}