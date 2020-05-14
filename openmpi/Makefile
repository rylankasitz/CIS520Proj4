CC        = gcc
PROGRAM   = openmpi					# name of the binary
SRCS      = openmpi.c				# source file
OBJS      = $(SRCS:.c=.o)			# object file

#
# Targets
#

default: all

all: $(PROGRAM) 

$(PROGRAM): $(OBJS)
	$(CC) $(OBJS) -o $(PROGRAM) -fopenmp 

clean:
	/bin/rm -f $(OBJS) $(PROGRAM)