
Memory Configuration

Name             Origin             Length             Attributes
*default*        0x0000000000000000 0xffffffffffffffff

Linker script and memory map


.text           0x0000000000000200      0xe00
                0x0000000000000200                code = .
                0x0000000000000200                _code = .
                0x0000000000000200                __code = .
 *(.head_x86)
 *(.text)
 .text          0x0000000000000200        0x6 nc2.o
                0x0000000000000200                fmain
                0x0000000000001000                . = ALIGN (0x1000)
 *fill*         0x0000000000000206      0xdfa 

.iplt           0x0000000000001000        0x0
 .iplt          0x0000000000001000        0x0 nc2.o

.eh_frame       0x0000000000001000       0x38
 .eh_frame      0x0000000000001000       0x38 nc2.o

.rel.dyn        0x0000000000001038        0x0
 .rel.got       0x0000000000001038        0x0 nc2.o
 .rel.iplt      0x0000000000001038        0x0 nc2.o

.data           0x0000000000002000     0x1000
                0x0000000000002000                data = .
                0x0000000000002000                _data = .
                0x0000000000002000                __data = .
 *(.data)
 .data          0x0000000000002000      0x400 nc2.o
                0x0000000000003000                . = ALIGN (0x1000)
 *fill*         0x0000000000002400      0xc00 

.got            0x0000000000003000        0x0
 .got           0x0000000000003000        0x0 nc2.o

.got.plt        0x0000000000003000        0x0
 .got.plt       0x0000000000003000        0x0 nc2.o

.igot.plt       0x0000000000003000        0x0
 .igot.plt      0x0000000000003000        0x0 nc2.o

.bss            0x0000000000003000        0x0
                0x0000000000003000                bss = .
                0x0000000000003000                _bss = .
                0x0000000000003000                __bss = .
 *(.bss)
 .bss           0x0000000000003000        0x0 nc2.o
                0x0000000000003000                . = ALIGN (0x1000)
                0x0000000000003000                end = .
                0x0000000000003000                _end = .
                0x0000000000003000                __end = .
LOAD nc2.o
OUTPUT(NC2.FON elf32-i386)

.comment        0x0000000000000000       0x11
 .comment       0x0000000000000000       0x11 nc2.o
                                         0x12 (size before relaxing)

.note.GNU-stack
                0x0000000000000000        0x0
 .note.GNU-stack
                0x0000000000000000        0x0 nc2.o
