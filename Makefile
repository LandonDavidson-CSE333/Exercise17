# Copyright ©2025 Hal Perkins.  All rights reserved.  Permission is
# hereby granted to students registered for University of Washington
# CSE 333 for use solely during Winter Quarter 2025 for purposes of
# the course.  No other use, copying, distribution, or modification
# is permitted without prior written consent. Copyrights for
# third-party components of this work must be honored.  Instructors
# interested in reusing these course materials should contact the
# author.

CXX = g++
OBJS = SimpleQueue.o ex17.o
EXE  = ex17
HEADERS = SimpleQueue.h
CPPFLAGS = -Wall -g -std=c++17
LDFLAGS += -pthread

# default target
all: $(OBJS)
	$(CXX) $(CPPFLAGS) -o $(EXE) $(OBJS) $(LDFLAGS)

# source files
%.o: %.cc $(HEADERS)
	$(CXX) $(CPPFLAGS) -c $<

# phony target - remove generated files and backups
clean:
	rm -rf $(EXE) *.o *~ *.dSYM
