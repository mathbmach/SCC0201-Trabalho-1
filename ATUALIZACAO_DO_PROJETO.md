# Atualização do projeto — Trabalho 1 de AED I

**Situação em 17/09/2026**

Pessoal, segue um resumo do que já está feito, das alterações mais recentes e do que ainda precisamos terminar. A inversão e a busca binária recursiva já têm código, experimento, resultados e análise teórica. Agora falta completar esse mesmo ciclo para a busca sequencial e a busca binária iterativa, além de montar os gráficos e o relatório final.

## 1. O que já está feito

| Parte | Situação |
| --- | --- |
| Programa principal | Leitura do vetor e comandos de 1 a 5 implementados. Ainda depende das duas buscas que faltam e da revisão final. |
| Inversão | Função implementada, auxiliar de experimento atualizado, CSV gerado e análise teórica preenchida. |
| Busca binária recursiva | Função implementada, auxiliar de experimento criado, CSV gerado e análise teórica preenchida. |
| Padrão de análise | README da pasta `analysis` preenchido com a convenção de contagem e o roteiro dos documentos. |
| Revisão inicial | Programa principal e dois auxiliares compilados; testes funcionais pequenos executados; contagens e médias dos CSVs conferidas. |

A compilação funcionou, mas ainda apresenta avisos relacionados às duas funções incompletas. Nos testes executados, a inversão e a busca recursiva funcionaram com vetores unitários, pares, ímpares, negativos e valores repetidos, além de buscas por valores presentes e ausentes. Isso ainda não substitui o teste final do programa completo.

## 2. Ferramentas e estruturas utilizadas

| Ferramenta ou estrutura | Como está sendo usada |
| --- | --- |
| C | Implementação do programa e dos experimentos. |
| GCC, disponível pelo MSYS2/UCRT64 | Compilação e verificação de avisos. Os comandos documentados dos experimentos usam `-Wall -Wextra -O0`. Na revisão também foram usados `-std=c11` e `-Wpedantic`. |
| PowerShell | Compilação, execução dos testes e conferência dos CSVs no Windows. |
| Git | Controle das alterações. Ainda há arquivos novos e modificados que precisam entrar na versão final do grupo. |
| Markdown (`.md`) | Documentação, análises teóricas e este resumo. |
| CSV | Armazenamento dos resultados para montar tabelas e gráficos. |
| `stdio.h` | Leitura, impressão e gravação de arquivos. |
| `stdlib.h` | Alocação e liberação de memória com `malloc` e `free`. |
| `time.h` | Medição com `clock()` e conversão para segundos usando `CLOCKS_PER_SEC`. |
| Vetores de inteiros alocados dinamicamente | Armazenamento das entradas, incluindo os tamanhos maiores. |
| Ponteiros | Passagem dos vetores e atualização dos contadores das versões instrumentadas. |
| Laços e recursão | A inversão usa dois índices que caminham das pontas para o centro; a busca recursiva reduz o intervalo aproximadamente pela metade a cada chamada. |

As funções originais ficam em `src/algoritmos.c`, e suas declarações ficam em `src/algoritmos.h`. O `src/main.c` cuida da entrada, dos comandos e da saída. Cada auxiliar de experimento tem seu próprio `main` e deve ser compilado separadamente com `src/algoritmos.c`.

## 3. Como as pastas estão organizadas

```text
src/           Programa principal e funções dos algoritmos
experiments/   Auxiliares de medição e contagem de operações
data/          Resultados experimentais em CSV
analysis/      Análises teóricas de cada algoritmo
graphs/        Gráficos — ainda faltam
report/        Relatório final — ainda falta
submission/    Arquivos preparados para envio — ainda faltam
```

Existe também um guia em `output/pdf/guia_do_projeto.pdf` e material de apoio em `tmp/`. O guia não substitui o relatório final, e os exemplos temporários não devem ser confundidos com os resultados oficiais dos experimentos.

## 4. Alterações recentes na inversão

O auxiliar é `experiments/auxiliar_inversao.c`. A função original `inverte()` continua em `src/algoritmos.c`; não há um arquivo separado chamado `inversao.c` nessa organização.

Em relação à versão anterior registrada no Git, o auxiliar foi reorganizado e recebeu:

- **Contadores por parâmetro:** comparações e atribuições deixaram de ser variáveis globais e passaram a ser variáveis locais, atualizadas por ponteiros.
- **Contagem mais explícita:** a versão instrumentada usa `while` e `break` para deixar visível cada avaliação da condição, inclusive a última, que encerra o laço. A função original continua usando `for`.
- **Aquecimento:** são feitas 10 inversões antes da medição, sem incluí-las no tempo calculado.
- **Funções auxiliares:** o preenchimento do vetor e o cálculo das repetições foram separados em funções.
- **Conferência automática da teoria:** o programa compara as contagens com as fórmulas esperadas e avisa se houver divergência.
- **Registro do tempo total:** o CSV passou a guardar tanto o tempo total do bloco quanto a média por execução, com mais casas na gravação.
- **Comentários e instruções de compilação mais detalhados.**

