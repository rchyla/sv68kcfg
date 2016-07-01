#include <stdio.h>

#include <exec/types.h>

#include "rom.h"
#include "hardware.h"
#include "file.h"

#define EXIT_WARNING		5
#define EXIT_SYNTAX_ERROR	10
#define EXIT_HARDWARE_ERROR	20 

extern BOOL debug;

BOOL rom_copy_self(BYTE *rombuf, ULONG romsize);

void
shadowrom_disable(void)
{
	cfgreg_unset(CFG_MAPROM_ADDR, CFG_SHADOWROM); 
}

/* copy rom over itself */
UBYTE
shadowrom_enable(UBYTE romsize_1M)
{
	
	UBYTE r1;

	r1 = cfgreg_read(CFG_MAPROM_ADDR);

	if ( (r1 & CFG_MAPROM) || (r1 & CFG_MAPROM_STATUS)) {
		printf("Cannot enable Shadow ROM if MAPROM enabled or currently active!\n");
		return EXIT_SYNTAX_ERROR;
	}
	
	if (r1 & CFG_SHADOWROM) {
		printf("SHADOWROM is already active!\n");
		return EXIT_WARNING;
	}
	


/*	cfgreg_set(CFG_R0_OFFSET, CFG_R0_WRITELOCKOFF);  */
	if(romsize_1M) {
	memcpy((void*) 0xE00000, (void*) 0xE00000, 512*1024);	
	memcpy((void*) 0xF80000, (void*) 0xF80000, 512*1024);
	cfgreg_set(CFG_MAPROM_ADDR, CFG_ROMSIZE_1M);
	}
	else {
	/* memcpy((void*) 0xE00000, (void*) 0xE00000, 512*1024);	*/
	memcpy((void*) 0xF80000, (void*) 0xF80000, 512*1024);
	cfgreg_unset(CFG_MAPROM_ADDR, CFG_ROMSIZE_1M);
	}

/*	cfgreg_unset(CFG_R0_OFFSET, CFG_R0_WRITELOCKOFF);  */

	cfgreg_set(CFG_MAPROM_ADDR, CFG_SHADOWROM);
	
	return 0;
}

/* faster memcpy routine */
/*void
fast_memcpy(ULONG *dest, ULONG *source, ULONG size) {
	
	ULONG i;
	for(i=source; i < (source + size); i+4) *dest = *source;
	
}
*/

void
maprom_disable()
{
	cfgreg_unset(CFG_MAPROM_ADDR, CFG_MAPROM); 
}

UBYTE
maprom_enable(BYTE *path)
{
	BYTE *rombuf;
	ULONG romsize;
	UBYTE r1, r2;

	r1 = cfgreg_read(CFG_MAPROM_ADDR);

	/* do some sanity checks first */	
	if (r1 & CFG_SHADOWROM) {
		printf("Cannot use MAPROM if Shadow ROM enabled. Please disable Shadow ROM and reboot first!\n");
		return EXIT_SYNTAX_ERROR;
	}

	if (r1 & CFG_MAPROM_STATUS) {
		printf("Cannot load new ROM if MAPROM currently active. Please disable MAPROM and reboot first!\n");
		return EXIT_WARNING;
	}


	if (debug)
		printf("DEBUG: will load ROM from %s\n", path);

	romsize = file_size(path);

	if (debug)
		printf("DEBUG: m'kay so apparanetly loaded ROM has size: %lx\n", romsize);

	rombuf = (char*) malloc(romsize);

	if (debug)
		printf("DEBUG: allocated %x bytes at address %p, ready to load data\n", (unsigned int) romsize, (void*) rombuf);

	file_load(path, rombuf, romsize);

	rom_copy_self(rombuf, romsize);
	/*rom_copy_bank(rombuf, romsize);*/

	cfgreg_set(CFG_MAPROM_ADDR, CFG_MAPROM); 

	free(rombuf);
	printf("Your Amiga should be restarted now...\n");
	
	return 0;
}

BOOL
rom_copy_self(BYTE *rombuf, ULONG romsize)
{
	switch (romsize) {
	case 262144:
		cfgreg_unset(CFG_MAPROM_ADDR, CFG_ROMSIZE_1M);
		/* memcpy((void*) 0xE00000, (void*) rombuf, S256K);	*/
		/* memcpy((void*) (0xE00000+S256K), (void*) rombuf, S256K);	*/
		memcpy((void*) 0xF80000, (void*) rombuf, S256K);	
		memcpy((void*) (0xF80000+S256K), (void*) rombuf, S256K);
		
		break;
	case 524288:
		/*memcpy((void*) 0xE00000, (void*) rombuf, S512K);	*/
		cfgreg_unset(CFG_MAPROM_ADDR, CFG_ROMSIZE_1M);
		memcpy((void*) 0xF80000, (void*) rombuf, S512K);
		
		break;
	case 1048576:
		cfgreg_set(CFG_MAPROM_ADDR, CFG_ROMSIZE_1M);
		memcpy((void*) 0xE00000, (void*) rombuf, S512K);
		memcpy((void*) 0xF80000, (void*) (rombuf + S512K), S512K);
		
		break;
	default:
		printf("Unsupported ROM size %x\n, ROM must be exactly 256kB, 512kB or 1MB\n", (unsigned int) romsize);
		return 0;
		break;
	}

	return 1;	
}

