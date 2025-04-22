# Makefile for Six Degrees of Collaboration
#
# Project Two, CS 15, April 2025
#
# STUDENT TODO: Ensure all necessary files are included for the provide target
#

CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -std=c++11
INCLUDES = $(shell echo *.h)

all: SixDegrees test_collabgraph test_sixdegrees

SixDegrees: main.o SixDegrees.o CollabGraph.o Artist.o
	${CXX} ${CXXFLAGS} -o $@ $^

test_collabgraph: test_collabgraph.o CollabGraph.o Artist.o
	${CXX} ${CXXFLAGS} -o $@ $^

test_sixdegrees: test_sixdegrees.o SixDegrees.o CollabGraph.o Artist.o
	${CXX} ${CXXFLAGS} -o $@ $^

main.o: main.cpp SixDegrees.h CollabGraph.h Artist.h
	${CXX} ${CXXFLAGS} -c $<

SixDegrees.o: SixDegrees.cpp SixDegrees.h CollabGraph.h Artist.h
	${CXX} ${CXXFLAGS} -c $<

CollabGraph.o: CollabGraph.cpp CollabGraph.h Artist.h
	${CXX} ${CXXFLAGS} -c $<

Artist.o: Artist.cpp Artist.h
	${CXX} ${CXXFLAGS} -c $<

test_collabgraph.o: test_collabgraph.cpp CollabGraph.h Artist.h
	${CXX} ${CXXFLAGS} -c $<

test_sixdegrees.o: test_sixdegrees.cpp SixDegrees.h CollabGraph.h Artist.h
	${CXX} ${CXXFLAGS} -c $<

clean:
	rm -rf SixDegrees test_collabgraph test_sixdegrees *.o *.dSYM