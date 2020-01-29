
//video.h 

 
/*
    3C0-3CF  EGA/VGA
	3C0 VGA attribute and sequencer register
	3C1 Other video attributes
	3C2 EGA, VGA, CGA input status 0
	3C3 Video subsystem enable
	3C4 CGA, EGA, VGA sequencer index
	3C5 CGA, EGA, VGA sequencer
	3C6 VGA video DAC PEL mask
	3C7 VGA video DAC state
	3C8 VGA video DAC PEL address
	3C9 VGA video DAC
	3CA VGA graphics 2 position
	3CC VGA graphics 1 position
	3CD VGA feature control
	3CE VGA graphics index
	3CF Other VGA graphics

	3D0-3DF Color Graphics Monitor Adapter (ports 3D0-3DB are
		write only, see 6845)
	3D0 port address decodes to 3D4
	3D1 port address decodes to 3D5
	3D2 port address decodes to 3D4
	3D3 port address decodes to 3D5
	3D4 6845 index register, selects which register [0-11h]
	    is to be accessed through port 3D5
	3D5 6845 data register [0-11h] selected by port 3D4,
	    registers 0C-0F may be read.  If a read occurs without
	    the adapter installed, FFh is returned.  (see 6845)
	3D6 port address decodes to 3D4
	3D7 port address decodes to 3D5
	3D8 6845 Mode control register (CGA, EGA, VGA, except PCjr)
	3D9 color select palette register (CGA, EGA, VGA, see 6845)
	3DA status register (read only, see 6845, PCjr VGA access)
	3DB Clear light pen latch (any write)
	3DC Preset Light pen latch
	3DF CRT/CPU page register (PCjr only)
	
*/ 
 
/*

MDA	B0000 to B0FFF	3B0 to 3BB
CGA	B8000 to BBFFF	3D0 to 3DF

Mono text	B0000 to B7FFF	3B0 to 3BB, 3C0 to 3CF
Mono graphics	A0000 to AFFFF	3B0 to 3BB, 3C0 to 3CF
Colour text	B8000 to BFFFF	3C0 to 3CF, 3D0 to 3DF
Colour graphics	A0000 to AFFFF or B8000 to BFFFF	3C0 to 3CF, 3D0 to 3DF
 */




