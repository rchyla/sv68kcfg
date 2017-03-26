/* Ninetails configuration registers */
#ifndef _HARDWARE_H_
#define _HARDWARE_H_

#define CFG_ADDRESS		0xE88000	


#define CFG_TURBO_ADDR		0x10
#define CFG_MEMORY_ADDR		0x14
#define CFG_MAPROM_ADDR		0x18
#define CFG_R3_OFFSET		0x1C

#define CFG_LOCK0_OFFSET	0x0
#define CFG_LOCK1_OFFSET	0x4
#define CFG_LOCK2_OFFSET	0x8
#define CFG_LOCK3_OFFSET	0xC

#define CFG_LOCK0_ID_REV0	0xE0	/* ID sequence for Savyna68_pus prototype rev 0 */
#define CFG_LOCK1_ID_REV0	0x70
#define CFG_LOCK2_ID_REV0	0x90
#define CFG_LOCK3_ID_REV0	0x60
#define CFG_LOCK_ID_MASK	0xF0



#define CFG_TURBO		0x80	/* 1xxxxxxx - Turbo on */
#define CFG_SPEED		0x40	/* x1xxxxxx - SPEED */

#define CFG_FASTMEM		0x80	/* 1xxxxxxx - fastmem enable */
#define CFG_PSLOWMEM	0x40	/* x1xxxxxx - pslowmem enable*/
#define CFG_HDD			0x20	/* xx1xxxxx - hdd enable */
#define CFG_MOREMEM		0x10	/* xxx1xxxx - moremem */

#define CFG_SHADOWROM	0x80	/* 1xxxxxxx - Shadow ROM now */
#define CFG_MAPROM		0x40	/* x1xxxxxx - MAPROM at next reset */
#define CFG_MAPROM_STATUS 0x20  /* xx1xxxxx */
#define CFG_ROMSIZE_1M	0x10	/* xxx1xxxx - rom size, 1 - 1MB, 0 - 512kB and less*/



#define CFG_RX_MASK		0xF0

#define MAPROM_BANK_ADDRESS	0xB80000

#define ADDMEM_0_BASE		0xA00000
#define ADDMEM_1_BASE		0xF00000
#define ADDMEM_PCMCIA_BASE	0x600000
#define ADDMEM_PRI		20

#define S256K			256*1024
#define S512K			512*1024

#endif /* _HARDWARE_H_ */

