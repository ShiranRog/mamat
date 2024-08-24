 CC=gcc
 CCLINK=$(CC)
 CFLAGS=-g -Wall -std=c99
 OBJS=grades.o 

# The first rule is invoked by default
libgrades.so: $(OBJS)
	$(CCLINK) -shared grades.o -o libgrades.so -llinked-list -L.
 
# Full syntax
# Custom Shell commands
grades.o: grades.c grades.h linked-list.h
	$(CC) -c -fPIC grades.c

clean:
	rm -rf grades.o libgrades.so
