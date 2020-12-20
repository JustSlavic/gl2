PROJECT = gl2_test
CXX = g++
CXX_STANDARD = c++17

INC_DIR = \
    /usr/include \
    src \
    libs \

LIB_DIR = \
    /usr/lib/ \

LOCAL_LIBS = \

LIBS = \
	gtest \
	gtest_main \

CXXFLAGS = \
    -Wall \
    -std=$(CXX_STANDARD) \
    -pthread \

CXXFLAGS += $(addprefix -I, $(INC_DIR))

LDFLAGS = \
    $(addprefix -L, $(LIB_DIR)) \
    $(addprefix -l, $(LIBS))

SOURCES = \
	string_id \

BIN_DIR   := ../bin
BUILD_DIR := ../build
SUB_DIR   := test

OBJECTS := $(addprefix $(BUILD_DIR)/$(SUB_DIR)/, $(addsuffix .o,   $(SOURCES)))
SOURCES := $(addprefix tests/,                   $(addsuffix .cpp, $(SOURCES)))

TEST_EXE := $(BIN_DIR)/$(SUB_DIR)/$(PROJECT)

# ================= RULES ================= #

# Unconditional rules
.PHONY: prebuild postbuild clean

# Silent rules
# .SILENT:

all: prebuild $(TEST_EXE) postbuild

# ================= UTILITY ================= #

prebuild:
	@mkdir -p $(BIN_DIR)/$(SUB_DIR)
	@mkdir -p $(BUILD_DIR)/$(SUB_DIR)

postbuild:

clean:
	find $(BIN_DIR) -type f -name $(TEST_EXE) -delete
	find $(BUILD_DIR) -type f -name '*.o' -delete

# ================= PROJECT ================= #


$(TEST_EXE): $(OBJECTS)
	c++ $(OBJECTS) -o $(TEST_EXE) $(CXXFLAGS) $(LDFLAGS)

-include $(OBJECTS:.o=.d)

$(BUILD_DIR)/$(SUB_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@g++ -MM -MT "$@" $(CXXFLAGS) $< > $(BUILD_DIR)/$(SUB_DIR)/$*.d
	g++ $< -c -o $@ $(CXXFLAGS)
