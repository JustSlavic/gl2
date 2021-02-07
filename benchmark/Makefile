PROJECT=benchmark_gl2
CXX = g++
CXX_STANDARD = c++17

LIB_DIR = \
    /usr/lib/ \

LIBS = \
	benchmark \

CXXFLAGS = \
    -std=$(CXX_STANDARD) \
    -Wall \
    -Werror \
    -pedantic \
    -fno-exceptions \
    -fno-rtti \
    -pthread \
    -O3 \
    -fno-omit-frame-pointer \

LDFLAGS = \
    $(addprefix -L, $(LIB_DIR)) \
    $(addprefix -l, $(LIBS))

SOURCES = \
	sandbox \

BIN_DIR   := ../bin
BUILD_DIR := ../build
SRC_DIR   := ../src
SUB_DIR   := benchmark

BENCH_OBJ   := $(addprefix $(BUILD_DIR)/$(SUB_DIR)/, $(addsuffix .o, $(SOURCES)))
BENCH_CPP   := $(addsuffix .cpp, $(SOURCES))

BENCH_EXE  := $(BIN_DIR)/$(SUB_DIR)/$(PROJECT)


# ================= RULES ================= #

all: prebuild $(BENCH_EXE) postbuild


prebuild:
	@mkdir -p $(BIN_DIR)/$(SUB_DIR)
	@mkdir -p $(BUILD_DIR)/$(SUB_DIR)


postbuild:


clean:
	find $(BIN_DIR) -type f -name $(notdir $(BENCH_EXE)) -delete
	find $(BUILD_DIR) -type f -name '*.o' -delete


# ================= PROJECT ================= #

$(BENCH_EXE): sandbox.cpp ./Makefile
	g++ sandbox.cpp -o $(BENCH_EXE) $(CXXFLAGS) $(LDFLAGS)

