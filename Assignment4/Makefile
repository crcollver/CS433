###################################
# OS COURSE - CS 433 example
# 
###################################
CC = g++			# use g++ for compiling c++ code or gcc for c code
CFLAGS = -g -Wall -std=c++11		# compilation flags: -g for debugging. Change to -O or -O2 for optimized code.
LIB = -lm			# linked libraries	
LDFLAGS = -lpthread			# link flags
PROG = prog4			# target executable (output)
SRCS = main.cpp     # .c or .cpp source files.
OBJ = $(SRCS:.cpp=.o) 	# object files for the target. 

all : $(PROG) depend

$(PROG): $(OBJ) 
	$(CC) -o $(PROG) $(OBJ) $(LDFLAGS) $(LIB)

.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ > ./.depend;

include .depend

# cleanup
clean:
	/bin/rm -f *.o $(PROG) .depend

# DO NOT DELETE
