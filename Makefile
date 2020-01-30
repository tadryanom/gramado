# Compiling Gramado on Manjaro Linux.
# BSD License
# SPDX-License-Identifier: GPL-3.0+
# History:
#     2018 - Created by Matheus Castello.
#     2019 - Fred Nora.
#     2019 - Luciano Gonçalez.


VERSION = 1
PATCHLEVEL = 28
SUBLEVEL = 0
EXTRAVERSION = -rc1
NAME = 



KERNELVERSION = $(VERSION)$(if $(PATCHLEVEL),.$(PATCHLEVEL)$(if $(SUBLEVEL),.$(SUBLEVEL)))$(EXTRAVERSION)

export KBUILD_IMAGE ?= KERNEL.BIN 


srctree := .
objtree := .
src := $(srctree)
obj := $(objtree)
 
 

# First of all, lemme discribe this documment!
# We just have two parts. The kernel and the extra stuff.

## The kernel stuff.
## Step1 KERNEL.BIN         - Creating the kernel image.
## Step2 kernel-image-link  - Linking the kernel image.
## Step3 /mnt/gramadovhd    - Creating the directory to mount the VHD.
## Step4 vhd-create         - Creating a VHD in Assembly language.
## Step5 vhd-mount          - Mounting the VHD.
## Step6 vhd-copy-files     - Copying files into the mounted VHD.
## Step7 vhd-unmount        - Unmounting the VHD.
## Step8 clean              - Deleting the object files.           

## The extra stuff.
## 1) ISO support.
## 2) HDD support.
## 3) VM support.
## 4) Serial debug support.
## 5) Clean files support.
## 6) Usage support.


# That's our default target when none is given on the command line
PHONY := _all
_all: all

	@echo "That's all!"


ARCH ?= x86


# Make variables (CC, etc...)
#AS		= as
#LD		= ld
#CC		= gcc
#AR		= ar
#OBJCOPY		= objcopy
#OBJDUMP		= objdump
#LEX		= flex
#YACC	= bison
#PERL		= perl
#PYTHON		= python
#PYTHON2		= python2
#PYTHON3		= python3
#RUBY		= ruby



#GRAMADOINCLUDE := -I include/


# Setup disk:
# For now we're using IDE disk on primary master.
# You can setup this options in:
# arch/x86/boot/bl/include/config/config.h for the boot loader and
# include/kernel/config/config.h for the kernel.



#tests
CFLAGS = -m32 \
	--std=gnu89 \
	-nodefaultlibs \
	-nostdinc \
	-nostdlib \
	-static \
	-fgnu89-inline \
	-ffreestanding \
	-fno-builtin \
	-fno-pie \
	-no-pie \
	-fleading-underscore \
	-fno-stack-protector \
	-s \
	-Werror=strict-prototypes 




##
## Defines
##

#
# DEFINES =  -DGRAMADO_VERSION=$(VERSION) \
#		-DGRAMADO_PATCHLEVEL=$(PATCHLEVEL) \
#		-DGRAMADO_SUBLEVEL=$(SUBLEVEL) \
#		-DGRAMADO_EXTRAVERSION=\"$(EXTRAVERSION)\" \
#		-DGRAMAD0_NAME=\"$(NAME)\"


##
## Objects
##

ifeq ($(ARCH),x86)

	#deveria ser headx86.o
	ENTRY_OBJECTS := boot.o main.o x86main.o 

	EXECVE_OBJECTS := pipe.o socket.o ctype.o  kstdio.o stdlib.o string.o unistd.o \
	devmgr.o \
	gde_serv.o \
	debug.o diskvol.o install.o object.o runtime.o \
	abort.o info.o io.o modules.o signal.o sm.o \
	init.o system.o \
	execve.o 
	
	HAL_OBJECTS := cpuamd.o portsx86.o syscall.o x86.o detect.o \
	hal.o 
	
	KDRIVERS_OBJECTS := ahci.o \
	ata.o atadma.o atainit.o atairq.o atapci.o hdd.o \
	channel.o network.o nicintel.o nsocket.o \
	pci.o pciinfo.o pciscan.o \
	tty.o pty.o vt.o\
	usb.o \
	video.o vsync.o screen.o xproc.o \
	i8042.o keyboard.o mouse.o ps2kbd.o ps2mouse.o ldisc.o \
	apic.o pic.o rtc.o serial.o timer.o  
	
	KSERVERS_OBJECTS := cf.o format.o fs.o read.o search.o write.o \
	cedge.o bg.o bmp.o button.o char.o createw.o dtext.o font.o grid.o \
	line.o menu.o menubar.o pixel.o rect.o sbar.o toolbar.o wm.o \
	logoff.o \
	logon.o \
	input.o output.o terminal.o \
	desktop.o room.o userenv.o usession.o \
	kgwm.o kgws.o \
	vfs.o 
	
	
	MK_OBJECTS := x86cont.o x86fault.o x86start.o \
	dispatch.o pheap.o process.o queue.o spawn.o \
	tasks.o theap.o thread.o threadi.o ts.o tstack.o \
	callout.o callfar.o ipc.o ipccore.o sem.o \
	memory.o mminfo.o mmpool.o pages.o \
	preempt.o priority.o sched.o schedi.o \
	create.o \
	mk.o 


	REQUEST_OBJECTS := request.o 
	PANIC_OBJECTS := panic.o 
	REBOOT_OBJECTS := reboot.o 
	SYS_OBJECTS := sys.o 
	
	OBJECTS := $(ENTRY_OBJECTS) \
	$(EXECVE_OBJECTS) \
	$(HAL_OBJECTS) \
	$(KDRIVERS_OBJECTS) \
	$(KSERVERS_OBJECTS) \
	$(MK_OBJECTS) \
	$(REQUEST_OBJECTS) \
	$(PANIC_OBJECTS) \
	$(REBOOT_OBJECTS) \
	$(SYS_OBJECTS)    
	
	
