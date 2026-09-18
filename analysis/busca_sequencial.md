# Busca sequencial — análise teórica

## 1. Objetivo e entrada

A busca sequencial verifica se um valor está presente em um vetor. A ideia é a mais direta possível: percorrer o vetor da primeira até a última posição, comparando cada elemento com o valor procurado. A busca termina assim que o valor é encontrado ou quando todas as posições já foram examinadas.

A função recebe:

- `v`: o vetor em que a busca será feita;
- `n`: a quantidade de elementos do vetor, considerando `n >= 0`;
- `valor`: o valor procurado.

O retorno é `1` quando o valor é encontrado e `0` quando ele não está no vetor.

Diferentemente das duas buscas binárias, a busca sequencial **não exige vetor ordenado**. Ela funciona com qualquer disposição dos elementos. No trabalho, o vetor de entrada é ordenado por exigência do enunciado, mas o algoritmo não aproveita essa informação: mesmo sabendo que os valores crescem, ele continua examinando uma posição por vez.

É exatamente essa diferença que a comparação com as buscas binárias pretende medir.

## 2. Código analisado

O código abaixo corresponde à função em `src/algoritmos.c`:

```c
int busca_sequencial(int *v, int n, int valor)
{
    int i;

    for (i = 0; i < n; i++)
    {
        if (v[i] == valor)
        {
            return 1;
        }
    }

    return 0;
}
```

## 3. Contagem das operações

Seguindo a convenção do `README.md` desta pasta, usamos:

- `C(n)` para o número de comparações;
- `A(n)` para o número de atribuições;
- `T(n) = C(n) + A(n)` para o custo total das operações contadas.

Esse custo não é o tempo em segundos. Não contamos separadamente as operações aritméticas, os acessos ao vetor, os retornos nem a declaração `int i;`, pois ela não inicializa a variável.

As operações do algoritmo são:

| Operação | Tipo | Quando ocorre |
| --- | --- | --- |
| `i = 0` | Atribuição | Uma única vez, na inicialização do laço. |
| `i < n` | Comparação | Uma vez por posição examinada e mais uma, falsa, quando o laço chega ao fim do vetor. |
| `v[i] == valor` | Comparação | Uma vez por posição examinada. |
| `i++` | Atribuição | Uma vez por posição examinada que não contém o valor. |

O ponto que exige atenção é a diferença entre **terminar por encontrar o valor** e **terminar por esgotar o vetor**.

Quando o valor é encontrado na posição `j`, a função retorna de dentro do laço: o `i++` daquela repetição não chega a ser executado e a condição `i < n` não é avaliada mais uma vez.

Quando o valor não está no vetor, o laço só termina quando `i` alcança `n`. Nesse momento a condição `i < n` é avaliada pela última vez e resulta falsa. Essa avaliação final conta, conforme a convenção adotada pelo grupo.

Chamando de `j` o índice, contado a partir de zero, em que o valor foi encontrado:

| Situação | Posições examinadas | `C` | `A` | `T = C + A` |
| --- | ---: | ---: | ---: | ---: |
| Valor encontrado em `j` | `j + 1` | `2(j + 1)` | `j + 1` | `3(j + 1)` |
| Valor ausente | `n` | `2n + 1` | `n + 1` | `3n + 2` |

Na busca bem-sucedida, cada uma das `j` posições anteriores gera duas comparações e uma atribuição; a posição `j` gera as duas comparações e o retorno. Isso dá `2(j+1)` comparações e `j + 1` atribuições, contando a inicialização `i = 0`.

Na busca malsucedida, as `n` posições geram `2n` comparações e `n` incrementos; somam-se a comparação final falsa e a inicialização.

## 4. Melhor caso, caso médio e pior caso

### Melhor caso

O melhor caso acontece quando o valor procurado está na primeira posição do vetor, ou seja, `j = 0`. A função faz uma comparação de condição, uma comparação de igualdade e retorna:

```text
C(n) = 2
A(n) = 1
T(n) = 3
```

