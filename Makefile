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

# that one is ok too
SDL_DIR = extern/SDL
SDL_BUILD_DIR = $(SDL_DIR)/build
SDL_LIB = $(SDL_BUILD_DIR)/libSDL2.la

# a simple config parser lib
INI_DIR = extern/inih

# file-names of the sources
SRC_NAME = main.cpp graphics.cpp \
           imgui_impl_sdl.cpp imgui_impl_opengl3.cpp \
           imgui.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp # imgui_demo.cpp

# folder-names of the sources
SRC_PATH = src $(IMGUI_DIR) $(IMGUI_DIR)/backends

# folder-names containing headers files
INC_PATH = include $(LSMRF_DIR)/include \
           $(INI_DIR) $(IMGUI_DIR) $(IMGUI_DIR)/backends

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
WWFLAGS = $(WFLAGS) -Wpedantic -Woverloaded-virtual \
                    -Wfloat-equal -Wwrite-strings -Wcast-align -Wconversion \
                    -Wshadow -Wredundant-decls -Winit-self \
                    -Wswitch-default -Wswitch-enum -Wundef -Winline \
                    -Wunreachable-code -Wcast-qual -Wctor-dtor-privacy \
                    -Wdisabled-optimization -Wsign-conversion -Wsign-promo \
                    -Wformat=2 -Wmissing-declarations -Wmissing-include-dirs \
                    -Wstrict-overflow=5 -Winline # -Weffc++ -Wpadded


# folder used to store all compilations sub-products (.o and .d mostly)
OBJ_DIR ?= obj
OBJ_PATH ?= $(OBJ_DIR)/rel
OBJ = $(addprefix $(OBJ_PATH)/, $(SRC_NAME:.cpp=.o))
DEP = $(OBJ:.o=.d)
VPATH = $(SRC_PATH)

# includes
INC = $(addprefix -I, $(INC_PATH))

# specify flags for commands used in the following rules
LN = ln -s
RM = rm -f
RMDIR = rmdir
MKDIR = mkdir -p
# CXX ?= g++
MAKE ?= make -j$(shell nproc 2>/dev/null || echo 1)
SUB_MAKE = make -C

# try to be cross-platform
UNAME_S = $(shell uname -s)
ifeq ($(OS), Windows_NT)
    AR = x86_64-w64-mingw32-gcc-ar
    CC = x86_64-w64-mingw32-gcc -static -static-libgcc
    CXX = x86_64-w64-mingw32-g++ -static -static-libgcc -static-libstdc++
    LDLIBS += -L extern/OpenGL/Binaries/ -lgdi32 -lopengl32 -limm32
    LDLIBS += $(shell $(SDL_BUILD_DIR)/bin/sdl2-config --static-libs)
    CPPFLAGS += $(shell $(SDL_BUILD_DIR)/bin/sdl2-config --cflags)
	SDL_LIB = $(SDL_BUILD_DIR)/bin/SDL2.dll

    # LDLIBS += -lgdi32 -lopengl32 -limm32
    # LDLIBS += $(shell pkg-config --static --libs sdl2)
    # CPPFLAGS += $(shell pkg-config --cflags sdl2)

else ifeq ($(UNAME_S), Linux)
    LDLIBS += -lGL -ldl
    LDLIBS += $(shell sdl2-config --libs)
    CPPFLAGS += $(shell sdl2-config --cflags)
    CC += -march=native
    CXX += -march=native

else ifeq ($(UNAME_S), Darwin)  # untested
    LDLIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
    LDLIBS += $(shell sdl2-config --libs)
    LDFLAGS += -L/usr/local/lib -L/opt/local/lib
    CPPFLAGS += $(shell sdl2-config --cflags)
    CPPFLAGS += -I/usr/local/include -I/opt/local/include

endif


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
	+$(MAKE) $(PROJECT)_dev \
		"PROJECT = $(PROJECT)_dev" "LSMRF_LIB = $(LSMRF_LIB:.a=_dev.a)" \
		"CFLAGS = $(DCFLAGS)" "OBJ_PATH = $(OBJ_DIR)/dev"

# build for runtime debugging (fsanitize)
san:
	+$(MAKE) -C $(LSMRF_DIR) san
	+$(MAKE) $(PROJECT)_san \
		"PROJECT = $(PROJECT)_san" "LSMRF_LIB = $(LSMRF_LIB:.a=_san.a)" \
		"CFLAGS = $(SCFLAGS)" "OBJ_PATH = $(OBJ_DIR)/san"

# remove all generated .o and .d
clean:
	$(RM) $(OBJ) $(DEP)
	$(RM) $(OBJ:$(OBJ_DIR)/rel%=$(OBJ_DIR)/dev%)
	$(RM) $(DEP:$(OBJ_DIR)/rel%=$(OBJ_DIR)/dev%)
	$(RM) $(OBJ:$(OBJ_DIR)/rel%=$(OBJ_DIR)/san%)
	$(RM) $(DEP:$(OBJ_DIR)/rel%=$(OBJ_DIR)/san%)
	test -d $(OBJ_DIR) && find $(OBJ_DIR) -name '*.[od]' | xargs $(RM) || true
	+$(MAKE) -C $(LSMRF_DIR) clean

# remove the generated binary, and all .o and .d
fclean: clean
	test -d $(OBJ_DIR) \
&& find $(OBJ_DIR) -type d | sort -r | xargs $(RMDIR) || true
	$(RM) $(PROJECT){,_san,_dev}{,.exe}
	+$(MAKE) -C $(LSMRF_DIR) fclean
	+$(MAKE) -C $(SDL_BUILD_DIR) distclean || true
	$(RM) $(SDL_LIB)

# some people like it real clean
mrproper:
	$(RM) -r $(OBJ_DIR)
	+$(MAKE) fclean
	+$(MAKE) -C $(LSMRF_DIR) mrproper

# clean build and recompile
re: fclean
	+$(MAKE) all

# run tests on project
test: all
	$(PRINTF) "All tests passed!\n"

# build sdl
sdl: $(SDL_LIB)


##
## PRIVATE RULES
##

# create binary (link)
$(PROJECT): $(OBJ) $(LSMRF_LIB) $(SDL_LIB)
	$(CXX) $(CFLAGS) $(INC) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $@

# create object files (compile)
$(OBJ_PATH)/%.o: %.cpp | $(OBJ_PATH)
	$(CXX) $(CPPFLAGS) $(CFLAGS) $(INC) -MMD -MP -c $< -o $@

# create directory for compilation sub-products
$(OBJ_PATH):
	$(MKDIR) $(dir $(OBJ))

# build sdl
$(SDL_LIB):
ifeq ($(OS), Windows_NT)
	$(MKDIR) $(SDL_BUILD_DIR)
	cd $(SDL_BUILD_DIR) \
&& ../configure --prefix=$(PWD)/$(SDL_BUILD_DIR) \
                --disable-audio \
                --target=x86_64-w64-mingw32 \
                --host=x86_64-w64-mingw32
	+$(MAKE) -C $(SDL_BUILD_DIR)
	+$(MAKE) -C $(SDL_BUILD_DIR) install
else
	touch $(SDL_LIB)
endif # otherwise assume there's a system install

# read dependencies list generated by -MMD flag
-include $(DEP)

# just to avoid conflicts between rules and files/folders names
.PHONY: all, dev, san, mecry, $(PROJECT), sdl, \
clean, fclean, mrproper, re, test, testdev, testsan
