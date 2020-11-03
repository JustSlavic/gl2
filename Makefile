PROJECT = gl2
CXX = g++
CXX_STANDARD = c++14

INC_DIR = \
    /usr/include \
    src \
    libs \

LIB_DIR = \
    /usr/lib/ \

LOCAL_LIBS = \
#     stb \
#     imgui \

# Libraries:
# GL - OpenGL
# GLU - OpenGL Utility
# glfw - Managing window and input
# GLEW - Extention Wrangler that dynamically loads supported OpenGL functions
# SDL2 - Simple DirectMedia Layer (managing window, input, audio, etc.)
LIBS = \
    GL \
    GLU \
    GLEW \
    SDL2 \
#     glfw \

CXXFLAGS = \
    -Wall \
    -std=$(CXX_STANDARD)

CXXFLAGS += $(addprefix -I, $(INC_DIR))

LDFLAGS = \
    $(addprefix -L, $(LIB_DIR)) \
    $(addprefix -l, $(LIBS))


# Settings for debug/release build configurations
ifndef MAKECMDGOALS
    # If nothing is set => no debug info is included in binary, also no flags are set
    SUB_DIR  := plain
else ifeq ($(MAKECMDGOALS),debug)
    # In debug build put debug info into binary, set DEBUG definition
    SUB_DIR  := debug
    CXXFLAGS += -ggdb3
    CXXFLAGS += -DDEBUG
else ifeq ($(MAKECMDGOALS),release)
    # In release build set optimisation level O2, set RELEASE definition
    SUB_DIR  := release
    CXXFLAGS += -O2 -DRELEASE
else
    SUB_DIR  := plain
endif


HEADERS = \
    application \
    defines \
    keymap \
    utils \
    api/graphics_api \
    api/keyboard \
    api/mouse \
    api/window \
    es/dispatcher \
    es/emitter \
    es/event_system \
    es/observer \
    fs/watcher \
    graphics/shader \
    logging/logging \
    logging/handler \


SOURCES = \
    application \
    keymap \
    utils \
    api/graphics_api \
    api/keyboard \
    api/mouse \
    api/window \
    fs/watcher \
    graphics/shader \
    logging/logging \
    logging/handler \


HEADERS      := $(addprefix src/,              $(addsuffix .h,   $(HEADERS)))
OBJECTS      := $(addprefix build/$(SUB_DIR)/, $(addsuffix .o,   $(SOURCES)))
SOURCES      := $(addprefix src/,              $(addsuffix .cpp, $(SOURCES)))

STATIC_LIBS  := $(foreach lib, $(LOCAL_LIBS), $(addprefix libs/$(lib)/bin/lib, $(addsuffix .a, $(lib))))

# CURRENT_DIR  := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

# ================= RULES ================= #

# Unconditional rules
.PHONY: prebuild postbuild clean version

# Silent rules
# .SILENT:

all debug release: prebuild bin/$(SUB_DIR)/$(PROJECT) postbuild


# ================= UTILITY ================= #

prebuild: version
	@mkdir -p bin/$(SUB_DIR)
	@mkdir -p build/$(SUB_DIR)

postbuild:
	ln -sfn bin/$(SUB_DIR)/$(PROJECT) run

version: src/version.cpp
	@chmod a+x version.sh

GIT_REF := $(addprefix .git/, $(subst ref: ,, $(shell cat .git/HEAD)))

# remake version file if:
#                v checked out to another branch
#                          v pushed/reseted current branch (commit's hash changed)
src/version.cpp: .git/HEAD $(GIT_REF)
	@./version.sh

clean:
	find build -type f -name '*.o' -delete
	find build -type f -name '*.d' -delete
	rm -fv build/*/lib$(PROJECT).a
	rm -fv bin/*/$(PROJECT)
	rm -fv run

# Cleaning local lib's builds
# .PHONY: $(LOCAL_LIBS)
# clean: $(LOCAL_LIBS)
# $(LOCAL_LIBS):
# 	$(MAKE) -C $(addprefix libs/, $@) clean


# ================= PROJECT ================= #

PROJECT_LIB := build/$(SUB_DIR)/lib$(PROJECT).a
PROJECT_EXE := bin/$(SUB_DIR)/$(PROJECT)

$(PROJECT_EXE): main.cpp build/$(SUB_DIR)/version.o $(PROJECT_LIB) $(STATIC_LIBS)
	g++ main.cpp build/$(SUB_DIR)/version.o $(PROJECT_LIB) $(STATIC_LIBS) -o $(PROJECT_EXE) $(CXXFLAGS) $(LDFLAGS)

$(PROJECT_LIB): $(OBJECTS)
	ar rcvs $(PROJECT_LIB) $(OBJECTS)

-include $(OBJECTS:.o=.d)

build/$(SUB_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	@g++ -MM -MT "$@" $(CXXFLAGS) $< > build/$(SUB_DIR)/$*.d
	g++ $< -c -o $@ $(CXXFLAGS)

${STATIC_LIBS}:
	$(MAKE) -C $(shell dirname $(dir $@))

