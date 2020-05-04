# Embedded Systems Programming on ARM Cortex-M3/M4 Processor
Repository for Udemy course : Embedded System Programming on ARM Cortex Mx.

## Introductioon
### Motivation to Learn Cortex Family of Processors
#### Motivation to learn ARM-Cortex-M Processors
* It's a 32 bit processor will boost the computational performance.
* Cusomizable processor to inlcude Floating Point unit, DSP unit, MPU, etc.
* Supports 240 external interrupts.
* RTOS friendly. Provides operational modes and access configurations.
### Processor core vs Processor
#### Cortex-M4 processor
[Technical Reference Manual Cortex-M4](http://infocenter.arm.com/help/topic/com.arm.doc.100166_0001_00_en/arm_cortexm4_processor_trm_100166_0001_00_en.pdf)
**Functional Description** Section 2
* Core consists of ALU where data computation takes place and result will be generated.
* It has the logic to decode and execute an instruction.
* It has many registers to store and manipulate data.
* It has a pipe line engine to boost the intruction execution.
* It consist of hardware multiplication and division engine
* Address generation unit

## Section 7: ARM Cortex Mx Processor: Architecture Details
### Operational modes of the Cortex Mx Processor
Processor gives 2 operational modes:
* **Thread mode**
All your applications code will execute under "**Tread mode**" of the processor. This is also called as "**User mode**".
* **Handler mode**
All the exception handlers or interrupt handlers will run under the "**Handler mode**" of the processor.

* Processor always starts with **Tread mode**.
* Whenever the core meets the system exception or any external interrupts then the core will change its mode to **Handler mode** in order to service the ISR associated with that system exception or the interrupt.

### Access levels of the Cortex Mx Processor
Processor offers 2 access levels:
* **Privileged access level** (**PAL**)
If your code is running with PAL, then your code has full access to all the processor specific resources and restricted registers.
* **Non-Privileged access level** (**NPAL**)
If your code is running with NPAL, then your code may not have access to some of the restricted registers of the processor.

By default your code will run in **PAL**. When the processor is in **Tread mode**, it is possible to move processor in to **NPAL**. Once you move out of the **PAL** to **NPAL** being in thread mode, then it is not possible to come to **PAL** unless you change the processor operational mode to **Handler mode**.

**Handler mode** code execution is always with **PAL**.

Use the **Control register** of the processor if you want to switch between the access levels.

### Core Registers
**Stack Pointer (SP)**
Register R13. In Thread mode, bit[1] of the CONTROL register
indicates the stack pointer to use:
• 0 = Main Stack Pointer (MSP). This is the reset value.
• 1 = Process Stack Pointer (PSP).

On reset, the processor loads the MSP with the value from address 0x0000_0000.

**Link Register**
Register R14. It stores the return information for subroutines, function
calls, and exceptions. On reset, the processor sets the LR value to 0xFFFFFFFF.

**Program Counter**
The Program Counter (PC) is register R15. It contains the current program address. On reset,
the processor loads the PC with the value of the reset vector, which is at address 0x00000004.
Bit[0] of the value is loaded into the EPSR T-bit at reset and must be 1.

**Program Status Register**
The Program Status Register (PSR) combines:
• **Application Program Status Register** (**APSR**)
Contains the current state of the condition flags from previous instruction executions.
• **Interrupt Program Status Register** (**IPSR**)
Contains the exception type number of the current Interrupt Service Routine (ISR).
• **Execution Program Status Register** (**EPSR**).
1) Various ARM processors support interworking, that means the ability to switch between ARM and Thumb instruction sets
2) If **T** bit is set, then processor thinks that the next instruction which it about to execute is from **Thumb instruction set**
3) If **T** bit is reset (0), then processor thinks that the next instruction which it about to execute is from **ARM instruction set**
4) The cortex M processors do not support **ARM instruction set**. Hence, the value of **T** bit must alwalys be 1. Failing to maintain this will result in **Usage fault exception**
5) The bit [0] of the **Program counter** ins linked to this T bit
6) Hence, any address you place in PC must has its 0th bit as 1. this is usually taken care by the compiler and programmers need not to worry most of the time.

**Priority Mask Register**
The PRIMASK register prevents activation of all exceptions with configurable priority.
* If you make it as 1, then all the exceptions will be disabled.
* If you make it as 0, then all the exceptions will be enabled.

**Fault Mask Register**
The FAULTMASK register prevents activation of all exceptions except for Non-Maskable Interrupt (NMI).

### Processor Reset Sequence
1. When you reset the processor, the pc is loaded with the value 0x0000_0000
2. Then, the processor reads the value @ memory location 0x0000_0000 into MSP.
MSP = value@0x0000_0000
MSP is a Main Stack Pointer register
That means, processor first initializes the Stack pointer
3. After that, processor reads the value @ memory location 0x0000_0004 into the PC. 
4. That value is actually address of the reset handler.
5. PC jumps to the reset handler.
6. A reset handler is just a C or assembly function written by you to carry out any initializations required.
7. From reset handler you call your main() function of the application.

