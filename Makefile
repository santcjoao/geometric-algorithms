# Compilador e flags
CXX      = g++
CXXFLAGS = -std=c++20 -O3 -Wall
INCLUDES = -Iinclude -Iinclude/physx -I.
INCLUDES = -Iinclude -I.
#INCLUDES = -Iinclude -Iinclude/graphics -Iinclude/util -Iinclude/physx -Iinclude/src

# Diretórios
BUILD_DIR = build
TARGET    = $(BUILD_DIR)/main.exe

# Sources: Main.cpp + todos os .cpp em include/util (e outras pastas se necessário)
# Se houver outros .cpp (ex: em include/physx), adicione-os aqui.
SOURCES   = Main.cpp
UTIL_CPP  = $(wildcard include/util/*.cpp)
SOURCES  += $(UTIL_CPP)

OBJECTS   = $(SOURCES:%.cpp=$(BUILD_DIR)/%.o)
# Para fontes dentro de subpastas, precisamos mapear os .o corretamente.
# Como os .cpp de util estão em include/util/, seus objetos ficarão em build/include/util/*.o
# Vamos ajustar a regra de compilação para criar a estrutura de diretórios.

.PHONY: all clean run

all: $(TARGET)

# Regra para compilar qualquer .cpp, mantendo a estrutura de pastas em build/
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -rf $(BUILD_DIR)

run: all
	@./$(TARGET)

# Opção: executar com argumentos extras (ex: make run args="--help")
# Descomente se quiser:
# run: all
#	@./$(TARGET) $(args)