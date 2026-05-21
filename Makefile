# Configurações do Compilador
CXX = g++
CXXFLAGS = -std=c++20 -O3 -Wall
INCLUDES = -I../include -I.

# Definição de Diretórios de Saída
BUILD_DIR = build
RAW_DIR = benchmarks/raw

# Configurações dos Alvos e Artefatos
TARGET = $(BUILD_DIR)/benchmark.exe
SOURCES = Main.cpp
OBJECTS = $(SOURCES:%.cpp=$(BUILD_DIR)/%.o)
CSV_OUT = $(RAW_DIR)/benchmark_results.csv

.PHONY: all clean run profile plot

all: $(TARGET)

# Linkagem do Executável final dentro da pasta build
$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

# Compilação dos objetos dentro da pasta build
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Execução de teste rápido
run: all
	./$(TARGET) 3 50000 250.0 0 0 42
	
# Limpeza
clean:
	rm -rf $(BUILD_DIR)
	rm -rf benchmarks

# Executa o perfilamento
profile: all
	@mkdir -p $(RAW_DIR)
	@echo "Mode,Container,N,BoxSize,Seed,NaiveCount,RTCount,Correct,NaiveMs,BuildMs,QueryMs,PeakRssKb" > $(CSV_OUT)
	@echo "Executando perfilamento (R2 e R3)..."
	@for mode in 2 3; do \
		for cont in 0 1; do \
			for n in 10000 25000 50000 100000 150000 200000 500000 1000000; do \
				./$(TARGET) $$mode $$n 250.0 $$cont 1 999 >> $(CSV_OUT); \
			done \
		done \
	done
	@echo "Perfilamento concluído. Resultados das iterações salvos em $(CSV_OUT)"