## Section 8: Memory System Architecture
### Memory System Features
* All Cortex-M processors have 32 bit memory addressing, as a result there is 4GB of addressable memory space.
* The memory is one unified space which is shared by code space, data space and peripheral space.
* Harvad bus architecture. It means concurrent instruction and data accesses using multiple bus interfaces.
* Support both little endian and big endian memory systems.
* Support for unaligned data transfers.
* Bit addressable memory space(bit-banding).
* MPU (Memory Protection Unit) support.

**CODE region**
* Range: from 0x0000_0000 to 0x1FFF_FFFF
* Size: 512MB
* Vector table
Holds the initial stack value and the addresses of various exception handlers.

**SRAM region**
* Range: from 0x2000_0000 to 0x3FFF_FFFF
* Size: 512MB
* Bit-Band region
The first 1MB of this region is bit addressible.
Bit band alias memory addresses are used to address the each single bit of bit band memory region.
You can also execu tecode from SRAM.

**Peripherals region**
* Range: from 0x4000_0000 to 0x5FFF_FFFF
* Size: 512MB
* Used for ADC, RTC, Serial Protocols, etc.
* Bit-Band region
The first 1MB of the memory region is bit addressible

**External RAM region**
* Range: from 0x6000_000 to 0x9FFF_FFF
* This region is intended for either on-chip or off-chip
* You can execute code in this region

**External Device region**
* Range: from 0xA000_0000 to 0xDFFF_FFFF
* This region is intended for external devices and/or shared memory
* It is a non-executable region

### Bus Protocols 
* **AHB Lite** (Main System Bus)
**A**MBA **H**igh **P**erformance **B**us
AMBA (Advanced Microcontroller Bus Architecture) specification.
In the Cortex M3 and M4 processors, the AHB Lite protocol used for the main bus interfaces.
* **APB**
Advanced Peripheral Bus
The APB is an AMBA-compliant bus optimized for minimum power and reduced interface complexity.
APB is much simpler (power optimized) and slower bus compared to AHB.

### Aligned and Un-aligned data transfer
* **Aligned transfer**
Means the data's address value is a multiple of the data type size
* **Un-aligned data** 
Can be generated by using **__Packed** keyword in your structure definition
There is no unused memory in SRAM
When an unaligned transfer is issued by the processor they are actually converted in to multiple aligned transfers by the processor bus interface unit.
Since it is broken into several separate aligned transfers and as a result it takes more clock cycles for a single data access and might not be good in situations in which high performance is required.

How unaligned data access can result?
* Direct manipulation of pointers
* Accessing data structure with __packed attributes that result in unaligned data
* Inline assembly code 

### Bit-banding
Capability to address a single bit of a memory address.
This feature is optional
Those memory regions in memory map of the processor, whose each bit can be uniquely addressed by using dedicated address(bit-addrassable).

## Section 10: Stacks
### Banked Stack Pointers
Cortex M Processor phyiscally has 3 stack pointers:
* **SP**: Current Stack Pointer, Register 13
* **MSP**: Main Stack Pointer
* **PSP**: Process Stack Pointer

* After processor reset, by default, **MSP** will be selected as current stack pointer. That means, **SP** copies the contents of **MSP**.
* **Thread mode** can change the current stack pointer to **PSP** by configuring the CONTROL register's **SPEL** bit.
* **Handler mode** code execution will always use **MSP** as the current stack pointer. This also means that, changing the value of **SPEL** bit being in the handler mode does not make any sense. The write will be ignored.
* **MSP** will be initialized automatically by the processor after reset by reading the content of the address 0x0000_0000
* If you want to use the **PSP** then make sure that you initialize the PSP to valid stack address in your code.

### Subroutines and stack
1. When a function is called, registers are used for parameter passing.

| Register  | Input parameter        | Return Value          |
| --------- |------------------------| ----------------------|
| R0        | First input parameter  | Function return value |
| R1        | Second input parameter | Function return value if size is 64 bit |
| R2        | Third input parameter  |                       |
| R3        | Fourth input parameter |                       |

2. It's the callee function responsability to push the contents of R4-R11,R13,R14 if the function is going to change these registers.


## Section 12: System Exceptions and Interrupts
Page 34 - 2.3 Exception Model
### ARM Cortex MX: System Exceptions
**Exception** - Anything which disturbs the normal operation of the program by changing the operational mode of the processor.

#### Types of exceptions
1. System exceptions
Generated by the processor itself
2. Interrupts
Come from the external world to the processor.

Whenever the processor meets with an exception it changes the operational mode to **Handler mode**.

There are 15 system exceptions and 240 interrupts, all supported by the Cortex M Processors.
So, in total Cortex M processors support 255 exceptions. 

### ARM Cortex MX: Different system Exceptions
There is room for 15 system Exceptions
- Exception number 1 - Reset exception (System exception)
- Exception number 16 - Interrupt 1 (IRQ 1)

Only 9 implemented System exceptions - 6 reserved for future implementations

