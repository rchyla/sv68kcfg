/*
Savyna68k PLUS configuration program

Based on 9tcfg by rkujawa (Radoslaw Kujawa) https://github.com/rkujawa/9tcfg

Modified by Rafał Chyła (sanjyuubi).


*/

#include <stdio.h>
#include <stdlib.h>

#include <exec/types.h>
#include <exec/execbase.h>
#include <workbench/startup.h>

#include <proto/dos.h>
#include <proto/exec.h>

#include "hardware.h"
#include "addmem.h"
#include "cfgreg.h"
#include "rom.h"
#include "cpu.h"

#define EXIT_WARNING		5
#define EXIT_SYNTAX_ERROR	10
#define EXIT_HARDWARE_ERROR	20 

/* -- function prototypes -- */

void reboot(void);
void status_display(void);
void status_print_reg(UBYTE reg, UBYTE bit);
void status_print_reg_inv(UBYTE reg, UBYTE bit);

/* -- global variables -- */

static const STRPTR version = "\0$VER: sv68kcfg 1.0 (01.05.2016)\0";
static const STRPTR id = "\0$Id: 0bd186d77dd243a27c26267de8870027047f7101 $\0";

static LONG *argArray;	/* arguments passed on the command line */

BOOL debug = FALSE;
BYTE hwrev;

/* -- implementation -- */

void
reboot(void)
{
	/* think about opening graphics.library, 
	   then LoadView(); 2x WaitTOF(); to work around V39 bug */

	/* for now let's just call Exec's ColdReboot()... */
	ColdReboot();
}

void
status_print_reg_inv(UBYTE reg, UBYTE bit)
{
	if (reg & bit)
		printf("disabled\n");
	else
		printf("enabled\n");
}

void
status_print_reg(UBYTE reg, UBYTE bit)
{
	if (reg & bit)
		printf("enabled\n");
	else
		printf("disabled\n");
}

/* read and display configuration registers */
void
status_display(void) 
{
	UBYTE r0, r1, r2;

	r0 = cfgreg_read(CFG_TURBO_ADDR);
	r1 = cfgreg_read(CFG_MEMORY_ADDR);
	r2 = cfgreg_read(CFG_MAPROM_ADDR);

	printf("\tSavyna68k PLUS PROTOTYPE:\t\t\t%d\n", hwrev);
	printf(" ================== Hardware settings  ================== \n");

	printf("\tTurbo (reset):\t\t");
	status_print_reg(r0, CFG_TURBO);

	printf("\tFASTMEM:\t");
	status_print_reg(r1, CFG_FASTMEM);

	printf("\tP-SLOWMEM:\t");
	status_print_reg(r1, CFG_PSLOWMEM);

	printf("\tHDD controller:\t");
	status_print_reg(r1, CFG_HDD);
	
	printf("\tMOREMEM:\t");
	status_print_reg(r1, CFG_MOREMEM);
	
	
	printf(" ======================= ROM options ==================== \n");

	printf("\tMAPROM at next reset:\t\t\t");
	status_print_reg(r2, CFG_MAPROM);

	printf("\tMAPROM status:\t\t\t\t");
	status_print_reg(r2, CFG_MAPROM_STATUS);

	printf("\tSHADOWROM:\t\t\t\t");
	status_print_reg(r2, CFG_SHADOWROM);
}

void
help(void) 
{
	printf("\n");
	printf("9tcfg     - Ninetails accelerator config tool by R. Kujawa\n\n");
	printf("Possible ARGS:\n");
	printf("HELP      - this help\n");
	printf("M68K      - MC68000 mode - turn off accelerator (ON/OFF)\n");
	printf("PCMCIA    - sacrifice 4MB FastRAM for PCMCIA sake (ON/OFF)\n");
	printf("SHADOWROM - kickstart shadowing (ON/OFF)\n");
	printf("MAPROM    - enable MAPROM, use with LOADROM (ON/OFF)\n");
	printf("LOADROM   - load kickstart file to reserved RAM (up to 1MB)\n");
	printf("MOREMEM   - 1.5MB more (A80000-B7FFFF, F00000-F7FFFF)\n");
	printf("INSTCACHE - instruction cache (ON/OFF)\n");
	printf("REBOOT    - die and rise from ashes \n");
	printf("DEBUG     - display informations useful only for developers\n");
	printf("\n");
	printf("Example:\n");
	printf("9tcfg MAPROM ON LOADROM=ks3.9.rom MOREMEM PCMCIA ON REBOOT\n");
	printf("Type 9tcfg without args to view status.\n");
	printf("\n");
}

BOOL
arg_switch_isempty(UBYTE argNo)
{
	if ( ((LONG) argArray[argNo] != 0))
		return FALSE;

	return TRUE;
}

BOOL
arg_toggle_val(UBYTE argNo)
{
#define TOGGLE_EMPTY	-2
#define TOGGLE_FALSE	0x0
#define TOGGLE_TRUE	0xFFFFFFFF
	if ((LONG) argArray[argNo] == TOGGLE_TRUE)
		return TRUE;
	else if ((LONG) argArray[argNo] == TOGGLE_FALSE)
		return FALSE;
/*	else
		// I wonder if we'll observe one of these, duh. 
		printf("DEBUG: toggle neither TRUE nor FALSE, this should not happen!\n");
*/

	return FALSE;
}

BOOL
arg_key_isempty(UBYTE argNo)
{
	if ((LONG) argArray[argNo] == 0)
		return TRUE;
	else
		return FALSE;
}

BOOL
arg_toggle_isempty(UBYTE argNo)
{
	if ((LONG) argArray[argNo] != TOGGLE_EMPTY)
		return FALSE;	
	else
		return TRUE;
}

