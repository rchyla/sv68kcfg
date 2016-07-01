#ifndef _ROM_H_
#define _ROM_H_

UBYTE maprom_enable(char *path);
void maprom_disable();
UBYTE shadowrom_enable(UBYTE romsize_1M);
void shadowrom_disable(void);

#endif /* _ROM_H_ */
