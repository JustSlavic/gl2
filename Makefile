PROJECT = gl2
CXX = g++
CXX_STANDARD = c++17

INC_DIR = \
	/usr/include \
	.generated \
	common \
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
	-Werror \
	-fno-rtti \
	-std=$(CXX_STANDARD) \
	-DGRAVITY \

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
	CXXFLAGS += -ggdb3 -DDEBUG
else ifeq ($(MAKECMDGOALS),release)
	# In release build set optimisation level O2, set RELEASE definition
	SUB_DIR  := release
	CXXFLAGS += -O2 -DRELEASE
else ifeq ($(MAKECMDGOALS),test)
	# Runs tests
else
	SUB_DIR  := plain
endif


SOURCES = \
	application \
	config \
	keymap \
	utils \
	graphics/graphics_api \
	api/keyboard \
	api/mouse \
	api/window \
	fs/watcher \
	graphics/shader \
	graphics/vertex_array \
	graphics/vertex_buffer \
	graphics/vertex_buffer_layout \
	graphics/index_buffer \
	graphics/renderer \
	logging/logging \
	logging/handler \
	math/color \
	math/vector2 \
	math/vector3 \
	math/vector4 \
	math/mat4 \
	modeling_2d/camera \
	modeling_2d/model \
	modeling_2d/creatures \
	service/shader_library \
	common/parse/reader \
	common/son/object \
	common/son/parser \
	common/son/visitor \


OBJECTS      := $(addprefix build/$(SUB_DIR)/, $(addsuffix .o,   $(SOURCES)))
SOURCES      := $(addprefix src/,              $(addsuffix .cpp, $(SOURCES)))

STATIC_LIBS  := $(foreach lib, $(LOCAL_LIBS), $(addprefix libs/$(lib)/bin/lib, $(addsuffix .a, $(lib))))

# CURRENT_DIR  := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

PROJECT_LIB := build/$(SUB_DIR)/lib$(PROJECT).a
PROJECT_EXE := bin/$(SUB_DIR)/$(PROJECT)


# ================= RULES ================= #

# Unconditional rules
.PHONY: prebuild postbuild clean test config

# Silent rules
# .SILENT:

all debug release: prebuild $(PROJECT_EXE) postbuild

test:
	$(MAKE) -C tests
	@echo "=========================\n"
	ln -sfn bin/test/$(PROJECT) test
	./test


# ================= UTILITY ================= #

prebuild: src/version.cpp
	@mkdir -p bin/$(SUB_DIR)
	@mkdir -p build/$(SUB_DIR)

postbuild:
	ln -sfn bin/$(SUB_DIR)/$(PROJECT) run

GIT_REF := $(addprefix .git/, $(subst ref: ,, $(shell cat .git/HEAD)))

src/version.cpp: .git/HEAD $(GIT_REF)
#                          ^^^^^^^^^^ if commit/reset current branch (commit's hash changed)
#                ^^^^^^^^^ if checked out to another branch
	@chmod a+x version.sh
	@./version.sh

clean:
	@find build -type f -name '*.o' -delete
	@find build -type f -name '*.d' -delete
	@rm -fv build/*/lib$(PROJECT).a
	@rm -fv bin/*/$(PROJECT)
	@rm -fv .generated/*
	@rm -fv run
	@rm -fv test
	@rm -fv perf.data
	@rm -fv perf.data.old
	$(MAKE) -C tests clean
	$(MAKE) -C config_builder clean

# Cleaning local lib's builds
# .PHONY: $(LOCAL_LIBS)
# clean: $(LOCAL_LIBS)
# $(LOCAL_LIBS):
# 	$(MAKE) -C $(addprefix libs/, $@) clean


-include $(OBJECTS:.o=.d)

# ================= CONFIG ================== #

config: .generated/config.cpp .generated/config.hpp

build/$(SUB_DIR)/config.o: .generated/config.cpp
	@mkdir -p $(dir $@)
	@g++ -MM -MT "$@" $(CXXFLAGS) $< > $*.d
	g++ $< -c -o $@ $(CXXFLAGS)

.generated/config.hpp .generated/config.cpp .generated/config.scheme.son: config_builder/builder
	./config_builder/builder config.son

config_builder/builder: config.son
	$(MAKE) -C config_builder debug


# ================= PROJECT ================= #

$(PROJECT_EXE): main.cpp build/$(SUB_DIR)/version.o $(PROJECT_LIB) $(STATIC_LIBS)
	g++ main.cpp build/$(SUB_DIR)/version.o $(PROJECT_LIB) $(STATIC_LIBS) -o $(PROJECT_EXE) $(CXXFLAGS) $(LDFLAGS)

$(PROJECT_LIB): $(OBJECTS)
	ar rcvs $(PROJECT_LIB) $(OBJECTS)

build/$(SUB_DIR)/%.o: src/%.cpp ./Makefile
	@mkdir -p $(dir $@)
	@g++ -MM -MT "$@" $(CXXFLAGS) $< > build/$(SUB_DIR)/$*.d
	g++ $< -c -o $@ $(CXXFLAGS)

build/$(SUB_DIR)/common/%.o: common/%.cpp ./Makefile
	@mkdir -p $(dir $@)
	@g++ -MM -MT "$@" $(CXXFLAGS) $< > build/$(SUB_DIR)/$*.d
	g++ $< -c -o $@ $(CXXFLAGS)

${STATIC_LIBS}:
	$(MAKE) -C $(shell dirname $(dir $@))