endif


ifeq ($(ARCH),arm)
   # NOTHING FOR NOW
endif

#
# Begin.
#

## =============================================================================
## The kernel stuff.
## Step1 KERNEL.BIN         - Creating the kernel image.
## Step2 kernel-image-link  - Linking the kernel image.
## Step3 /mnt/gramadovhd    - Creating the directory to mount the VHD.
## Step4 vhd-create         - Creating a VHD in Assembly language.
## Step5 vhd-mount          - Mounting the VHD.
## Step6 vhd-copy-files     - Copying files into the mounted VHD.
## Step7 vhd-unmount        - Unmounting the VHD.
## Step8 clean              - Deleting the object files.           

PHONY := all

all: KERNEL.BIN kernel-image-link /mnt/gramadovhd  vhd-create vhd-mount vhd-copy-files vhd-unmount clean


	#Givin permitions to run ./run hahaha
	chmod 755 ./run

	@echo "Gramado $(VERSION) $(PATCHLEVEL) $(SUBLEVEL) $(EXTRAVERSION) $(NAME) "
	@echo "Arch x86"


## Step1 KERNEL.BIN         - Creating the kernel image.
KERNEL.BIN: 
	@echo "================================="
	@echo "(Step 1) Creating the kernel image ..."

	# /entry
	
	nasm kernel/sci/arch/x86/entry/head/boot.asm -I kernel/sci/arch/x86/entry/head/ -f elf -o boot.o
	
	gcc -c kernel/sci/arch/x86/entry/x86main.c  -I include/ $(CFLAGS) -o x86main.o
	
    #main
	gcc -c kernel/main.c  -I include/ $(CFLAGS) -o main.o

	# /hal
	gcc -c kernel/sysio/hal/hal.c                -I include/ $(CFLAGS) -o hal.o
	gcc -c kernel/sysio/hal/arch/detect.c        -I include/ $(CFLAGS) -o detect.o
	gcc -c kernel/sysio/hal/arch/amd/cpuamd.c    -I include/ $(CFLAGS) -o cpuamd.o
	gcc -c kernel/sysio/hal/arch/x86/portsx86.c  -I include/ $(CFLAGS) -o portsx86.o
	gcc -c kernel/sysio/hal/arch/x86/syscall.c   -I include/ $(CFLAGS) -o syscall.o
	gcc -c kernel/sysio/hal/arch/x86/x86.c       -I include/ $(CFLAGS) -o x86.o

	# /mk
	gcc -c  kernel/sysmk/mk.c -I include/  $(CFLAGS) -o mk.o

	# /ps/arch
	gcc -c  kernel/sysmk/ps/arch/x86/x86cont.c   -I include/  $(CFLAGS) -o x86cont.o
	gcc -c  kernel/sysmk/ps/arch/x86/x86fault.c  -I include/  $(CFLAGS) -o x86fault.o
	gcc -c  kernel/sysmk/ps/arch/x86/x86start.c  -I include/  $(CFLAGS) -o x86start.o

	# /ps/action
	gcc -c  kernel/sysmk/ps/action/dispatch.c  -I include/  $(CFLAGS) -o dispatch.o
	gcc -c  kernel/sysmk/ps/action/pheap.c     -I include/  $(CFLAGS) -o pheap.o
	gcc -c  kernel/sysmk/ps/action/process.c   -I include/  $(CFLAGS) -o process.o
	gcc -c  kernel/sysmk/ps/action/queue.c     -I include/  $(CFLAGS) -o queue.o
	gcc -c  kernel/sysmk/ps/action/spawn.c     -I include/  $(CFLAGS) -o spawn.o
	gcc -c  kernel/sysmk/ps/action/tasks.c     -I include/  $(CFLAGS) -o tasks.o
	gcc -c  kernel/sysmk/ps/action/theap.c     -I include/  $(CFLAGS) -o theap.o
	gcc -c  kernel/sysmk/ps/action/thread.c    -I include/  $(CFLAGS) -o thread.o
	gcc -c  kernel/sysmk/ps/action/threadi.c   -I include/  $(CFLAGS) -o threadi.o
	gcc -c  kernel/sysmk/ps/action/ts.c        -I include/  $(CFLAGS) -o ts.o
	gcc -c  kernel/sysmk/ps/action/tstack.c    -I include/  $(CFLAGS) -o tstack.o

	# /ps/ipc
	gcc -c  kernel/sysmk/ps/ipc/callfar.c  -I include/  $(CFLAGS) -o callfar.o
	gcc -c  kernel/sysmk/ps/ipc/callout.c  -I include/  $(CFLAGS) -o callout.o
	gcc -c  kernel/sysmk/ps/ipc/ipc.c      -I include/  $(CFLAGS) -o ipc.o
	gcc -c  kernel/sysmk/ps/ipc/ipccore.c  -I include/  $(CFLAGS) -o ipccore.o
	gcc -c  kernel/sysmk/ps/ipc/sem.c      -I include/  $(CFLAGS) -o sem.o


	# /ps/mm (memory manager)

	#x86
	gcc -c  kernel/sysmk/ps/mm/x86/memory.c  -I include/ $(CFLAGS) -o memory.o
	gcc -c  kernel/sysmk/ps/mm/x86/mminfo.c  -I include/ $(CFLAGS) -o mminfo.o
	gcc -c  kernel/sysmk/ps/mm/x86/mmpool.c  -I include/ $(CFLAGS) -o mmpool.o
	gcc -c  kernel/sysmk/ps/mm/x86/pages.c   -I include/ $(CFLAGS) -o pages.o

	#arm

	# /ps/sched
	gcc -c  kernel/sysmk/ps/sched/preempt.c   -I include/ $(CFLAGS) -o preempt.o
	gcc -c  kernel/sysmk/ps/sched/priority.c  -I include/ $(CFLAGS) -o priority.o
	gcc -c  kernel/sysmk/ps/sched/sched.c     -I include/ $(CFLAGS) -o sched.o
	gcc -c  kernel/sysmk/ps/sched/schedi.c    -I include/ $(CFLAGS) -o schedi.o

	#sysmk
	gcc -c kernel/sysmk/create.c  -I include/  $(CFLAGS) -o create.o

	#kernel
	gcc -c  kernel/request.c  -I include/ $(CFLAGS) -o request.o
	gcc -c  kernel/panic.c    -I include/ $(CFLAGS) -o panic.o
	gcc -c  kernel/reboot.c   -I include/ $(CFLAGS) -o reboot.o


	# /execve
	gcc -c kernel/syssm/execve/execve.c  -I include/ $(CFLAGS) -o execve.o


	# kernel/syslib/libcore
	gcc -c kernel/syslib/libcore/pipe.c      -I include/ $(CFLAGS) -o pipe.o
	gcc -c kernel/syslib/libcore/socket.c  -I include/ $(CFLAGS) -o socket.o
	gcc -c kernel/syslib/libcore/ctype.c   -I include/ $(CFLAGS) -o ctype.o
	gcc -c kernel/syslib/libcore/kstdio.c   -I include/ $(CFLAGS) -o kstdio.o
	gcc -c kernel/syslib/libcore/stdlib.c  -I include/ $(CFLAGS) -o stdlib.o
	gcc -c kernel/syslib/libcore/string.c  -I include/ $(CFLAGS) -o string.o
	gcc -c kernel/syslib/libcore/unistd.c  -I include/ $(CFLAGS) -o unistd.o


	gcc -c kernel/sysio/kdrivers/apic.c    -I include/ $(CFLAGS) -o apic.o
	gcc -c kernel/sysio/kdrivers/pic.c     -I include/ $(CFLAGS) -o pic.o
	gcc -c kernel/sysio/kdrivers/rtc.c     -I include/ $(CFLAGS) -o rtc.o
	gcc -c kernel/sysio/kdrivers/serial.c  -I include/ $(CFLAGS) -o serial.o
	gcc -c kernel/sysio/kdrivers/timer.c   -I include/ $(CFLAGS) -o timer.o


	# kdrivers/ahci 
	# todo
	gcc -c kernel/sysio/kdrivers/ahci/ahci.c  -I include/ $(CFLAGS) -o ahci.o
	
		
	#ide support
	gcc -c kernel/sysio/kdrivers/ide/hdd.c      -I include/ $(CFLAGS) -o hdd.o
	gcc -c kernel/sysio/kdrivers/ide/ata.c      -I include/ $(CFLAGS) -o ata.o
	gcc -c kernel/sysio/kdrivers/ide/atainit.c  -I include/ $(CFLAGS) -o atainit.o
	gcc -c kernel/sysio/kdrivers/ide/atairq.c   -I include/ $(CFLAGS) -o atairq.o
	gcc -c kernel/sysio/kdrivers/ide/atapci.c   -I include/ $(CFLAGS) -o atapci.o	
	gcc -c kernel/sysio/kdrivers/ide/atadma.c   -I include/ $(CFLAGS) -o atadma.o	
	
	# kdrivers/network
	gcc -c kernel/sysio/kdrivers/network/channel.c   -I include/ $(CFLAGS) -o channel.o
	gcc -c kernel/sysio/kdrivers/network/nicintel.c  -I include/ $(CFLAGS) -o nicintel.o
	gcc -c kernel/sysio/kdrivers/network/network.c   -I include/ $(CFLAGS) -o network.o
	gcc -c kernel/sysio/kdrivers/network/nsocket.c   -I include/ $(CFLAGS) -o nsocket.o

	# kdrivers/pci
	gcc -c kernel/sysio/kdrivers/pci/pci.c      -I include/ $(CFLAGS) -o pci.o
	gcc -c kernel/sysio/kdrivers/pci/pciscan.c  -I include/ $(CFLAGS) -o pciscan.o
	gcc -c kernel/sysio/kdrivers/pci/pciinfo.c  -I include/ $(CFLAGS) -o pciinfo.o



	# kdrivers/tty
	gcc -c kernel/sysio/kdrivers/tty/console.c -I include/ $(CFLAGS) -o cedge.o
	gcc -c kernel/sysio/kdrivers/tty/tty.c   -I include/ $(CFLAGS) -o tty.o
	gcc -c kernel/sysio/kdrivers/tty/pty.c   -I include/ $(CFLAGS) -o pty.o

	gcc -c kernel/sysio/kdrivers/tty/vt.c   -I include/ $(CFLAGS) -o vt.o

	# kdrivers/usb
	gcc -c kernel/sysio/kdrivers/usb/usb.c  -I include/ $(CFLAGS) -o usb.o
	
	# kdrivers/x
	gcc -c kernel/sysio/kdrivers/x/xproc.c   -I include/ $(CFLAGS) -o xproc.o

	# kdrivers/x/video
	gcc -c kernel/sysio/kdrivers/x/video/screen.c  -I include/ $(CFLAGS) -o screen.o
	gcc -c kernel/sysio/kdrivers/x/video/video.c   -I include/ $(CFLAGS) -o video.o
	gcc -c kernel/sysio/kdrivers/x/video/vsync.c   -I include/ $(CFLAGS) -o vsync.o


	# kdrivers/x/i8042
	gcc -c kernel/sysio/kdrivers/x/i8042/i8042.c     -I include/ $(CFLAGS) -o i8042.o
	gcc -c kernel/sysio/kdrivers/x/i8042/keyboard.c  -I include/ $(CFLAGS) -o keyboard.o
	gcc -c kernel/sysio/kdrivers/x/i8042/mouse.c     -I include/ $(CFLAGS) -o mouse.o
	gcc -c kernel/sysio/kdrivers/x/i8042/ps2kbd.c    -I include/ $(CFLAGS) -o ps2kbd.o
	gcc -c kernel/sysio/kdrivers/x/i8042/ps2mouse.c  -I include/ $(CFLAGS) -o ps2mouse.o
	gcc -c kernel/sysio/kdrivers/x/i8042/ldisc.c     -I include/ $(CFLAGS) -o ldisc.o


	# devices/
	gcc -c kernel/sysio/devmgr.c  -I include/ $(CFLAGS) -o devmgr.o



	# /fs
	gcc -c kernel/sysio/kservers/fs/fs.c      -I include/ $(CFLAGS) -o fs.o
	gcc -c kernel/sysio/kservers/fs/read.c    -I include/ $(CFLAGS) -o read.o
	gcc -c kernel/sysio/kservers/fs/write.c   -I include/ $(CFLAGS) -o write.o
	gcc -c kernel/sysio/kservers/fs/cf.c      -I include/ $(CFLAGS) -o cf.o
	gcc -c kernel/sysio/kservers/fs/search.c  -I include/ $(CFLAGS) -o search.o
	gcc -c kernel/sysio/kservers/fs/format.c  -I include/ $(CFLAGS) -o format.o
	
	# /vfs
	gcc -c kernel/sysio/kservers/vfs/vfs.c  -I include/ $(CFLAGS) -o vfs.o




	# /sm
	gcc -c kernel/syssm/execve/sm/init.c    -I include/ $(CFLAGS) -o init.o
	gcc -c kernel/syssm/execve/sm/system.c  -I include/ $(CFLAGS) -o system.o
	gcc -c kernel/syssm/execve/sm/disk/diskvol.c     -I include/ $(CFLAGS) -o diskvol.o
	gcc -c kernel/syssm/execve/sm/install/install.c  -I include/ $(CFLAGS) -o install.o
	gcc -c kernel/syssm/execve/sm/rt/runtime.c       -I include/ $(CFLAGS) -o runtime.o
	gcc -c kernel/syssm/execve/sm/sys/abort.c    -I include/ $(CFLAGS) -o abort.o
	gcc -c kernel/syssm/execve/sm/sys/info.c     -I include/ $(CFLAGS) -o info.o
	gcc -c kernel/syssm/execve/sm/sys/modules.c  -I include/ $(CFLAGS) -o modules.o
	gcc -c kernel/syssm/execve/sm/sys/signal.c   -I include/ $(CFLAGS) -o signal.o
	gcc -c kernel/syssm/execve/sm/sys/sm.c       -I include/ $(CFLAGS) -o sm.o