A separação entre função original para tempo e função instrumentada para contagem já existia e foi mantida. A regra de repetições também foi mantida: `100000000 / n`, com mínimo de 100 execuções.

O arquivo `data/resultados_inversao.csv` foi atualizado com o novo formato e novos tempos. As contagens permaneceram iguais e coincidem com a teoria. Mais casas decimais no arquivo não significam maior precisão do relógio.

Também foi preenchido `analysis/inversao.md`, com código, contagem passo a passo, três casos, fórmulas para tamanhos pares e ímpares e comparação com o CSV.

Chamando de `m = floor(n / 2)` o número de repetições do laço:

```text
C(n) = m + 1
A(n) = 2 + 5m
T(n) = C(n) + A(n) = 6m + 3
```

`floor` significa arredondar para baixo. A complexidade é `O(n)` no melhor, médio e pior caso, porque a quantidade de operações depende apenas do tamanho do vetor.

## 5. Alterações recentes na busca binária recursiva

O auxiliar é `experiments/auxiliar_buscabinaria_rec.c`. A função original `busca_binaria_recursiva()` fica em `src/algoritmos.c`.

Foi adicionado um experimento seguindo o mesmo padrão da inversão:

- **Função original para medir tempo** e versão `busca_binaria_recursiva_conta()` para contar operações separadamente.
- **Contadores por ponteiros**, seguindo a mesma convenção do restante do trabalho.
- **100 buscas de aquecimento**, fora da medição.
- **1.000.000 de execuções por tamanho**, pois cada busca é muito rápida.
- **Pior caso definido:** vetor com `0, 1, ..., n - 1`, procurando `p = n`. O valor não existe e a busca segue pela direita até o intervalo ficar vazio.
- **Verificação do retorno:** tanto a função original quanto a instrumentada devem retornar `0` nesse cenário.
- **CSV com o mesmo cabeçalho da inversão**, salvo em `data/resultados_binaria_recursiva.csv`.
- **Análise teórica preenchida** em `analysis/busca_binaria_recursiva.md`, incluindo melhor caso, hipóteses do caso médio, recorrências e comparação com os resultados.

No pior caso, para `n >= 1`, chamamos de `k` o número de chamadas com intervalo não vazio:

```text
k = floor(log₂ n) + 1
C(n) = 3k + 1
A(n) = k
T(n) = C(n) + A(n) = 4k + 1
```

Além das `k` chamadas, existe uma chamada final com intervalo vazio, que faz apenas uma comparação. A complexidade é `O(1)` no melhor caso e `O(log n)` no caso médio analisado e no pior caso.

As contagens do CSV já foram conferidas com essas fórmulas. Essa conferência foi feita na revisão do projeto; o auxiliar recursivo atualmente verifica o retorno, mas ainda não compara automaticamente as contagens com as fórmulas, como o auxiliar da inversão faz.

## 6. Padrão dos experimentos e resultados já conferidos

Os dois auxiliares usam os cinco tamanhos previstos no resumo do trabalho. Alocação, preenchimento, impressão e gravação dos arquivos ficam fora da região cronometrada. O tempo medido inclui o laço que repete as chamadas; os contadores são executados separadamente.

```text
algoritmo,n,repeticoes,tempo_total_s,tempo_medio_s,comparacoes,atribuicoes
```

O tempo médio é `tempo_total_s / repeticoes`. Já as comparações e atribuições representam uma única execução do algoritmo.

| n | Execuções da inversão | Comparações / atribuições da inversão | Execuções da busca recursiva | Comparações / atribuições da busca recursiva |
| ---: | ---: | ---: | ---: | ---: |
| 1.000 | 100.000 | 501 / 2.502 | 1.000.000 | 31 / 10 |
| 50.000 | 2.000 | 25.001 / 125.002 | 1.000.000 | 49 / 16 |
| 100.000 | 1.000 | 50.001 / 250.002 | 1.000.000 | 52 / 17 |
| 500.000 | 200 | 250.001 / 1.250.002 | 1.000.000 | 58 / 19 |
| 1.000.000 | 100 | 500.001 / 2.500.002 | 1.000.000 | 61 / 20 |

Todas essas contagens coincidem com a teoria. As médias dos dois CSVs também conferem com o tempo total dividido pelas repetições.

O `analysis/README.md` define o padrão que devemos seguir: cada comparação e atribuição tem peso 1, e `T(n) = C(n) + A(n)`. Incrementos e decrementos contam como atribuições. Não contamos separadamente operações aritméticas, acessos ao vetor, passagem de parâmetros, retornos ou atualizações dos próprios contadores.

## 7. O que ainda falta para concluir

### Implementação e testes

