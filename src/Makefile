CC=vc
RMRF=rm -rf
GROFF=groff

CFLAGS=
LDFLAGS=-lamiga

OBJECTS=cpu.o file.o cfgreg.o addmem.o rom.o sv68kcfg.o
ADDHEADERS=config.h hardware.h


EXECUTABLE=sv68kcfg


all: $(EXECUTABLE) 

sv68kcfg : $(OBJECTS)
	$(CC) -o $(EXECUTABLE) $(LDFLAGS) $(OBJECTS)

%.o : %.c %.h $(ADDHEADERS)
	$(CC) $(CFLAGS) -c $<

clean :
	$(RMRF) $(OBJECTS) $(EXECUTABLE) $(MANPAGEPS) 