#debug
	gcc -c kernel/syssm/debug/debug.c  -I include/ $(CFLAGS) -o debug.o

#io
	gcc -c kernel/sysio/io.c      -I include/ $(CFLAGS) -o io.o

#ob
	gcc -c kernel/syssm/ob/object.c  -I include/ $(CFLAGS) -o object.o


	# kservers/kgwm - Kernel Gramado Window Manager.

	gcc -c kernel/sysio/kservers/kgwm/kgwm.c  -I include/ $(CFLAGS) -o kgwm.o
	gcc -c kernel/sysio/kservers/kgwm/wm.c    -I include/ $(CFLAGS) -o wm.o


	# kservers/kgws - Kernel Gramado Window Server.

	gcc -c kernel/sysio/kservers/kgws/kgws/comp/bg.c       -I include/ $(CFLAGS) -o bg.o
	gcc -c kernel/sysio/kservers/kgws/kgws/comp/bmp.c      -I include/ $(CFLAGS) -o bmp.o
	gcc -c kernel/sysio/kservers/kgws/kgws/comp/button.c   -I include/ $(CFLAGS) -o button.o
	gcc -c kernel/sysio/kservers/kgws/kgws/comp/char.c     -I include/ $(CFLAGS) -o char.o
	gcc -c kernel/sysio/kservers/kgws/kgws/comp/createw.c  -I include/ $(CFLAGS) -o createw.o
	gcc -c kernel/sysio/kservers/kgws/kgws/comp/dtext.c    -I include/ $(CFLAGS) -o dtext.o
	gcc -c kernel/sysio/kservers/kgws/kgws/comp/font.c     -I include/ $(CFLAGS) -o font.o
	gcc -c kernel/sysio/kservers/kgws/kgws/comp/grid.c     -I include/ $(CFLAGS) -o grid.o
	gcc -c kernel/sysio/kservers/kgws/kgws/comp/line.c     -I include/ $(CFLAGS) -o line.o
	gcc -c kernel/sysio/kservers/kgws/kgws/comp/menu.c     -I include/ $(CFLAGS) -o menu.o
	gcc -c kernel/sysio/kservers/kgws/kgws/comp/menubar.c  -I include/ $(CFLAGS) -o menubar.o
	gcc -c kernel/sysio/kservers/kgws/kgws/comp/pixel.c    -I include/ $(CFLAGS) -o pixel.o
	gcc -c kernel/sysio/kservers/kgws/kgws/comp/rect.c     -I include/ $(CFLAGS) -o rect.o
	gcc -c kernel/sysio/kservers/kgws/kgws/comp/sbar.c     -I include/ $(CFLAGS) -o sbar.o
	gcc -c kernel/sysio/kservers/kgws/kgws/comp/toolbar.c  -I include/ $(CFLAGS) -o toolbar.o

	
	gcc -c kernel/sysio/kservers/kgws/logon/logon.c    -I include/ $(CFLAGS) -o logon.o
	gcc -c kernel/sysio/kservers/kgws/logoff/logoff.c  -I include/ $(CFLAGS) -o logoff.o

	gcc -c kernel/sysio/kservers/kgws/terminal/input.c     -I include/ $(CFLAGS) -o input.o
	gcc -c kernel/sysio/kservers/kgws/terminal/output.c    -I include/ $(CFLAGS) -o output.o
	gcc -c kernel/sysio/kservers/kgws/terminal/terminal.c  -I include/ $(CFLAGS) -o terminal.o
	
	gcc -c kernel/sysio/kservers/kgws/user/userenv.c   -I include/ $(CFLAGS) -o userenv.o
	gcc -c kernel/sysio/kservers/kgws/user/desktop.c   -I include/ $(CFLAGS) -o desktop.o
	gcc -c kernel/sysio/kservers/kgws/user/room.c      -I include/ $(CFLAGS) -o room.o
	gcc -c kernel/sysio/kservers/kgws/user/usession.c  -I include/ $(CFLAGS) -o usession.o
	
	gcc -c kernel/sysio/kservers/kgws/kgws.c  -I include/ $(CFLAGS) -o kgws.o



	#
	#  /sci - System Call Interface.
	#
	
	gcc -c kernel/sci/gde_serv.c  -I include/ $(CFLAGS) -o gde_serv.o
	gcc -c kernel/sci/sys.c       -I include/ $(CFLAGS) -o sys.o
	
	
	

