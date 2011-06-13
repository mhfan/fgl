# Define the library properties here:

APP_NAME = fgl_spinning_cube



# Define the compiler settings here:

CPP = g++
CC  = gcc
LD  = ld

SOURCE    = .

INCLUDE   =

W_OPTS    = -Wall -Wextra
CPP_OPTS  = -O3 $(INCLUDE) $(W_OPTS) -c
CC_OPTS   = -O3 $(INCLUDE) $(W_OPTS) -c

LIB_PATH  =
LIBS      = -lfgl -lfixmath -lglfw
LD_OPTS   = $(LIB_PATH) $(LIBS) -o $(APP_NAME)



# Find all source files

SRC_CPP = $(foreach dir, $(SOURCE), $(wildcard $(dir)/*.cpp))
SRC_C   = $(foreach dir, $(SOURCE), $(wildcard $(dir)/*.c))
OBJ_CPP = $(patsubst %.cpp, %.o, $(SRC_CPP))
OBJ_C   = $(patsubst %.c, %.o, $(SRC_C))
OBJ     = $(OBJ_CPP) $(OBJ_C) $(OBJ_S)



# Compile rules.

.PHONY : all

all : $(APP_NAME)

$(APP_NAME) : $(OBJ)
	$(LD) $(OBJ) $(LD_OPTS)

$(OBJ_CPP) : %.o : %.cpp
	$(CPP) $(CPP_OPTS) -o $@ $<

$(OBJ_C) : %.o : %.c
	$(CC) $(CC_OPTS) -o $@ $<



# Clean rules

.PHONY : clean

clean :
	rm -f $(OBJ) $(APP_NAME)
