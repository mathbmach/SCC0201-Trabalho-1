"""
Gera os graficos do Trabalho 1 a partir dos CSVs da pasta data/.

Execucao, a partir da raiz do projeto:

    python graphs/gerar_graficos.py

Requer apenas matplotlib. Os arquivos .png sao gravados em graphs/.
Sempre que os experimentos forem reexecutados, basta rodar este script
novamente para atualizar todas as figuras.

As figuras nao trazem texto explicativo dentro da area do grafico: toda a
leitura fica no relatorio, logo abaixo de cada grafico.
"""

import csv
import math
import os

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter

RAIZ = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DATA = os.path.join(RAIZ, "data")
SAIDA = os.path.join(RAIZ, "graphs")

# arquivo CSV -> (rotulo na legenda, cor, marcador)
ALGORITMOS = [
    ("resultados_inversao.csv",           "Inversao do vetor",       "#1f77b4", "o"),
    ("resultados_busca_sequencial.csv",   "Busca sequencial",        "#d62728", "s"),
    ("resultados_binaria_iterativa.csv",  "Busca binaria iterativa", "#2ca02c", "^"),
    ("resultados_binaria_recursiva.csv",  "Busca binaria recursiva", "#9467bd", "D"),
]

LINEARES = ALGORITMOS[:2]
BINARIAS = ALGORITMOS[2:]

# Fontes maiores: as figuras entram no relatorio reduzidas a 95% da
# largura do texto, e com os tamanhos padrao os rotulos ficam pequenos.
plt.rcParams.update({
    "font.size": 13,
    "axes.titlesize": 15,
    "axes.labelsize": 13,
    "xtick.labelsize": 12,
    "ytick.labelsize": 12,
    "legend.fontsize": 12,
})


def ler(arquivo):
    """Le um CSV de resultados e devolve as colunas como listas de numeros."""
    caminho = os.path.join(DATA, arquivo)
    ns, tempos, comps, atribs = [], [], [], []
    with open(caminho, newline="", encoding="utf-8") as f:
        for linha in csv.DictReader(f):
            ns.append(int(linha["n"]))
            tempos.append(float(linha["tempo_medio_s"]))
            comps.append(int(linha["comparacoes"]))
            atribs.append(int(linha["atribuicoes"]))
    return ns, tempos, comps, atribs


DADOS = {arq: ler(arq) for arq, _, _, _ in ALGORITMOS}

TAMANHOS = DADOS["resultados_inversao.csv"][0]


def milhar(x, _=None):
    """Formata um numero inteiro com ponto como separador de milhar."""
    return f"{int(x):,}".replace(",", ".")


def base(titulo, ylabel, logx=True, logy=False, marcar_tamanhos=True):
    fig, ax = plt.subplots(figsize=(9, 5.2))
    ax.set_title(titulo, pad=12)
    ax.set_xlabel("Tamanho da entrada n (numero de elementos)", labelpad=8)
    ax.set_ylabel(ylabel)
    if logx:
        ax.set_xscale("log")
        ax.minorticks_off()
    if marcar_tamanhos:
        # Marca o eixo exatamente nos cinco tamanhos do experimento. Os
        # rotulos vao inclinados porque 500.000 e 1.000.000 ficam muito
        # proximos em escala logaritmica e se sobrepoem na horizontal.
        ax.set_xticks(TAMANHOS)
    ax.get_xaxis().set_major_formatter(FuncFormatter(milhar))
    for rotulo in ax.get_xticklabels():
        rotulo.set_rotation(30)
        rotulo.set_horizontalalignment("right")
    if logy:
        ax.set_yscale("log")
    ax.grid(True, which="both", linestyle=":", linewidth=0.7, alpha=0.7)
    return fig, ax


def salvar(fig, nome):
    fig.tight_layout()
    caminho = os.path.join(SAIDA, nome)
    fig.savefig(caminho, dpi=160, bbox_inches="tight")
    plt.close(fig)
    print("gravado:", os.path.relpath(caminho, RAIZ))


# ----------------------------------------------------------------------
# Grafico 1. Tempo medio por execucao, os quatro algoritmos
# ----------------------------------------------------------------------
fig, ax = base(
    "Grafico 1: tempo medio por execucao no pior caso",
    "Tempo medio por execucao (s)",
    logx=True,
    logy=True,
)
for arq, rotulo, cor, marca in ALGORITMOS:
    ns, tempos, _, _ = DADOS[arq]
    ax.plot(ns, tempos, marker=marca, color=cor, label=rotulo, linewidth=1.8)
ax.legend()
salvar(fig, "g1_tempo_medio.png")


# ----------------------------------------------------------------------
# Grafico 2. Comparacoes, os quatro algoritmos
# ----------------------------------------------------------------------
fig, ax = base(
    "Grafico 2: comparacoes por execucao no pior caso",
    "Comparacoes em uma execucao",
    logx=True,
    logy=True,
)
for arq, rotulo, cor, marca in ALGORITMOS:
    ns, _, comps, _ = DADOS[arq]
    ax.plot(ns, comps, marker=marca, color=cor, label=rotulo, linewidth=1.8)
