build: hashtable.obj
	cl hashtable.obj /Fetema1.exe hash.lib
hashtable.obj: hashtable.c
	cl /c hashtable.c
clean:
	del tema1.exe