## Step2 link-kernel-image  - Linking the kernel image.
kernel-image-link:
	@echo "================================="
	@echo "(Step 2) Linking the kernel image ..."

	ld -m elf_i386 -T kernel/link.ld -o KERNEL.BIN $(OBJECTS) -Map docs/kernel.map
	mv KERNEL.BIN bin/boot/



## Step3 /mnt/gramadovhd    - Creating the directory to mount the VHD.
/mnt/gramadovhd:
	@echo "================================="
	@echo "(Step 3) Creating the directory to mount the VHD ..."

	sudo mkdir /mnt/gramadovhd


## Step4 vhd-create         - Creating a VHD in Assembly language.
vhd-create:
	@echo "================================="
	@echo "(Step 4) Creating a VHD in Assembly language ..."

	nasm -I arch/x86/boot/vhd arch/x86/boot/vhd/main.asm -o GRAMADO.VHD 


## Step5 vhd-mount          - Mounting the VHD.
vhd-mount:
	@echo "================================="
	@echo "(Step 5) Mounting the VHD ..."

	-sudo umount /mnt/gramadovhd
	sudo mount -t vfat -o loop,offset=32256 GRAMADO.VHD /mnt/gramadovhd/


## Step6 vhd-copy-files     - Copying files into the mounted VHD.
vhd-copy-files:
	@echo "================================="
	@echo "(Step 6) Copying files into the mounted VHD ..."


