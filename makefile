CHAIN=$(CHAINPATH)arm-none-eabi
CFLAGS=-std=c99 -Wall -mfpu=neon -mhard-float -mcpu=cortex-a8
AFLAGS=-mfpu=neon
IPATH=-Iinc/
SRC=src/
OBJ=obj/
BIN=bin/

all: app

app: start.o main.o gpio.o uart.o timer.o pad.o control_module.o clock_module.o wdt.o interrupt.o led_animations.o system.o display.o random.o
	$(CHAIN)-ld $(OBJ)start.o $(OBJ)main.o $(OBJ)gpio.o $(OBJ)led_animations.o $(OBJ)timer.o $(OBJ)wdt.o $(OBJ)random.o $(OBJ)interrupt.o $(OBJ)display.o $(OBJ)system.o $(OBJ)uart.o $(OBJ)pad.o $(OBJ)control_module.o $(OBJ)clock_module.o -T $(SRC)memmap.ld -o $(OBJ)main.elf
	$(CHAIN)-objcopy $(OBJ)main.elf $(BIN)spl.boot -O binary
	cp $(BIN)spl.boot /tftpboot/app.bin

start.o: $(SRC)start.s
	$(CHAIN)-as $(AFLAGS) $(SRC)start.s -o $(OBJ)start.o

main.o: $(SRC)main.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)main.c -o $(OBJ)main.o
                                        
gpio.o: $(SRC)gpio.c                          
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)gpio.c -o $(OBJ)gpio.o
 
uart.o: $(SRC)uart.c                          
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)uart.c -o $(OBJ)uart.o

timer.o: $(SRC)timer.c                          
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)timer.c -o $(OBJ)timer.o
                                        
pad.o: $(SRC)pad.c                            
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)pad.c -o $(OBJ)pad.o

control_module.o: $(SRC)control_module.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)control_module.c -o $(OBJ)control_module.o

clock_module.o: $(SRC)clock_module.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)clock_module.c -o $(OBJ)clock_module.o

wdt.o: $(SRC)wdt.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)wdt.c -o $(OBJ)wdt.o

system.o: $(SRC)system.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)system.c -o $(OBJ)system.o

interrupt.o: $(SRC)interrupt.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)interrupt.c -o $(OBJ)interrupt.o

led_animations.o: $(SRC)led_animations.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)led_animations.c -o $(OBJ)led_animations.o

display.o: $(SRC)display.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)display.c -o $(OBJ)display.o

random.o: $(SRC)random.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)random.c -o $(OBJ)random.o


copy:
	cp $(BIN)spl.boot /tftpboot/app.bin

clean:
	rm -rf $(OBJ)*.o
	rm -rf $(OBJ)*.elf
	rm -rf $(BIN)*.boot

dump:
	$(CHAIN)-objdump -D $(OBJ)main.elf
