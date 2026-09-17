# Análise teórica dos algoritmos

Esta pasta reúne as análises matemáticas dos quatro algoritmos do trabalho. Cada documento deve explicar a contagem de operações, deduzir a função de custo e analisar o melhor caso, o caso médio e o pior caso. O conteúdo servirá de base para a parte teórica do relatório final.

## Organização da pasta

| Arquivo | Conteúdo |
| --- | --- |
| [inversao.md](inversao.md) | Análise da inversão do vetor. |
| [busca_sequencial.md](busca_sequencial.md) | Análise da busca sequencial. |
| [busca_binaria_iterativa.md](busca_binaria_iterativa.md) | Análise da busca binária iterativa. |
| [busca_binaria_recursiva.md](busca_binaria_recursiva.md) | Análise da busca binária recursiva, incluindo as recorrências. |

Cada integrante deve desenvolver a análise do algoritmo sob sua responsabilidade, seguindo a convenção e a estrutura descritas abaixo. As contagens exatas devem corresponder à implementação analisada em [src/algoritmos.c](../src/algoritmos.c).

## Convenção de contagem

Adotamos as seguintes notações:

| Notação | Significado |
| --- | --- |
| `n` | Quantidade de elementos do vetor; nas chamadas recursivas, tamanho do intervalo em análise. |
| `C(n)` | Número de comparações executadas. |
| `A(n)` | Número de atribuições executadas. |
| `T(n)` | Custo das operações contadas: `T(n) = C(n) + A(n)`. |

Cada comparação e cada atribuição têm peso 1 neste modelo. `T(n)` é uma contagem de operações, não uma medida de tempo em segundos nem uma contagem de todas as instruções executadas pelo computador.

### Comparações

Contamos cada avaliação de uma expressão de comparação, incluindo condições de laços e de decisões. Exemplos: `i < n / 2`, `inicio > fim` e `v[meio] == p`.

A avaliação final que encerra um laço também deve ser contada. Em condições compostas com `&&` ou `||`, contamos apenas as comparações efetivamente avaliadas, respeitando o curto-circuito da linguagem C.

### Atribuições

Contamos inicializações e alterações de variáveis ou posições do vetor. Exemplos:

- `int meio = inicio + (fim - inicio) / 2`: uma atribuição;
- `i = 0` e `j = n - 1`: duas atribuições;
- `temp = v[i]`, `v[i] = v[j]` e `v[j] = temp`: três atribuições;
- `i++` e `j--`: uma atribuição cada.

### Operações excluídas

Não contamos separadamente:

- declarações sem inicialização;
- operações aritméticas e acessos ao vetor;
- passagem de parâmetros e instruções `return`;
- atualizações dos próprios contadores da instrumentação;
- preparação da entrada, alocação do vetor, medição de tempo, impressão e gravação dos resultados realizadas pelo programa auxiliar.

Essas exclusões definem o modelo de contagem usado no trabalho. A mesma convenção deve ser aplicada aos quatro algoritmos e às respectivas versões instrumentadas.

## Estrutura de cada documento

### 1. Objetivo e entrada

Explique o que o algoritmo faz, seus parâmetros, o significado de `n` e o resultado produzido. Declare as condições necessárias à execução, como o vetor ordenado nas buscas binárias.

### 2. Código analisado

Apresente a função original, sem os contadores, em um bloco de código C. O trecho deve corresponder à versão usada nos experimentos. Sempre que a implementação mudar, revise as contagens e as fórmulas.

### 3. Contagem das operações

Monte uma tabela com a operação, seu tipo, a quantidade de execuções e a justificativa. Separe inicializações, condições, corpo dos laços, atualizações e casos de parada. Indique a qual caso cada contagem se refere.

### 4. Melhor caso, caso médio e pior caso

Descreva as entradas que caracterizam cada caso e explique como elas afetam a execução.

No caso médio, declare as hipóteses de probabilidade: por exemplo, uma busca bem-sucedida em um vetor de elementos distintos, com cada elemento igualmente provável. Se forem consideradas buscas malsucedidas, explique também sua probabilidade e a distribuição das chaves. O caso médio não é, em geral, a média aritmética entre o melhor e o pior caso.

Na inversão, os valores do vetor não alteram a quantidade de iterações; para um mesmo `n`, os três casos têm a mesma contagem.

### 5. Dedução da função de custo

Desenvolva `C(n)` e `A(n)` e obtenha `T(n) = C(n) + A(n)`. Mostre as etapas do cálculo, usando somatórios ou recorrências quando necessário.

Para algoritmos recursivos, defina o caso-base, o custo de cada chamada e o tamanho do subproblema. Distinga o número de chamadas com intervalo não vazio da chamada final com intervalo vazio.

Use arredondamentos quando necessários. Na inversão, por exemplo, o número de iterações é `floor(n / 2)`, em que `floor` significa arredondar para baixo. A expressão `n / 2`, interpretada matematicamente sem arredondamento, só é exata para `n` par.

### 6. Termo dominante e complexidade

Identifique o termo que determina o crescimento da função de custo e justifique a classificação em Grande O para cada caso. Quando apropriado, use também Theta (`Θ`) para indicar uma ordem de crescimento justa.

A classificação deve resultar da dedução matemática. Os tempos medidos ajudam a comparar teoria e prática, mas não provam a complexidade assintótica.

### 7. Conferência com os experimentos

Calcule os valores previstos pelas fórmulas para os tamanhos utilizados no trabalho: 1.000, 50.000, 100.000, 500.000 e 1.000.000 elementos. Compare-os com as contagens dos CSVs em [data/](../data/), indicando a entrada e o caso testados.

As contagens devem representar uma execução do algoritmo. O número de repetições usado para medir o tempo médio não deve multiplicar essas contagens na comparação com a teoria.

Se houver divergência, revise a fórmula, a implementação, a instrumentação e a entrada utilizada antes de concluir a análise.

## Relação com as demais pastas

- [src/](../src/): implementações originais que fundamentam a análise.
- [experiments/](../experiments/): programas auxiliares de medição de tempo e contagem de operações.
- [data/](../data/): resultados experimentais em CSV.
- [graphs/](../graphs/): gráficos usados na apresentação e discussão dos resultados.
- [report/](../report/): relatório que reúne a análise teórica, a metodologia experimental e a discussão dos resultados.

## Revisão antes da integração ao relatório

- [ ] O código apresentado corresponde à implementação utilizada nos experimentos.
- [ ] As contagens seguem a convenção comum do grupo.
- [ ] As avaliações finais das condições e os casos-base foram considerados.
- [ ] Melhor caso, caso médio e pior caso estão explicados.
- [ ] As hipóteses do caso médio estão explícitas.
- [ ] As fórmulas de `C(n)`, `A(n)` e `T(n)` foram deduzidas e seus domínios estão claros.
- [ ] O termo dominante e a classificação em Grande O foram justificados.
- [ ] As contagens teóricas foram comparadas com os resultados experimentais disponíveis.