int
main(int argc, char *argv[])
{
	/*
	 * AmigaOS ReadArgs-style argument parsing is an inconsistent shit.
	 * Pile of shit. Period.
	 */
	UBYTE returnCode = 0;
	UBYTE returnCode_EXIT = 0;
	
	struct RDArgs *result;
	CONST_STRPTR argTemplate =
	    "TURBO/T,FASTMEM/T,PSLOWMEM/T,HDD/T,MAPROM/T,SHADOWROM/T,SHADOWROM1M/T,LOADROM/K,MOREMEM/S,REBOOT/S,DEBUG/S,HELP/S";
#define ARGNUM		12

#define TURBO_ARG	0
/*#define MODE68KMEMORY_ARG 1 */
#define FASTMEM_ARG 1
#define PSLOWMEM_ARG 2
#define HDD_ARG 3
#define MAPROM_ARG	4
#define SHADOWROM_ARG	5
#define SHADOWROM1M_ARG 6
#define LOADROM_ARG	7	
#define MOREMEM_ARG	8
#define REBOOT_ARG	9
#define DEBUG_ARG	10
#define HELP_ARG	11

	argArray = AllocVec(ARGNUM*sizeof(LONG), MEMF_ANY|MEMF_CLEAR);

	argArray[TURBO_ARG] = TOGGLE_EMPTY;
	argArray[FASTMEM_ARG] = TOGGLE_EMPTY;
	argArray[PSLOWMEM_ARG] = TOGGLE_EMPTY;
	argArray[HDD_ARG] = TOGGLE_EMPTY;
	argArray[MAPROM_ARG] = TOGGLE_EMPTY;
	argArray[SHADOWROM_ARG] = TOGGLE_EMPTY;

	result = ReadArgs(argTemplate, argArray, NULL);

	if (!arg_switch_isempty(DEBUG_ARG)) {
		debug = TRUE; 
	}

	if (!arg_switch_isempty(HELP_ARG)) {
		help();
		return 0;
	}

	/* 
	 * Some RURUs for correct usage of this program...
	 */
	if ((!arg_toggle_isempty(MAPROM_ARG)) &&
	   (!arg_toggle_isempty(SHADOWROM_ARG))) {
		printf("MAPROM and SHADOWROM can't be used together!\n");
		return EXIT_SYNTAX_ERROR;
	}

	hwrev = hw_detect();

	if (hwrev == -1) {
		printf("Savyna68k PLUS board not detected! :(\n");
		return EXIT_HARDWARE_ERROR;
	}

	cfgreg_unlock();



	
		if (!arg_switch_isempty(MOREMEM_ARG))
	{
/*
		printf("DEBUG: MOREMEM arugment passed\n");
*/

		if(!(cfgreg_read(CFG_MEMORY_ADDR) & CFG_MOREMEM)) {
		moremem_enable();
		memory_add_misc();
		}
	}
	
	/* MAPROM ON only if LOADROM passed. */
	if (!arg_toggle_isempty(MAPROM_ARG)) {
		if (arg_toggle_val(MAPROM_ARG)) 
			if (arg_key_isempty(LOADROM_ARG)){
				printf("MAPROM ON must be used with LOADROM!\n");
				return EXIT_SYNTAX_ERROR;
				}
			else {
				returnCode = maprom_enable((STRPTR) argArray[LOADROM_ARG]);
				if ( returnCode > 5 ){
					cfgreg_lock();
					return returnCode;
					}
				else{
					if(returnCode > returnCode_EXIT) returnCode_EXIT = returnCode;
				}
				
			}
		else
			maprom_disable();
	}

	if (!arg_toggle_isempty(SHADOWROM_ARG)) {
		if (arg_toggle_val(SHADOWROM_ARG)) {
			returnCode = shadowrom_enable(0);
			if( returnCode > 5 ){
				cfgreg_lock();
				return returnCode;
				}
			else{
				if(returnCode > returnCode_EXIT) returnCode_EXIT = returnCode;
			}
		}
		else
			shadowrom_disable();
	}

	
	if (!arg_toggle_isempty(SHADOWROM1M_ARG)) {
		if (arg_toggle_val(SHADOWROM1M_ARG)) {
			returnCode = shadowrom_enable(1);
			if( returnCode > 5 ){
				cfgreg_lock();
				return returnCode;
				}
			else{
				if(returnCode > returnCode_EXIT) returnCode_EXIT = returnCode;
			}
		}
		else
			shadowrom_disable();
	}

	if (!arg_toggle_isempty(TURBO_ARG)) {
		if (arg_toggle_val(TURBO_ARG)) 
			turbo_enable();
		else 
			turbo_disable();
	}


	if (!arg_toggle_isempty(FASTMEM_ARG)) {
		if (arg_toggle_val(FASTMEM_ARG)) 
			fastmem_enable();
		else
			fastmem_disable();
	}
	
	if (!arg_toggle_isempty(PSLOWMEM_ARG)) {
		if (arg_toggle_val(PSLOWMEM_ARG)) 
			pslowmem_enable();
		else
			pslowmem_disable();
	}
	
	if (!arg_toggle_isempty(HDD_ARG)) {
		if (arg_toggle_val(HDD_ARG)) 
			hdd_enable();
		else
			hdd_disable();
	}
	
	


	/* Display status only if no arguments were given. */
	if (argc == 1) {
		status_display();
	}

	cfgreg_lock();

	
	
	
	if (!arg_switch_isempty(REBOOT_ARG) & (returnCode_EXIT == 0)) {
	reboot();
	}

	
	/* Free everything and return to OS. */
	FreeArgs(result);
	FreeVec(argArray);
	
	
	return returnCode_EXIT;
}

/* reboot the machine */


