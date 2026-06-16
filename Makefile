CXX      = g++
CXXFLAGS = -std=c++20 -O3 -Wall
INCLUDES = -Iinclude -Iinclude/core -Iinclude/graphics -Iinclude/physx -Iinclude/util -Isrc -I.

BUILD_DIR = build
TARGET    = $(BUILD_DIR)/main.exe

SOURCES   = src/Main.cpp
UTIL_CPP  = $(wildcard src/util/*.cpp)
PHYSX_CPP = $(wildcard src/physx/*.cpp)
SOURCES  += $(UTIL_CPP) $(PHYSX_CPP)

OBJECTS   = $(SOURCES:%.cpp=$(BUILD_DIR)/%.o)

.PHONY: all clean run format

all: $(TARGET)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "Compilando $<..."
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	@echo "Gerando executável $@..."
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Compilação concluída."

clean:
	@echo "Limpando..."
	rm -rf $(BUILD_DIR)
	@echo "Limpeza concluída."

run: all
	@./$(TARGET)

format:
	@echo "Formatando arquivos..."
	@find . -name "*.cpp" -o -name "*.h" -o -name "*.hpp" | grep -v "/build/" | xargs clang-format -i
	@echo "Formatação concluída."