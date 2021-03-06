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

## Section 9: Stack memory and placement
### Stack memory
* Stack memory is part of the main memory (Internal RAM or External RAM) reserver for temporary storage of data (transient data).
* Mainly used during function, interrupt/exception handling.
* Accessed in last in first out fashion.
* Stack memory can be accessed using _PUSH_ and _POP_ instructions or using any memory manipulation instructions (_LD_, _STR_).
* The stack is traced using a _Stack Pointer_ (SP) register. _PUSH_ and _POP_ instructions affect (decrement or increment) stack pointer register (SP, R13).

    #### Stack Memory uses
    1. The temporary storage of processor register values
    2. The temporary storage of local variables of the function
    3. During system exception or interrupt, stack memory will be used to save the context (some general purpose registers, processor status register, return address) of the currently executing code

### Stack operation models
* Full ascending stack (FA)
    - SP value is incrementing
    - SP is pointed to the last item
* Full descending stack (FD) **ARM Cortex Mx processors use this**
    - SP will decrease to lower memory address
    - SP will be pointing to the last item
* Empty ascending stak (EA)
    - SP will be always pointing to the current empty item 
    - SP value is moving to incremental address
* Empty descending stack (ED)
    - SP will be always pointing to the current empty item

    #### Stack placement
    Application should be like this:
    - Stack 
    - Unused space
    - Heap
    - Data

PUSH decrements
POP increments

### Banked stack pointers
1. Cortex Mx processor physically has 3 stack pointers:
    - SP - Stack Pointer (Register 13)
    - MSP - Main Stack Pointer
    - PSP - Process Stack Pointer
2. After processor reset, by defualt, _MSP_ will be selected as current _Stack Pointer_. That means, SP copies the contents of _MSP_.
3. _Thread mode_ can change the current stack pointer to _PSP_ by configuring the **_CONTROL_** register's **_SPEL_** bit.
4. _Handler mode_ code execution **will always use MSP as the current stack pointer**. This also means that, changing the value of _SPEL_ bit being in handler mode doesn't make any sense. The write will be ignored.
5. MSP will be initialized automatically by the processor after reset by reading the content of the address 0x0000_0000
6. If you want to use the PSP then make sure that you initialize the PSP to valid stack address in your code.

### Procedure Call Standard for the ARM architecture
Describes procedure call standard used by _Application Binary Interface_ (ABI) for ARM architecture.
#### Scope
Defines how subroutines can be separately writen, separately compiled and separately assembled to work together. It describes a contract between a calling subroutine and a called routine that defines:
    - Obligations on the caller to create  aprogram state in which the called routine may start to execute.
    - Obligations on the called routine to preserve the program state of the caller across the call.
    - The rights of the called routine to alter the program state of its caller. 
    - When a 'C' compiler compiles code for the ARM architecture, it should follow the AAPCS specification to generate code.
    - According to this standard, a 'C' function can modify the registers R0,R1...R14(LR) and PSR. It's not the responsability of the function to save these registers before any modification
    - If a function wants to make use of R4 to R11 registers, then it's the responsibility of the function to save its previous contents before modifying those registers and retrieve it back before exiting the function. 
    - R0,R1,R2,R3,R12,R14(LR) registers are called _caller save registers_, it's the responsability of the caller to save these register on stack before calling the function if those values will still be needed after the function call and retrieve it back once the called function returns. Register values that are not required after the function call don't have to be saved. 
    - R4 to R11 are called _callee saved registers_ the function or subroutine being called needs to make sure that, contents of these registers will be unaltered before exiting the function.
    - According to this standard, caller function uses R0,R1,R2,R3 registers to send input arguments to the callee function.
    - The callee function uses registers R0 and R1 to send the result back to the caller function.

