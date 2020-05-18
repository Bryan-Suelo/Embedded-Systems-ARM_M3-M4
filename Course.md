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
1. **Privileged Access Level** (PAL)
* Code has full access to all the processor specific resources
* Full access to restricted registers
* By default code will run in PAL
* Handler mode execution is always with PAL.
2. **Non-Privileged Access Level** (NPAL)
* Code may not have access to all the processor specific resources

When the processor is in **thread mode**, it is possible to move procesor into **NPAL**. Once switched in thread mode, then it is not possible to come back to PAL unless you change the processor operational mode to **handler mode**.

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

## Section 5: ARM GCC inline assembly coding
### C variable and inline assembly
Move the content of 'C' variable data to ARM register R0
Move te content of the CONTROL register to the 'C' variable 'control reg.'

General form of an inline assembler statement:
```__asm volatile (code:output operand list: input operand list: clobber list);```

_volatile_ - Volatile attribute to the asm statement to instruct the compiler not to optimize the assembler code.
_code_ - Assembly mnemonic defined as single string
_output operand list_ - List of output operands, separated by commas.
_input operand list_ - List of input operands, separated by commas.
_clobber list_ - Mainly used to tell the compiler about modifications done by the assembler code.

Examples of use:
```
__asm volatile("MOV R0,R1");
__asm volatile("MOV R0,R1":::);
```
#### Input/ouptut operands and constraint string
Input/output operand format:
```<Constraint string> (<C expression>)```

Constraint string = constraint character + constraint modifier

Example:
```__asm volatile ("MOV R0,%0"::"r"(val));```

