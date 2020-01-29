/*
 * File: cpu.h 
 *
 * Descri��o:
 *     Header para gerenciamento de cpus.
 *
 *     CPUs em Desktops:
 *         Intel, AMD, ARM etc ...
 *
 * #todo: boa parte do que est� aqui pode ir 
 * para /hardware/intel.h e hardware/amd.h
 *
 * 2015 - Vreated by Fred Nora.
 */
 


// Supported brands.
#define Processor_NULL  0 
#define Processor_INTEL 1
#define Processor_AMD   2

// #todo:
// More brands.
// #define Processor_ARM   3
//...



// Intel modes.
#define SI_RM 0    // Real Mode.
#define SI_PM 1    // Protected Mode.
#define SI_LM 2    // Long Mode.
#define SI_SM 3    // SMM.



  
/*
 * Structures.
 */  
  
 
// processor info
// #bugbug: This is only for Intel.

struct tagProcessor   
{

    // Object
    object_type_t objectType;
    object_class_t objectClass;


    int id;

    // Struct validation.
    int used;
    int magic;



	// #todo: 
	// Processor State ???
	
	
	//@todo: Reservar espa�o ???
	//ULONG   KernelReserved[16];         // For use by the kernel
    //ULONG   HalReserved[16];            // For use by Hal
	
	//@todo: quantas vezes um processador foi interrompido.
	// ULONG   InterruptCount;             // per precessor counts
	
	
    unsigned char Type;
	
	//@todo: ULONG MHz;
	
	
	//0x0.
	//cpuid vars.
    unsigned long Vendor[4];


	//0x1(eax)
	//unsigned long stepping;
	//model
	//family
	//processor type
	//extended model
	//extended family
	
	//0x1(ebx)
	//Brand Index
	//CLFLUSH cache line size / 8
	//APIC Physical ID
	//...
	
	//0x1(ecx)
	//0 SSE3 New Instructions
	//1-2 Reserved
	//3 MONITOR/MWAIT
	//4 CPL Qualified Debug Store
	//5-7 Reserved
	//8 Thermal Monitor 2
	//9 Reserved
	//10 L1 Context ID
	//11-31 Reserved
	
	//0x1(edx)
    //0 FPU x87 FPU on Chip
	//1 VME Virtual-8086 Mode Enhancement
	//2 DE Debugging Extensions
	//3 PSE Page Size Extensions
	//4 TSC Time Stamp Counter
	//5 MSR RDMSR and WRMSR Support
	//6 PAE Physical Address Extensions
	//7 MCE Machine Check Exception
	//8 CX8 CMPXCHG8B Inst.
    unsigned long isApic;    //O processador suporta APIC.	
	//10 n/a Reserved
	//11 SEP SYSENTER and SYSEXIT
	//12 MTRR Memory Type Range Registers
	//13 PGE PTE Global Bit
	//14 MCA Machine Check Architecture
	//15 CMOV Conditional Move/Compare Instruction
	//16 PAT Page Attribute Table
	//17 PSE Page Size Extension
	//18 PSN Processor Serial Number
	//19 CLFSH CFLUSH Instruction
	//20 n/a Reserved
	//21 DS Debug Store
	//22 ACPI Thermal Monitor and Clock Ctrl
	//23 MMX MMX Technology
	//24 FXSR FXSAVE/FXRSTOR
	//25 SSE SSE Extensions
	//26 SSE2 SSE2 Extensions
	//27 SS Self Snoop
	//28 HTT Hyper-threading technology
	//29 TM Thermal Monitor
	//30 n/a Reserved
	//31 PBE Pend. Brk. En.	
	//...
	
	
	//0x80000000 (eax) (Max feature id.).
	//Maximum meaningful value of InfoType for extended function CPUID information.
    unsigned long MaxFeatureId;
	
    //0x80000002,0x80000003,0x80000004 (Brand name)	
    unsigned long BrandName[4*3];
	
	//0x80000006(ecx) (L2 cache information (Intel)).
    unsigned long L2LineSize;
    unsigned long L2Associativity;
    unsigned long L2Cachesize;

    unsigned long L1LineSize;
    unsigned long L3LineSize;

    unsigned long L1Associativity;
    unsigned long L3Associativity;

    unsigned long L1Cachesize;
    unsigned long L3Cachesize;

	
	//0x80000008(eax??) (Virtual and physical memory sizes.)
    unsigned long Physical_Address_Size;
    unsigned long Virtual_Address_Size;
	//...
	
	
	//unsigned long MemorySize; 
	
	
	// cada processador tem sua gdt, idt, tss.
	// #todo usar ponteiro para estrutura.

    unsigned long Gdt;
    unsigned long Idt;
    unsigned long Tss;



	//...


	// Next.
    struct tagProcessor *next;
};
struct tagProcessor *processor;
 


 
/*
 ******************************************************
 * ProcessorBlock_d:
 * 
 *     Processor Block.
 *     Used in the task switch routines.
 *     It handles information about CPU and the process running
 * into the CPU.
 *     It handles the running threads count.
 */

struct ProcessorBlock_d
{
	// Object
    object_type_t objectType;
    object_class_t objectClass;	


    int id;

    int used;
    int magic;


    // Processor Info. 
    // see: cpu.h

    struct tagProcessor *processorInfo;


	//
	// ==== Process ====
	//
	
	//int running_processes;
    int processes_counter;

    struct process_d *CurrentProcess;  
    struct process_d *KernelProcess;  
    //struct process_d *CurrentProcess;  


	//
	// ==== Threads ====
	//


    // Number of total threads in this processor.
    int threads_counter;
    
    // Number of threads running in this processor.
    //int running_threads;   

    // more for other states.
    // ...


    // special threads.
    struct thread_d *CurrentThread;    
    struct thread_d *NextThread;
    struct thread_d *IdleThread;
    //...

    // ...
};




/*
 * Prototypes.
 */
 


int cpu_get_parameters (void);


//#bugbug
//Esse nome n�o � apropriado.
//@todo: Isso deve isr para include/ke.h.
int KeTestCPU (void);   


void init_cpu (void);

//
// End.
//

