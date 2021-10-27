#******************************************************************************#
#                                                               _.._..,_,_     #
#    Makefile                                                  (          )    #
#                                                               ]~,'-.-~~[     #
#    By: mc <mc@rmrf.fr>                                      .=])' (;  ([     #
#                                                             | ]:)   '  [     #
#      "THE BEER-WARE LICENSE" (Revision 42):                 | ]  :: '  [     #
#    As long as you retain this notice you can do whatever    '=]): .)  ([     #
#    you want with this stuff. If we meet some day, and you     |:: '   :|     #
#    think this is worth it, you can buy me a beer in return.    ~~----~~      #
#                                                                              #
#******************************************************************************#


##
## CUSTOM CONFIG
##

# name of the binary to make
PROJECT = smrfhck

# your favorite lib ever
LSMRF_DIR = extern/libsmrf
LSMRF_LIB = $(LSMRF_DIR)/libsmrf.a

# another cool lib
IMGUI_DIR = extern/imgui

# file-names of the sources
SRC_NAME = main.cpp \
           imgui_impl_sdl.cpp imgui_impl_opengl3.cpp \
           imgui.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp # imgui_demo.cpp

# folder-names of the sources
SRC_PATH = src $(IMGUI_DIR) $(IMGUI_DIR)/backends

# folder-names containing headers files
INC_PATH = include $(LSMRF_DIR)/include $(IMGUI_DIR) $(IMGUI_DIR)/backends

# where are your tests?
TEST_DIR = test

# extra libraries needed for linking
LDLIBS = $(LSMRF_LIB)

# linking flags
LDFLAGS =

# compilation flags
CPPFLAGS =



##
## GLOBAL VARIABLES
##

# compilation/linking flags for the differents public rules
WFLAGS = -std=c++11 -Wextra -Wall  # warnings
RCFLAGS = $(WFLAGS) -O2  # release
DCFLAGS = $(WFLAGS) -g -Og -DNDEBUG  # debug
SCFLAGS = $(DCFLAGS) -fsanitize=address,undefined  # sanitize
WWFLAGS = $(WFLAGS) -Wpedantic -Wold-style-cast -Woverloaded-virtual \
                    -Wfloat-equal -Wwrite-strings -Wcast-align -Wconversion \
                    -Wshadow -Wredundant-decls -Winit-self \
                    -Wswitch-default -Wswitch-enum -Wundef -Winline \
                    -Wunreachable-code -Wcast-qual -Wctor-dtor-privacy \
                    -Wdisabled-optimization -Wsign-conversion -Wsign-promo \
                    -Wformat=2 -Wmissing-declarations -Wmissing-include-dirs \
                    -Wstrict-overflow=5  # -Weffc++ -Wpadded


# folder used to store all compilations sub-products (.o and .d mostly)
OBJ_DIR ?= obj
OBJ_PATH ?= $(OBJ_DIR)/rel
OBJ = $(addprefix $(OBJ_PATH)/, $(SRC_NAME:.cpp=.o))
DEP = $(OBJ:.o=.d)
VPATH = $(SRC_PATH)

# includes
INC = $(addprefix -I, $(INC_PATH))

# specify flags for commands used in the following rules
LN =		ln -s
RM =		rm -f
RMDIR =		rmdir
MKDIR =		mkdir -p
CXX ?=      g++
MAKE ?=		make -j$(shell nproc 2>/dev/null || echo 1)
SUB_MAKE =	make -C

# try to be cross-platform
UNAME_S = $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	LDLIBS += -lGL -ldl
	LDLIBS += $(shell sdl2-config --libs)
	CPPFLAGS += $(shell sdl2-config --cflags)
endif
ifeq ($(UNAME_S), Darwin)
	LDLIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	LDLIBS += $(shell sdl2-config --libs)
	LDFLAGS += -L/usr/local/lib -L/opt/local/lib
	CPPFLAGS += $(shell sdl2-config --cflags)
	CPPFLAGS += -I/usr/local/include -I/opt/local/include
