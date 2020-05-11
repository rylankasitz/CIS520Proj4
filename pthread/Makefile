CC        = gcc
PROGRAM   = pthreads_solution		# name of the binary
SRCS      = pthreads_solution.c		# source file
OBJS      = $(SRCS:.c=.o)			# object file

#
# Targets
#

default: all

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) -pthread $(OBJS) -o $(PROGRAM)

clean:
	/bin/rm -f $(OBJS) $(PROGRAM)
