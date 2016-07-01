del *.o
vc -c sv68kcfg.c cfgreg.c rom.c file.c cpu.c addmem.c
vc -o sv68kcfg -lamiga sv68kcfg.o cfgreg.o rom.o file.o cpu.o addmem.o