Como essa quantidade não depende de `n`, o melhor caso é constante: **O(1)**.

Vale notar que o melhor caso ser constante não torna o algoritmo rápido: ele depende de uma coincidência favorável na posição do elemento, e não de uma propriedade do algoritmo.

### Caso médio

Para definir o caso médio, adotamos as hipóteses usuais: o vetor tem elementos distintos, a busca é bem-sucedida e cada uma das `n` posições tem a mesma probabilidade de conter o valor procurado.

Se o valor está na primeira posição, a busca examina uma posição. Se está na segunda, examina duas. E assim por diante, até `n` posições quando o valor está na última. Os valores possíveis vão de `1` até `n`, de um em um, e todos têm a mesma chance. A média de uma sequência assim é o valor do meio, isto é, a média entre o primeiro e o último:

```text
posições examinadas, em média = (1 + n) / 2
```

Ou seja, em média a busca percorre metade do vetor. Substituindo esse valor nas fórmulas da seção anterior:

```text
C(n) = n + 1
A(n) = (n + 1) / 2
T(n) = 3(n + 1) / 2
```

Essa média considera apenas buscas bem-sucedidas. Se incluíssemos também as buscas que falham, o custo médio ficaria em algum ponto entre `3(n+1)/2`, quando nenhuma falha, e `3n + 2`, quando todas falham. Em qualquer proporção entre as duas, o crescimento continua proporcional a `n`.

Por isso o caso médio é **Θ(n)** e, portanto, **O(n)**.

Examinar metade do vetor em média reduz o custo por um fator constante, mas não muda a ordem de crescimento. Esse é exatamente o tipo de diferença que a notação assintótica descarta.

### Pior caso

O pior caso acontece quando a função precisa examinar todas as `n` posições. Isso ocorre em duas situações:

1. o valor está na última posição (`j = n - 1`), com `T = 3n`;
2. o valor não está no vetor, com `T = 3n + 2`.

A segunda situação é a mais cara, pois acrescenta a avaliação final falsa da condição do laço e o último incremento de `i`. É ela que usamos como pior caso no experimento.

No experimento, o vetor contém `0, 1, 2, ..., n - 1` e procuramos `p = n`. Esse valor é maior que todos os elementos e não pertence ao vetor, o que força a varredura completa.

## 5. Dedução da função de custo no pior caso

No pior caso, o corpo do laço executa `n` vezes.

A condição `i < n` é avaliada uma vez antes de cada uma das `n` repetições, mais uma vez ao final, quando se torna falsa:

```text
avaliações de i < n = n + 1
```

A comparação `v[i] == valor` é avaliada uma vez por repetição:

```text
avaliações de v[i] == valor = n
```

Somando as duas:

```text
C(n) = (n + 1) + n = 2n + 1
```

As atribuições são a inicialização `i = 0` e os `n` incrementos `i++`:

```text
A(n) = 1 + n = n + 1
```

Portanto:

```text
T(n) = C(n) + A(n) = (2n + 1) + (n + 1) = 3n + 2
```

Para `n = 0`, a fórmula dá `T(0) = 2`: uma atribuição `i = 0` e uma comparação `i < n` já falsa. O corpo do laço não executa.

## 6. Termo dominante e complexidade

No pior caso, `T(n) = 3n + 2`. O termo dominante é `3n`, que é linear em `n`. O fator constante `3` e a parcela `+2` não alteram a ordem de crescimento.

| Caso | Função de custo no modelo adotado | Grande O |
| --- | --- | --- |
| Melhor | `T(n) = 3` | `O(1)` |
| Médio, sob as hipóteses da seção 4 | `T(n) = 3(n + 1) / 2` | `O(n)` |
| Pior | `T(n) = 3n + 2` | `O(n)` |

No caso médio e no pior caso podemos usar também **Θ(n)**, pois o crescimento é efetivamente linear. No melhor caso o custo é constante, **Θ(1)**.

A classificação que descreve o comportamento do algoritmo diante de uma entrada qualquer é a do pior caso e a do caso médio, ambas lineares.

