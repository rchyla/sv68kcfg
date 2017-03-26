/* Various CPU/memory options */

#include <stdio.h>

#include <exec/types.h>

#include "hardware.h"
#include "cfgreg.h"
#include "cpu.h"

void
turbo_enable()
{
	cfgreg_set(CFG_TURBO_ADDR, CFG_TURBO);
}

void
turbo_disable()
{
	cfgreg_unset(CFG_TURBO_ADDR, CFG_TURBO);
}


void
fastmem_enable()
{
	cfgreg_set(CFG_MEMORY_ADDR, CFG_FASTMEM);
}

void
fastmem_disable()
{
	cfgreg_unset(CFG_MEMORY_ADDR, CFG_FASTMEM);
}

void
pslowmem_enable()
{
	cfgreg_set(CFG_MEMORY_ADDR, CFG_PSLOWMEM);
}

void
pslowmem_disable()
{
	cfgreg_unset(CFG_MEMORY_ADDR, CFG_PSLOWMEM);
}

void 
hdd_disable()
{
	cfgreg_unset(CFG_MEMORY_ADDR, CFG_HDD);
}

void 
hdd_enable()
{
	cfgreg_set(CFG_MEMORY_ADDR, CFG_HDD);
}

void 
moremem_enable()
{
	cfgreg_set(CFG_MEMORY_ADDR, CFG_MOREMEM);
}
