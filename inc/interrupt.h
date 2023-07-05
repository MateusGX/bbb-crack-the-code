#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "hw_intc.h"
#include "hw_types.h"
#include "gpio.h"
#include "soc_AM335x.h"
#include "uart.h"

#define GPIOINT2A 32
#define GPIOINT1A 98
#define DEBOUNCING_TIME 0xFF

void InitIrq(void);
unsigned int getIntcMirClear(unsigned int iqrNum);
void configureMirClear(unsigned int iqrNum);
#endif