### Stack activites during interruption and exception
* To allow a 'C' function to be used as an exception/interrupt handler, the exception mechanism needs to save R0 to R13,R12,LR and XPSR at exception entrance automatically and restore them at exception exit under the control of the processor hardware 
In this way, when turned to the interrupt program,all the registers would have the same value as when the interrupt entry sequence started.
#### Stack initialization
- Before reaching main
- After reaching the main function, you may again reinitialize the stack pointer
##### Tips
- Evaluate your targeted application. Decide the amount of stack that would be needed for the worst-case scenario of your application run time.
- Know your processor's stack operation model (FD, FA, ED, EA)
- Decide stack placement in the _RAM_ (middle, end, external memory)
- In many applications, there may be second stage stack init. For example, if you want to allocate stack in external _SDRAM_ then first start with internal _RAM_, in the main or startup code initialize the _SDRAM_ then change the stack pointer to point to _SDRAM_.
- If you are using the ARM Cortex Mx processor, make sure that the first location of the _vector table_ contains the initial stack address (_MSP_). The startup code of the project usually does this.
- You may also use the linker script to decide the stack, heap and other RAM area boundaries. **Startup code** usually fetches boundary information from linker scripts.
- In an RTOS scenario, the kernel code may use _MSP_ to trace its own stack and configure _PSP_ for use task's stack.