#
# ======== Files in the root dir. ========
#

# bin/boot
	sudo cp bin/boot/BM.BIN       /mnt/gramadovhd
	sudo cp bin/boot/BL.BIN       /mnt/gramadovhd


#userland
	sudo cp userland/base/bin/INIT2.BIN      /mnt/gramadovhd
	sudo cp userland/base/bin/GRAMMGR.BIN    /mnt/gramadovhd


#base
	sudo cp base/GRAMADO.TXT        /mnt/gramadovhd
	
#base	ini files
	sudo cp base/ini/GUI.INI         /mnt/gramadovhd
	sudo cp base/ini/INIT.INI        /mnt/gramadovhd
	sudo cp base/ini/USER.INI        /mnt/gramadovhd


#base fonts
	sudo cp  base/res/fonts/NC2.FON        /mnt/gramadovhd
	sudo cp  base/res/fonts/LIN8X8.FON     /mnt/gramadovhd
	sudo cp  base/res/fonts/LIN8X16.FON    /mnt/gramadovhd
#...


	
#base Cursors
	sudo cp  base/res/cursors/CURSOR.BMP    /mnt/gramadovhd
	sudo cp  base/res/cursors/MOUSE.BMP     /mnt/gramadovhd
	
#base Icons
	sudo cp  base/res/icons/APP.BMP       /mnt/gramadovhd
	sudo cp  base/res/icons/BMP1.BMP      /mnt/gramadovhd
	sudo cp  base/res/icons/FILE.BMP      /mnt/gramadovhd
	sudo cp  base/res/icons/FOLDER.BMP    /mnt/gramadovhd
	sudo cp  base/res/icons/TERMINAL.BMP  /mnt/gramadovhd


