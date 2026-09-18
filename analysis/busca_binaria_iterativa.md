# Busca binária iterativa — análise teórica

## 1. Objetivo e entrada

A busca binária iterativa verifica se um valor está presente em um vetor ordenado em ordem crescente. A ideia é comparar o valor procurado com o elemento central do intervalo de busca: se forem iguais, a busca termina; se o elemento central for menor, o valor só pode estar na metade direita; se for maior, só pode estar na metade esquerda. Assim, cada repetição descarta aproximadamente metade dos elementos restantes.

A diferença em relação à versão recursiva está apenas na forma de repetir: aqui, o intervalo é reduzido atualizando duas variáveis dentro de um laço, em vez de passar novos limites para uma chamada da própria função.

A função recebe:

- `v`: o vetor em que a busca será feita;
- `n`: a quantidade de elementos do vetor;
- `valor`: o valor procurado.

O retorno é `1` quando o valor é encontrado e `0` quando ele não está no vetor.

Internamente, o intervalo de busca é delimitado por `inicio` e `fim`, ambos inclusivos, e começa em `[0, n - 1]`. Enquanto o intervalo não estiver vazio, seu tamanho é `fim - inicio + 1`. O intervalo vazio é representado por `inicio > fim`, que encerra o laço.

A análise considera que o vetor já está ordenado, então o custo de ordenação não entra nas contas. **O algoritmo só está correto sob essa condição.**

## 2. Código analisado

O código abaixo corresponde à função em `src/algoritmos.c`:

```c
int busca_binaria_iterativa(int *v, int n, int valor)
{
    int inicio = 0;
    int fim = n - 1;
    int meio;

    while (inicio <= fim)
    {
        meio = inicio + (fim - inicio) / 2;

        if (v[meio] == valor)
        {
            return 1;
        }

        if (v[meio] < valor)
        {
            inicio = meio + 1;
        }
        else
        {
            fim = meio - 1;
        }
    }

    return 0;
}
```

A expressão `inicio + (fim - inicio) / 2` é matematicamente igual a `(inicio + fim) / 2` para índices não negativos, mas evita o estouro do tipo `int` que a soma direta poderia provocar com índices grandes. É a mesma forma usada na versão recursiva.

## 3. Contagem das operações

Seguindo a convenção do `README.md` desta pasta, usamos:

- `C(n)` para o número de comparações;
- `A(n)` para o número de atribuições;
- `T(n) = C(n) + A(n)` para o custo total das operações contadas.

Esse custo não é o tempo em segundos. Não contamos separadamente as operações aritméticas, os acessos ao vetor, os retornos nem a declaração `int meio;`, pois ela não inicializa a variável.

Antes do laço ocorrem duas atribuições: `inicio = 0` e `fim = n - 1`.

Em uma repetição com intervalo não vazio, quando o elemento central não é o procurado, temos:

| Operação | Tipo | Quantidade na repetição |
| --- | --- | ---: |
| `inicio <= fim` | Comparação | 1 |
| `meio = inicio + (fim - inicio) / 2` | Atribuição | 1 |
| `v[meio] == valor` | Comparação | 1 |
| `v[meio] < valor` | Comparação | 1 |
| `inicio = meio + 1` **ou** `fim = meio - 1` | Atribuição | 1 |
| **Total** | **3 comparações e 2 atribuições** | **5** |

As duas últimas atribuições são exclusivas: exatamente uma delas é executada em cada repetição, conforme o resultado da comparação `v[meio] < valor`.

Se o elemento for encontrado, a função retorna antes de avaliar `v[meio] < valor` e antes de atualizar os limites. Nesse caso, a repetição faz **duas comparações e uma atribuição**.

Quando o intervalo fica vazio, apenas `inicio <= fim` é avaliada, resulta falsa e encerra o laço. Essa avaliação final **conta** como uma comparação, conforme a convenção do grupo, e não há atribuição.

## 4. Melhor caso, caso médio e pior caso

### Melhor caso

Para `n >= 1`, o melhor caso acontece quando o valor procurado já está exatamente no meio do intervalo inicial. A função encontra o elemento na primeira repetição:

```text
C(n) = 2
A(n) = 3
T(n) = 5
```

As três atribuições são `inicio = 0`, `fim = n - 1` e `meio`. Como essa quantidade não depende de `n`, o melhor caso é constante: **O(1)**.

Se a entrada estiver vazia (`n = 0`), então `fim = -1` e a condição `inicio <= fim` já é falsa na primeira avaliação: duas atribuições, uma comparação e retorno `0`.

### Caso médio

Para definir o caso médio, adotamos as mesmas hipóteses da busca sequencial: vetor com elementos distintos, busca bem-sucedida e mesma probabilidade de procurar qualquer um dos `n` elementos.