#### Exception types
- **Reset** - Invoked on power up or a warm reset. 
When reset is asserted, the operation of the processor stops, potentially at any point in an instruction. 
When reset is deasserted, execution restarts from the address provided by the reset entry in the vector table.
- **Non Maskable Interrupt** (NMI) - Signaled by a peripheral or triggered by software. It is permanently enabled and has a fixed priority of -2.
- **HardFault** - Occurs because of an error during exception processing, or because an exception cannot be managed by any other exception mechanism.
- **MemManage** - Occurs because of a memory protection related fault. Fixed memory protection constraints determines this fault.
- **BusFault** - Exception that occurs because of a memory related fault for an instruction or data memory transaction. Error detected on a bus.
- **UsageFault** - Occurs because of a fault related to instruction execution. This includes:
    * an undefined instruction
    * an ilegal unaligned access
    * invalid state on instruction execution
    * an error on exception return
- **SVCall** - Triggered by the SVC instruction. Implements System Call Interface Layer.
- **PendSV** - FreeRTOS and OSRTOS use this exception for context switching when no other exception is active  
- **SysTick** - Exception the system timer generates when it reaches zero. 

### System Exception Activation and Exception Escalation
*Cortex-M4 Devices Generic User Guide*
* 4.3 System Control Block
* 4.3.9 System Handler Control and State Register

### NVIC, IRQ numbers and Enabling/Disabling Interrupts
Nested Vector Interrupt controller (NVIC)
* It is one of the peripherals of the Cortex M Processor Core.
* Used to configure the 240 interrupts.
* Using NVIC registers you can Enable/Disable/Pend various interrupts and read the status of the active pending interrupts.
* Configure the priority and priority grouping of various interruots.
* NVIC is called as "Nested" because, it supports pre-empting a lower priority interrupt handler when higher priority interrupt arrives.

#### Enable/Disable/Pend various interrupts
Cortex M processor supports 240 interrupts.
These interrupts can be anything which are actually external to the processor core.

*Cortex-M4 Devices Generic User Guide*
* 4.2 Nested Vectored Interrupt Controller
* 4.2.2 Interrupt Set-enable Registers
* 4.2.3 Interrupt Clear-enable Registers
* 4.2.4 Interrupt Set-pending Registers
* 4.2.4 Interrupt Active Bit Registers

### Interrupt Priority register
Priority value for a particular interrupt is configured using a register called **Priority Register** (PR) whose size is 8 bits.
Processor provides 60 registers each of 32 bits. ```NVIC_IPR0-NVIC_IPR59```

### Priority grouping
- **Pre-Empt Priority** - When the processor is running interrupt handler, and another interrupt appears, **then the pre-empt priority values will be compared** and exception with higher pre-empt priority(less in number) will be allowed to run.
- **Sub Priority** - This value is used **only when two exceptions with same pre-empt priority level occur at the same time**. In this case, the exception with higher sub-priority(less in number) will be handled first.

The priority group field is a 3 bit field in the application interrupt.

### Interrupt Priority and Pre-emption
* ARM Cortex M3 and M4 processor has a 24-bit system timer called SysTick.
* The counter inside the SysTick is 24 decrement counter. 
When you start the counter by loading some value, it starts decrementing for every processor clock cycle.
* If it reaches zero, then it will raise a SysTick timer exception, and the again reloads the value and continue.
* SysTick timer can be used for time keeping, time measurement, or as an interrupt source for task that need to be executed regularly.

## Section 14: System Exceptions and Interrupts-II
### Pending Interrupt behavior
#### Single pended interrupt
Processor Operation - Thread mode
Interrupt Request - Interrupt assertion
Interrupt Pending Status - Pending bit is set
Processor Mode - Handler Mode
Interrupt Active Status bit - Interrupt active bit set
Processor Mode - Interrupt exit
Processor Operation - Interrupt active bit cleared
Processor Operation - Unstacking
Processor Operation - Thread Mode
Interrupt Pending Status - Pending bit is cleared

### Exception entry sequence
#### Exception entry sequence
1. Pending bit set - Pending register from NVIC
2. Stacking - Pushing the content of the registers in the content stack. 
3. Vector fetch - Fetching the address of the exception handler from the vector table.
4. Entry to the handler - Active bit set in the NVIC register. 
5. Clear pending status - Processor does it automatically.
6. Processor mode changed to handler mode.
7. Handler code is executing.
8. MSP will be used for any stack operations inside the handler.

#### Exception exit sequence
1. The exception return mechanism is triggered using a special return address called **```EXC_RETURN```**.
2. ```EXC_RETURN``` is generated during exception entry and is stored in the LR.
3. When ```EXC_RETURN``` is written to PC it triggers the exception return.
4. During an exception handler entry, the value of the return address(PC) is not stored in the LR as it is done during calling of a normal C function. Instead the exception mechanism stores the special value called EXC_RETURN in LR.

### Programming and configuring LED using registers
#### Steps to configure LEDs
* Enable the clock for the GPIO Port where LED is connected