## Section 10: Exception model of ARM Cortex Mx Processor
### Exception types
[Cortex-M4 Devices Generic User Guide](http://infocenter.arm.com/help/topic/com.arm.doc.dui0553b/DUI0553.pdf)
> 2.3.2 Exception types

#### Reset 
Invoked on power up or a warm reset. When reset is asserted, the operation of the processor stops, potentially at any point in an instruction. When reset is deasserted, execution restarts from the address provided by the reset entry in the vector table. Execution restarts as privileged execution in _Thread mode_.
#### Non Maskable Interrupt (NMI)
Signalled by a peripheral or triggered by software. It is permanently enabled and has a fixed priority of -2. NMIs cannot
be:
    - masked or prevented from activation by any other exception
    - preempted by any exception other than Reset.
#### HardFault
Exception that occurs because of an error during exception processing, or because an exception cannot be managed by any other exception mechanism. HardFaults have a fixed priority of -1.
#### MemManage
Exception that occurs because of a memory protection related fault. The fixed memory protection constraints determines this fault, for both instruction and data memory transactions.This fault is always used to abort instruction accesses to _Execute Never_ (XN) memory regions.
#### BusFault
Exception that occurs because of a memory related fault for an instruction or data memory transaction. This might be from an error detected on a bus in the memory system.
#### UsageFault
Exception that occurs because of a fault related to instruction execution. This includes:
• an undefined instruction
• an illegal unaligned access
• invalid state on instruction execution
• an error on exception return.
The following can cause a _UsageFault_ when the core is configured to report them:
• an unaligned address on word and halfword memory access
• division by zero.
#### Supervisor Call (SVCall)
Exception that is triggered by the SVC instruction. In an OS environment, applications can use SVC instructions to access OS kernel functions and device drivers.
#### PendSV
Interrupt-driven request for system-level service. In an OS environment, use PendSV for context switching when no other exception is active.
#### SysTick 
Exception the system timer generates when it reaches zero. Software can also generate a SysTick exception. In an OS environment, the processor can use this exception as system tick.
#### Interrupt (IRQ) 
Exception signalled by a peripheral, or generated by a software request. All interrupts are asynchronous to instruction execution. In the system, peripherals use interrupts to communicate with the processor.

### System exception and CONTROL registers
[Cortex-M4 Devices Generic User Guide](http://infocenter.arm.com/help/topic/com.arm.doc.dui0553b/DUI0553.pdf)
> 4.1 About the Cortex-M4 peripherals
> 4.2 Nested Vectored Interrupt Controller

#### System Control Block (SCB)
Provides system implementation information, and system control. This includes configuration, control, and reporting of the system exceptions.
##### Registers
* Enable fault handlers
* Get pending status of the fault exceptions
* Trap processor for divide by zero and unaligned data access attempts
* Control sleep and sleep wakeup settings
* Configure the priority of system exceptions
* SysTick timer control and status

#### Nested Vector Interrupt Controller (NVIC)
* An implementation-defined number of interrupts, in the range 1-240 interrupts.
* A programmable priority level of 0-255 for each interrupt. * A higher level corresponds to a lower priority, so level 0 is the highest interrupt priority.
* Level and pulse detection of interrupt signals.
* Dynamic reprioritization of interrupts.
* Grouping of priority values into group priority and subpriority fields.
* Enable, disable, pend various interrupts and read the status of the active and pending interrupts
* It is called _nested_ because, it supports pre-emting a lower priority interrupt handler when higher priority interrupt arrives

## Section 11: Interrupt priority and configuration
### Priority grouping
* **Pre-Empt Priority**: When the processor is running interrupt handler, and another interrupt appears, _then the pre-empt priority values will be compared_, and interrupt with higher pre-empt priority (less in value) will be allowed to run. 
* **Sub Priority**: This value is used _only when two interrupts with the same pre-empt priority values occur at the same time_. in this case, the exception with higher sub-priority (less in value) will be handled first.

## Section 12: Exception entry and exit sequence
### Exception entry sequence
1. Pending bit set.
2. Stacking and Vector fetch
3. Entry to the handler and Active bit set
4. Clears the pending status
5. Now processor mode changed to handler mode
6. Handler code is executing
7. MSP will be used for any stack operations inside the handler

### Exception exit sequence
* In Cortex-M3/M4 processors the exception return mechanism is triggered using a special return address called ```EXC_RETURN```
* ```EXC_RETURN```is generated during exception entry and is stored in the **LR**
* When ```EXC_RETURN``` is written to PC it triggers the exception return

#### EXC_RETURN
During an exception handler entry, the value of the return address (PC) is not stored in the LR as it is done during calling of a normal C function. Instead the exception mechanism stores _the special value called **EXC_RETURN** in LR_

## Section 13: Fault handling and analysis
* Fault - Exception generated by the processor to indicate an error.
* Why fault happens?    
    - Faults happen because of programmers handling processor by violating the design rules or may be due to interfaces with which the processor deals
    - Whenever a fault happens, internal processor registers will be updated to record the type of fault, the address of instruction at which the fault happened, and if an associated exception is enabled, the exception handler will be called by the processor
    - In the exception handler programmers may implement the code to report, resolve, or recover from the fault
    - For example, if your code tries to divide a number by zero, then divide by 0, fault will be raised from the hardware, which will invoke usage fault exception handler (if enabled). In the exception handler, you can make certain decisions to get rid of the problem. like closing the task, etc.
    - most of the time, fault happens by programmer's code not adhering to processor programming guidelines
* Types of fault exceptions 
    - _Hard fault exception_ - enabled by default, non-configurable priority
    - _Usage fault exception_ - Disabled by default, configurable priority
    - _MemManage fault exception_
    - _Bus fault exception_

    Hard-fault exception can be disabled by cde using _FAULTMASK_ register

### Hard-Fault exception
Exception that occurs because of an error during exception processing, or because an exception cannot be managed by any other exception mechanism. It has 3rd highest fixed priority (-1) after reset and NMI meaning it has higher priority than any exception with configurable priority.

#### Causes
1. Escalation of configurable fault exceptions
2. Bus error returned during a vector fetch
3. Execution of break point instruction when both halt mode and debug monitor is disabled
4. Executing SVC instruction inside SVC handler

### MemManage fault exception
* Configurable fault exception. Disabled by default
* Enable this exception by configuring the processor register _System Handler Control and State Register (SHCSR)_
* When _MemManage_ fault happens _MemManage_ fault exception handler will be executed by the processor
* Priority of this fault exception is configurable

#### Causes
1. This fault exception triggers when memory access violation is detected (access permission by the processor or MPU)
2. Unpriviledged thread mode code tries to access a memory which is marked as _priviledge access only_ by the MPU.
3. Writing to memory regions which are marked as read-only by the MPU4.
4. This fault can also be triggered when trying to execute program code from peripheral memory regions. Peripheral memory regions are marked as XN(eXecute Never) regions by the processor design to avoid code injection attacks through peripherals.

### Bus fault exception
* Configurable fault exception. Disabled by default
* Enable this exception by configuring the processor register _System Handler Control and State Register (SHCSR)_
* When _Bus fault_ happens _Bus fault_ exception handler will be executed by the processor
* Priority of this fault exception is configurable

#### Causes
* Due to error response returned by the processor bus interfaces during access to memory devices
    - during instruction fetch
    - during data read or write to memory devices
* If bus error happens during vector fetch, it will be scalated to a hard fault even if bus fault exception is enabled
* Memory device sends error response when the processor bus interface tries to access invalid or restricted memory locations whic coud generate a bus fault
* When the device is not ready to accept memory transfer
* Issue encountered when played with external devices such as SDRAM connected via DRAM controllers
* Unpriviledged access to private peripheral bus

### Usage fault exception
* Configurable fault exception. Disabled by default
* Enable this exception by configuring the processor register _System Handler Control and State Register (SHCSR)_
* When _Usage fault_ happens _Usage fault_ exception handler will be executed by the processor
* Priority of this fault exception is configurable

#### Causes
* Execution of undefined instruction (Cortex M4 supports only thumb ISA, so executing any instruction outside this ISA would result in a fault)
* Executing floating point instruction keeping floating point unit disabled
Trying to switch to ARM state to execute ARM ISA instructions. The T bit of the processor decides ARM state or THUMB state. For Cortex M it should be mantained at 1. Making T bit 0 (may happen during function call using functio pointers whose 0th bit is not mantained as 1) result in a fault.
* Trying to return to thread mode when an exception/interrupt is still active
* Unaligned memory access with multiple load or multiple store instrucctions
* Attempt to divide by zero (if enabled, by default divide by zero results in zero)
* For all unaligned data access from memory (only if enabled, otherwise cortex m supports unaligned data access)

### Example 
4.3 System Control block
4.3.9  System Handler Control and State Register

#### __atribute__((naked)) functions
* This attribute tells the compiler that the function is an embedded assembly function. You can write the body of the function entirely in assembly code using ```__asm``` statements
* The compiler does not generate prologue and epilogue sequences for functions with ```__atribute__((naked))```
* Use naked functions only to wirte some assembly instructions (__asm statements). Mixing 'C' code might not work properly

### Reporting errors when fault happens
* Implement the handler which takes some remedial actions
* Implement a user callback to report errors
* Reset the microcontroller/Processor
* For an OS environment, the task that triggered the fault can terminated and restarted
* Report the fault status register and fault address register values
* Report additional information of stack frame through debug interface such as ```printf```

## Section 14: Exceptions for system-level services
* ARM Cortex Mx processors supports two important system-level service exceptions: **SVC** (SuperVisor Call) and **PendSV** (Pendable SerVice)
* Supervisory calls are typically used to request priviledged operations or access to system resources from an operating system
* **SVC** exception is mainly used in an OS environment. For example: A less priviledged user task can trigger SVC exceptions to get system-level services (like accessing device drivers, peripherals) from the kernel of the OS
* **PendSV** is mainly used in an OS environment to carry out context switching between 2 or more task when no other exceptions are active in the system.

### SuperVisor Call (SVC)
* SVC is a thumb ISA instructions which causes SVC exception
* In an RTOS scenario, user tasks can execute SVC instructions with an associated argument to make supervisory calls to seek priviledged resources from the kernel code
* Unpriviledged user tasks use the SVC instruction to change the processor mode to priviledged mode to access priviledged resources like peripherals
* SVC instruction is always used along with a number, which can be used to identify the request type by the kernel code
* The SVC handler executes right after the SVC instruction (no delay. Unless a higher priority exception arrives at the same time) 

#### Methods to trigger SVC exception
There are two ways:
1. Direct execution of SVC instruction with an immediate value
2. Setting the exception pending bit in _System Handler Control and State Register_

##### How to extract the SVC number?
1. The _**SVC instruction** has a number embedded with it_, often referred to as the SVC number
2. In the SVC handler, you should fetch the opcode of the SVC instruction and then extract the **SVC number**
3. To fetch the opcode of SVC instruction from program memory, _we should have the value of **PC (return address)**_ where the user code had interrupted while triggering the SVC exception
4. The value of the PC where the user code had interrupted is **stored in the stack** as a part of exception entry sequence by the processor.

### Pendable SerVice (PendSV)
* It is a exception type 14 and has a programmable priority level
* This exception is triggered by setting its pending status by writing to the _Interrupt Control and State Register_ of processor
* Triggering a PendSV system exception is a way of invoking the pre-emptive kernel to carry out the context switch in an OS environment
* In an OS environment, PendSV handler is _set to the lowest priority level_, and the PendSV handler carries out the context switch operation 

#### Typical use of PendSV
1. Typically this exception is triggered inside a higher priority exception handler, and it gets executed when the higher priority handler finishes
2. Using this characteristic, we can schedule the PendSV exception handler to be executed after all the other interrupt processing task are done
3. This is very useful for a context switching operation, which is crucial operation in various OS design
4. Using PendSV in context switching will be more efficient in an interrupt noisy environment
5. In an interrupt noisy environment, and we need to delay the context switching until all IRQ are executed
6. If a higher priority handler is doing time-consuming work, then the other lower priority interrupts will suffer, and systems responsiveness may reduce. This can be solved using a combination of ISR and PendSV handler

#### Offloading interrupt processing
Interrupts may be serviced in 2 halves:
1. The first half is the time critical part that needs to be executed as a part of ISR
2. the second half is called _bottom half_, is basically delayed execution where rest of the time consuming work will be done

So, PendSV can be used in these cases, to handle the second half execution by triggering it in the first half

## Section 15: Implementation of task scheduler
Round robin scheduling method - Time slices are assigned to each task in equal portions and in circular order

### What is a task?
* Task is nothing but a piece of code, or you can call it a **C function**, which _does a specific jo when it is allowed to run_ on the CPU
* A task has its own stack to create its local variables when it runs on the CPU. Also when the scheduler decides to remove a task from CPU scheduler first saves the context (state) of the task in task's private stack
* A piece of code or function is called task when it is schedulable and never loses it's state unless it is deleted permanently

### What is scheduling?
* Algorithm which takes the decision of pre-empting a running task from the CPU and takes the decision about which task should run on the CPU next
* The decision could be based on many factors such as system load, the priority of task, shared resource access, or a simple round-robin method

### What is context switching?
* Procedure of **switching out** of the currently running task from the CPU after saving the task's execution context state and switching in the next task's to run on the CPU by retrieving the past execution context or state of the task

### Systick count value
[Cortex-M4 Devices Generic User Guide](http://infocenter.arm.com/help/topic/com.arm.doc.dui0553b/DUI0553.pdf)
4.4 System timer, Systick

## Section 16: Bare metal embedded and linker scripts
### Cross Compilation
Process in which the cross-toolchain runs on the host machine(PC) and creates executales that run on different machines(ARM).

### Cross-compilation toolchains
* Tool or a cross-compilation is a collection of binaries which allows you to compile, assemble, link your applications.
* It also contains binaries to debug the application on the target
* Toolchain also comes with other binaries which help you to analyze the executables
    - dissect different sections of the executable
    - disassemble
    - extract symbol and size information
    - convert executables to other formats such as bin, ihex
    - provides 'C' standard libraries

#### Popular tool-chains
1. GNU Tools(GCC) for ARM Embedded Processors (free and open source)
2. armcc from ARM Ltd. (ships with KEIL, code restriction version, requires licensing)

#### Downloading GCC toolchain for ARM embedded processors
https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm

#### Cross tolchain
1. **Compiler, linker, assembler**
_arm-none-eabi-gcc_ - Not only does compilation, it also assembles and links object files to create to create final executable file
2. **Linker**
_arm-none-eabi-ld_
3. **Assembler**
_arm-none-eabi-as_
4. **Elf file analyzer**
_arm-none-eabi-objdump_
_arm-none-eabi-readelf_
_arm-none-eabi-nm_
5. **Format converter**
_arm-none-eabi-objcopy_

### Build process
1. **Preprocessing stage**
* All pre-processing directives of the source file will be resolved
* **main.i** is created from **main.c** (Output .i)
* Pre-processing directives such macros, #includes or the conditional compilation macros
2. **Code generation**
* Translating a source file into assembly language
* Output will be **main.s**
* higher level language code statements will be converted into processor architecural level mnemonics
3. **Assembler stage**
* All the things are done by the compiler itself. No need to invoke any other commands
* compiler is arm-none-eabi-gcc
* Assemly level mnemonics are converted into opcodes (machine codes for instructions)
* Output will be **main.o** (Relocatable object file)
* Relocatable - Processor architecture specific machine codes with no absolute address
4. **Linking stage**
* All relocatable object files will be taken up by the linker
* It will resolve all the symbols and other information
* It will merge the different sections of the .o files to create one executable
* The format of final executable is **.elf** (Executable and Linkable format)
* All relocatable object files are merged together to create one executable

Using tool **objcopy** we can use **.elf** format to create some other formats as **.bin**(binary format) and **.ihex**(Intel hex format)  

### Compilation and compiler flags
[Using the GNU Compiler Collection](https://gcc.gnu.org/onlinedocs/gcc-9.2.0/gcc.pdf)
3.2 Options Controlling the Kind of Output
3.18 Machine dependent options
3.18 ARM options

```arm-none-eabi-gcc -c -mcpu=cortex-m4 -mthumb main.c -o main.o```

### Makefile
Automate process of generating files

[Using the GNU Compiler Collection](https://gcc.gnu.org/onlinedocs/gcc-9.2.0/gcc.pdf)
3.4 Options controlling C dialect

main.o:main.c
target:dependency $@ $^ 
    $(CC) (CFLAGS) main.c -o main.o
    $(CC) (CFLAGS) $^ -o $@
    $(CC) (CFLAGS) -o $@ $^
recipe

#### Install make for windows
http://gnuwin32.sourceforge.net/packages/make.htm
http://gnuwin32.sourceforge.net/downlinks/make.php

Follow the instructions:
http://lifeofageekadmin.com/install-gnuwin32-tools-on-windows/

### Analyzing .o files (Relocatable object files)
* main.o is in elf format(Executable and linkable format)
* ELF is a standard file format for object files and executable files when you use GCC
* A file format standard describes a way of organizing various elements (data, read-only data, code, uninitialized data, etc.) of a program in different sections

File formats:
* the _Common Object File Format_ (**COFF**): Introduced by Unix System V
* _Arm Image Format_ (**AIF**): Introduced by ARM
* **SRECORD**: Introduced by Motorola

Analyze relocatable files
* _arm-none-eabi-objdump_ 
* _arm-none-eabi-objdump -h main.o_  
* _arm-none-eabi-objdump -d main.o_
* _arm-none-eabi-objdump -D main.o > main_log_
* _arm-none-eabi-objdump -s main.o_  

### different sections of the program in an ELF format
* .data - contains initialized data. Stored in RAM
* .rodata - contains Read only data. Stored in ROM
* .bss - contains data which are unitialized. Stored in RAM
* .text - this section contains code(instructions). Stored in ROM
* User defined sections - contains data/code which programmer demands to put in user defined sections. Stored in RAM or ROM
* Some special sections - added by the compiler, contains some special data. Stored in ROM

### Linker and locator
* Use the linker to merge similar sections of different object files and to resolve all undefined symbols of different object files.
* Locator (part of linker) takes the help of a linker script to understand how you wish to merge different sections and assigns mentioned addresses to different sections.

### Different data of a program and related sections
![](Files/Sections_of_data.PNG)

#### .bss and .data section
Block Started by symbol (bss)
1. All the unitialized global variables and uninitialized static variables are stored in the _.bss_ section
2. Since those variables do not have any initial values, they are not required to be stored in the _.data_ section since the _.data_ section consumes _FLASH_ space
Imagine what would happen if there is a large global uninitialized array in the program, and if that is kep in the _.data_ section, it would unnecessarily consume flash space yet carries unuseful information at all
3. _.bss_ section does not consume any _Flash_ space unlike _.data_ section
4. You must reserve RAM space for the _.bss_ section by knowing its size and initialize those memory space to zero. This is typically done in startup code
5. Linker helps you to determine the final size of the _.bss_ section. So, obtain the size information from a linker script symbols
 
### Start-up file
The importance of start-up file
1. The start-up file is responsible for setting up the right environment for the main user code to run
2. Code written in startup file runs before _main()_. So, you can say startup file calls _main()_
3. Some parts of the start-up code file is the target (processor) dependent
4. Start-up code takes care of vector table placement in code memory as required by the ARM Cortex Mx processor
5. Start-up code may also take care of stack reinitialization
6. Start-up code is responsible of _.data_, _.bss_ section initialization in main memory

#### Creating a start-up file  
1. Create a vector table for your microcontroller. Vector tables are MCU specific
    * Look for information: 
    [RM0368 Reference manual](https://www.st.com/resource/en/reference_manual/dm00096844-stm32f401xb-c-and-stm32f401xd-e-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf)
    Table 38. Vector table for STM32F401xB/CSTM32F401xD/E

    Total - 100 word addressable memory locations

    15 - system exceptions
    84 - interrupts
    1 - MSP
    100 * 4 = 400 bytes

    * Create an array to hold MSP and handlers addresses
    ```uint32_t vectors[] = { store MSP  and address of various handlers here };```
    * Instruct the compiler not to include the above array in _.data_ section but in a different user defined section
    * Add new intructions to makefile 
    * Define a section for vector table: 
    [Using the GNU Compiler Collection](https://gcc.gnu.org/onlinedocs/gcc-9.2.0/gcc.pdf)
    6.34.1 Common Variable Attributes
    * Validate section was created
    ```arm-none-eabi-objdump.exe -h stm32_startup.o```
    * Look for information:
    [RM0368 Reference manual](https://www.st.com/resource/en/reference_manual/dm00096844-stm32f401xb-c-and-stm32f401xd-e-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf)
    Table 38. Vector table
    * Add all the IRQs and System handlers

2. Write a start-up code which initilizes _.data_, _.bss_ section in _SRAM_
    *  
3. Call _main()_

### Linker scripts
1. Linker script is a text file which explains how different sections of the object files should be merged to create an output file
2. Linker and locator combination assigns unique absolute addresses to different sections of the output file by referring to address information mentioned in the linker script
3. Linker script also includes the code and data memory address and size information
4. Linker scripts are written using the GNU linker command language
5. GNU linker script has the file extension of .ld
6. User must supply linker script at the linking phase to the linker using _-T_ option

#### Linker script commands
1. ENTRY
    * This command is used to set the _Entry point address_ infomation in the header or final elf file generated
    * In our case, _Reset handler_ is the entry point into the application. The first piece of code that executes right after the processor reset
    * The debugger uses this information to locate the first function to execute
    * Not a mandatory command to use, but required when you debug the elf file using the debugger (GDB)
    * Syntax: ```Entry(symbol_name)``` ```Entry(Reset_Handler)```
2. MEMORY
    * This command allows you to describe the different memories present in the target and their start address and size information
    * The linker uses information mentioned in this command to assign addresses to merged section
    * The information is given under this command also helps the linker to calculate total code and data memory consumed so far and throw an error message if data, code, heap or stack areas cannot fit into available size
    * By using memory command, you can fine-tune various memories available in your target and allow different sections to occupy different memory
    * Typically one linker script has one memory command
    * Syntax: ```MEMORY {name(attr): ORIGIN = origin, LENGTH = len }```
        * **name** - defines name of the memory region which will be later referenced by other parts of the linker script
        * **attr** - defines the attribute list of the memory region
        
        | Attribute letter| Meaning|
        | ----------------|-------------------------------------|
        |  R              | Read-only sections                  |
        |  W              | Read and write sections             |
        |  X              | Sections containing executable code |
        |  A              | Allocated sections                  |
        |  I              | Initialized sections                |
        |  L              | Same as 'I'                         |
        |  !              | Invert the sense of any of the following attribute|

        * **ORIGIN** - defines origin address of the memory region
        * **LENGTH** - defines the length information
 3. SECTIONS
    * Used to create different output sections in the final elf executable generated
    * Important command by which you can instruct the linker how to merge the input sections to yield an output section
    * This command also controls the order in which different output sections appear in the elf file generated
    * By using SECTIONS, you also mention the placement of a section in a memory region. For example, you instruct the linker to place the _.text_ section in the _FLASH memory region_, which is described by the memory command
    * Syntax: ```SECTIONS{.text{}>(vma) AT>(lma)}```
        * **vma** - Virtual memory address
        * **lma** - Load memory address
4. Location counter (.)
    * Special linker symbol denoted by a dot '.'
    * Symbol called _location counter_ since linker automatically updates this symbol with location (address) information
    * Use location counter inside the linker script to track and define boundaries of various sections
    * You can also set location counter to any specific value while writing linker script
    * Location counter should appear only inside the _SECTIONS_ command
    * The location counter is incremented by the size of the output section

#### Linker script symbols
* A symbol is the name of an address
* A symbol declaration is not equivalent to a variable declaration what you do in your 'C' application

#### Linking and linker flags
```arm-none-eabi-gcc -nostdlib -T stm32_ls.ld *.o -o final.elf```
```arm-none-eabi-gcc-objdump.exe -h final.elf```

### Open on Chip Debugger (OpenOCD)
* Aims to provide debugging, in-systems programming, and boundary scan testing for embedded target devices
* Its free and opensource host application allows you to program, debug, and analyze your applications using GDB
* It supports various target boards based on different processor architecture
* Currently supports many types of debug adapters: USB-based, parallel port-based, and other standalone boxes that run OpenOCD internally

#### Programming adapters
* Used to get access to debug interface of the target with native protocol signaling such as SWD or JTAG since HOST does not support such interfaces
* It does protocol conversion
* Mainly debug adapter helps you to download and debug code
* Advaced debug adapters help to capture trace events such as on the fly instruction trace and profiling information

#### Steps to download the code using OpenOCD
1. Download and install OpenOCD
    * http://openocd.org/getting-openocd/
    * Unofficial binary packages
    * https://github.com/ilg-archived/openocd/releases
    * Copy address installation
    * Add address to Path in environment variables
2. Install telnet client(Putty) o GDB client
    * https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html
3. Run OpenOCD with the board configuration file
    * Add this command to makefile: ```load:
	openocd -f board/st_nucleo_f4.cfg```
4. Connect to the OpenOCD via Telnet Client or GDB client
    *  ```arm-none-eabi-gdb.exe```
    *  ```target remote localhost:3333```
    *  ```monitor reset init``` - used to differentiate between gdb command and openocd command
5. Issue commands over Telnet or GDB client to OpenOCD to download and debug the code
    * General commands: http://openocd.org/doc/html/General-Commands.html
    * ```monitor flash write_image erase final.elf```
    * ```monitor reset halt```
    * ```monitor resume``` - Start program
    * ```monitor halt`` - Stop of halt
    * ```monitor reset``` - Reset program
    * When using semihosting mode ```arm semihosting enable```
    * ```monitor mdw 0x20000000 4``` - access to specific location of code
    * ```monitor bp 0x080001c6 2``` - create a breakpoint
    * ```monitor rbp 0x080001c6``` - remove a breakpoint

### C standard library integration
#### Newlib  
* 'C' standard library implementation intended for use on embedded systems, introduced by Cygnus Solutions (Red Hat) 
* written as a _Glibc_ (GNU libc) replacement for embedded systems. It can be used with no OS (bare metal) or with a lightweight RTOS
* Newlib ships with GNU ARM toolchain installation as the default C standard library
* GNU libc (glibc) includes ISO C, POSIX, System V, and XPG interfaces. uClibc provides ISO C, POSIX and System V, while Newlib provides only ISO C
#### Newlib-nano
* Due to the increased feature set in newlib, it has become too bloated to use on the systems where the amount of memory is very much limited
* To provide a C library with a minimal memory footprint, suited for use with micro-controllers, ARM introduced newlib-nano based on newlib
#### Low level system Calls
* The idea of Newlib is to implement the hardware-independent parts of the standard C library and rely on a few low-level system calls that must be implemented with the target hardware in mind.
* When you are using newlib, you must implement the system calls appropiately to support devices, file systems and memory management.