#base Images
#	-sudo cp base/res/images/DENNIS.BMP    /mnt/gramadovhd
#	-sudo cp base/res/images/DENNIS2.BMP   /mnt/gramadovhd


#base  Tests
#	-sudo cp base/tests/*.C      /mnt/gramadovhd
#	-sudo cp base/tests/*.LUA    /mnt/gramadovhd


#base Wallpapers
#	-sudo cp base/res/wall/GRAMADO.BMP   /mnt/gramadovhd



# garden

	#gwin
	-sudo cp userland/ul1/garden/bin/GWINMGR.BIN   /mnt/gramadovhd
	-sudo cp userland/ul1/garden/bin/GWS.BIN       /mnt/gramadovhd
#	-sudo cp userland/ul1/garden/bin/GWM.BIN       /mnt/gramadovhd

	#ili
	-sudo cp userland/ul1/garden/bin/ILIINIT.BIN   /mnt/gramadovhd
	-sudo cp userland/ul1/garden/bin/GLOGON.BIN    /mnt/gramadovhd

	#apps
	-sudo cp userland/ul1/garden/bin/NORATERM.BIN  /mnt/gramadovhd
	-sudo cp userland/ul1/garden/bin/GDETERM.BIN   /mnt/gramadovhd 
	-sudo cp userland/ul1/garden/bin/GDESHELL.BIN  /mnt/gramadovhd 
	-sudo cp userland/ul1/garden/bin/GRAMCODE.BIN  /mnt/gramadovhd 
	-sudo cp userland/ul1/garden/bin/GRAMTEXT.BIN  /mnt/gramadovhd 
	-sudo cp userland/ul1/garden/bin/GFE.BIN       /mnt/gramadovhd 
	-sudo cp userland/ul1/garden/bin/SPR.BIN       /mnt/gramadovhd 
	-sudo cp userland/ul1/garden/bin/SYSMON.BIN    /mnt/gramadovhd
	-sudo cp userland/ul1/garden/bin/REBOOT2.BIN   /mnt/gramadovhd
	-sudo cp userland/ul1/garden/bin/LAUNCHER.BIN  /mnt/gramadovhd
	-sudo cp userland/ul1/garden/bin/WINTEST.BIN   /mnt/gramadovhd 
#	-sudo cp userland/ul1/garden/bin/GDEINIT.BIN   /mnt/gramadovhd 
#	-sudo cp userland/ul1/garden/bin/GDETM.BIN     /mnt/gramadovhd 
	-sudo cp userland/ul1/garden/bin/GDEWM.BIN     /mnt/gramadovhd


# atacama
	-sudo cp userland/ul2/atacama/bin/TRUE.BIN      /mnt/gramadovhd
	-sudo cp userland/ul2/atacama/bin/FALSE.BIN     /mnt/gramadovhd