- [ ] Implementar `busca_sequencial()` e `busca_binaria_iterativa()`. Atualmente as duas apenas retornam `0`, então os comandos 2 e 3 imprimem `NAO` mesmo para valores presentes.
- [ ] Testar as três buscas com valores no início, no meio, no fim e ausentes.
- [ ] Comparar os retornos das três buscas para as mesmas entradas.
- [ ] Testar sequências de comandos, tamanho máximo e exemplos oficiais.
- [ ] Conferir se a inversão deve apenas ser exibida ou alterar o vetor permanentemente. Hoje o `main` inverte uma cópia, preservando o original ordenado.
- [ ] Revisar verificações de `malloc`, retornos de `scanf` e limites permitidos de `n`. O comando de inversão do `main` pressupõe `n >= 1`.
- [ ] Preencher os nomes e números USP no cabeçalho e conferir a divergência entre `SCC0502`, no `main`, e `SCC0201`, no nome do projeto.

### Experimentos e análises

- [ ] Criar os auxiliares da busca sequencial e da busca binária iterativa.
- [ ] Gerar os dois CSVs restantes com o mesmo padrão de colunas, tamanhos e contagem.
- [ ] Preencher `analysis/busca_sequencial.md` e `analysis/busca_binaria_iterativa.md`, que ainda estão vazios.
- [ ] Conferir as fórmulas com as implementações finais e com os resultados medidos.
- [ ] Registrar o ambiente real usado nas medições: computador, sistema operacional, compilador, versão e opções de compilação. Os comandos documentados não substituem esse registro.

### Gráficos e relatório

- [ ] Produzir gráficos de tempo médio, comparações e atribuições em função de `n`.
- [ ] Identificar títulos, legendas, unidades e escalas; separar comparações quando necessário para que os valores das buscas binárias continuem legíveis.
- [ ] Montar o relatório com integrantes, objetivo, algoritmos, teoria, metodologia, resultados, gráficos, discussão, conclusão e referências utilizadas.
- [ ] Comparar o crescimento linear com o logarítmico e discutir as diferenças entre busca binária iterativa e recursiva. O modelo de contagem não inclui separadamente o custo das chamadas recursivas.
- [ ] Explicar que os tempos podem variar com o ambiente e que os gráficos apoiam a análise, enquanto a classificação assintótica vem da dedução matemática.
- [ ] Registrar as contribuições reais de cada integrante.

### Documentação e entrega

- [ ] Completar o README principal com instruções de compilação e execução.
- [ ] Documentar como executar os experimentos, interpretar os CSVs e gerar os gráficos. Os READMEs de `experiments`, `data`, `graphs`, `report` e `submission` ainda estão vazios.
- [ ] Confirmar o formato exigido pelo RunCodes, inclusive se o código deve ser entregue em um único arquivo C.
- [ ] Preparar os arquivos finais de submissão e testá-los no formato de entrega.
- [ ] Separar o relatório e os códigos auxiliares para o e-disciplinas, conforme o resumo do trabalho.
- [ ] Revisar os arquivos que devem entrar no Git e registrar a versão final, sem misturar resultados reais com exemplos temporários.
- [ ] Confirmar prazo, horário, regras de envio e resultado das submissões nas plataformas.

O enunciado oficial completo não está no projeto revisado. As exigências de entrega precisam ser conferidas nele; o resumo da conversa menciona entrega em 18/09.

## 8. Como dividir os próximos passos

Mantendo a divisão sugerida anteriormente, sem atribuir nomes ou autoria que ainda não foram confirmados:

| Responsabilidade | Próxima entrega |
| --- | --- |
| Inversão + integração | Revisar o programa principal, a inversão sobre cópia e os testes integrados; levar os resultados da inversão para o relatório. |
| Busca sequencial + medição | Entregar implementação, auxiliar, CSV e análise da busca sequencial; ajudar a registrar o ambiente e padronizar as medições. |
| Busca binária iterativa + contagem | Entregar implementação, auxiliar, CSV e análise da busca iterativa; revisar a convenção de contagem entre os quatro algoritmos. |
| Busca binária recursiva + gráficos/relatório | Revisar os resultados da recursiva e organizar os gráficos e o relatório com os textos enviados por todos. |
| Todo o grupo | Conferir resultados, revisar a contribuição de outro integrante e validar a entrega final. |

Cada integrante continua responsável pelo ciclo completo do seu algoritmo. Quem organiza o relatório reúne e padroniza os textos; cada responsável entrega sua análise e sua discussão dos resultados.

## 9. Melhorias recomendadas, se houver tempo

- Repetir as medições em várias rodadas para avaliar a variação dos tempos.
- Adicionar ao auxiliar recursivo a comparação automática entre contagens medidas e fórmulas.
- Fazer os auxiliares encerrarem com erro quando uma validação falhar, em vez de apenas imprimir um aviso e continuar.
- Guardar casos de teste com entrada e saída esperada para repetir a validação após mudanças.

Essas melhorias não foram confirmadas como exigências do enunciado. A prioridade é terminar as duas buscas, seus experimentos e análises, depois produzir os gráficos, o relatório e os arquivos de entrega.
