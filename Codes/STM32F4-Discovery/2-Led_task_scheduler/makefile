CC=arm-none-eabi-gcc
MACH=cortex-m4
CFLAGS= -c -mcpu=$(MACH) -mthumb -std=gnu11 -Wall -O0
#CFLAGS= -c -mcpu=$(MACH) -mthumb -mfloat-abi=soft -std=gnu11 -Wall -O0
#LDFLAGS=-mcpu=$(MACH) -mthumb -mfloat-abi=soft --specs=nano.specs -T stm32_ls.ld -Wl,Map=final.map
#LDFLAGS_SH=-mcpu=$(MACH) -mthumb -mfloat-abi=soft --specs=rdimon.specs -T stm32_ls.ld -Wl,Map=final.map

all:main.o led.o stm32_startup.o 
#syscalls.o final.elf
#semi:main.o led.o stm32_startup.o final_sh.elf

main.o:main.c
	$(CC) $(CFLAGS) main.c -o main.o 

led.o:led.c
	$(CC) $(CFLAGS) $^ -o $@ 

stm32_startup.o:stm32_startup.c	
	$(CC) $(CFLAGS) -o $@ $^

#syscalls.o: syscalls.c
#	$(CC) $(CFLAGS) main.c -o main.o

#final.elf: main.o led.o stm32_startup.o syscalls.o
#	$(CC) $(LDFLAGS) -o $a $^

# When use semihosting syscalls in not necessary
#final_sh.elf: main.o led.o stm32_startup.o 
#	$(CC) $(LDFLAGS_SH) -o $a $^

clean:
	rm -rf *.o *.elf		
	