ax.legend()
salvar(fig, "g2_comparacoes.png")


# ----------------------------------------------------------------------
# Grafico 3. Atribuicoes, os quatro algoritmos
# ----------------------------------------------------------------------
fig, ax = base(
    "Grafico 3: atribuicoes por execucao no pior caso",
    "Atribuicoes em uma execucao",
    logx=True,
    logy=True,
)
for arq, rotulo, cor, marca in ALGORITMOS:
    ns, _, _, atribs = DADOS[arq]
    ax.plot(ns, atribs, marker=marca, color=cor, label=rotulo, linewidth=1.8)
ax.legend()
salvar(fig, "g3_atribuicoes.png")


# ----------------------------------------------------------------------
# Grafico 4. Algoritmos lineares, eixo x linear
# ----------------------------------------------------------------------
fig, ax = base(
    "Grafico 4: algoritmos lineares, operacoes contadas",
    "Operacoes em uma execucao (milhoes)",
    logx=False,
)
for arq, rotulo, cor, marca in LINEARES:
    ns, _, comps, atribs = DADOS[arq]
    ax.plot(ns, [c / 1e6 for c in comps], marker=marca, color=cor,
            label=f"{rotulo} - comparacoes", linewidth=1.8)
    ax.plot(ns, [a / 1e6 for a in atribs], marker=marca, color=cor,
            label=f"{rotulo} - atribuicoes", linewidth=1.8, linestyle="--")
ax.legend()
salvar(fig, "g4_lineares_escala_linear.png")


# ----------------------------------------------------------------------
# Grafico 5. Buscas binarias: operacoes, eixo x logaritmico
# ----------------------------------------------------------------------
fig, ax = base(
    "Grafico 5: buscas binarias, operacoes por execucao",
    "Operacoes em uma execucao",
    logx=True,
)
for arq, rotulo, cor, marca in BINARIAS:
    ns, _, comps, atribs = DADOS[arq]
    ax.plot(ns, comps, marker=marca, color=cor,
            label=f"{rotulo} - comparacoes", linewidth=1.8)
    ax.plot(ns, atribs, marker=marca, color=cor,
            label=f"{rotulo} - atribuicoes", linewidth=1.8, linestyle="--")
ax.legend()
salvar(fig, "g5_binarias_operacoes.png")


# ----------------------------------------------------------------------
# Grafico 6. Buscas binarias: operacoes, eixo x LINEAR
#
# O eixo x logaritmico transforma o crescimento logaritmico em uma reta.
# Aqui o eixo x e linear, para que a forma da curva apareca: subida rapida
# no inicio e achatamento progressivo.
#
# A curva teorica C(n) = 3k + 1, com k = floor(log2 n) + 1, e desenhada
# para todos os n de 1 a 1.000.000, o que produz os degraus: cada degrau
# corresponde a uma duplicacao de n. Os cinco pontos medidos sao marcados
# sobre ela.
# ----------------------------------------------------------------------
fig, ax = base(
    "Grafico 6: busca binaria, comparacoes em escala linear",
    "Comparacoes em uma execucao",
    logx=False,
    marcar_tamanhos=False,
)

n_denso = list(range(1, 1000001, 200))
c_denso = [3 * (int(math.log2(n)) + 1) + 1 for n in n_denso]
ax.plot(n_denso, c_denso, color="#2ca02c", linewidth=1.8,
        label="Curva teorica C(n) = 3k + 1, k = piso(log2 n) + 1")

ns_i, _, comps_i, _ = DADOS["resultados_binaria_iterativa.csv"]
ax.plot(ns_i, comps_i, marker="^", color="#1a5c1a", linestyle="none",
        markersize=9, label="Valores medidos nos cinco tamanhos")

ax.set_xlim(left=-20000, right=1020000)
ax.set_ylim(bottom=0)
ax.set_xticks([0, 200000, 400000, 600000, 800000, 1000000])
for rotulo in ax.get_xticklabels():
    rotulo.set_rotation(30)
    rotulo.set_horizontalalignment("right")
ax.legend(loc="lower right")
salvar(fig, "g6_binarias_escala_linear.png")


# ----------------------------------------------------------------------
# Grafico 7. Buscas binarias: tempo medio
# ----------------------------------------------------------------------
fig, ax = base(
    "Grafico 7: buscas binarias, tempo medio por busca",
    "Tempo medio por busca (ns)",
    logx=True,
)
for arq, rotulo, cor, marca in BINARIAS:
    ns, tempos, _, _ = DADOS[arq]
    ax.plot(ns, [t * 1e9 for t in tempos], marker=marca, color=cor,
            label=rotulo, linewidth=1.8)
ax.set_ylim(bottom=0)
ax.legend()
salvar(fig, "g7_binarias_tempo.png")

print("\nTodos os graficos foram gerados em graphs/.")
