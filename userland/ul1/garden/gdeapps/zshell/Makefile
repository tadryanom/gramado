
# Gramado Core - SHELL
# It's a mini shell for Gramado Core environment.

#history:
#2019 - Created by Fred Nora and Luciano Gonçalez.

#todo:
#fazer um makefile pra compilar o shell
#obs: #bugbug tem que arrumas as coisas aqui.



VERSION = 0
PATCHLEVEL = 0
SUBLEVEL = 0
EXTRAVERSION =
#NAME = shell  

#todo:
#We need to simplify these flags

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
	-fno-stack-protector \
	-s

#	-fleading-underscore \
#	-fno-stack-protector \
#	-s


 
API     = ../../lib/api02/include/
APIOBJ  = ../../lib/api02/obj
LIBC    = ../../lib/libcore/include/
LIBCOBJ = ../../lib/libcore/obj


	#
	# Objects
	#

myObjects = crt0.o main.o \
shellui.o \
api.o \
ctype.o \
stdio.o \
stdlib.o \
string.o \
conio.o \
builtins.o \
desktop.o \
unistd.o \
termios.o \
ioctl.o \
stubs.o    




.PHONY: all all-c shell-link finalize clean

all: crt0.o main.o all-c shell-link finalize clean
	@echo "Ok?"

crt0.o:
#	gcc  -c  crt0.c  $(CFLAGS) -I $(LIBC) -I $(API) -o crt0.o

main.o:
	gcc  -c  main.c  $(CFLAGS) -I $(LIBC) -I $(API) -o main.o

all-c:
	gcc  -c  shellui.c   -I $(LIBC) -I $(API)  $(CFLAGS) -o shellui.o
	gcc  -c  builtins.c  -I $(LIBC) -I $(API)  $(CFLAGS) -o builtins.o
	gcc  -c  desktop.c   -I $(LIBC) -I $(API)  $(CFLAGS) -o desktop.o
	
#	gcc  -c  parser.c    -I ../../lib/libc01/include/ -I ../../lib/api01/include/ $(CFLAGS) -o parser.o
#	gcc  -c  lexer.c     -I ../../lib/libc01/include/ -I ../../lib/api01/include/ $(CFLAGS) -o lexer.o
#	gcc  -c  net.c       -I ../../lib/libc01/include/ -I ../../lib/api01/include/ $(CFLAGS) -o net.o
#	gcc  -c  terminal.c  -I ../../lib/libc01/include/ -I ../../lib/api01/include/ $(CFLAGS) -o terminal.o

	cp $(LIBCOBJ)/crt0.o .	
	cp $(LIBCOBJ)/stubs.o .	
	
	cp $(LIBCOBJ)/ctype.o   .
	cp $(LIBCOBJ)/stdio.o .
	cp $(LIBCOBJ)/stdlib.o .
	cp $(LIBCOBJ)/string.o .
	cp $(LIBCOBJ)/conio.o .
	cp $(LIBCOBJ)/wait.o .
	cp $(LIBCOBJ)/unistd.o .

	cp $(LIBCOBJ)/termios.o .
	cp $(LIBCOBJ)/ioctl.o .
	
	cp $(APIOBJ)/api.o      .


shell-link:
	ld -m elf_i386 -T link.ld -o ZSHELL.BIN $(myObjects) -Map shell_map.s

finalize:
	cp ZSHELL.BIN ../../bin

clean:
	-rm *.o
	-rm ZSHELL.BIN 
	
	