endif
ifeq ($(OS), Windows_NT)
    LDLIBS += -lgdi32 -lopengl32 -limm32
	LDLIBS += $(shell pkg-config --static --libs sdl2)
    CPPFLAGS += $(shell pkg-config --cflags sdl2)
endif


# default to "pretty" Makefile, but you can run ´VERBOSE=t make´
# ifndef VERBOSE
#  ifndef TRAVIS
# .SILENT:
#  endif
# endif
# PRINTF = test $(VERBOSE)$(TRAVIS) || printf

# some colors for pretty printing
WHITE =		\033[37m
RED =		\033[31m
GREEN =		\033[32m
YELLOW =	\033[33m
BLUE =		\033[34m
BASIC =		\033[0m


##
## PUBLIC RULES
##

# release build
all:
	+$(MAKE) -C $(LSMRF_DIR)
	+$(MAKE) $(PROJECT) "CFLAGS = $(RCFLAGS)" "OBJ_PATH = $(OBJ_DIR)/rel"

# masochist build
mecry:
	+$(MAKE) -C $(LSMRF_DIR) mecry
	+$(MAKE) $(PROJECT) "CFLAGS = $(WWFLAGS)" "OBJ_PATH = $(OBJ_DIR)/rel"

# build for gdb/valgrind debugging
dev:
	+$(MAKE) -C $(LSMRF_DIR) dev
	+$(MAKE) $(PROJECT).dev \
		"PROJECT = $(PROJECT).dev" "LSMRF_LIB = $(LSMRF_LIB).dev" \
		"CFLAGS = $(DCFLAGS)" "OBJ_PATH = $(OBJ_DIR)/dev"

# build for runtime debugging (fsanitize)
san:
	+$(MAKE) -C $(LSMRF_DIR) san
	+$(MAKE) $(PROJECT).san \
		"PROJECT = $(PROJECT).san" "LSMRF_LIB = $(LSMRF_LIB).san" \
		"CFLAGS = $(SCFLAGS)" "OBJ_PATH = $(OBJ_DIR)/san"

# remove all generated .o and .d
clean:
	$(RM) $(OBJ) $(DEP)
	$(RM) $(OBJ:$(OBJ_DIR)/rel%=$(OBJ_DIR)/dev%)
	$(RM) $(DEP:$(OBJ_DIR)/rel%=$(OBJ_DIR)/dev%)
	$(RM) $(OBJ:$(OBJ_DIR)/rel%=$(OBJ_DIR)/san%)
	$(RM) $(DEP:$(OBJ_DIR)/rel%=$(OBJ_DIR)/san%)
	test -d $(OBJ_DIR) && find $(OBJ_DIR) -name '*.[od]' | xargs $(RM) || true

# remove the generated binary, and all .o and .d
fclean: clean
	test -d $(OBJ_DIR) \
&& find $(OBJ_DIR) -type d | sort -r | xargs $(RMDIR) || true
	$(RM) $(PROJECT) $(PROJECT).san $(PROJECT).dev

# some people like it real clean
mrproper:
	$(RM) -r $(OBJ_DIR)
	$(RM) -r $(TEST_DIR)
	+$(MAKE) fclean

# clean build and recompile
re: fclean
	+$(MAKE) all

# run tests on project
test: all
	$(PRINTF) "All tests passed!\n"


##
## PRIVATE RULES
##

# create binary (link)
$(PROJECT): $(OBJ) $(LSMRF_LIB)
	$(CXX) $(CFLAGS) $(INC) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $@

# create object files (compile)
$(OBJ_PATH)/%.o: %.cpp | $(OBJ_PATH)
	$(CXX) $(CPPFLAGS) $(CFLAGS) $(INC) -MMD -MP -c $< -o $@

# create directory for compilation sub-products
$(OBJ_PATH):
	$(MKDIR) $(dir $(OBJ))

# read dependencies list generated by -MMD flag
-include $(DEP)

# just to avoid conflicts between rules and files/folders names
.PHONY: all, dev, san, mecry, $(PROJECT), \
clean, fclean, mrproper, re, test, testdev, testsan
