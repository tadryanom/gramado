
#history:
#2019 - Created by Fred Nora.

# Testando a libc3
# Tentando colocar os caracteres em stdout ao invés de pintá-los na tela.


VERSION = 0
PATCHLEVEL = 0
SUBLEVEL = 0
EXTRAVERSION =
#NAME = 

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



LIBC    = ../../lib/libc03/include/
LIBCOBJ = ../../lib/libc03/obj


	##
	## Objects
	##

myObjects = crt0.o main.o \
ctype.o \
stdio.o \
fcntl.o \
stdlib.o \
string.o \
time.o \
unistd.o \
termios.o \
ioctl.o \
stubs.o    




.PHONY: all all-c hello3-link finalize clean

all: main.o all-c hello3-link finalize clean
	@echo "Ok?"

main.o:
	gcc  -c  main.c  $(CFLAGS) -I $(LIBC)  -o main.o

all-c:
#	cp $(APIOBJ)/api.o      .
	
	cp $(LIBCOBJ)/ctype.o   .
	cp $(LIBCOBJ)/stdio.o .
	cp $(LIBCOBJ)/fcntl.o .
	cp $(LIBCOBJ)/stdlib.o .
	cp $(LIBCOBJ)/string.o .
	cp $(LIBCOBJ)/time.o    .
	cp $(LIBCOBJ)/unistd.o  .
	
	cp $(LIBCOBJ)/termios.o  .
	cp $(LIBCOBJ)/ioctl.o  .
	
	cp $(LIBCOBJ)/stubs.o  .
	cp $(LIBCOBJ)/crt0.o  .

hello3-link:
	ld -m elf_i386 -T link.ld -o HELLO3.BIN $(myObjects) -Map app_map.s

finalize:
	cp HELLO3.BIN ../../bin

clean:
	-rm *.o
	-rm HELLO3.BIN 
	
	
