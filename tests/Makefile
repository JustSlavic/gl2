PROJECT = gl2_test
CXX = g++
CXX_STANDARD = c++17

INC_DIR = \
    /usr/include \
    ../src \
    ../libs \

LIB_DIR = \
    /usr/lib/ \

LOCAL_LIBS = \

LIBS = \
	GL \
    GLU \
    GLEW\
	gtest \
	gtest_main \

CXXFLAGS = \
    -Wall \
    -std=$(CXX_STANDARD) \
    -fno-rtti \
    -pthread \
    -ggdb3 \
    -DDEBUG \
#     -fsanitize=address \
#     -fsanitize=undefined \
#     -pedantic \

CXXFLAGS += $(addprefix -I, $(INC_DIR))

LDFLAGS = \
    $(addprefix -L, $(LIB_DIR)) \
    $(addprefix -l, $(LIBS))


SOURCES = \
	math/vec2 \
	math/vec3 \
	math/vec4 \
	math/mat4 \
	math/color \
	graphics/shader \
	logging/logging \
	logging/handler \
	service/str_id \
	service/shader_library \
	utils \

TESTCPP = \
	str_id \
	bit_masks \
	float \
	vec2 \
	vec3 \
	vec4 \
	mat4 \
	color \
	shader_library \

BIN_DIR   := ../bin
BUILD_DIR := ../build
SRC_DIR   := ../src
SUB_DIR   := test

OBJECTS   := $(addprefix $(BUILD_DIR)/debug/, $(addsuffix .o,   $(SOURCES)))
SOURCES   := $(addprefix $(SRC_DIR)/,         $(addsuffix .cpp, $(SOURCES)))

TESTOBJ   := $(addprefix $(BUILD_DIR)/$(SUB_DIR)/, $(addsuffix .o, $(TESTCPP)))
TESTCPP   := $(addsuffix .cpp, $(TESTCPP))

TEST_EXE  := $(BIN_DIR)/$(SUB_DIR)/$(PROJECT)

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

# 	@echo "OBJECTS:"
# 	@echo $(OBJECTS)

# 	@echo "TESTOBJ:"
# 	@echo $(TESTOBJ)

postbuild:

clean:
	find $(BIN_DIR) -type f -name $(notdir $(TEST_EXE)) -delete
	find $(BUILD_DIR) -type f -name '*.o' -delete

# ================= PROJECT ================= #

# Links test executable
$(TEST_EXE): $(OBJECTS) $(TESTOBJ)
	g++ $(OBJECTS) $(TESTOBJ) -o $(TEST_EXE) $(CXXFLAGS) $(LDFLAGS)


-include $(TESTOBJ:.o=.d)

# Builds tests' object files
$(BUILD_DIR)/$(SUB_DIR)/%.o: %.cpp ./Makefile
	@mkdir -p $(dir $@)
	@g++ -MM -MT "$@" $(CXXFLAGS) $< > $(BUILD_DIR)/$(SUB_DIR)/$*.d
	g++ $< -c -o $@ $(CXXFLAGS)

-include $(OBJECTS:.o=.d)

# Builds app's object files
$(BUILD_DIR)/debug/%.o: $(SRC_DIR)/%.cpp ./Makefile
	@mkdir -p $(dir $@)
	@g++ -MM -MT "$@" $(CXXFLAGS) $< > $(BUILD_DIR)/debug/$*.d
	g++ $< -c -o $@ $(CXXFLAGS)
