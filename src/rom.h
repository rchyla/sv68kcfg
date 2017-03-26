#ifndef _ROM_H_
#define _ROM_H_

UBYTE maprom_enable(BYTE *path);
UBYTE maprom_disable();
UBYTE shadowrom_enable(UBYTE romsize_1M);
void shadowrom_disable(void);
void memcpy_fast(ULONG *dest, ULONG *src, ULONG size);

#endif /* _ROM_H_ */
