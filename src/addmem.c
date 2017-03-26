#include <stdio.h>


#include <exec/types.h>
#include <exec/execbase.h>
#include <proto/exec.h>

#include "hardware.h"
#include "addmem.h"

#pragma dontwarn 113 

extern BOOL debug;



/* add non-autoconfiguring memory to the system */
void
memory_add_misc(void)
{
	if (!memory_check_added(ADDMEM_0_BASE))
		AddMemList(((1024 + 512)*1024), MEMF_FAST, ADDMEM_PRI, (void *) ADDMEM_0_BASE, NULL);

	/*if (!memory_check_added(ADDMEM_1_BASE))
		AddMemList(512*1024, MEMF_FAST, ADDMEM_PRI, ADDMEM_1_BASE, NULL);*/
}

/* check if memory at given address is already added to system memory list */
BOOL
memory_check_added(ULONG address)
{
	struct MemHeader *m, *nm;

	for (m  = (void *) SysBase->MemList.lh_Head;
	    nm = (void *) m->mh_Node.ln_Succ; m = nm) {
		if (address == ( (ULONG) m->mh_Lower & 0xFFFF00)) {
			if (debug)
				printf("DEBUG: memory at address %p already added\n", (void*) address);
			return 1;
		}
	}
	return 0;
}

