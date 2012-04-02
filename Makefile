# Author: Severin Smith

##Tool##

CXX = g++

CXXCOMPILE = $(CXX)
INCLUDES = -I./ -I$(SYSTEMC_PREFIX)/include
COMPILEFLAGS = $(INCLUDES) -DSC_INCLUDE_FX -O3

CXXLINK = $(CXX)
LINKFLAGS = -L$(SYSTEMC_PREFIX)/lib-$(TARGET_ARCH) -lsystemc -lm

##SYSTEM C##

TARGET_ARCH = macosx386
SYSTEMC_PREFIX = /usr/local/Cellar/systemc/2.3

##Files/Dirs##

NAME       = main
SOURCES    = $(wildcard *.c) $(wildcard *.cpp)
OBJECTS    = $(patsubst %.cpp, %.o, $(SOURCES))

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CXXLINK) $(OBJECTS) $(LINKFLAGS) -o $(NAME)

.cpp.o:
	$(CXXCOMPILE) $(COMPILEFLAGS) -c $< -o $@

clean:
	rm -f *.o $(NAME) *.vcd