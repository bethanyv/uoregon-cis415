
CC=gcc
LIBS=-lpthread -lm 
CFLAGS=-g -Wall -std=gnu99
OBJECTS=bounded_queue.o thread_safe_bounded_queue.o

PROGRAMS= bqueue_test ts_bqueue_test part1 part2

all: bq_test ts_bq_test

#//Your TEST program names go here. Add the list of object dependencies and c file with a main function.
# See instructions below for creating objects from C files
# Add the command as below to replacing with your program name (bq_test|ts_bq_test) in the example below:
# Then add it to the list above, and it will be part of the make build process.



bq_test: utilities.o bounded_queue.o bq_test_main.c
	$(CC) -o bq_test $(CFLAGS) bq_test_main.c utilities.o bounded_queue.o $(LIBS)

ts_bq_test: utilities.o thread_safe_bounded_queue.o ts_bq_test_main.c
	$(CC) -o ts_bq_test $(CFLAGS) ts_bq_test_main.c utilities.o bounded_queue.o thread_safe_bounded_queue.o $(LIBS)

#//Your Program name here. Follow allong from above^^^^^^

part1: part1.o
	$(CC) -o part1 $(CFLAGS) part1.o $(LIBS)

part2: part2.o
	$(CC) -o part2 $(CFLAGS) part2.o $(LIBS)

# EACH LINE BELOW COMPILES AS FOLLOWS:
# Compile each C file using default, example for bounded_queue.o below:
#
#       bounded_queue.o: bounded_queue.c bounded_queue.h
#
# Expands to:
#
#       $(CC) $(CFLAG) -o boundounded_queue.o bounded_queueue.c bounded_queue.h $(LIBS)
#
# Expands to:
#
#       gcc -g -W -Wall -pedantic -o boundounded_queue.o bounded_queueue.c bounded_queue.h -lpthread -lm
# So just add your object file name, c source file, header dependencies
# then add it to the list for your program above.


part1.o: part1.c
part2.o: part2.c
bounded_queue.o: bounded_queue.c bounded_queue.h
thread_safe_bounded_queue.o: thread_safe_bounded_queue.c thread_safe_bounded_queue.h
utilities.o: utilities.c utilities.h

# The general order of an instruction is:
#       <target>: <dependN...>
#               <cmd1>
#               <cmd2>
#               
#  If the <target> is not found (file on disk) then check for <dependN> (including other make command)
#       if all dependencies satisfied,
#               run cmd1
#               run cmd2...
clean:
	rm -f *.o $(PROGRAMS)
	
	
	
	
	
	
	
	
	
	
	
	
	
	

