# Inversão do vetor — análise teórica

## 1. Objetivo e entrada

A inversão do vetor troca a ordem dos elementos: o primeiro passa a ser o último, o segundo passa a ser o penúltimo, e assim por diante.

A função recebe:

- `v`: o vetor que será invertido;
- `n`: a quantidade de elementos do vetor, considerando `n >= 0`.

A função altera o próprio vetor e não retorna um valor. Por exemplo:

```text
Antes:  0 1 2 3 4
Depois: 4 3 2 1 0
```

Para fazer isso, usamos dois índices: `i` começa na primeira posição e `j` começa na última. A cada repetição, os elementos dessas posições são trocados, `i` aumenta e `j` diminui.

Só precisamos percorrer metade do vetor, pois cada troca já coloca dois elementos nas posições corretas. Quando o tamanho é ímpar, o elemento central continua na mesma posição.

O vetor não precisa estar ordenado para ser invertido. No experimento usamos valores de `0` até `n - 1`, mas o algoritmo funciona com qualquer sequência de valores.

## 2. Código analisado

O código abaixo corresponde à função em [src/algoritmos.c](../src/algoritmos.c):

```c
void inverte(int *v, int n)
{
    int i, j, temp;

    for (i = 0, j = n - 1; i < n / 2; i++, j--)
    {
        temp = v[i];
        v[i] = v[j];
        v[j] = temp;
    }
}
```

## 3. Contagem das operações

Seguindo a convenção do [README](README.md), usamos:

- `C(n)` para o número de comparações;
- `A(n)` para o número de atribuições;
- `T(n) = C(n) + A(n)` para o custo total das operações contadas.

Esse custo não é o tempo em segundos. Não contamos separadamente as operações aritméticas, os acessos ao vetor nem a declaração `int i, j, temp;`, pois ela não inicializa as variáveis.

Vamos chamar de `m` a quantidade de repetições do laço:

$$
m = \left\lfloor\frac{n}{2}\right\rfloor
$$

O símbolo de piso significa arredondar para baixo. No código em C, como `n` é inteiro e não negativo, a divisão `n / 2` já faz esse arredondamento. Por exemplo, tanto para `n = 4` quanto para `n = 5`, o laço executa duas vezes.

| Operação | Tipo | Quantidade na execução |
| --- | --- | ---: |
| `i = 0` | Atribuição | 1 |
| `j = n - 1` | Atribuição | 1 |
| `i < n / 2` | Comparação | `m + 1` |
| `temp = v[i]` | Atribuição | `m` |
| `v[i] = v[j]` | Atribuição | `m` |
| `v[j] = temp` | Atribuição | `m` |
| `i++` | Atribuição | `m` |
| `j--` | Atribuição | `m` |

A condição do laço é avaliada `m + 1` vezes: uma para cada repetição e mais uma no final, quando se torna falsa e encerra o laço. As atualizações `i++` e `j--` acontecem inclusive depois da última troca, antes dessa comparação final.

Assim, temos duas atribuições iniciais, cinco atribuições por repetição e uma comparação a mais que o número de repetições.

## 4. Melhor caso, caso médio e pior caso

### Melhor caso

Para um mesmo tamanho `n`, não existe uma organização dos valores que faça essa implementação terminar antes. Mesmo que todos os elementos sejam iguais, a função executa todas as trocas previstas.

Por isso, o melhor caso também faz `floor(n / 2)` repetições e tem complexidade **O(n)**.

### Caso médio

A quantidade de operações depende apenas do tamanho do vetor. Não importa se os valores estão em ordem crescente, decrescente, repetidos ou misturados.

Assim, qualquer distribuição de probabilidade sobre os vetores de um mesmo tamanho produz a mesma média de operações. Não precisamos supor que uma organização dos valores seja mais provável que outra, pois todas têm o mesmo custo.

Logo, o caso médio também faz `floor(n / 2)` repetições e tem complexidade **O(n)**.

### Pior caso

Pelo mesmo motivo, nenhum vetor de tamanho `n` exige mais repetições que outro do mesmo tamanho. O pior caso tem a mesma contagem do melhor e do caso médio, com complexidade **O(n)**.

Portanto, as fórmulas da próxima seção valem para os três casos. Essa comparação é feita mantendo o tamanho `n` fixo e variando os valores do vetor.

Para `n = 0` ou `n = 1`, o corpo do laço não executa. Ainda assim, ocorrem as duas atribuições iniciais e a primeira avaliação da condição, totalizando três operações. Esses casos pequenos também estão incluídos nas fórmulas.

## 5. Dedução da função de custo

Como o laço executa `m = floor(n / 2)` vezes e sua condição é avaliada uma vez a mais, o número de comparações é:

$$
C(n) = m + 1 = \left\lfloor\frac{n}{2}\right\rfloor + 1
$$

Nas atribuições, temos duas inicializações e cinco operações por repetição: três para a troca e duas para atualizar os índices. Portanto:

$$
A(n) = 2 + 5m = 2 + 5\left\lfloor\frac{n}{2}\right\rfloor
$$

Somando as duas contagens:

$$
T(n) = C(n) + A(n)
$$

$$
T(n) = (m + 1) + (2 + 5m) = 6m + 3
$$

