# Define the library properties here:
 
LIB_NAME = fgl
 
CC=libtool --mode=compile mipsel-linux-gcc
RANLIB=mipsel-linux-ranlib
LD=mipsel-linux-gcc -shared
DESTDIR=/opt/mipsel-linux-uclibc/usr
LIB_SUFFIX=/lib/
HEADER_SUFFIX=/include/fgl/
INST_LIB=install -m 644
INST_HEADER=install -m 644
lib_LTLIBRARIES= libfgl.a \
         libfgl.so

lib_LTHEADERS= $(wildcard *.h) \
         $(wildcard *.hpp)
 
# Define the compiler settings here:
 
SOURCE    = .
 
INCLUDE   = -I .. -I include -lfixmath
 
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
 
all : $(LIB_NAME).a \
      $(LIB_NAME).so
 
$(LIB_NAME).a : $(OBJ)
    $(AR) rcs lib$(LIB_NAME).a $(OBJ) 
    $(RANLIB) lib$(LIB_NAME).a

$(LIB_NAME).so : $(OBJ)
    $(LD) -o lib$(LIB_NAME).so $(OBJ)
 
$(OBJ_CPP) : %.o : %.cpp
    $(CPP) $(CPP_OPTS) -o $@ $<
 
$(OBJ_C) : %.o : %.c
    $(CC) $(CC_OPTS) -fPIC -o $@ $<
 
 
 
# Clean rules
 
.PHONY : clean
 
clean :
    rm -f $(OBJ) $(LIB_NAME).a $(LIB_NAME).so

# Install rules.

.PHONY : install

install : $(LIB_NAME).a $(LIB_NAME).so $(install_libs) $(install_headers) 
    -rm -rf $(DESTDIR)$(LIB_SUFFIX)
    mkdir $(DESTDIR)$(LIB_SUFFIX)
    -chmod 777 $(DESTDIR)$(LIB_SUFFIX)
    -rm -rf $(DESTDIR)$(HEADER_SUFFIX)
    mkdir $(DESTDIR)$(HEADER_SUFFIX)
    -chmod 777 $(DESTDIR)$(HEADER_SUFFIX)
    @list='$(lib_LTLIBRARIES)'; for p in $$list; do \
          $(INST_LIB) $$p $(DESTDIR)$(LIB_SUFFIX); \
    done; \
    list2='$(lib_LTHEADERS)'; for p in $$list2; do \
          $(INST_HEADER) $$p $(DESTDIR)$(HEADER_SUFFIX); \
    done 
