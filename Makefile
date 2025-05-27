CXX := g++
CXX_ARM := arm-linux-gnueabihf-g++

COMMON_INC_DIRS := $(shell find common -type d)
YGG_INC_DIRS := $(shell find yggdrasill -type d)
GREIN_INC_DIRS := $(shell find grein -type d)

CXXFLAGS := -std=c++17 -Wall -Wextra \
	$(addprefix -I, $(COMMON_INC_DIRS)) \
	$(addprefix -I, $(YGG_INC_DIRS)) \
	$(addprefix -I, $(GREIN_INC_DIRS))

LDFLAGS_X86 := -lhidapi-hidraw
LDFLAGS_ARM := -static

BUILD_DIR := build
BIN_DIR := $(BUILD_DIR)/bin

# Папки с исходниками common для каждого таргета
COMMON_DIRS_YGG := common
COMMON_DIRS_GREIN := common/rot-packet common/protocols

# Собираем списки .cpp по папкам для каждого таргета
COMMON_SRCS_YGG := $(foreach dir,$(COMMON_DIRS_YGG),$(shell find $(dir) -name '*.cpp'))
COMMON_SRCS_GREIN := $(foreach dir,$(COMMON_DIRS_GREIN),$(shell find $(dir) -name '*.cpp'))

YGG_SRCS := $(shell find yggdrasill -name '*.cpp')
GREIN_SRCS := $(shell find grein -name '*.cpp')

COMMON_OBJS_YGG := $(patsubst %.cpp, $(BUILD_DIR)/x86/%.o, $(COMMON_SRCS_YGG))
COMMON_OBJS_GREIN := $(patsubst %.cpp, $(BUILD_DIR)/arm/%.o, $(COMMON_SRCS_GREIN))
YGG_OBJS := $(patsubst %.cpp, $(BUILD_DIR)/x86/%.o, $(YGG_SRCS))
GREIN_OBJS := $(patsubst %.cpp, $(BUILD_DIR)/arm/%.o, $(GREIN_SRCS))

YGG_TARGET := $(BIN_DIR)/yggdrasill-ctrl
GREIN_TARGET := $(BIN_DIR)/grein-ctrl

all: $(YGG_TARGET) $(GREIN_TARGET)

$(BIN_DIR):
	mkdir -p $@

$(YGG_TARGET): $(BIN_DIR) $(COMMON_OBJS_YGG) $(YGG_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(COMMON_OBJS_YGG) $(YGG_OBJS) $(LDFLAGS_X86)

$(GREIN_TARGET): $(BIN_DIR) $(COMMON_OBJS_GREIN) $(GREIN_OBJS)
	$(CXX_ARM) $(CXXFLAGS) -o $@ $(COMMON_OBJS_GREIN) $(GREIN_OBJS) $(LDFLAGS_ARM)

$(BUILD_DIR)/x86/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/arm/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX_ARM) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean