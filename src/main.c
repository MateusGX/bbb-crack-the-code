#include "bbb_regs.h"
#include "soc_AM335x.h"
#include "hw_intc.h"
#include "hw_types.h"
#include "interrupt.h"
#include "timer.h"
#include "gpio.h"
#include "wdt.h"
#include "uart.h"
#include "system.h"
#include "led_animations.h"
#include "display.h"
#include "random.h"

char game1 = 'X';
char game2 = 'X';
char game3 = 'X';
char game4 = 'X';

char response1 = '.';
char response2 = '.';
char response3 = '.';
char response4 = '.';

bool pause = false;

char *getGame(Button btn)
{
	if (btn == BTN1)
		return &game1;
	if (btn == BTN2)
		return &game2;
	if (btn == BTN3)
		return &game3;
	if (btn == BTN4)
		return &game4;
}

void gameSetup()
{
	RandomAddEntropy(DMTimerCounterGet(SOC_DMTIMER_2_REGS));
	RandomSeed();
	unsigned int num = RandomNumber();

	response1 = (num % 10) + '0';
	response2 = ((num / 10) % 10) + '0';
	response3 = ((num / 100) % 10) + '0';
	response4 = ((num / 1000) % 10) + '0';
	uartPutC(UART0, '[');
	uartPutC(UART0, response4);
	uartPutC(UART0, response3);
	uartPutC(UART0, response2);
	uartPutC(UART0, response1);
	uartPutC(UART0, ']');
	uartPutC(UART0, ' ');
}

int btnOkPress()
{
	if (pause == true)
		return 0;

	pause = true;
	if ((game1 != 'X' && game2 != 'X' && game3 != 'X' && game4 != 'X') &&
			(game1 != '-' && game2 != '-' && game3 != '-' && game4 != '-'))
	{
		if (game1 == response1 && game2 == response2 && game3 == response3 && game4 == response4)
		{
			game1 = '-';
			game2 = '-';
			game3 = '-';
			game4 = '-';
			ledOn(GPIO1, 14);
			gameSetup();
			return 0;
		}

		game1 = game1 == response1 ? '-' : 'X';
		game2 = game2 == response2 ? '-' : 'X';
		game3 = game3 == response3 ? '-' : 'X';
		game4 = game4 == response4 ? '-' : 'X';

		ledOn(GPIO1, 12);
	}
	return 0;
}

void reset()
{
	game1 = 'X';
	game2 = 'X';
	game3 = 'X';
	game4 = 'X';
}

int btnNumPress(Button btn)
{
	uartPutString(UART0, "B1\r\n", 4);

	char *game = getGame(btn);
	if (pause == true)
		return 0;

	uartPutString(UART0, "B2\r\n", 4);
	if (*game == 'X')
	{
		*game = '0';
		uartPutString(UART0, "B3\r\n", 4);
	}
	else
	{
		uartPutString(UART0, "B4\r\n", 4);

		int number = (int)*game - '0';
		if (number < 9)
		{
			number++;
		}
		else
		{
			number = 0;
		}
		*game = (char)number + '0';
	}
}

void setupGpio()
{
	gpioInitModule(GPIO1);
	gpioPinMuxSetup(GPIO1, 12);
	gpioPinMuxSetup(GPIO1, 14);
	gpioSetDirection(GPIO1, 12, OUTPUT);
	gpioSetDirection(GPIO1, 14, OUTPUT);

	gpioPinMuxSetup(GPIO2, 3); // TIMER 7 -btn
	gpioPinMuxSetup(GPIO2, 4); // TIMER 6 -btn
	gpioPinMuxSetup(GPIO2, 2); //  -btn
	gpioPinMuxSetup(GPIO2, 5); // action -btn
	gpioPinMuxSetup(GPIO2, 1); //  -btn

	gpioSetDirection(GPIO2, 3, INPUT);
	gpioSetDirection(GPIO2, 4, INPUT);
	gpioSetDirection(GPIO2, 2, INPUT);
	gpioSetDirection(GPIO2, 5, INPUT);
	gpioSetDirection(GPIO2, 1, INPUT);

	// IRQ
	configureMirClear(GPIOINT2A);
	configureIrqGpio(GPIO2, 3);
	configureIrqGpio(GPIO2, 4);
	configureIrqGpio(GPIO2, 1);
	configureIrqGpio(GPIO2, 2);
	configureIrqGpio(GPIO2, 5);
}

void gpio2Handle(void)
{
	if (checkIrqGpioPin(GPIO2, 3))
	{
		uartPutString(UART0, "IRQ -> GPIO -> 2_3\r\n", 20);
		btnNumPress(BTN2);
		clearIrqGpio(GPIO2, 3);
	}
	else if (checkIrqGpioPin(GPIO2, 4))
	{
		uartPutString(UART0, "IRQ -> GPIO -> 2_4\r\n", 20);
		btnOkPress();
		ClearScreenIrq(3000);
		clearIrqGpio(GPIO2, 4);
	}
	else if (checkIrqGpioPin(GPIO2, 1))
	{
		uartPutString(UART0, "IRQ -> GPIO -> 2_1\r\n", 20);
		btnNumPress(BTN3);
		clearIrqGpio(GPIO2, 1);
	}
	else if (checkIrqGpioPin(GPIO2, 2))
	{
		uartPutString(UART0, "IRQ -> GPIO -> 2_2\r\n", 20);
		btnNumPress(BTN4);
		clearIrqGpio(GPIO2, 2);
	}
	else if (checkIrqGpioPin(GPIO2, 5))
	{
		uartPutString(UART0, "IRQ -> GPIO -> 2_5\r\n", 20);
		btnNumPress(BTN1);
		clearIrqGpio(GPIO2, 5);
	}
}

void clearScreen()
{
	uartPutString(UART0, "SISTEMA RESET___\r\n", 18);
	reset();
	ledOff(GPIO1, 12);
	ledOff(GPIO1, 14);
	pause = false;
	HWREG(SOC_DMTIMER_4_REGS + DMTIMER_IRQSTATUS) = 0x2;
	HWREG(SOC_DMTIMER_4_REGS + DMTIMER_IRQENABLE_CLR) = 0x2;
	DMTimerDisable(SOC_DMTIMER_4_REGS);
}

int main(void)
{
	watchdogTimerDisable();
	uartPutString(UART0, "SISTEMA WDT_____\r\n", 18);

	uartInitModule(UART0, 115200, STOP1, PARITY_NONE, FLOW_OFF);
	uartPutString(UART0, "SISTEMA UART____\r\n", 18);

	DMTimerSetUp();
	uartPutString(UART0, "SISTEMA TIMER___\r\n", 18);

	InitDisplay();
	uartPutString(UART0, "SISTEMA DISPLAY_\r\n", 18);

	setupGpio();
	uartPutString(UART0, "SISTEMA GPIO____\r\n", 18);

	// IRQs
	AddIrq(TINT7, timerIrqHandler);
	AddIrq(TINT4, clearScreen);
	AddIrq(GPIOINT2A, gpio2Handle);

	gameSetup();

	uartPutString(UART0, "SISTEMA INICIADO\r\n", 18);

	while (true)
	{
		printOnDisplay(D1, game1);
		Delay(5);
		printOnDisplay(D2, game2);
		Delay(5);
		printOnDisplay(D3, game3);
		Delay(5);
		printOnDisplay(D4, game4);
		Delay(5);
	}

	return (0);
}