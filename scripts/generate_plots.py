import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import sys
from pathlib import Path

# Configuração visual minimalista e acadêmica
plt.rcParams.update({
    "font.family": "serif",
    "font.size": 10,
    "axes.labelsize": 11,
    "axes.titlesize": 11,
    "xtick.labelsize": 9,
    "ytick.labelsize": 9,
    "legend.fontsize": 9,
    "figure.titlesize": 13,
    "axes.spines.top": False,
    "axes.spines.right": False
})

# Paleta de alto contraste
COLOR_NAIVE  = "#555555"  # Cinza Escuro
COLOR_WORST  = "#B22222"  # Firebrick (Vinho saturado)
COLOR_NORMAL = "#00509E"  # YInMn Blue (Azul acadêmico)
COLOR_BEST   = "#2E8B57"  # SeaGreen (Verde limpo)

def linear_puro(x, a): return a * x
def polylog3_puro(x, a): return a * (np.log2(x)**3)

def carregar_dados(path, cenario):
    try:
        df = pd.read_csv(path)
        df['Cenario'] = cenario
        df['Struct'] = df['Struct'].astype(str).str.strip().str.lower()
        return df
    except FileNotFoundError:
        print(f"[ERRO] Arquivo indispensável não encontrado: {path}")
        sys.exit(1)

def extrair(df, dim, struct_type='array'):
    is_struct = df['Struct'].isin(['0', 'array', 'structtype::array']) if struct_type == 'array' else df['Struct'].isin(['1', 'vector', 'structtype::vector'])
    return df[(df['Dim'] == dim) & is_struct].sort_values('N')

