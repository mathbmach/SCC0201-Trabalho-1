# Busca binária recursiva — análise teórica

## 1. Objetivo e entrada

A busca binária recursiva verifica se um valor está presente em um vetor ordenado em ordem crescente. A ideia é comparar o valor procurado com o elemento do meio. Se forem iguais, a busca termina. Caso contrário, a função continua apenas na metade em que o valor pode estar.

A função recebe:

- `v`: o vetor em que a busca será feita;
- `inicio`: primeiro índice do intervalo;
- `fim`: último índice do intervalo;
- `p`: valor procurado.

O retorno é `1` quando o valor é encontrado e `0` quando ele não está no vetor. Para buscar em um vetor inteiro de tamanho `n`, a chamada é `busca_binaria_recursiva(v, 0, n - 1, p)`.

Durante a recursão, o tamanho do intervalo é `fim - inicio + 1`, enquanto ele não estiver vazio. A análise considera que o vetor já está ordenado, então o custo de ordenação não entra nas contas.

## 2. Código analisado

O código abaixo corresponde à função em [src/algoritmos.c](../src/algoritmos.c):

```c
int busca_binaria_recursiva(
    int *v,
    int inicio,
    int fim,
    int p)
{
    if (inicio > fim)
    {
        return 0;
    }

    int meio = inicio + (fim - inicio) / 2;

    if (v[meio] == p)
    {
        return 1;
    }

    if (v[meio] < p)
        return busca_binaria_recursiva(v, meio + 1, fim, p);

    else
        return busca_binaria_recursiva(v, inicio, meio - 1, p);
}
```

## 3. Contagem das operações

Seguindo a convenção do [README](README.md), usamos:

- `C(n)` para o número de comparações;
- `A(n)` para o número de atribuições;
- `T(n) = C(n) + A(n)` para o custo total das operações contadas.

Esse custo não é o tempo em segundos. Também não contamos separadamente as operações aritméticas, os acessos ao vetor, os retornos ou a passagem dos parâmetros.

Em uma chamada com intervalo não vazio, quando o elemento do meio não é o procurado, temos:

| Operação | Tipo | Quantidade na chamada |
| --- | --- | ---: |
| `inicio > fim` | Comparação | 1 |
| `meio = inicio + (fim - inicio) / 2` | Atribuição | 1 |
| `v[meio] == p` | Comparação | 1 |
| `v[meio] < p` | Comparação | 1 |
| **Total** | **3 comparações e 1 atribuição** | **4** |

Se o elemento for encontrado, a função retorna antes de avaliar `v[meio] < p`. Nesse caso, a chamada faz duas comparações e uma atribuição.

Se o intervalo estiver vazio, apenas `inicio > fim` é avaliado. Essa chamada faz uma comparação e nenhuma atribuição.

## 4. Melhor caso, caso médio e pior caso

### Melhor caso

Para `n >= 1`, o melhor caso acontece quando o valor procurado já está no meio do intervalo inicial. A função encontra o elemento na primeira chamada:

$$
C_{melhor}(n) = 2
$$

$$
A_{melhor}(n) = 1
$$

$$
T_{melhor}(n) = 3
$$

Como essa quantidade não depende de `n`, o melhor caso é constante: **O(1)**.

Se a entrada estiver vazia (`n = 0`), a função faz apenas uma comparação e retorna `0`.

### Caso médio

Para definir o caso médio, vamos considerar um vetor com elementos distintos, uma busca que encontra o valor e a mesma chance de procurar qualquer um dos `n` elementos.

Chamando de `d` a quantidade de chamadas até encontrar o elemento, incluindo a primeira, as primeiras `d - 1` chamadas fazem três comparações e uma atribuição cada. A última faz duas comparações e uma atribuição. Assim:

$$
C = 3(d - 1) + 2 = 3d - 1
$$

$$
A = d
$$

$$
T = 4d - 1
$$

Se `d_i` é a quantidade de chamadas para encontrar o elemento da posição `i`, a média de chamadas é:

$$
\bar d = \frac{1}{n}\sum_{i=0}^{n-1} d_i
$$

Portanto:

$$
C_{medio}(n) = 3\bar d - 1, \qquad
A_{medio}(n) = \bar d
$$

$$
T_{medio}(n) = 4\bar d - 1
$$

Para entender o crescimento dessa média, podemos olhar os níveis da busca. O primeiro nível tem um elemento, o segundo tem no máximo dois, o terceiro tem no máximo quatro, e assim por diante. A maior parte dos elementos fica nos níveis mais profundos, e a profundidade máxima cresce como `log₂(n)`. Por isso, a média de chamadas também cresce nessa ordem.

Por exemplo, quando `n = 2^h - 1`, todos os `h` níveis estão completos. Nesse caso:

$$
\bar d = \frac{\sum_{d=1}^{h} d\,2^{d-1}}{2^h - 1}
= \frac{(h-1)2^h + 1}{2^h - 1}
$$

Como `h = log₂(n + 1)`, a média cresce de forma logarítmica. Para os demais tamanhos, o último nível pode ficar incompleto, mas a divisão equilibrada mantém essa ordem de crescimento. Assim, sob as hipóteses adotadas, o caso médio é **Θ(log n)** e, portanto, **O(log n)**.

Essa média considera apenas buscas bem-sucedidas. Para incluir valores que não estão no vetor, seria necessário definir também a probabilidade dessas buscas e quais valores seriam procurados.

### Pior caso

O pior caso acontece quando a busca percorre o maior número de níveis e ainda precisa chegar a um intervalo vazio.

No experimento, o vetor contém `0, 1, 2, ..., n - 1`, e procuramos `p = n`. Esse valor é maior que todos os elementos e não está no vetor. Por isso, a função sempre segue para a metade direita até o intervalo ficar vazio.

