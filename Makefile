# Compilação
CXX      = g++
CXXFLAGS = -std=c++20 -O3 
INCLUDES = -I../include -I.

BUILD_DIR = build
RAW_DIR   = benchmarks/raw
TARGET    = $(BUILD_DIR)/main.exe

SOURCES   = Main.cpp
OBJECTS   = $(SOURCES:%.cpp=$(BUILD_DIR)/%.o)

# Parâmetros Padrão (Run)
algo   ?= range
func   ?= all
struct ?= array
dim    ?= 3
count  ?= 50k
box    ?= 200.0
seed   ?= 42

# Parâmetros Padrão (Profile)
start        ?= 10k
end          ?= 100k
stride       ?= 10k
out          ?= profiling.csv
profile_algo ?= both

# Configuração de Argumentos CLI
BASE_ARGS = func=$(func) struct=$(struct) dim=$(dim) box=$(box)
ifneq ($(seed),)
    BASE_ARGS += seed=$(seed)
endif

RUN_ARGS     = algo=$(algo) count=$(count) $(BASE_ARGS)
PROFILE_ARGS = algo=$(profile_algo) start=$(start) end=$(end) stride=$(stride) $(BASE_ARGS)

# Regras Principais
.PHONY: all clean run profile profile-all help

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) benchmarks

# Execução e Profiling
run: all
	@./$(TARGET) $(RUN_ARGS) $(args)

profile: all
	@mkdir -p $(RAW_DIR)
	@./$(TARGET) $(PROFILE_ARGS) $(args) > $(RAW_DIR)/$(out)
	@echo "[+] Amostragem salva em $(RAW_DIR)/$(out)"

define rodar_matriz
	@echo "N,Dim,Box,Struct,Match,NaiveMs,RtBuildMs,RtQueryMs,RtQueryRssKb" > $(RAW_DIR)/$(1).csv
	@for d in 2 3; do \
		for s in array vector; do \
			echo "[->] [Cenario: $(1)] Dimensao $$d | Estrutura $$s..."; \
			./$(TARGET) algo=both func=$(2) start=100k end=1000k stride=100k box=$(3) dim=$$d struct=$$s seed=123456789 header=false >> $(RAW_DIR)/$(1).csv; \
		done \
	done
endef

profile-all: all
	@mkdir -p $(RAW_DIR)
	@echo "[*] Perfilamento Trilinear (100k a 1M)..."
	$(call rodar_matriz,worst,all,2000.0)
	$(call rodar_matriz,normal,all,200.0)
	$(call rodar_matriz,best,all,0.0)
	@echo "[+] Matrizes geradas em $(RAW_DIR)/"

help:
	@echo "make run algo=both func=all count=150k dim=2"
	@echo "make profile start=10k end=100k stride=10k out=analise.csv"