Chamando de `d` a quantidade de repetições até encontrar o elemento, incluindo a última, as primeiras `d - 1` repetições fazem três comparações e duas atribuições cada. A última faz duas comparações e uma atribuição. Somando as duas atribuições iniciais:

```text
C = 3d - 1
A = 2d + 1
T = 5d
```

Falta saber quanto vale `d` na média. Para isso, ajuda pensar na busca por níveis. No primeiro nível está um único elemento, o do meio do vetor, encontrado em uma repetição. No segundo nível estão no máximo dois elementos, encontrados em duas repetições. No terceiro, no máximo quatro. O número de elementos dobra a cada nível, e o último nível é alcançado em `k` repetições, o mesmo `k` do pior caso.

Como a quantidade dobra a cada nível, o último nível sozinho contém mais da metade dos elementos do vetor. Isso significa que, para mais da metade dos valores que se pode procurar, a busca gasta exatamente `k` repetições, e para os demais gasta menos. A média fica portanto um pouco abaixo de `k` e cresce junto com ele.

Como `k` é proporcional a `log n`, o caso médio também é, ou seja, **Θ(log n)** e, portanto, **O(log n)**.

Como o algoritmo iterativo examina exatamente a mesma sequência de elementos centrais que o recursivo, a média de `d` é a mesma nas duas versões. O que difere entre elas é apenas a quantidade de atribuições por repetição.

Essa média considera apenas buscas bem-sucedidas. Para incluir valores que não estão no vetor, seria necessário definir também a probabilidade dessas buscas e quais valores seriam procurados.

### Pior caso

O pior caso acontece quando a busca percorre o maior número possível de níveis e ainda assim precisa chegar a um intervalo vazio.

No experimento, o vetor contém `0, 1, 2, ..., n - 1`, e procuramos `p = n`. Esse valor é maior que todos os elementos e não está no vetor. Por isso, a comparação `v[meio] < valor` é sempre verdadeira e a busca sempre segue para a metade direita, até o intervalo ficar vazio.

Com o cálculo de `meio` usado no código, a metade direita tem `floor(n / 2)` elementos e a esquerda tem `floor((n - 1) / 2)`. Seguir sempre para a direita mantém, a cada passo, uma das maiores metades possíveis. Isso justifica a escolha dessa entrada como pior caso.

Aqui, `floor` significa arredondar para baixo.

## 5. Dedução da função de custo no pior caso

Seja `k` o número de repetições com intervalo não vazio.

Cada uma dessas repetições faz três comparações e duas atribuições, e deixa um intervalo de tamanho `floor(n / 2)` para a repetição seguinte. Depois de `k` reduções, o tamanho do intervalo é `floor(n / 2^k)`, e o laço termina quando esse valor chega a zero, ou seja, quando `2^k > n`.

Para `n >= 1`, o menor inteiro que satisfaz essa condição é:

```text
k = floor(log2 n) + 1
```

Além das `k` repetições, ocorre uma avaliação final de `inicio <= fim`, que é falsa e encerra o laço.

Somando as comparações:

```text
C(n) = 3k + 1 = 3 * floor(log2 n) + 4
```

Somando as atribuições, com as duas inicializações e duas por repetição:

```text
A(n) = 2 + 2k = 2 * floor(log2 n) + 4
```

Logo:

```text
T(n) = C(n) + A(n) = 5k + 3 = 5 * floor(log2 n) + 8
```

Essas fórmulas valem para o pior caso descrito. Uma busca que encontra o elemento antes pode fazer menos operações.

Podemos escrever a mesma dedução como recorrência. Chamando de `T(n)` o custo de resolver um intervalo de `n` elementos, já contadas as inicializações à parte:

```text
T(0) = 1
T(n) = 5 + T(floor(n / 2))
```

Desdobrando até o caso-base, obtemos `5k + 1`; somando as duas atribuições iniciais, chegamos a `5k + 3`.

### Comparação com a versão recursiva

As duas versões visitam exatamente os mesmos elementos centrais e fazem o mesmo número `k` de passos não vazios. As comparações, portanto, são idênticas. A diferença está nas atribuições:

| Medida | Iterativa | Recursiva |
| --- | ---: | ---: |
| Comparações `C(n)` | `3k + 1` | `3k + 1` |
| Atribuições `A(n)` | `2k + 2` | `k` |
| Custo `T(n)` | `5k + 3` | `4k + 1` |

A versão iterativa conta mais atribuições porque atualiza explicitamente `inicio` ou `fim` em cada repetição, além das duas inicializações. Na versão recursiva, os novos limites são passados como parâmetros, e a convenção adotada pelo grupo não conta passagem de parâmetros como atribuição.