Além do vetor que recebe, a função usa apenas a variável `i`. Essa quantidade não muda com o tamanho da entrada, então a memória extra é constante, `O(1)`.

## 7. Conferência com os experimentos

O experimento está em `experiments/auxiliar_busca_sequencial.c`, e os resultados estão em `data/resultados_busca_sequencial.csv`.

Para cada tamanho, foi usado um vetor com os valores de `0` até `n - 1`, procurando `p = n`. Portanto, comparamos os resultados com as fórmulas do pior caso.

| n | Comparações previstas `2n + 1` | Comparações medidas | Atribuições previstas `n + 1` | Atribuições medidas | `T(n) = 3n + 2` |
| ---: | ---: | ---: | ---: | ---: | ---: |
| 1.000 | 2.001 | 2.001 | 1.001 | 1.001 | 3.002 |
| 50.000 | 100.001 | 100.001 | 50.001 | 50.001 | 150.002 |
| 100.000 | 200.001 | 200.001 | 100.001 | 100.001 | 300.002 |
| 500.000 | 1.000.001 | 1.000.001 | 500.001 | 500.001 | 1.500.002 |
| 1.000.000 | 2.000.001 | 2.000.001 | 1.000.001 | 1.000.001 | 3.000.002 |

As contagens medidas coincidem com as previstas em todos os tamanhos. O programa auxiliar faz essa verificação automaticamente e avisa se houver divergência; nenhuma divergência foi registrada.

Por exemplo, para `n = 1.000.000`:

```text
C(n) = 2 * 1.000.000 + 1 = 2.000.001
A(n) = 1.000.000 + 1     = 1.000.001
T(n) = 2.000.001 + 1.000.001 = 3.000.002
```

O CSV também apresenta os seguintes tempos médios:

| n | Execuções usadas na medição | Tempo médio por busca |
| ---: | ---: | ---: |
| 1.000 | 100.000 | 0,45 µs |
| 50.000 | 2.000 | 22,50 µs |
| 100.000 | 1.000 | 44,00 µs |
| 500.000 | 200 | 225,00 µs |
| 1.000.000 | 100 | 440,00 µs |

O tempo médio foi calculado dividindo o tempo total do bloco de repetições pelo número de execuções. Já as comparações e atribuições foram contadas separadamente, em uma execução da versão instrumentada, e por isso não são multiplicadas pelo número de repetições.

Quando o tamanho passou de 1.000 para 1.000.000, a entrada aumentou mil vezes, o custo contado passou de 3.002 para 3.000.002 operações, também cerca de mil vezes, e o tempo médio passou de `0,45 µs` para `440,00 µs`, um aumento de aproximadamente 978 vezes. O custo por elemento ficou praticamente constante em toda a faixa, o que é o que se espera de um algoritmo linear.

Vale registrar que esse acordo entre tempo e contagem não estava garantido. Um vetor de um milhão de inteiros ocupa cerca de 4 MB e não cabe nas caches menores do processador, então seria possível ver o custo por elemento subir nos tamanhos maiores. Não foi o que aconteceu nesta coleta, provavelmente porque a busca sequencial percorre o vetor em ordem, que é o padrão de acesso mais fácil de o processador antecipar.

De todo modo, a classificação assintótica vem da dedução matemática, não da medição: os tempos confirmam a tendência linear, mas a prova de que o algoritmo é `O(n)` está na contagem.

## Relação com os demais algoritmos

A busca sequencial e a inversão do vetor são os dois algoritmos lineares do trabalho, mas por motivos diferentes:

- a inversão executa `floor(n/2)` repetições sempre, independentemente dos valores;
- a busca sequencial executa entre `1` e `n` repetições, dependendo de onde o valor está.

Já as buscas binárias resolvem o mesmo problema da busca sequencial em `O(log n)`, mas exigem o vetor ordenado. A busca sequencial é a única das três que funciona em um vetor qualquer, e é isso que ela troca por seu custo maior.
