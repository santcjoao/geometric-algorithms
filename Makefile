CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O3 -I. -Iinclude -Iinclude/core -Iinclude/graphics -Iinclude/util

BUILD_DIR := build
TARGET := $(BUILD_DIR)/p2_exec
OBJ_DIR := $(BUILD_DIR)/obj
VPATH := src

SRCS := Main.cpp OBJReader.cpp TriangleMesh.cpp
OBJS := $(SRCS:%.cpp=$(OBJ_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
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
	@./$(TARGET)

.PHONY: all clean run