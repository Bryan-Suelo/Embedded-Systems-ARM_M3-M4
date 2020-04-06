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




