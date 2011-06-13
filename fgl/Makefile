# Define the library properties here:

LIB_NAME = fgl



# Define the compiler settings here:

CPP = g++
CC  = gcc
LD  = ld
AR  = ar

SOURCE    = .

INCLUDE   = -I .. -I include

W_OPTS    = -Wall -Wextra
CPP_OPTS  = -O3 $(INCLUDE) $(W_OPTS) -c
CC_OPTS   = -O3 $(INCLUDE) $(W_OPTS) -c

LIB_PATH  = 
LIBS      = 
LD_OPTS   = $(LIB_PATH) $(LIBS) -o $(LIB_NAME)



# Find all source files

SRC_CPP = $(foreach dir, $(SOURCE), $(wildcard $(dir)/*.cpp))
SRC_C   = $(foreach dir, $(SOURCE), $(wildcard $(dir)/*.c))
OBJ_CPP = $(patsubst %.cpp, %.o, $(SRC_CPP))
OBJ_C   = $(patsubst %.c, %.o, $(SRC_C))
OBJ     = $(OBJ_CPP) $(OBJ_C) $(OBJ_S)



# Compile rules.

.PHONY : all

all : $(LIB_NAME).a

$(LIB_NAME).a : $(OBJ)
	$(AR) rcs lib$(LIB_NAME).a $(OBJ)

$(OBJ_CPP) : %.o : %.cpp
	$(CPP) $(CPP_OPTS) -o $@ $<

$(OBJ_C) : %.o : %.c
	$(CC) $(CC_OPTS) -o $@ $<



# Clean rules

.PHONY : clean

clean :
	rm -f $(OBJ) $(LIB_NAME).a
