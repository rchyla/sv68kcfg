/* Ninetails configuration program */

#include <stdio.h>

#include <exec/types.h>
#include <dos/dos.h>

#include <proto/dos.h>
#include <proto/exec.h>

#define EXIT_SUCCESS 0
#define EXIT_SYNTAX_ERROR 20
#define EXIT_DOS_ERROR 30

extern BOOL debug;

ULONG file_size(BYTE *path);
BOOL file_load(BYTE *path, BYTE *filebuf, ULONG filesize);


/* get file size */
ULONG
file_size(BYTE *path)
{ 
	BPTR file;
	LONG size;
	struct FileInfoBlock *fib;
	struct Library *DOSBase;

	/* XXX: possibly explicitly opening dos.library here is unnecessary */
	DOSBase = OpenLibrary("dos.library", 36L);
	if (!DOSBase) {
		printf("Error opening dos.library!\n");
		exit(EXIT_DOS_ERROR);
	}

	fib = (struct FileInfoBlock *) AllocDosObject(DOS_FIB, TAG_END);
	if (!fib) {
		printf("Couldn't allocate dos object!\n");

		CloseLibrary(DOSBase);
		exit(EXIT_DOS_ERROR);
	}

	if (file = Lock(path, SHARED_LOCK)) {

		if (Examine(file, fib)) {
			size = fib->fib_Size;
			if (debug)
				printf("DEBUG: Examine() returns file size %lx\n", size);
		} else {
			printf("Couldn't Examine() file!\n"); /* XXX */
		}

		UnLock(file);
	} else {
		printf("Couldn't lock file!\n");

		FreeDosObject(DOS_FIB, fib);
		CloseLibrary(DOSBase);
		exit(EXIT_DOS_ERROR);
	}

	FreeDosObject(DOS_FIB, fib);
	CloseLibrary(DOSBase);
	
	return size;
}

/* load file to memory buffer */
BOOL
file_load(BYTE *path, BYTE *filebuf, ULONG filesize)
{
	BPTR fh;

	if ((fh = Open(path, MODE_OLDFILE)) == -1)  {	
		perror("Error openinig file");
		return FALSE;
	}

	if (debug)
		printf("DEBUG: loading %lx bytes long file at %p\n", (ULONG) filesize, (void*) filebuf);

	if (Read(fh, filebuf, filesize) == -1) {
		perror("Error reading file");

		Close(fh);
		return FALSE;
	}

	Close(fh);

	return TRUE;
}