Com o cálculo de `meio` usado no código, a metade direita tem `floor(n / 2)` elementos, e a esquerda tem `floor((n - 1) / 2)`. Ou seja, seguir para a direita mantém uma das maiores metades possíveis em cada passo. Isso justifica a escolha dessa entrada como pior caso.

Aqui, `floor` significa arredondar para baixo. A dedução desse caso está a seguir.

## 5. Dedução da função de custo no pior caso

Cada chamada com intervalo não vazio faz três comparações e uma atribuição. Depois, continua com um intervalo de tamanho `floor(n / 2)`.

Quando o intervalo fica vazio, resta apenas uma comparação. Portanto, para `n >= 1`:

$$
C(0) = 1, \qquad C(n) = 3 + C\left(\left\lfloor\frac{n}{2}\right\rfloor\right)
$$

$$
A(0) = 0, \qquad A(n) = 1 + A\left(\left\lfloor\frac{n}{2}\right\rfloor\right)
$$

Somando as duas contagens:

$$
T(0) = 1, \qquad T(n) = 4 + T\left(\left\lfloor\frac{n}{2}\right\rfloor\right)
$$

A cada passo, o intervalo fica aproximadamente pela metade. Depois de `k` reduções, seu tamanho é `floor(n / 2^k)`. A busca termina quando esse valor chega a zero, ou seja, quando `2^k > n`.

Para `n >= 1`, o menor inteiro que satisfaz essa condição é:

$$
k = \lfloor\log_2 n\rfloor + 1
$$

Esse `k` também é a quantidade de chamadas com intervalo não vazio. Além delas, existe uma chamada final com intervalo vazio, totalizando `k + 1` chamadas.

Como cada uma das `k` chamadas faz três comparações, e a última faz mais uma:

$$
C(n) = 3k + 1 = 3\lfloor\log_2 n\rfloor + 4
$$

Cada chamada com intervalo não vazio inicializa `meio` uma vez:

$$
A(n) = k = \lfloor\log_2 n\rfloor + 1
$$

Logo:

$$
T(n) = C(n) + A(n) = 4k + 1
$$

$$
\boxed{T(n) = 4\lfloor\log_2 n\rfloor + 5 \quad \text{para } n \geq 1}
$$

Essas fórmulas são do pior caso descrito. Uma busca que encontra o elemento antes pode fazer menos operações.

## 6. Termo dominante e complexidade

No pior caso, o termo que determina o crescimento de `T(n)` é o logaritmo. O fator 4, a constante 5 e o arredondamento não mudam a ordem de crescimento.

| Caso | Função de custo no modelo adotado | Grande O |
| --- | --- | --- |
| Melhor, com `n >= 1` | `T(n) = 3` | `O(1)` |
| Médio, sob as hipóteses da seção 4 | `T(n) = 4d̄ - 1`, com `d̄ = Θ(log n)` | `O(log n)` |
| Pior, com `n >= 1` | `T(n) = 4 floor(log₂ n) + 5` | `O(log n)` |

No caso médio e no pior caso, também podemos usar **Θ(log n)**, pois o crescimento é efetivamente logarítmico.

## 7. Conferência com os experimentos

O experimento está em [experiments/auxiliar_buscabinaria_rec.c](../experiments/auxiliar_buscabinaria_rec.c), e os resultados estão em [data/resultados_binaria_recursiva.csv](../data/resultados_binaria_recursiva.csv).

Para cada tamanho, foi usado um vetor com os valores de `0` até `n - 1`, procurando `p = n`. Portanto, comparamos os resultados com as fórmulas do pior caso.

| n | k | Comparações previstas | Comparações medidas | Atribuições previstas | Atribuições medidas | T(n) = C(n) + A(n) |
| ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| 1.000 | 10 | 31 | 31 | 10 | 10 | 41 |
| 50.000 | 16 | 49 | 49 | 16 | 16 | 65 |
| 100.000 | 17 | 52 | 52 | 17 | 17 | 69 |
| 500.000 | 19 | 58 | 58 | 19 | 19 | 77 |
| 1.000.000 | 20 | 61 | 61 | 20 | 20 | 81 |

As contagens medidas coincidem com as previstas em todos os tamanhos. Por exemplo, para `n = 1.000.000`, temos `k = 20`. Então, são `3 × 20 + 1 = 61` comparações e `20` atribuições, totalizando `81` operações no modelo adotado.

O CSV também apresenta os seguintes tempos médios:

| n | Repetições | Tempo médio por busca (s) |
| ---: | ---: | ---: |
| 1.000 | 1.000.000 | 0,000000024 |
| 50.000 | 1.000.000 | 0,000000034 |
| 100.000 | 1.000.000 | 0,000000037 |
| 500.000 | 1.000.000 | 0,000000042 |
| 1.000.000 | 1.000.000 | 0,000000044 |

O tempo médio foi calculado dividindo o tempo total do bloco de repetições por 1.000.000. Já as comparações e atribuições foram contadas separadamente e representam uma única busca, por isso não são multiplicadas pelo número de repetições.

Quando o vetor passou de 1.000 para 1.000.000 de elementos, seu tamanho aumentou mil vezes, mas a quantidade de chamadas com intervalo não vazio passou de 10 para 20. As comparações passaram de 31 para 61. Isso corresponde ao crescimento logarítmico encontrado na análise.

Os tempos também cresceram pouco nesse intervalo, o que é compatível com a teoria. Como são tempos muito pequenos, fatores como a resolução do relógio, o compilador, a memória cache e o custo do laço de medição podem afetar os valores. A classificação em `O(log n)` vem da dedução matemática; os resultados experimentais ajudam a conferir esse comportamento.
