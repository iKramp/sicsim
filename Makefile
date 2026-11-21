
# Toolchain
CC  := gcc
CXX := g++
LD  := g++

# Directories
SRC_DIR   := src
BUILD_DIR := build

GLFW_DIR  ?= $(GLFW_DIR)
IMGUI_DIR ?= $(IMGUI_DIR)

OUT := $(BUILD_DIR)/sicsim

CXXFLAGS := \
    -std=c++23 \
    -g \
    -Wall \
    -Wextra \
    -I$(GLFW_DIR)/include \
    -I$(IMGUI_DIR) \
    -I$(IMGUI_DIR)/backends \
	`pkg-config --cflags glfw3 vulkan`

LDFLAGS :=
LDLIBS := -lGL `pkg-config --static --libs glfw3 vulkan`

# Sources
SRCS += $(shell find $(SRC_DIR) -type f -name '*.cpp')

SRCS += \
    $(IMGUI_DIR)/imgui.cpp \
	$(IMGUI_DIR)/imgui_demo.cpp \
    $(IMGUI_DIR)/imgui_draw.cpp \
    $(IMGUI_DIR)/imgui_tables.cpp \
    $(IMGUI_DIR)/imgui_widgets.cpp \
    $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp \
    $(IMGUI_DIR)/backends/imgui_impl_vulkan.cpp

# Object files
OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(filter %.c,$(SRCS)))
OBJS += $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(filter %.cpp,$(SRCS)))

.PHONY: all clean compile_commands

all: $(OUT)

$(OUT): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(OBJS:.o=.d)

clean:
	rm -rf $(BUILD_DIR)

compile_commands:
	$(MAKE) clean
	bear -- $(MAKE) all