Isso é uma consequência do modelo de contagem, e **não** significa que a recursiva seja mais barata na prática. O modelo não cobre o custo de criar e destruir o bloco de memória de cada chamada recursiva, nem o espaço de pilha que essas chamadas ocupam. Os tempos medidos confirmam: a versão recursiva executa menos operações contadas e ainda assim foi mais lenta que a iterativa nos cinco tamanhos.

## 6. Termo dominante e complexidade

No pior caso, o termo que determina o crescimento de `T(n)` é o logaritmo. O fator `5`, a constante `8` e o arredondamento não mudam a ordem de crescimento.

| Caso | Função de custo no modelo adotado | Grande O |
| --- | --- | --- |
| Melhor, com `n >= 1` | `T(n) = 5` | `O(1)` |
| Médio, sob as hipóteses da seção 4 | proporcional a `log n` | `O(log n)` |
| Pior, com `n >= 1` | `T(n) = 5 * floor(log2 n) + 8` | `O(log n)` |

No caso médio e no pior caso também podemos usar **Θ(log n)**, pois o crescimento é efetivamente logarítmico.

A base do logaritmo não altera a classificação, pois mudar de base multiplica a função por uma constante. Usamos base 2 porque o intervalo é dividido ao meio a cada passo.

Além do vetor que recebe, a função usa apenas as variáveis `inicio`, `fim` e `meio`. Essa quantidade não muda com o tamanho da entrada, então a memória extra é constante, `O(1)`. É aqui que está a diferença mais concreta em relação à versão recursiva, que usa `O(log n)`.

## 7. Conferência com os experimentos

O experimento está em `experiments/auxiliar_buscabinaria_iter.c`, e os resultados estão em `data/resultados_binaria_iterativa.csv`.

Para cada tamanho, foi usado um vetor com os valores de `0` até `n - 1`, procurando `p = n`. Portanto, comparamos os resultados com as fórmulas do pior caso.

| n | `k` | Comparações previstas `3k + 1` | Comparações medidas | Atribuições previstas `2k + 2` | Atribuições medidas | `T(n) = 5k + 3` |
| ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| 1.000 | 10 | 31 | 31 | 22 | 22 | 53 |
| 50.000 | 16 | 49 | 49 | 34 | 34 | 83 |
| 100.000 | 17 | 52 | 52 | 36 | 36 | 88 |
| 500.000 | 19 | 58 | 58 | 40 | 40 | 98 |
| 1.000.000 | 20 | 61 | 61 | 42 | 42 | 103 |

As contagens medidas coincidem com as previstas em todos os tamanhos. O programa auxiliar calcula `k` por divisões inteiras sucessivas, faz essa verificação automaticamente e avisa se houver divergência; nenhuma divergência foi registrada.

Por exemplo, para `n = 1.000.000`, temos `floor(log2 1.000.000) = 19`, logo `k = 20`. Então:

```text
C(n) = 3 * 20 + 1 = 61
A(n) = 2 * 20 + 2 = 42
T(n) = 61 + 42 = 103
```

As comparações coincidem exatamente com as da versão recursiva, como previsto na seção 5, e as atribuições são maiores (`42` contra `20`).

O CSV também apresenta os seguintes tempos médios:

| n | Repetições | Tempo médio por busca |
| ---: | ---: | ---: |
| 1.000 | 1.000.000 | 13 ns |
| 50.000 | 1.000.000 | 23 ns |
| 100.000 | 1.000.000 | 25 ns |
| 500.000 | 1.000.000 | 28 ns |
| 1.000.000 | 1.000.000 | 27 ns |

O tempo médio foi calculado dividindo o tempo total do bloco por 1.000.000. Já as comparações e atribuições foram contadas separadamente e representam uma única busca, por isso não são multiplicadas pelo número de repetições.

Quando o vetor passou de 1.000 para 1.000.000 de elementos, seu tamanho aumentou mil vezes, mas o número de repetições com intervalo não vazio passou apenas de 10 para 20, e as comparações passaram de 31 para 61. O tempo médio cresceu de `13 ns` para `27 ns`, cerca de duas vezes. Esse comportamento corresponde ao crescimento logarítmico obtido na análise.

Duas observações sobre a precisão desses tempos:

1. Os valores aparecem sem casas decimais porque vêm de dividir o tempo do bloco, medido em milissegundos inteiros, por um milhão de repetições. A resolução do relógio é de um milissegundo, o que equivale a 1 ns por busca.

2. Por isso, a queda de `28 ns` para `27 ns` entre `n = 500.000` e `n = 1.000.000` não deve ser interpretada: ela corresponde a exatamente um milissegundo no tempo do bloco e está dentro do erro de medição. O custo de um algoritmo `O(log n)` não diminui quando a entrada cresce.

A classificação em `O(log n)` vem da dedução matemática; os resultados experimentais ajudam a conferir esse comportamento.
