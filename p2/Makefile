# ==========================================
# Configurações do Compilador
# ==========================================
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O3 -I. -Iinclude

# ==========================================
# Estrutura de Diretórios
# ==========================================
SRC_DIR = src
OBJ_DIR = obj

# VPATH instrui o Make a buscar código-fonte em múltiplos diretórios.
# Ele olha primeiro na raiz (onde está o Main.cpp) e depois em src/.
VPATH = $(SRC_DIR)

# Nome do executável final
TARGET = p2_exec

# ==========================================
# Arquivos do Projeto
# ==========================================
# Listamos apenas o nome dos arquivos .cpp. O VPATH descobrirá onde eles estão.
SRCS = OBJReader.cpp \
       TriangleMesh.cpp \
       Main.cpp

# Transforma a lista de .cpp em caminhos obj/.o
OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# ==========================================
# Regras de Construção
# ==========================================

all: $(TARGET)

# Geração do executável
$(TARGET): $(OBJS)
	@echo "Linkando o executável: $@"
	@$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Build concluído com sucesso!"

# Regra unificada para compilar os .o
# O $< pega automaticamente o caminho correto (seja "./Main.cpp" ou "src/OBJReader.cpp")
$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@echo "Compilando: $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Cria a pasta obj/
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# ==========================================
# Regras Utilitárias
# ==========================================

clean:
	@echo "Limpando arquivos compilados..."
	@rm -rf $(OBJ_DIR) $(TARGET)

run: $(TARGET)
	@echo "Executando $(TARGET)..."
	@echo "----------------------------------------"
	@./$(TARGET)

.PHONY: all clean run