###############################################################################
# purpose: makefile for Lab 3
# author:  Joe Crumpton
#
# 'make'        build executable file
# 'make clean'  removes all intermediate (lex.yy.c and *.o) and executable files
#
# This makefile purposely avoids macros to make the rules more clear.
# For more information about makefiles:
#      http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
#      http://www.cs.swarthmore.edu/~newhall/unixhelp/howto_makefiles.html
#      http://www.gnu.org/software/make/manual/make.html
#
###############################################################################

# variables used in the following rules
CXX      = g++
RM       = rm
# -g generate debug information for gdb
# -Wno-c++11-extensions silence the c++11 error warnings
# -std=c++11 assert that we are using c++11
CXXFLAGS = -g
CXXFLAGS = -g -std=c++11
# don't ever remove these file types
.PRECIOUS = *.l *.h *.cpp [Mm]akefile

lab3: nodes.o driver.o
	$(CXX) $(CXXFLAGS) -o lab3 nodes.o driver.o
#     -o flag specifies the output file
#
#     The above rule could be written with macros as
#        $(CXX) $(CXXFLAGS) -o $@ $^

nodes.o: nodes.cpp nodes.h
	$(CXX) $(CXXFLAGS) -o nodes.o -c nodes.cpp 

driver.o: driver.cpp nodes.h 
	$(CXX) $(CXXFLAGS) -o driver.o -c driver.cpp 
#      -c flag specifies stop after compiling, do not link

clean: 
	$(RM) *.o lab3