##### Exercises
[Cortex-M4 Devices Generic User Guide](http://infocenter.arm.com/help/topic/com.arm.doc.dui0553b/DUI0553.pdf)
> 3.5 General data processing intructions
3.12 Miscellaneous instructions

1. Load 2 values from memory, add them and store the result back to the memory using inline assembly statements.
```c
int main(void)
{
    __asm volatile("LDR R1,=#0x20001000");
    __asm volatile("LDR R2,=#0x20001004");
    __asm volatile("LDR R0,[R1]");
    __asm volatile("LDR R1,[R2]");
    __asm volatile("ADD R0,R0,R1");
    __asm volatile("STR R0,[R2]");
}
```

2. Move the content of 'C' variable 'val' to ARM register R0.
```c
int main(void)
{
    int val = 50;
    __asm volatile("MOV R0,%0"::"r"(val):);
}
```

3. Move the content of CONTROL register to 'C' variable "control reg".
```c
int main(void)
{
    int control_reg;
    __asm volatile("MRS %0,CONTROL": "=r"(control_reg));
}
```

4. Copy the content 'C' variable "var1" to "var2".
```c
int main(void)
{
    int var1 = 10;
    int var2;
    __asm volatile("MOV %0,%1":"=r"(var2):"r"(var1));
}
```

5. Copy the contents of a pointer into another variable.
```c
int main(void)
{
    int p1, *p2;
    p1 = (int*)0x20000008;
    __asm volatile("LDR 0%,[%1]":"=r"(p1):"r"(p2));
}
```

## Section 6: Reset sequence of the processor
### Main features
1. When reset the processor, The **PC register** is loaded with the value ```0x000_0000```
2. The processor reads the value @ memory location ```0x000_0000``` into _MSP_.
    - MSP value = ```0x000_0000```
    - MSP stands for Main Stack Pointer register
    - The processor first initializes the Stack Pointer
3. After that, processor reads the value @ memory location ```0x000_0000``` into _PC_.
    - The value is actually the address of the _Reset Handler_.
4. _PC_ jumps to _Reset Handler_.    
5. A _Reset Handler_ is just a C or assembly function written by you to carry out any initialization required.
6. From _Reset Handler_ you can call your main() function of the application

## Section 7: Access level and T bit
[Cortex-M4 Devices Generic User Guide](http://infocenter.arm.com/help/topic/com.arm.doc.dui0553b/DUI0553.pdf)
> Page 22 CONTROL Register
3.5 General data processing instructions

### Importance of T-bit of the ESPR
1. Various ARM processors support ARM-Thumb interworking, that means the ability to switch between ARM and Thumb state.
2. The processor must be in ARM state to execute instructions which are from ARM ISA and the processor must be in Thumb state to execute instructions of _Thumb ISA_.
3. If _T bit_ of the _ESPR_ is set(1), processor thinks that the next instruction which it is about to execute is from _Thumb ISA_.
4. If  _T bit_ of the _ESPR_ is reset(0), processor thinks that the next instruction which it is about to execute is from _ARM ISA_.
5. The _Cortex Mx_ processor does not support the _ARM state_.
    * Hence, the value of _T bit_ must always be 1. 
    * Failing to mantain this, is illegal and this will result in the _Usage fault_ exception.
6. The _LSB_ (bit 0) of the _Program Counter_(PC) is linked to this _T bit_.
    * When you load a value or an address into _PC_ the bit 0 of the value is loaded into the _T bit_.
7. Hence, any address you place in _PC_ must have its 0th bit as 1.
    * This is usually taken care by compilers and programmers not need to worry about this.
8. This is the reason why you see all vector addresses are incremented by 1 in the vector table.

## Section 8: Memory map and bus interfaces of ARM Cortex Mx Processor
### Memory Map
* Explains mapping of different peripherals registers and memories in the processor addressable memory location range.
* The processor, addressable memory location range, depends upon the size of the address bus.
* The mapping of different regions in the addressable memory location range is called _memory map_.
    #### CODE region
    - 512 MB size
    - This is the region where the MCU vendors should connect CODE memory.
    - Different type of Code memories are: embedded flash, ROM, OTP, EEPROM, etc..
    - Processor by default fetches vector table information from this region right after reset.
    #### SRAM region
    - Primarly for connecting SRAM, mostly on-chip SRAM.
    - The first 1 MB of this region is bit addressable.
    - Execute program code from this region is available.
    #### Peripherals region
    - 512 MB size.
    - Used mostly for on-chip peripherals
    - The first 1 MB of this region is bit addressable.
    - This is an eXecute Never(XN) region
    - Trying to execute code from this region will trigger fault exeption.
    #### External RAM region
    - 1 GB size
    - this region is intended for either on-chip or off-chip memory
    - Execute code from this region is available
    - E.g, connecting external SDRAM
    #### External device region
    - 1 GB size
    - This region is intended for external devices and/or shared memory
    - This is an eXecute Never(XN) region
    #### Private Peripheral Bus region
    - 511 MB size
    - This region includes the NVIC, System timer, and system control block
    - This is an eXecute Never(XN) region.

### Bus Protocols and interfaces
* In the Cortex Mx processors the bus interfaces are based on _**A**dvanced **M**icrocontroller **B**us **A**rchitecture_ (AMBA) specification.
* AMBA is a specification designed by ARM which governs standard for on-chip communication inside the system on-chip
* AMBA specification supports several bus protocols
    - AHB lite (AMBA High-performance Bus)
    - APB (AMBA Peripheral Bus)
    
    #### AHB & APB
    - AHB lite bus is mainly used for the main bus interfaces
    - APB bus is used for PPB (_Private Peripheral Bus_) access and some on-chip peripheral access using an AHB-APB bridge
    - AHB lite bus majorly used for high-speed communication with peripherals that demand high operation speed
    - APB is used for low speed communication compared to AHB. Most of the peripherals which donot require high operation speed are connected to this bus.

### Bit banding
[Cortex-M4 Devices Generic User Guide](http://infocenter.arm.com/help/topic/com.arm.doc.dui0553b/DUI0553.pdf)
> 2.2 Memory model

* Capability to address a single bit of a memory address
* MCU manufactures supports it or many not support this feature. Refer to the reference manual.
* The regions for SRAM and peripherals include optional
* Bit-banding provides atomic operations to bit data

    #### Calculate alias address
    ``` Alias_address = alias_base + (32 * (bit_band_memory_addr - bit_band_base)) + bit * 4 ```