#	-sudo cp userland/ul2/atacama/bin/CAT.BIN       /mnt/gramadovhd
#	-sudo cp userland/ul2/atacama/bin/DUMPIT.BIN    /mnt/gramadovhd
	-sudo cp userland/ul2/atacama/bin/HELLO.BIN     /mnt/gramadovhd
	-sudo cp userland/ul2/atacama/bin/HELLO2.BIN    /mnt/gramadovhd
	-sudo cp userland/ul2/atacama/bin/HELLO3.BIN    /mnt/gramadovhd
#	-sudo cp userland/ul2/atacama/bin/JACKPOT.BIN   /mnt/gramadovhd
#	-sudo cp userland/ul2/atacama/bin/LISP.BIN      /mnt/gramadovhd
	-sudo cp userland/ul2/atacama/bin/REBOOT.BIN    /mnt/gramadovhd
#	-sudo cp userland/ul2/atacama/bin/TASCII.BIN    /mnt/gramadovhd
#	-sudo cp userland/ul2/atacama/bin/GLIBCT1.BIN   /mnt/gramadovhd 
#	-sudo cp userland/ul2/atacama/bin/SHELL3.BIN    /mnt/gramadovhd


# atacama/gt/gramcc
	-sudo cp userland/ul2/atacama/bin/GRAM.BIN      /mnt/gramadovhd 
	-sudo cp userland/ul2/atacama/bin/GRAMC.BIN     /mnt/gramadovhd 

# 3rd party
# Each app has your own folder.
#	-sudo cp userland/ul2/atacama/bin/LUA.BIN       /mnt/gramadovhd
#	-sudo cp userland/ul2/atacama/bin/BISON125.BIN  /mnt/gramadovhd
#	-sudo cp userland/ul2/atacama/bin/FASM.BIN      /mnt/gramadovhd
#...




#
# ======== Creating the all the folders in root dir ========
#		

# Creating standard folders
	-sudo mkdir /mnt/gramadovhd/BIN
	-sudo mkdir /mnt/gramadovhd/BOOT
	-sudo mkdir /mnt/gramadovhd/DEV
	-sudo mkdir /mnt/gramadovhd/DEV/PTS
	-sudo mkdir /mnt/gramadovhd/EFI
	-sudo mkdir /mnt/gramadovhd/EFI/BOOT
	-sudo mkdir /mnt/gramadovhd/ETC
	-sudo mkdir /mnt/gramadovhd/GARDEN
	-sudo mkdir /mnt/gramadovhd/GARDEN/BIN
	-sudo mkdir /mnt/gramadovhd/HOME
	-sudo mkdir /mnt/gramadovhd/LIB
	-sudo mkdir /mnt/gramadovhd/MNT
	-sudo mkdir /mnt/gramadovhd/SBIN
	-sudo mkdir /mnt/gramadovhd/TMP


#
# ======== Files in the BIN/ folder. ========
#

# bugbug
# Suspendendo isso por falta de espaço na partição.

	#-sudo cp userland/ul1/garden/bin/*         /mnt/gramadovhd/BIN 
	#-sudo cp userland/ul2/atacama/bin/*        /mnt/gramadovhd/BIN 
	-sudo cp userland/ul2/atacama/bin/HELLO3.BIN  /mnt/gramadovhd/BIN 
	
#
# ======== Files in the /BOOT/ folder. ========
#

	sudo cp bin/boot/KERNEL.BIN   /mnt/gramadovhd/BOOT
	sudo cp bin/boot/BM.BIN       /mnt/gramadovhd/BOOT
	sudo cp bin/boot/BL.BIN       /mnt/gramadovhd/BOOT


#colocaremos drivers e servidores na pasta boot/
#	sudo cp bin/drivers/??.BIN       /mnt/gramadovhd/BOOT	
#	sudo cp bin/servers/??.BIN       /mnt/gramadovhd/BOOT


#
# ======== Files in the /DEV/ folder. ========
#

# TTY SERIAL DEVICES
#	-sudo cp user/config/TTYS0     /mnt/gramadovhd/DEV
#	-sudo cp user/config/TTYS1     /mnt/gramadovhd/DEV
#	-sudo cp user/config/TTYS2     /mnt/gramadovhd/DEV
#	-sudo cp user/config/TTYS3     /mnt/gramadovhd/DEV

# VIRTUAL CONSOLES
#	-sudo cp user/config/TTY0     /mnt/gramadovhd/DEV
#	-sudo cp user/config/TTY1     /mnt/gramadovhd/DEV
#	-sudo cp user/config/TTY2     /mnt/gramadovhd/DEV
#	-sudo cp user/config/TTY3     /mnt/gramadovhd/DEV
# ...

# NULL
#	-sudo cp user/config/NULL     /mnt/gramadovhd/DEV

# PSEUDO TERMINAL
#	-sudo cp user/config/0        /mnt/gramadovhd/DEV/PTS
#	-sudo cp user/config/1        /mnt/gramadovhd/DEV/PTS
#	-sudo cp user/config/PTMX     /mnt/gramadovhd/DEV/PTS


