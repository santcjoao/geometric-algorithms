import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

# Configuração de estilo para os gráficos
sns.set_theme(style="whitegrid")
plt.rcParams.update({'font.size': 11, 'figure.titlesize': 14})

# Caminhos de execução
CSV_PATH = "benchmarks/raw/benchmark_results.csv"
OUTPUT_DIR = "benchmarks/plots"
os.makedirs(OUTPUT_DIR, exist_ok=True)

def main():
    if not os.path.exists(CSV_PATH):
        print(f"[-] Erro: Arquivo {CSV_PATH} não encontrado. Rode 'make profile' primeiro.")
        return

    # Carrega os dados coletados
    df = pd.read_csv(CSV_PATH)
    
    # Mapeamento de labels legíveis
    df['Dimensão'] = df['Mode'].map({2: '2D (R2)', 3: '3D (R3)'})
    df['Coleção'] = df['Container'].map({0: 'cg::Array', 1: 'std::vector'})

    print("[+] Consumindo dados e gerando gráficos analíticos...")

    # -------------------------------------------------------------------------
    # PLOT 1: Tempo de Consulta (Naive vs Range Tree) - Escala Logarítmica em Y
    # -------------------------------------------------------------------------
    plt.figure(figsize=(9, 5))
    df_r3 = df[df['Mode'] == 3]
    
    sns.lineplot(data=df_r3, x='N', y='NaiveMs', marker='o', label='Busca Linear (Naive)', color='crimson', linewidth=2)
    sns.lineplot(data=df_r3, x='N', y='QueryMs', marker='s', label='Range Tree (Query)', color='dodgerblue', linewidth=2)
    
    plt.yscale('log') # Escala logarítmica evidencia a disparidade brutal de tempo
    plt.title("Tempo de Consulta em R3 (Escala Logarítmica em Y)")
    plt.xlabel("Número de Pontos (N)")
    plt.ylabel("Tempo de Processamento (ms)")
    plt.legend(loc="upper left")
    plt.tight_layout()
    plt.savefig(f"{OUTPUT_DIR}/01_tempo_consulta_naive_vs_rt.png", dpi=150)
    plt.close()

    # -------------------------------------------------------------------------
    # PLOT 2: Tempo de Construção da Árvore (Comportamento O(N log^2 N))
    # -------------------------------------------------------------------------
    plt.figure(figsize=(9, 5))
    sns.lineplot(data=df, x='N', y='BuildMs', hue='Dimensão', style='Coleção', marker='X', markersize=8, linewidth=2)
    plt.title("Tempo de Construção (Build) da Range Tree por Dimensão")
    plt.xlabel("Número de Pontos (N)")
    plt.ylabel("Tempo de Construção (ms)")
    plt.tight_layout()
    plt.savefig(f"{OUTPUT_DIR}/02_tempo_construcao_rt.png", dpi=150)
    plt.close()

    # -------------------------------------------------------------------------
    # PLOT 3: Uso de Memória (Peak RSS) por Coleção de Dados
    # -------------------------------------------------------------------------
    plt.figure(figsize=(9, 5))
    # RSS convertido para Megabytes para melhor leitura
    df['PeakRssMb'] = df['PeakRssKb'] / 1024.0
    sns.barplot(data=df[df['N'] == df['N'].max()], x='Dimensão', y='PeakRssMb', hue='Coleção', palette="muted")
    plt.title(f"Pico de Uso de Memória (RSS) na Carga Máxima (N = {df['N'].max()})")
    plt.xlabel("Dimensão do Espaço")
    plt.ylabel("Consumo de Memória (MB)")
    plt.tight_layout()
    plt.savefig(f"{OUTPUT_DIR}/03_consumo_memoria_maxima.png", dpi=150)
    plt.close()

    # -------------------------------------------------------------------------
    # PLOT 4: Simulação Espacial 3D do Domínio e da Caixa de Busca (Região Ortogonal)
    # -------------------------------------------------------------------------
    fig = plt.figure(figsize=(8, 7))
    ax = fig.add_subplot(111, projection='3d')
    
    # Simula uma nuvem sintética representativa de pontos
    np.random.seed(42)
    sample_points = np.random.uniform(-1000, 1000, (800, 3))
    
    # Filtra pontos dentro da caixa hipotética de tamanho 400
    inside = (sample_points[:, 0] >= -200) & (sample_points[:, 0] <= 200) & \
             (sample_points[:, 1] >= -200) & (sample_points[:, 1] <= 200) & \
             (sample_points[:, 2] >= -200) & (sample_points[:, 2] <= 200)
             
    ax.scatter(sample_points[~inside, 0], sample_points[~inside, 1], sample_points[~inside, 2], 
               color='gray', alpha=0.3, s=10, label='Pontos Fora')
    ax.scatter(sample_points[inside, 0], sample_points[inside, 1], sample_points[inside, 2], 
               color='dodgerblue', alpha=0.9, s=25, label='Pontos Capturados')
    
    # Desenha o contorno da Hiper-caixa de consulta centralizada
    r = [-200, 200]
    for s, e in [(0,1), (0,2), (0,4), (1,3), (1,5), (2,3), (2,6), (3,7), (4,5), (4,6), (5,7), (6,7)]:
        # Transforma os vértices do cubo em coordenadas para as linhas
        v = np.array([[x, y, z] for x in r for y in r for z in r])
        ax.plot3D(*zip(v[s], v[e]), color="black", linestyle="--", linewidth=1.5)

    ax.set_title("Visualização Geométrica da Consulta Ortogonal em R3")
    ax.set_xlabel("Eixo X")
    ax.set_ylabel("Eixo Y")
    ax.set_zlabel("Eixo Z")
    ax.legend(loc="upper right")
    plt.tight_layout()
    plt.savefig(f"{OUTPUT_DIR}/04_simulacao_espacial_3d.png", dpi=150)
    plt.close()

    print(f"[+] Sucesso! Todos os gráficos foram exportados e salvos em '{OUTPUT_DIR}/'.")

if __name__ == "__main__":
    main()