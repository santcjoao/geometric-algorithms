import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

plt.rcParams.update({
    'font.family': 'sans-serif',
    'font.size': 11,
    'axes.labelsize': 12,
    'axes.titlesize': 13,
    'xtick.labelsize': 10,
    'ytick.labelsize': 10,
    'legend.fontsize': 10,
    'figure.titlesize': 14,
    'text.usetex': False
})

CSV_PATH = "benchmarks/raw/benchmark_results.csv"
OUTPUT_DIR = "benchmarks/plots"
os.makedirs(OUTPUT_DIR, exist_ok=True)

def main():
    if not os.path.exists(CSV_PATH):
        print(f"[-] Erro: O arquivo {CSV_PATH} não foi localizado. Execute 'make profile' primeiro.")
        return

    df = pd.read_csv(CSV_PATH)
    
    df['Dimensão'] = df['Mode'].map({2: '2D', 3: '3D'})
    df['Estrutura'] = df['Container'].map({0: 'cg::Array', 1: 'std::vector'})

    print("[+] Gerando gráficos de análise...")

    # -------------------------------------------------------------------------
    # PLOT 1: Tempo de Consulta (Naive vs Range Tree)
    # -------------------------------------------------------------------------
    fig, ax = plt.subplots(figsize=(8, 5))
    df_r3 = df[df['Mode'] == 3]
    
    sns.lineplot(data=df_r3, x='N', y='NaiveMs', marker='o', label='Busca Linear (Naive)', 
                 color='#b22222', linewidth=2, ax=ax)
    sns.lineplot(data=df_r3, x='N', y='QueryMs', marker='s', label='Range Tree', 
                 color='#1f77b4', linewidth=2, ax=ax)
    
    ax.set_yscale('log')
    ax.set_title("Tempo de Consulta em 3D", pad=15)
    ax.set_xlabel("Número de Pontos (N)")
    ax.set_ylabel("Tempo de Busca (ms) [Escala Log]")
    ax.grid(True, which="both", ls="--", alpha=0.6)
    ax.legend(loc="upper left", frameon=True)
    
    plt.tight_layout()
    plt.savefig(f"{OUTPUT_DIR}/01_tempo_consulta.png", dpi=300)
    plt.close()

    # -------------------------------------------------------------------------
    # PLOT 2: Tempo de Construção da Árvore
    # -------------------------------------------------------------------------
    fig, ax = plt.subplots(figsize=(8, 5))
    
    sns.lineplot(data=df, x='N', y='BuildMs', hue='Dimensão', style='Estrutura', 
                 markers=True, dashes=True, linewidth=2, palette="Blues_r", ax=ax)
    
    ax.set_title("Tempo de Construção (Build) da Range Tree", pad=15)
    ax.set_xlabel("Número de Pontos (N)")
    ax.set_ylabel("Tempo de Construção (ms)")
    ax.grid(True, ls="--", alpha=0.6)
    ax.legend(loc="upper left", frameon=True)
    
    plt.tight_layout()
    plt.savefig(f"{OUTPUT_DIR}/02_tempo_construcao.png", dpi=300)
    plt.close()

    # -------------------------------------------------------------------------
    # PLOT 3: Uso de Memória (Peak RSS)
    # -------------------------------------------------------------------------
    fig, ax = plt.subplots(figsize=(8, 5))
    df['PeakRssMb'] = df['PeakRssKb'] / 1024.0
    
    max_n = df['N'].max()
    sns.barplot(data=df[df['N'] == max_n], x='Dimensão', y='PeakRssMb', hue='Estrutura', 
                palette="bone", ax=ax)
    
    ax.set_title(f"Uso de Memória com Carga Máxima (N = {max_n})", pad=15)
    ax.set_xlabel("Dimensão do Espaço")
    ax.set_ylabel("Memória Consumida (MB)")
    ax.grid(True, ls="--", alpha=0.4, axis='y')
    ax.legend(loc="lower right", frameon=True)
    
    plt.tight_layout()
    plt.savefig(f"{OUTPUT_DIR}/03_uso_memoria.png", dpi=300)
    plt.close()

    print(f"[+] Gráficos salvos com sucesso em: '{OUTPUT_DIR}/'.")

if __name__ == "__main__":
    main()