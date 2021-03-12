CC=arm-none-eabi-gcc
MACH=cortex-m4
CFLAGS=-c -mcpu=$(MACH) -mthumb -std=gnu11 -Wall -O0
LDFLAGS=-spec=nano.specs -T stm32_ls.ld -Wl,Map=final.map
#LDFLAGS_SH=-mcpu=$(MACH) -mthumb -mfloat-abi=soft --specs=rdimon.specs -T stm32_ls.ld -Wl,Map=final.map

all:main.o stm32_startup.o syscalls.o final.elf

main.o: main.c
	$(CC) $(CFLAGS) main.c -o main.o

stm32_startup.o: stm32_startup.c
	$(CC) $(CFLAGS) stm32_startup.c -o stm32_startup.o	

syscalls.o: syscalls.c
	$(CC) $(CFLAGS) main.c -o main.o

final.elf: main.o 
	$(CC) $(LDFLAGS) -o $a $^

clean:
	rm -rf *.o *.elf

load:
	openocd -f board/st_nucleo_f4.cfg
	