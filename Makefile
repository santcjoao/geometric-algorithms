CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O3 -I. -Iinclude -Iinclude/core -Iinclude/graphics -Iinclude/util

BUILD_DIR := build
BIN_DIR := $(BUILD_DIR)/bin
OBJ_DIR := $(BUILD_DIR)/obj
TARGET := $(BIN_DIR)/p2_exec
VPATH := src

SRCS := Main.cpp OBJReader.cpp TriangleMesh.cpp
OBJS := $(SRCS:%.cpp=$(OBJ_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	@echo "Linkando: $@"
	@$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@echo "Compilando: $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $@

clean:
	@rm -rf $(BUILD_DIR)

run: $(TARGET)
	@cd $(BIN_DIR) && ./p2_exec

.PHONY: all clean run