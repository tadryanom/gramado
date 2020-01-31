/*
 * File: ports.h
 * 
 * Descrição:
 *     Header para rotinas de acesso as portas na arquitetura x86 intel.
 *
 * Versão 1.0, 2015.
 */
 
 
/*

  @todo:
  
  + Listar as portas de i/o.
  + Criar constantes padronizadas para as portas.
  
  
  ports:
  =====
  
  I/O address range	Device:
  ========================
  
  00–1F = First DMA controller 8237 A-5.
  20–3F = First Programmable Interrupt Controller, 8259A, Master.
  40–5F = Programmable Interval Timer (System Timer), 8254.
  60–6F = Keyboard, 8042.
  70–7F = Real Time Clock, NMI mask.
  80–9F = DMA Page Register, 74LS612.
     87 = DMA Channel 0.
     83 = DMA Channel 1
     81 = DMA Channel 2.
     82 = DMA Channel 3.
     8B = DMA Channel 5.
     89 = DMA Channel 6.
     8A = DMA Channel 7.
     8F	= Refresh.
  A0–BF = Second Programmable Interrupt Controller, 8259A, Slave.
  C0–DF = Second DMA controller 8237 A-5.
     F0 = Clear 80287 Busy.
     F1 = Reset 80287.
  F8–FF = Math coprocessor, 80287.
  F0–F5 = PCjr Disk Controller.
  F8–FF = Reserved for future microprocessor extensions.
  100–10F = POS Programmable Option Select (PS/2).
  110–1EF = System I/O channel.
  140–15F = Secondary SCSI host adapter.
  170–177 = Secondary Parallel ATA Disk Controller.
  1F0–1F7 = Primary Parallel ATA Hard Disk Controller.
  200–20F = Game port.
  210–217 = Expansion Unit.
  220–233 = Sound Blaster and most other sound cards.
  278–27F = Parallel port 3.
  280–29F = LCD on Wyse 2108 PC SMC Elite default factory setting.
  2B0–2DF = Alternate Enhanced Graphics Adapter (EGA) display control.
  2E8–2EF = Serial port 4.
      2E1 = GPIB/IEEE-488 Adapter 0.
  2E2–2E3 = Data acquisition.
  2F8–2FF = Serial port 2.
  300–31F = Prototype Card.
  300–31F = Novell NE1000 compatible Ethernet network interfaces.
  300–31F = AMD Am7990 Ethernet network interface, IRQ=5.
  320–323 = ST-506 and compatible hard disk drive interface.
  330–331 = MPU-401 MIDI Processing Unit on most sound cards.
  340–35F = Primary SCSI host adapter.
  370–377 = Secondary floppy disk drive controller.
  378–37F = Parallel port 2.
  380–38C = Secondary Binary Synchronous Data Link Control (SDLC) adapter.
  388–389 = AdLib Music Synthesizer Card.
  3A0–3A9 = Primary Binary Synchronous Data Link Control (SDLC) adapter.
  3B0–3BB = Monochrome Display Adapter (MDA) display control.
  3BC–3BF = Parallel port 1 on MDA card.
  3C0–3CF = Enhanced Graphics Adapter (EGA) display control.
  3D0–3DF = Color Graphics Adapter (CGA).
  3E8–3EF = Serial port 3.
  3F0–3F7 = Primary floppy disk drive controller. 
            Primary IDE controller (slave drive) (3F6–3F7h).
  3F8–3FF = Serial port 1.
  CF8–CFC = PCI configuration space.
  
  
  outras:
  ======
 
  keyboard.
  60-6F.
  
  Standard Settings for COM (Serial) Ports 1–4.
  1 - 3F8-3FFh.
  2 - 2F8-2FFh.
  3 - 3E8-3EFh.
  4 - 2E8-2EFh.
  
  Typical Parallel Port Hardware Configuration Settings:
  LPT1 378-37Fh or 3BC-38Fh.
  LPT2 278-27Fh or 378-37Fh.
  LPT3 278-27Fh.
  
  Floppy	03F0 [- 03F5].		
  Video Cards	3B0 [- 3DF].
  
  Parallel Port LPT1 0378 - 037F.
  Parallel Port LPT2 0278 - 027F.
  
  Serial Port, Com 1 03F8 - 03FF.
  Serial Port, Com 2 02F8 - 02FF.
  Serial Port, Com 3 03E8 - 03EF.
  Serial Port, Com 4 02E8 - 02EF.
  
  1st IDE drive	1F0.
  2nd IDE drive	170.
  3rd IDE drive	1E8.
  4th IDE drive	168.
  
  NE200 Ethernet Network Cards	300-31F	irq=3,11.
  Sound Blaster	220	220 - 22E.
  
  Video cards.
  0x03B0-0x03DF	The range used for the IBM VGA, its direct predecessors, 
  as well as any modern video card in legacy mode.
  
  0x03B0-0x03BB	VIA Tech CPU to AGP Controller
  0x03C0-0x03DF	VIA Tech CPU to AGP Controller.
  
  
*/ 
 

//
// Definições.
//
 
 
// IO Delay.
#define io_delay() asm("out %%al,$0x80"::);
 
 
 

//
// #todo: 
// Criar a estrutura e o objeto do tipo porta.
//
 
 
 
// goal
// in8 in16 in32
// out8 out16 out32

 
unsigned char  in8  (int port); 
unsigned short in16 (int port);
unsigned long  in32 (int port);
 
void out8  (int port, unsigned char data);
void out16 (int port, unsigned short data);
void out32 (int port, unsigned long data);

 
//
// ----------
//





void __x86_io_delay (void);
void wait_ns(int count);

unsigned long portsx86_IN ( int bits, unsigned long port );
void portsx86_OUT ( int bits, unsigned long port, unsigned long value );

//
// End.
//

