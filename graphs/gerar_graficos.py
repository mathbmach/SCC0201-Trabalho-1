"""
Gera os graficos do Trabalho 1 a partir dos CSVs da pasta data/.

Execucao, a partir da raiz do projeto:

    python graphs/gerar_graficos.py

Requer apenas matplotlib. Os arquivos .png sao gravados em graphs/.
Sempre que os experimentos forem reexecutados, basta rodar este script
novamente para atualizar todas as figuras.
"""

import csv
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
    ("resultados_inversao.csv",           "Inversao do vetor",      "#1f77b4", "o"),
    ("resultados_busca_sequencial.csv",   "Busca sequencial",       "#d62728", "s"),
    ("resultados_binaria_iterativa.csv",  "Busca binaria iterativa", "#2ca02c", "^"),
    ("resultados_binaria_recursiva.csv",  "Busca binaria recursiva", "#9467bd", "D"),
]

LINEARES = ALGORITMOS[:2]
BINARIAS = ALGORITMOS[2:]


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


def base(titulo, ylabel, logx=True, logy=False):
    fig, ax = plt.subplots(figsize=(9, 5.5))
    ax.set_title(titulo, fontsize=13, pad=12)
    ax.set_xlabel("Tamanho da entrada n (numero de elementos)")
    ax.set_ylabel(ylabel)
    if logx:
        ax.set_xscale("log")
        ax.set_xticks(TAMANHOS)
        ax.get_xaxis().set_major_formatter(FuncFormatter(milhar))
        ax.minorticks_off()
    if logy:
        ax.set_yscale("log")
    ax.grid(True, which="both", linestyle=":", linewidth=0.7, alpha=0.7)
    return fig, ax


def salvar(fig, nome):
    fig.tight_layout()
    caminho = os.path.join(SAIDA, nome)
    fig.savefig(caminho, dpi=160)
    plt.close(fig)
    print("gravado:", os.path.relpath(caminho, RAIZ))


# ----------------------------------------------------------------------
# 1. Tempo medio por execucao, os quatro algoritmos
# ----------------------------------------------------------------------
fig, ax = base(
    "Tempo medio por execucao no pior caso (escala log-log)",
    "Tempo medio por execucao (s)",
    logx=True,
    logy=True,
)
for arq, rotulo, cor, marca in ALGORITMOS:
    ns, tempos, _, _ = DADOS[arq]
    ax.plot(ns, tempos, marker=marca, color=cor, label=rotulo, linewidth=1.8)
ax.legend()
ax.text(
    0.01, -0.17,
    "Escala logaritmica nos dois eixos: sem ela, as duas buscas binarias ficariam "
    "coladas no eixo horizontal.",
    transform=ax.transAxes, fontsize=8.5, color="#555555",
)
salvar(fig, "g1_tempo_medio.png")


# ----------------------------------------------------------------------
# 2. Comparacoes, os quatro algoritmos
# ----------------------------------------------------------------------
fig, ax = base(
    "Comparacoes por execucao no pior caso (escala log-log)",
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
# 3. Atribuicoes, os quatro algoritmos
# ----------------------------------------------------------------------
fig, ax = base(
    "Atribuicoes por execucao no pior caso (escala log-log)",
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
# 4. Algoritmos lineares em escala linear
# ----------------------------------------------------------------------
fig, ax = base(
    "Algoritmos lineares: operacoes contadas em escala linear",
    "Operacoes em uma execucao (milhoes)",
    logx=False,
)
for arq, rotulo, cor, marca in LINEARES:
    ns, _, comps, atribs = DADOS[arq]
    ax.plot(ns, [c / 1e6 for c in comps], marker=marca, color=cor,
            label=f"{rotulo} - comparacoes", linewidth=1.8)
    ax.plot(ns, [a / 1e6 for a in atribs], marker=marca, color=cor,
            label=f"{rotulo} - atribuicoes", linewidth=1.8, linestyle="--")
ax.get_xaxis().set_major_formatter(FuncFormatter(milhar))
ax.legend()
ax.text(
    0.01, -0.17,
    "Com o eixo x linear, as retas mostram que dobrar n dobra a quantidade de operacoes.",
    transform=ax.transAxes, fontsize=8.5, color="#555555",
)
salvar(fig, "g4_lineares_escala_linear.png")


# ----------------------------------------------------------------------
# 5. Buscas binarias: operacoes em escala propria
# ----------------------------------------------------------------------
fig, ax = base(
    "Buscas binarias: operacoes por execucao no pior caso",
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
ax.text(
    0.01, -0.17,
    "Grafico separado para que a escala das buscas lineares nao esconda esses valores. "
    "O eixo x e logaritmico; as retas indicam crescimento logaritmico em n.",
    transform=ax.transAxes, fontsize=8.5, color="#555555",
)
salvar(fig, "g5_binarias_operacoes.png")


# ----------------------------------------------------------------------
# 6. Buscas binarias: tempo medio em escala propria
# ----------------------------------------------------------------------
fig, ax = base(
    "Buscas binarias: tempo medio por busca no pior caso",
    "Tempo medio por busca (nanossegundos)",
    logx=True,
)
for arq, rotulo, cor, marca in BINARIAS:
    ns, tempos, _, _ = DADOS[arq]
    ax.plot(ns, [t * 1e9 for t in tempos], marker=marca, color=cor,
            label=rotulo, linewidth=1.8)
ax.set_ylim(bottom=0)
ax.legend()
ax.text(
    0.01, -0.17,
    "Tempos da ordem de dezenas de nanossegundos: diferencas pequenas sofrem influencia "
    "do relogio, do cache e do laco de medicao.",
    transform=ax.transAxes, fontsize=8.5, color="#555555",
)
salvar(fig, "g6_binarias_tempo.png")

print("\nTodos os graficos foram gerados em graphs/.")