def gerar_plots_2d(raw_dir, out_dir):
    out_dir.mkdir(parents=True, exist_ok=True)

    df_w = carregar_dados(raw_dir / "worst.csv", "Worst")
    df_n = carregar_dados(raw_dir / "normal.csv", "Normal")
    df_b = carregar_dados(raw_dir / "best.csv", "Best")

    # =========================================================================
    # IMAGEM 1: Janela de Desempenho
    # =========================================================================
    fig1, (ax1, ax2) = plt.subplots(1, 2, figsize=(11, 4.2))
    fig1.suptitle('Aceleração Espacial: Range Tree vs Busca Linear', fontweight='bold', y=1.02)

    for ax, d in zip([ax1, ax2], [2, 3]):
        n_arr = extrair(df_n, d, 'array')
        if not n_arr.empty:
            x_dados = n_arr['N'].values
            
            ax.plot(x_dados, n_arr['NaiveMs'], 'o-', color=COLOR_NAIVE, linewidth=1.5, label='Naive')
            ax.plot(x_dados, n_arr['RtQueryMs'], 's-', color=COLOR_NORMAL, linewidth=1.5, label='Range Tree')
            
            popt_l, _ = curve_fit(linear_puro, x_dados, n_arr['NaiveMs'], bounds=(0, np.inf))
            ax.plot(x_dados, linear_puro(x_dados, *popt_l), '--', color=COLOR_NAIVE, alpha=0.5, label=r'Teórico $\mathcal{O}(N)$')
            
            popt_p, _ = curve_fit(polylog3_puro, x_dados, n_arr['RtQueryMs'], bounds=(0, np.inf))
            ax.plot(x_dados, polylog3_puro(x_dados, *popt_p), '--', color=COLOR_NORMAL, alpha=0.4, label=r'Teórico $\mathcal{O}(\log^3 N)$')

        ax.set_title(f'Consulta em {d}D')
        ax.set_xlabel('Pontos ($N$)')
        ax.set_ylabel('Tempo ($ms$)')
        ax.set_xscale('log')
        ax.set_yscale('log')
        ax.legend(loc='upper left', frameon=True, facecolor='#F8F9FA', edgecolor='none')
        ax.grid(True, which="both", ls=":", alpha=0.4, color="#CCCCCC")

    fig1.tight_layout()
    fig1.savefig(out_dir / "analysis_01_performance.png", dpi=300, bbox_inches='tight')

    # =========================================================================
    # IMAGEM 2: Sensibilidade Geométrica
    # =========================================================================
    fig2, (ax1, ax2) = plt.subplots(1, 2, figsize=(11, 4.2))
    fig2.suptitle('Sensibilidade Algorítmica da Range Tree', fontweight='bold', y=1.02)

    for ax, d in zip([ax1, ax2], [2, 3]):
        w_arr = extrair(df_w, d, 'array')
        n_arr = extrair(df_n, d, 'array')
        b_arr = extrair(df_b, d, 'array')

        if not w_arr.empty: ax.plot(w_arr['N'], w_arr['RtQueryMs'], 's-', color=COLOR_WORST, label='Pior Caso ($k=N$)')
        if not n_arr.empty: ax.plot(n_arr['N'], n_arr['RtQueryMs'], 'd-', color=COLOR_NORMAL, label='Caso Médio')
        if not b_arr.empty: ax.plot(b_arr['N'], b_arr['RtQueryMs'], '^-', color=COLOR_BEST, label='Melhor Caso ($k=0$)')
        
        ax.set_title(f'Espaço em {d}D')
        ax.set_xlabel('Pontos ($N$)')
        ax.set_ylabel('Tempo ($ms$)')
        ax.set_xscale('log')
        ax.set_yscale('log')
        ax.legend(loc='upper left', frameon=True, facecolor='#F8F9FA', edgecolor='none')
        ax.grid(True, which="both", ls=":", alpha=0.4, color="#CCCCCC")

    fig2.tight_layout()
    fig2.savefig(out_dir / "analysis_02_sensitivity.png", dpi=300, bbox_inches='tight')

    # =========================================================================
    # IMAGEM 3: Engenharia de Software
    # =========================================================================
    fig3, (ax1, ax2) = plt.subplots(1, 2, figsize=(11, 4.2))
    fig3.suptitle('Métricas de Infraestrutura e Amortização (3D)', fontweight='bold', y=1.02)

    w_arr = extrair(df_w, 3, 'array')
    w_vec = extrair(df_w, 3, 'vector')
    
    if not w_arr.empty: ax1.plot(w_arr['N'], w_arr['RtQueryRssKb'] / 1024.0, 'o-', color=COLOR_NORMAL, label='cg::Array')
    if not w_vec.empty: ax1.plot(w_vec['N'], w_vec['RtQueryRssKb'] / 1024.0, 's--', color=COLOR_WORST, alpha=0.8, label='std::vector')
    ax1.set_title('Alocação de Memória RAM')
    ax1.set_xlabel('Pontos ($N$)')
    ax1.set_ylabel('Pico RSS ($MB$)')
    ax1.legend(loc='upper left', frameon=True, facecolor='#F8F9FA', edgecolor='none')
    ax1.grid(True, ls=":", alpha=0.4, color="#CCCCCC")

    n_arr = extrair(df_n, 3, 'array')
    n_vec = extrair(df_n, 3, 'vector')
    
    if not n_arr.empty:
        g_arr = np.where((n_arr['NaiveMs'] - n_arr['RtQueryMs']) <= 0, 0.001, n_arr['NaiveMs'] - n_arr['RtQueryMs'])
        ax2.plot(n_arr['N'], n_arr['RtBuildMs'] / g_arr, 'o-', color=COLOR_NORMAL, label='cg::Array')
    if not n_vec.empty:
        g_vec = np.where((n_vec['NaiveMs'] - n_vec['RtQueryMs']) <= 0, 0.001, n_vec['NaiveMs'] - n_vec['RtQueryMs'])
        ax2.plot(n_vec['N'], n_vec['RtBuildMs'] / g_vec, 's--', color=COLOR_WORST, alpha=0.8, label='std::vector')
        
    ax2.set_title('Consultas para Break-Even')
    ax2.set_xlabel('Pontos ($N$)')
    ax2.set_ylabel('Qtd. Consultas')
    ax2.legend(loc='upper left', frameon=True, facecolor='#F8F9FA', edgecolor='none')
    ax2.grid(True, ls=":", alpha=0.4, color="#CCCCCC")

    fig3.tight_layout()
    fig3.savefig(out_dir / "analysis_03_tradeoff.png", dpi=300, bbox_inches='tight')

    print("\n[+] Gráficos otimizados salvos em: benchmarks/plot/")

if __name__ == "__main__":
    root = Path(__file__).resolve().parent.parent
    gerar_plots_2d(root / "benchmarks" / "raw", root / "benchmarks" / "plot")