#
# ======== Files in the /EFI/ folder. ========
#
	
#test efi
#	-sudo cp arch/x86/boot/efi/BOOTIA32.EFI  /mnt/gramadovhd/EFI/BOOT

#
# ======== Files in the /GARDEN/ folder. ========
#

#garden
#	-sudo cp arch/x86/boot/vhd/tests/TEST1.ASM  /mnt/gramadovhd/GARDEN
#	-sudo cp userland/ul1/garden/bin/*                    /mnt/gramadovhd/GARDEN/BIN 


#
# ======== Files in the /LIB/ folder. ========
#	
	
#	-sudo cp arch/x86/boot/vhd/tests/TEST1.ASM  /mnt/gramadovhd/LIB
	

#
# ======== Files in the SBIN/ folder. ========
#
#	-sudo cp userland/ul2/atacama/bin/HELLO3.BIN  /mnt/gramadovhd/SBIN 

#
# ======== Files in the /TMP/ folder. ========
#	

	-sudo cp base/tests/TEST1.C  /mnt/gramadovhd/TMP



## Step7 vhd-unmount        - Unmounting the VHD.
vhd-unmount:
	@echo "================================="
	@echo "(Step 7) Unmounting the VHD ..."

	sudo umount /mnt/gramadovhd


## Step8 clean              - Deleting the object files.           
clean:
	@echo "================================="
	@echo "(Step 8) Deleting the object files ..."

	-rm *.o
	@echo "Success?"


## ====================================================================================
## The extra stuff.
## 1) ISO support.
## 2) HDD support.
## 3) VM support.
## 4) Serial debug support.
## 5) Clean files support.
## 6) Usage support.
	
#
# ======== ISO ======== 
#

# test
# todo
# Create a .ISO file using nasm.
makeiso-x86:
	#todo:  
	#nasm -I arch/x86/boot/iso/stage1/ \
	#-I arch/x86/boot/iso/???/  arch/x86/boot/iso/main.asm  -o  GRAMADO.ISO
	
	@echo "#todo Create ISO using nasm"
	
# ISO
# Mount stage1.bin file with nasm.
# Create a .ISO file and move all the content of the /bin folder
# into the .ISO file.
geniso-x86:
	
	#stage1
	nasm arch/x86/boot/iso/stage1/stage1.asm -f bin -o stage1.bin
	cp stage1.bin bin/boot/gramado/
	rm stage1.bin

	#.ISO
	mkisofs -R -J -c boot/gramado/boot.catalog -b boot/gramado/stage1.bin -no-emul-boot -boot-load-size 4 -boot-info-table -o GRAMADO.ISO bin
	
	@echo "iso Success?"	




#
# ======== HDD ========
#

	
hdd-mount:
	-sudo umount /mnt/gramadohdd
	sudo mount -t vfat -o loop,offset=32256 /dev/sda /mnt/gramadohdd/
#	sudo mount -t vfat -o loop,offset=32256 /dev/sdb /mnt/gramadohdd/
	
hdd-unmount:
	-sudo umount /mnt/gramadohdd
	
hdd-copy-kernel:
	sudo cp bin/boot/KERNEL.BIN /mnt/gramadohdd/BOOT 

danger-hdd-clone-vhd:
	sudo dd if=./GRAMADO.VHD of=/dev/sda
#	sudo dd if=./GRAMADO.VHD of=/dev/sdb




#
# ======== VM ========
#


# Oracle Virtual Box 
oracle-virtual-box-test:
	VBoxManage startvm "Gramado"

# qemu 
qemu-test:
	qemu-system-x86_64 -hda GRAMADO.VHD -m 128 -serial stdio 
#	qemu-system-x86_64 -hda GRAMADO.VHD -m 128 -device e1000 -show-cursor -serial stdio -device e1000





#install-kvm-qemu:
#	sudo pacman -S virt-manager qemu vde2 ebtables dnsmasq bridge-utils openbsd-netcat



#
# ======== SERIAL DEBUG ========
#

serial-debug:
	cat ./docs/sdebug.txt
	

#
# ======== CLEAN ========
#

clean2:
	-rm *.ISO
	-rm *.VHD

clean-userland-base:
#	-rm userland/base/bin/*.o
#	-rm userland/base/bin/*.BIN



clean-garden:
	-rm userland/ul1/garden/bin/*.o
	-rm userland/ul1/garden/bin/*.BIN


clean-atacama:
	-rm userland/ul2/atacama/bin/*.o
	-rm userland/ul2/atacama/bin/*.BIN




kernel-version:
	@echo $(KERNELVERSION)

image-name:
	@echo $(KBUILD_IMAGE)


#
# #test
# make lib
#

#userland:


#
# ======== USAGE ========
#

usage:
	@echo "> make"
	@echo "> make clean"
	@echo "> make clean2"
	@echo "> make qemu-test"
	@echo "> make oracle-virtual-box-test"
	@echo "> make vhd-mount"
	@echo "> make vhd-unmount"
	@echo "..."

