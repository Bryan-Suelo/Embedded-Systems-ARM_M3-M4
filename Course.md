# Embedded Systems Programming on ARM Cortex-M3/M4 Processor

## Section 3: Embedded Hello World
### Serial Wire Debug (SWD)
This is a two-wire protocol for accessing the ARM debug interface.
Part of ARM Debug Interface Specification v5 and is an alternative to JTAG.
The physical layer of SWD consist of two lines:
* SWDIO: A bidirectional data line
* SWCLK: a clock driven by the host
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
* Processor by default starts in thread Mode
* All your application code will execute under this mode.
* This is also called as **User Mode**.

2. **Handler Mode**
* All the exception handlers or interrupt handlers will run under this mode. 
* Whenever the core meets the system exception or any external interrupts then the core will change its mode to handler mode.