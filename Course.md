# Embedded Systems Programming on ARM Cortex-M3/M4 Processor

## Section 3: Embedded Hello World
### Serial Wire Debug (SWD)
This is a two-wire protocol for accessing the ARM debug interface.
Part of ARM Debug Interface Specification v5 and is an alternative to JTAG.
The physical layer of SWD consist of two lines:
1. **SWDIO**: A bidirectional data line
2. **SWCLK**: a clock driven by the host
By using SWD interface should be able to program MCUs internal flash, you can access memory regions, add breakpoints, stop/run CPU.

## Section 4: Access Level and operation modes of the processor
### Features of Cortex Mx Processor
* Operational mode of the processor
* Access levels
* Register set of the processor
* Banked Stack design
* Exception and exception handling
* Interrupt handling
* Bus interfaces and bus matrix
* Memory architecture, bit banding, memory map
* Endianess
* Aligned and unaligned data transfer
* Bootloader and IAP

### Operational modes of the Cortex Mx processor
Processor gives 2 operational modes:
1. **Thread Mode**
* Processor by default starts in Thread mode
* All your application code will execute under this mode.
* This is also called as **User Mode**.

2. **Handler Mode**
* All the exception handlers or interrupt handlers will run under this mode. 
* Whenever the core meets the system exception or any external interrupts then the core will change its mode to handler mode.

### Access level of the processor
Process offers 2 access levels:
1. Privileged Access Level (PAL)
* Code has full access to all the processor specific resources
* Full access to restricted registers
* By default code will run in PAL
* Handler mode execution is always with PAL.
2. Non-Privileged Access Level (NPAL)
* Code may not have access to all the processor specific resources

When the processor is in **thread mode**, it is possible to move procesor into **NPAL**. Once you move out of the **PAL** to **NPAL** being in thread mode, then it is not possible to come back to PAL unless you change the processor operational mode to **handler mode**.

Use the **CONTROL** register of the processor if you want to switch between the access levels.

### Core Registers
[Cortex-M4 Devices Generic User Guide](http://infocenter.arm.com/help/topic/com.arm.doc.dui0553b/DUI0553.pdf)
> 2.1.3 Core registers

All core registers are 32 bit wide.

1. General Purpose registers
* 13 registers
* From R0 to R12
2. Stack Pointer (SP)
* R13
* Used to track the stack memory
* _Processor Stack Pointer_ (PSP)
* _Main Stack Pointer_ (MSP)
3. Link Register (LR)
* R14
* Stores the return information for subroutines, function calls, and exceptions
4. Program Counter
* R15
* Contains the current program address
* On reset processor loads the PC with the value of the reset vector.

#### Special registers
1. Program Status Register (PSR)
* Holds the state of the current execution
* It is a collection of 3 different registers:
    1. _Application Program Status Register_ (APSR)
    - Consumes 5 bits
    - Contains all the conditional flags
    2. _Interrupt Program Status Register_ (IPSR)
    - Consumes 8 bits
    - Contains the exception type number of the current _Interrupt Service Routine_ (ISR)
    3. _Execution Program Status Register_ (EPSR)
    - Consumes from bit 26 to 25, bit 24 (T bit)

2. PRIMASK
3. FAULTMASK
4. BASEPRI
5. CONTROL

### Memory mapped registers
* Every register has its address in the processor memory map
* You can access these registers in a 'C' program using address dereferencing.
* There are two groups that belongs to peripherals:
    1. _Processor specific peripherals_ (NVIC, MPU, SCB, DEBUB, etc.)
    2. _Microcontroller specific peripherals_ (RTC, I2C, TIMER, CAN, USB, etc.)
### Non-Memory mapped registers
* The registers do not have unique addresses to access them. Hence there are not part of the processor memory map.
* You cannot access these registers in a 'C' program using address dereferencing.
To access these registers, you have to use assembly instructions.