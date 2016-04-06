MAIN=AppGen

OBJECTS=$(MAIN).o ag_variable.o ag_option.o ag_parse.o filelist.o

#CXX=g++
#CXX=icc

CFLAGS= -O3 $(OPT) -std=c++11 -g -Wall -fno-strict-aliasing

INCLUDE=
LIBRARY=
LIBS=

# source code beautifier
ASTYLE := astyle
# format options for beautifier
ASTYLE_OPT = --style=gnu --unpad-paren --pad-header --pad-oper --indent-namespaces --indent=spaces=2 --convert-tabs --align-pointer=type --align-reference=type --remove-brackets

%.o:%.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $*.c

%.o:%.cpp
	$(CXX) $(CFLAGS) $(INCLUDE) -c $*.cpp

%: %.o
	$(CXX) $(CFLAGS) $(LIBRARY) -o $* $*.o $(LIBS)

%:%.cpp
	$(CXX) $(CFLAGS) $(INCLUDE) -c $*.cpp
	$(CXX) $(CFLAGS) $(LIBRARY) -o $* $*.o $(LIBS)

all:	AppGen agtest

$(MAIN): dep $(OBJECTS)
	$(CXX) $(OPT) $(LIBRARY) -o $(MAIN) $(OBJECTS) $(LIBS)

format:
	$(ASTYLE) $(ASTYLE_OPT) *.cpp *.h

clean:
	-rm -f *.o depend *~ *.orig

dep:
	-rm depend 
	touch depend
	$(CXX) -M $(COPT) $(INCLUDE) *.cpp >> depend	

agtest: AppGen agtest.cpp
	AppGen -l -d agtest.cpp agtest_out.cpp
	g++ agtest_out.cpp filelist.o -o agtest

-include depend
