PROJECT = config_builder
CXX = g++
CXX_STANDARD = c++17

INC_DIR = \
	/usr/include \
	src \
	../common \

LIB_DIR = \
	/usr/lib/ \

LOCAL_LIBS = \

LIBS = \

CXXFLAGS = \
	-Wall \
	-Werror \
	-fno-rtti \
	-fno-exceptions \
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
	CXXFLAGS += -ggdb3 -DDEBUG
else ifeq ($(MAKECMDGOALS),asan)
	SUB_DIR  := asan
	CXX_FLAGS += -ggdb3 -DDEBUG -fsanitize=address
else ifeq ($(MAKECMDGOALS),ubsan)
	SUB_DIR  := ubsan
	CXX_FLAGS += -ggdb3 -DDEBUG -fsanitize=undefined
else ifeq ($(MAKECMDGOALS),release)
	# In release build set optimisation level O2, set RELEASE definition
	SUB_DIR  := release
	CXXFLAGS += -O2 -DRELEASE
else
	SUB_DIR  := plain
endif

SOURCES = \
	common/object \
	common/parse/reader \
	common/parse/object_parser
	

OBJECTS      := $(addprefix build/$(SUB_DIR)/, $(addsuffix .o,   $(SOURCES)))
SOURCES      := $(addprefix src/,              $(addsuffix .cpp, $(SOURCES)))


PROJECT_LIB := build/$(SUB_DIR)/lib$(PROJECT).a
PROJECT_EXE := bin/$(SUB_DIR)/$(PROJECT)

# ================= RULES ================= #

# Unconditional rules
.PHONY: prebuild postbuild clean version test

all debug release asan ubsan: prebuild $(PROJECT_EXE) postbuild

prebuild:
	@mkdir -p bin/$(SUB_DIR)
	@mkdir -p build/$(SUB_DIR)

postbuild:
	ln -sfn bin/$(SUB_DIR)/$(PROJECT) builder


clean:
	@rm -rfv build/*
	@rm -rfv bin/*


# ================= PROJECT ================= #

$(PROJECT_EXE): main.cpp $(PROJECT_LIB) $(STATIC_LIBS)
	g++ main.cpp $(PROJECT_LIB) $(STATIC_LIBS) -o $(PROJECT_EXE) $(CXXFLAGS) $(LDFLAGS)


$(PROJECT_LIB): $(OBJECTS)
	ar rcvs $(PROJECT_LIB) $(OBJECTS)


-include $(OBJECTS:.o=.d)


build/$(SUB_DIR)/%.o: src/%.cpp ./Makefile
	@mkdir -p $(dir $@)
	@g++ -MM -MT "$@" $(CXXFLAGS) $< > build/$(SUB_DIR)/$*.d
	g++ $< -c -o $@ $(CXXFLAGS)


build/$(SUB_DIR)/%.o: ../%.cpp ./Makefile
	@mkdir -p $(dir $@)
	@g++ -MM -MT "$@" $(CXXFLAGS) $< > build/$(SUB_DIR)/$*.d
	g++ $< -c -o $@ $(CXXFLAGS)
