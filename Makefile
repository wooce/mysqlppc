
SHELL = /bin/sh

CC  = g++

CFLAGS = -Wall -g -std=c++0x -I.
INCLUDES = -I/usr/local/include
LDFLAGS = -L. -lmysqlppc -L/usr/lib/mysql -lmysqlclient
LIBS = \
	-L. -lmysqlppc -L/usr/lib/mysql -lmysqlclient	
DEFS =
OBJECTS = binder.o exception.o connection.o result_metadata.o resultset.o statement.o
DEPENDENCIES=

DEMOOBJS = demo.o

.cpp.o:
	$(CC) -c $(CFLAGS) $<

all: libmysqlppc.a demo

demo: $(DEMOOBJS)
	$(CC) $(CPPFLAGS) -o demo $(DEMOOBJS) $(LIBS)

libmysqlppc.a: $(OBJECTS) $(DEPENDENCIES)
	-rm -f libmysqlppc.a
	ar cru libmysqlppc.a $(OBJECTS) 
	ranlib libmysqlppc.a

.c.o:
	$(CCCOMPILE) -c $<

clean: 
	rm *.o *.a 