$$
\boxed{T(n) = 6\left\lfloor\frac{n}{2}\right\rfloor + 3 \quad \text{para } n \geq 0}
$$

### Quando n é par

Se `n` é par, não precisamos do arredondamento:

$$
C(n) = \frac{n}{2} + 1
$$

$$
A(n) = 2 + \frac{5n}{2}
$$

$$
T(n) = 3n + 3
$$

Todos os tamanhos usados no experimento são pares, então essa forma simplificada vale para os resultados do CSV.

### Quando n é ímpar

Se `n` é ímpar, o número de repetições é `(n - 1) / 2`. Assim:

$$
C(n) = \frac{n - 1}{2} + 1
$$

$$
A(n) = 2 + 5\frac{n - 1}{2}
$$

$$
T(n) = 6\frac{n - 1}{2} + 3 = 3n
$$

Por exemplo, para `n = 5`, fazemos duas trocas. São três comparações e doze atribuições, totalizando quinze operações. O elemento central não precisa ser trocado.

## 6. Termo dominante e complexidade

A função de custo é `3n + 3` quando `n` é par e `3n` quando `n` é ímpar. Nos dois casos, o termo dominante é linear em `n`.

O fator constante e a diferença de três operações não mudam a ordem de crescimento. Por isso, a inversão tem complexidade **O(n)**. Também podemos usar **Θ(n)**, pois o crescimento é efetivamente linear.

| Caso | Função de custo | Grande O |
| --- | --- | --- |
| Melhor | `T(n) = 6 floor(n / 2) + 3` | `O(n)` |
| Médio | `T(n) = 6 floor(n / 2) + 3` | `O(n)` |
| Pior | `T(n) = 6 floor(n / 2) + 3` | `O(n)` |

Percorrer apenas metade do vetor reduz a quantidade de operações, mas não muda a classificação para uma ordem menor que linear. Se dobrarmos `n`, a quantidade de repetições também cresce aproximadamente para o dobro.

## 7. Conferência com os experimentos

O experimento está em [experiments/auxiliar_inversao.c](../experiments/auxiliar_inversao.c), e os resultados estão em [data/resultados_inversao.csv](../data/resultados_inversao.csv).

Os vetores foram preenchidos inicialmente com os valores de `0` até `n - 1`. Como a contagem depende apenas de `n`, essas entradas representam tanto o melhor quanto o caso médio e o pior caso.

| n | Repetições do laço por inversão | Comparações previstas | Comparações medidas | Atribuições previstas | Atribuições medidas | T(n) = C(n) + A(n) |
| ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| 1.000 | 500 | 501 | 501 | 2.502 | 2.502 | 3.003 |
| 50.000 | 25.000 | 25.001 | 25.001 | 125.002 | 125.002 | 150.003 |
| 100.000 | 50.000 | 50.001 | 50.001 | 250.002 | 250.002 | 300.003 |
| 500.000 | 250.000 | 250.001 | 250.001 | 1.250.002 | 1.250.002 | 1.500.003 |
| 1.000.000 | 500.000 | 500.001 | 500.001 | 2.500.002 | 2.500.002 | 3.000.003 |

As contagens medidas coincidem com as previstas em todos os tamanhos. Por exemplo, para `n = 1.000.000`, o laço executa `500.000` vezes. Assim:

$$
C(n) = 500.000 + 1 = 500.001
$$

$$
A(n) = 2 + 5 \times 500.000 = 2.500.002
$$

$$
T(n) = 500.001 + 2.500.002 = 3.000.003
$$

O CSV também apresenta os seguintes tempos médios:

| n | Execuções usadas na medição | Tempo médio por inversão (s) |
| ---: | ---: | ---: |
| 1.000 | 100.000 | 0,000000730 |
| 50.000 | 2.000 | 0,000036500 |
| 100.000 | 1.000 | 0,000075000 |
| 500.000 | 200 | 0,000400000 |
| 1.000.000 | 100 | 0,000770000 |

O número de execuções da medição é diferente do número de repetições do laço dentro de uma inversão. Por exemplo, para `n = 1.000`, o experimento executou a função `100.000` vezes, e cada execução fez `500` repetições do laço.

O tempo médio foi calculado dividindo o tempo total pelo número de execuções da função. Já as comparações e atribuições foram contadas separadamente e representam uma única inversão, por isso não são multiplicadas pelo número de execuções do experimento.

Repetir a função no mesmo vetor alterna entre a ordem original e a ordem invertida. Isso não altera a contagem, porque a função sempre faz a mesma quantidade de trocas para um mesmo `n`.

Quando o tamanho passou de 1.000 para 1.000.000, a entrada aumentou mil vezes e o custo contado passou de 3.003 para 3.000.003 operações, um aumento de aproximadamente mil vezes. O tempo médio passou de `0,000000730 s` para `0,000770000 s`, um aumento de aproximadamente 1.055 vezes, compatível com o crescimento linear esperado.

Os tempos não precisam seguir uma proporção exata, pois fatores como o compilador, a memória cache, a resolução do relógio e o custo do laço de medição podem afetar os valores. A classificação em `O(n)` vem da dedução matemática; os resultados experimentais ajudam a conferir esse comportamento.
