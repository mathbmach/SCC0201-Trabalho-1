from pathlib import Path
from xml.sax.saxutils import escape
from reportlab.pdfgen import canvas
from reportlab.platypus import Paragraph, Table, TableStyle
from reportlab.lib.styles import ParagraphStyle
from reportlab.lib.colors import HexColor, white
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.ttfonts import TTFont
from reportlab.lib.enums import TA_LEFT
import json

ROOT=Path(__file__).resolve().parents[2]
OUT=ROOT/'output/pdf/guia_do_projeto.pdf'
OUT.parent.mkdir(parents=True,exist_ok=True)
for name,file in [('UI','segoeui.ttf'),('Bold','segoeuib.ttf'),('Light','segoeuil.ttf'),('Mono','consola.ttf')]:
    pdfmetrics.registerFont(TTFont(name,'C:/Windows/Fonts/'+file))
pdfmetrics.registerFontFamily('UI',normal='UI',bold='Bold',italic='UI',boldItalic='Bold')
W,H=595.276,841.89
INK=HexColor('#1d1d1f'); GRAY=HexColor('#626269'); BLUE=HexColor('#0066cc'); LIGHT=HexColor('#f5f5f7')
styles={
 'body':ParagraphStyle('body',fontName='UI',fontSize=10.5,leading=15.5,textColor=INK,spaceAfter=9),
 'small':ParagraphStyle('small',fontName='UI',fontSize=8.5,leading=12,textColor=GRAY),
 'h':ParagraphStyle('h',fontName='Bold',fontSize=13.5,leading=18,textColor=INK),
 'cell':ParagraphStyle('cell',fontName='UI',fontSize=9,leading=12.8,textColor=INK),
}
c=canvas.Canvas(str(OUT),pagesize=(W,H))
c.setTitle('Guia do projeto | Algoritmos, experimentos e GitHub')
c.setAuthor('Guia de apoio à equipe')
pages=[]; y=0; page_no=0
def p(text,kind='body'):
 global y
 obj=Paragraph(text,styles[kind]); _,height=obj.wrap(W-100,700)
 if y-height<58: raise ValueError(f'Overflow page {page_no}: {text[:80]} y={y} h={height}')
 obj.drawOn(c,50,y-height); y-=height+(9 if kind=='body' else 7)
def h(text):
 global y
 y-=7; p(text,'h'); y-=2
def code(text,size=9):
 global y
 lines=text.strip('\n').split('\n'); lh=size*1.4; height=lh*len(lines)+22
 if y-height<58:raise ValueError(f'Code overflow {page_no} {y} {height}')
 c.setFillColor(LIGHT); c.roundRect(50,y-height,W-100,height,8,fill=1,stroke=0)
 c.setFillColor(INK);c.setFont('Mono',size)
 for i,line in enumerate(lines):
  if pdfmetrics.stringWidth(line,'Mono',size)>W-126:raise ValueError(f'Wide line {page_no}: {line}')
  c.drawString(63,y-15-i*lh,line)
 y-=height+12
def table(headers,rows,widths):
 global y
 data=[[Paragraph(escape(str(v)),styles['cell']) for v in row] for row in [headers]+rows]
 t=Table(data,colWidths=widths,hAlign='LEFT');t.setStyle(TableStyle([
 ('BACKGROUND',(0,0),(-1,0),LIGHT),('VALIGN',(0,0),(-1,-1),'TOP'),
 ('LEFTPADDING',(0,0),(-1,-1),9),('RIGHTPADDING',(0,0),(-1,-1),9),
 ('TOPPADDING',(0,0),(-1,-1),9),('BOTTOMPADDING',(0,0),(-1,-1),9),
 ('LINEBELOW',(0,0),(-1,-1),.4,HexColor('#dedee3'))]))
 _,ht=t.wrap(W-100,700)
 if y-ht<58:raise ValueError(f'Table overflow {page_no}')
 t.drawOn(c,50,y-ht);y-=ht+14
def note(title,text):
 h(title);p(text)
def page(section,title,subtitle=''):
 global y,page_no
 if page_no:c.showPage()
 page_no+=1;pages.append((page_no,section,title));c.bookmarkPage('p'+str(page_no));c.addOutlineEntry(title,'p'+str(page_no),0)
 c.setFillColor(GRAY);c.setFont('UI',8);c.drawString(50,H-35,'GUIA DA EQUIPE  /  TRABALHO 1')
 c.drawRightString(W-50,32,f'{page_no:02d}')
 c.setStrokeColor(HexColor('#e6e6e9'));c.line(50,49,W-50,49)
 c.setFont('Bold',9);c.setFillColor(BLUE);c.drawString(50,H-76,section.upper())
 title_style=ParagraphStyle('title',fontName='Light',fontSize=29,leading=34,textColor=INK)
 ob=Paragraph(title,title_style);_,ht=ob.wrap(W-100,120);ob.drawOn(c,50,H-96-ht);y=H-110-ht
 if subtitle:p(subtitle,'small');y-=8

page('Manual de trabalho','Entender.<br/>Experimentar.<br/>Construir juntos.','ALGORITMOS E ESTRUTURAS DE DADOS I')
y-=25
p('Um guia passo a passo para implementar os quatro algoritmos, medir seu comportamento e transformar os resultados em uma análise clara.','h')
y-=22
table(['01 / Código','02 / Experimentos','03 / Colaboração'],[
 ['Correção e contratos','Tempo e operações','GitHub direto na main'],
 ['Inversão e buscas','Dados, gráficos e teoria','Integração e entrega']],[165,165,165])
y-=20
p('Feito para todos os membros do projeto. Leia na ordem na primeira vez; depois use o sumário e os marcadores do PDF como referência.')
p('Edição de apoio • 17 de setembro de 2026','small')
note('O que este material representa','O guia combina o resumo do trabalho fornecido na conversa, o código local e a estrutura exibida na imagem. As melhorias de método são recomendações. O enunciado oficial completo não foi disponibilizado; ele prevalece em requisitos de entrada, saída, prazo e formato de entrega.')

page('Roteiro de leitura','Do primeiro teste à entrega')
table(['Páginas','O que você vai encontrar'],[
 ['3–5','Escopo, estado do projeto, pastas e responsabilidades'],
 ['6–8','Correção, funcionamento dos algoritmos e desenho experimental'],
 ['9–13','Contagem de operações e fórmulas verificáveis'],
 ['14–17','Cronômetro, repetições, rodadas e interpretação estatística'],
 ['18–20','CSV, gráficos e análise teórica'],
 ['21–23','GitHub: primeiro acesso, rotina na main e conflitos'],
 ['24–25','Relatório, integração e checklist de entrega'],
 ['26–29','Exemplo completo de experimento em C, pronto para estudar'],
 ['30','Referências e decisões a confirmar']],[70,425])
h('Se você vai começar agora')
p('<b>1.</b> Leia o contrato das funções e combine o padrão de contagem com a equipe.<br/><b>2.</b> Faça os testes pequenos antes de medir desempenho.<br/><b>3.</b> Estude um experimento completo e só depois adapte o seu algoritmo.<br/><b>4.</b> Salve os dados brutos, interprete os gráficos e escreva sua parte.<br/><b>5.</b> Atualize e envie alterações em pequenos commits na main.')
note('Três palavras que não significam a mesma coisa','<b>Teste:</b> verifica se o resultado está certo. <b>Experimento:</b> mede comportamento sob condições definidas. <b>Análise:</b> explica o resultado e o relaciona à teoria.')

page('01 / Escopo','O que a equipe precisa produzir')
p('Segundo o resumo fornecido pelo grupo, o trabalho reúne um programa em C, uma análise experimental e uma análise teórica. Cada algoritmo precisa aparecer nas três frentes.')
table(['Frente','Resultado esperado'],[
 ['Programa','Comandos 1: inversão; 2: busca sequencial; 3: binária iterativa; 4: binária recursiva; 5: encerrar.'],
 ['Experimento','Cinco tamanhos: 1.000, 50.000, 100.000, 500.000 e 1.000.000. Pelo menos 100 execuções por tamanho; tempo médio; comparações e atribuições no pior caso.'],
 ['Teoria','Função de custo T(n), melhor, médio e pior caso; termo dominante; classificação em Grande O.'],
 ['Comunicação','Tabelas, gráficos, metodologia reproduzível e discussão dos resultados.']],[92,403])
h('Requisito relatado × melhoria sugerida')
p('<b>Relatado pelo grupo:</b> os cinco tamanhos, o mínimo de 100 execuções, a análise e as entregas acima.<br/><b>Sugerido neste guia:</b> aquecimento, 10 rodadas por configuração, cronômetro de alta resolução, calibração de repetições, desvio-padrão e preservação dos tempos de cada rodada.')
p('Essas melhorias tornam as conclusões mais defensáveis, mas não garantem uma nota específica. O critério de avaliação é o do professor.')
note('Entrega informada na conversa','Código principal pelo RunCodes; relatório e códigos auxiliares pelo e-disciplinas. O resumo menciona 18/09: confirmem ano, horário e arquivos exigidos no enunciado ou na página da disciplina.')

page('02 / Ponto de partida','O projeto que existe hoje')
table(['Arquivo / evidência','Situação observada em 17/09/2026'],[
 ['src/algoritmos.c','Inversão implementada; binária recursiva implementada. Sequencial e binária iterativa ainda retornam 0 sem fazer busca.'],
 ['src/main.c','Lê comandos e imprime SIM/NAO. Usa cópia para inverter, preservando o vetor ordenado das buscas.'],
 ['experiments/auxiliar_inversao.c','Mede lote com clock(), usa contadores globais e grava CSV. Ainda não mede variação entre rodadas.'],
 ['data/resultados_inversao.csv','Possui cinco linhas de resultados. As contagens coincidem com as fórmulas da inversão; faltam metadados para reproduzir os tempos.']],[170,325])
h('Contrato que este guia adota')
p('As buscas retornam <b>1 se encontrarem o valor e 0 se não encontrarem</b>, como o main atual espera. Não copiem exemplos que retornam índice e -1 sem alterar também quem chama a função: o índice zero seria interpretado como ausência, e -1 como presença.')
p('A busca recursiva recebe limites inclusivos: <b>inicio = 0</b> e <b>fim = n - 1</b>. As duas buscas binárias exigem vetor em ordem crescente para a lógica apresentada aqui.')
note('Nome do projeto','A captura mostra SCC0502-Trabalho-1; a pasta, o README e o remoto local usam SCC0201-Trabalho-1. O guia usa um título neutro. Confirmem o código correto da disciplina antes de fechar a capa do relatório.')

page('03 / Organização','Cada pasta tem uma função')
table(['Pasta','O que guardar'],[
 ['src/','main.c, algoritmos.c e algoritmos.h: implementação oficial, sem contadores.'],
 ['experiments/','Um auxiliar por algoritmo; versões instrumentadas; rotinas de tempo compartilhadas, se adotadas.'],
 ['data/','CSV bruto por rodada, resumos e metadados da execução.'],
 ['analysis/','Derivações de T(n), convenção de contagem e scripts de análise.'],
 ['graphs/','Gráficos finais com títulos, legendas e unidades.'],
 ['report/','Texto do relatório e PDF final.'],
 ['submission/','Somente a versão preparada para envio conforme o enunciado.']],[100,395])
p('<b>Fluxo:</b> função correta → experimento → CSV → análise e gráficos → relatório → entrega.')
h('Divisão entre quatro integrantes')
p('<b>Pessoa 1:</b> inversão e integração do main.<br/><b>Pessoa 2:</b> sequencial e padrão de tempo.<br/><b>Pessoa 3:</b> binária iterativa e padrão de contagem.<br/><b>Pessoa 4:</b> binária recursiva e organização dos gráficos/relatório.')
p('Cada pessoa entrega código, testes, contagens, dados, teoria e texto do próprio algoritmo. A organização do relatório não transfere a escrita de todos para uma única pessoa. Combinem quem edita algoritmos.c a cada momento, pois ele é compartilhado.')

page('04 / Antes de medir','Primeiro, prove que funciona')
p('Um programa incorreto pode parecer muito rápido. As duas buscas ainda incompletas do projeto sempre retornam 0; medir esse retorno não mede uma busca. Só iniciem a coleta depois dos testes abaixo.')
table(['Teste','Entrada / resultado esperado'],[
 ['Inversão par','[0,1,2,3] → [3,2,1,0].'],
 ['Inversão ímpar','[0,1,2,3,4] → [4,3,2,1,0]. O centro fica no lugar.'],
 ['Uma posição','[7] permanece [7]; busca de 7 retorna 1, de 8 retorna 0.'],
 ['Buscas','Em [0,1,2,3,4], procurar 0, 2 e 4 retorna 1; procurar -1 e 5 retorna 0.'],
 ['Repetidos','Em [1,1,2,2], buscar 1 ou 2 retorna 1. Não exigimos um índice específico.'],
 ['Integração','Executar inversão e depois uma busca binária: a busca deve continuar correta.'],
 ['Instrumentação','Versão original e contada dão o mesmo resultado sobre entradas equivalentes.']],[105,390])
h('Como executar e conferir')
p('Compile com avisos habilitados. Para cada caso, escreva a entrada e a saída esperada antes de executar. Se houver erro, corrija a lógica e repita os casos relacionados. Depois valide também as contagens à mão em n = 1, 2, 3, 4 e 5.')
code('gcc -std=c11 -Wall -Wextra -O2 src/main.c \\*\n    src/algoritmos.c -o programa.exe'.replace(' \\*\n    ',' '),size=8.3)
p('No PowerShell: execute <b>.\\programa.exe</b> e forneça os dados no formato pedido. Não adicione mensagens de menu ou instruções à saída se o juiz espera apenas os resultados.','small')

page('05 / Mecanismos','O que cada algoritmo faz')
h('Inversão: trocar as extremidades')
p('Dois índices começam nas pontas. A cada passo, uma variável temporária permite trocar os valores sem perder nenhum deles. O índice esquerdo avança e o direito recua. Em n = 5, são feitas as trocas (0,4) e (1,3); a posição 2 permanece no centro.')
code('0 1 2 3 4   ->   4 1 2 3 0   ->   4 3 2 1 0')
h('Sequencial: olhar uma posição por vez')
p('Começa na posição zero e compara cada elemento com o alvo. Para ao encontrar ou ao terminar o vetor. Se o alvo é n e o vetor vai de 0 a n-1, todos os elementos são examinados.')
h('Binária: descartar metade do intervalo')
p('Com o vetor ordenado, examine o meio. Se o alvo for maior, descarte a metade esquerda; se for menor, descarte a direita. Exemplo: em [0,1,2,3,4,5,6], procurar 7 visita os valores 3, 5 e 6; o intervalo então fica vazio.')
h('Iteração e recursão: duas formas de repetir')
p('A iterativa atualiza inicio e fim em um laço. A recursiva chama a si mesma com um intervalo menor. O caso-base <b>inicio &gt; fim</b> encerra a busca ausente. O cálculo <b>inicio + (fim-inicio)/2</b> evita somar diretamente os limites.')
note('Por que não buscar no vetor invertido?','A decisão de descartar metade supõe a ordem crescente. Recrie o vetor antes de cada experimento de busca; no programa principal, mantenha a estratégia atual de inverter uma cópia.')

page('06 / Desenho experimental','Uma configuração por vez')
p('Uma configuração é a combinação de <b>algoritmo + tamanho n + cenário</b>. Para a coleta principal, use os cinco tamanhos e um cenário de pior caso por algoritmo: são 20 configurações.')
table(['Algoritmo','Preparação','O que acontece'],[
 ['Inversão','v[i] = i','Sempre faz piso(n/2) trocas. Os valores não alteram a contagem.'],
 ['Sequencial','v[i] = i; alvo = n','O alvo não existe; percorre o vetor inteiro.'],
 ['Binária iterativa','v[i] = i; alvo = n','Segue a direita até esvaziar o intervalo.'],
 ['Binária recursiva','v[i] = i; alvo = n','Mesmo caminho, com chamada final ao intervalo vazio.']],[115,140,240])
p('Para os limites inclusivos e o meio arredondado para baixo usados neste guia, alvo = n produz um caminho de comprimento máximo: <b>k = piso(log₂ n) + 1</b>, para n ≥ 1. Isso precisa ser revisto se a implementação mudar.')
h('Prepare fora do cronômetro')
code('int *v = malloc((size_t)n * sizeof *v);\nif (v == NULL) { /* informar erro e encerrar */ }\nfor (int i = 0; i < n; i++) v[i] = i;\nint alvo = n;')
p('Um milhão de inteiros ocupa cerca de 4 MB quando sizeof(int) = 4. Use alocação dinâmica e verifique falhas. Ao terminar uma configuração, libere o vetor com free(v).')
p('Melhor caso e caso médio podem ser experimentos adicionais. Não misture cenários no mesmo resumo. Uma busca por um valor fixo do meio não representa, sozinha, o caso médio.','small')

page('07 / Instrumentação','A regra de contagem do grupo')
p('Definam a convenção antes de derivar fórmulas. Neste guia, contamos operações no código-fonte; não instruções de máquina nem incrementos dos próprios contadores.')
table(['Conta como comparação','Conta como atribuição'],[
 ['Cada avaliação de condição original: i < n, inicio <= fim, v[i] == p, v[meio] < p.','Inicialização com valor, cópia e atualização: i = 0, meio = ..., v[i] = ..., i++, j--.'],
 ['Inclui a condição final falsa do laço, quando ela ocorre.','i++ conta uma atribuição; a soma não vira outra atribuição.']],[247,248])
p('<b>Não entram:</b> declaração sem inicialização, return, passagem de parâmetros, chamadas, aritmética isolada, preparação do vetor, malloc, impressão, estatísticas e contadores auxiliares. Se o professor pedir outro modelo, ajustem todas as fórmulas e os quatro contadores juntos.')
h('O contador descreve o algoritmo original')
p('Uma instrumentação pode usar while (1) e break para tornar cada teste explícito. Esses mecanismos adicionais não contam: cada incremento representa uma operação existente no algoritmo original. Zere os contadores antes de cada chamada completa.')
code('typedef struct {\n    long long comparacoes;\n    long long atribuicoes;\n} Contagem;\n\nContagem op = {0, 0};  /* no programa auxiliar */')
note('Contagem não é tempo','Execute a versão contada uma vez por entrada determinística. Meça tempo com a função original. Cronometrar os contadores mede também o custo da instrumentação.')

page('08 / Inversão','Da troca à função de custo')
p('Seja <b>m = piso(n/2)</b>. Existem duas inicializações, m avaliações verdadeiras do laço, uma avaliação falsa, três atribuições de troca e duas atualizações por iteração.')
code('void inverte_conta(int *v, int n, Contagem *op) {\n    int i = 0, j = n - 1, temp;\n    op->atribuicoes += 2;\n    for (;;) {\n        op->comparacoes++;\n        if (!(i < n / 2)) break;\n        temp = v[i];\n        v[i] = v[j];\n        v[j] = temp;\n        op->atribuicoes += 3;\n        i++; j--;\n        op->atribuicoes += 2;\n    }\n}')
table(['Medida','Fórmula','Para n = 5'],[
 ['Comparações C(n)','m + 1','3'],['Atribuições A(n)','5m + 2','12'],['Custo T(n) = C(n) + A(n)','6m + 3','15']],[145,200,150])
p('Para os cinco tamanhos pares, C(n) = n/2 + 1 e A(n) = 5n/2 + 2. Para tamanhos ímpares, preserve o piso: não use uma contagem fracionária.')
p('Melhor, médio e pior caso têm a mesma contagem para n fixo. O termo dominante é linear; portanto, T(n) é Θ(n), e também O(n). O espaço auxiliar da função é O(1). A cópia feita pelo main usa O(n), mas fica fora da função medida.')

page('09 / Sequencial','Conte também a saída do laço')
p('O exemplo retorna 1 ou 0, como o projeto. A instrumentação abaixo corresponde a um for que avança até achar o alvo ou terminar o vetor.')
code('int sequencial_conta(int *v, int n, int p,\n                     Contagem *op) {\n    int i = 0;\n    op->atribuicoes++;\n    for (;;) {\n        op->comparacoes++;\n        if (!(i < n)) break;\n        op->comparacoes++;\n        if (v[i] == p) return 1;\n        i++;\n        op->atribuicoes++;\n    }\n    return 0;\n}')
table(['Situação','C','A','T = C + A'],[
 ['Alvo ausente','2n + 1','n + 1','3n + 2'],
 ['Alvo no índice j','2(j + 1)','j + 1','3(j + 1)'],
 ['Melhor: índice 0','2','1','3']],[170,110,95,120])
p('Com alvo ausente, o teste i &lt; n ocorre n + 1 vezes; a igualdade, n vezes. Encontrar o último elemento evita o teste final falso, então a contagem exata difere da ausência, embora ambos sejam O(n).')
p('Para sucesso igualmente provável em cada uma das n posições, a média de elementos visitados é (n+1)/2: E[T] = 3(n+1)/2. Esse caso médio pressupõe uma distribuição; se houver buscas ausentes, declare também sua probabilidade.')

page('10 / Binária iterativa','O intervalo diminui a cada passo')
code('int binaria_conta(int *v, int n, int p,\n                  Contagem *op) {\n    int inicio = 0, fim = n - 1;\n    op->atribuicoes += 2;\n    for (;;) {\n        op->comparacoes++;\n        if (!(inicio <= fim)) break;\n        int meio = inicio + (fim - inicio) / 2;\n        op->atribuicoes++;\n        op->comparacoes++;\n        if (v[meio] == p) return 1;\n        op->comparacoes++;\n        if (v[meio] < p) inicio = meio + 1;\n        else fim = meio - 1;\n        op->atribuicoes++;\n    }\n    return 0;\n}')
p('No cenário v[i] = i e p = n, cada uma das k iterações realiza três comparações e duas atribuições. Somam-se as duas inicializações e a comparação final do laço.')
table(['Cenário','C','A','T'],[
 ['Ausência: k = piso(log₂ n) + 1','3k + 1','2k + 2','5k + 3'],
 ['Encontra no primeiro meio','2','3','5'],
 ['Sucesso após k iterações','3k - 1','2k + 1','5k']],[245,83,83,84])
p('Em n = 1.000, k = 10: C = 31 e A = 22. Em n = 1.000.000, k = 20: C = 61 e A = 42. O vetor fica mil vezes maior, mas o caminho apenas dobra de comprimento.')

page('11 / Binária recursiva','Cada chamada tem seu próprio intervalo')
code('int recursiva_conta(int *v, int inicio, int fim,\n                    int p, Contagem *op) {\n    op->comparacoes++;\n    if (inicio > fim) return 0;\n    int meio = inicio + (fim - inicio) / 2;\n    op->atribuicoes++;\n    op->comparacoes++;\n    if (v[meio] == p) return 1;\n    op->comparacoes++;\n    if (v[meio] < p)\n        return recursiva_conta(v, meio + 1, fim, p, op);\n    return recursiva_conta(v, inicio, meio - 1, p, op);\n}')
p('Passe o mesmo ponteiro op pelas chamadas. Ele acumula o total; não zere os contadores dentro da função, pois isso apagaria as operações anteriores.')
table(['Cenário','C','A','T'],[
 ['Ausência: k chamadas não vazias','3k + 1','k','4k + 1'],
 ['Encontra no primeiro meio','2','1','3'],
 ['Sucesso após k chamadas','3k - 1','k','4k - 1']],[245,83,83,84])
p('A ausência acrescenta uma chamada final com intervalo vazio. Ela faz uma comparação e nenhuma atribuição. Parâmetros de entrada não são contados como atribuições neste modelo.')
note('Menos atribuições não significa menor tempo','A contagem escolhida não representa o custo de chamadas nem de controle da pilha. A recursiva pode ter tempos diferentes da iterativa, apesar da mesma ordem O(log n). O espaço auxiliar é O(log n) sem eliminação de chamadas de cauda pelo compilador; a iterativa usa O(1).')

page('12 / Cronometragem','Meça um intervalo confiável')
p('Uma busca binária isolada pode ser rápida demais para a resolução prática do relógio. Cronometre um <b>lote de R chamadas</b> e divida o tempo total por R. Assim, o custo de consultar o relógio é diluído.')
h('No Windows')
p('Use QueryPerformanceCounter para ler um contador de alta resolução e QueryPerformanceFrequency para conhecer quantos ticks representam um segundo. A duração é a diferença entre leituras dividida pela frequência. O exemplo completo está nas páginas 26–29. [1]')
code('tempo_total_s = (fim.QuadPart - inicio.QuadPart)\n                / (double)frequencia.QuadPart;\ntempo_por_chamada_s = tempo_total_s / repeticoes;')
h('O que fica dentro e fora')
table(['Fora do cronômetro','Dentro do cronômetro'],[
 ['Alocação, geração, cópia de preparação, aquecimento e calibração.','Laço de repetições e chamadas da função original.'],
 ['Contagem de operações, validação, média, desvio-padrão, printf e CSV.','Nas buscas, um pequeno consumo do retorno pode ficar no lote para tornar o resultado observável.']],[247,248])
p('O lote inclui algum custo do laço e das chamadas: o resultado é uma estimativa do custo por execução sob esse protocolo. Em algoritmos muito rápidos, esse custo pode importar; documente-o e mantenha a mesma estratégia nas buscas.')
note('E o clock() atual?','Não é automaticamente inválido, mas sua resolução e semântica dependem da implementação. Não trate CLOCKS_PER_SEC como garantia de precisão real. Se trocar o relógio, refaça a série completa; não una tempos obtidos com métodos diferentes. [1]')

page('13 / Repetições','R chamadas formam uma rodada')
h('1. Aqueça')
p('Execute, por exemplo, 20 chamadas sem medir. Isso reduz o peso do primeiro acesso a código e dados. O experimento passa a caracterizar execuções repetidas com dados reutilizados, não uma operação com cache frio.')
h('2. Calibre R para cada algoritmo e tamanho')
p('Comece com R = 100. Meça um lote piloto. Se durar menos que uma meta como 0,05 s, dobre R e tente novamente. Pare quando atingir a meta ou um limite de segurança. O piloto não entra nas estatísticas finais.')
code('R = 100;\nrepetir:\n    medir R chamadas;\n    se tempo_total >= 0.05 s: terminar calibracao;\n    se dobrar R exceder limite: registrar falha;\n    R = 2 * R;')
h('3. Fixe R e colete M rodadas')
p('Uma sugestão é M = 10 rodadas, todas com o R calibrado para aquela configuração. Se R = 10.000, cada rodada resume 10.000 chamadas; dez rodadas representam 100.000 chamadas medidas. Aquecimento e pilotos são adicionais.')
note('Por que R pode mudar?','O que será comparado é o tempo por chamada. A inversão de um vetor grande precisa de menos repetições para formar um lote longo; uma busca binária precisa de mais. A regra de calibração deve ser a mesma e o R utilizado deve ser salvo.')
p('A fórmula atual max(100, 100000000/n) é uma heurística razoável para trabalho linear. Não a copie automaticamente para as buscas binárias: a duração delas não cresce proporcionalmente a n.','small')

page('14 / Execução','A sequência de uma configuração')
table(['Passo','Ação','Por quê'],[
 ['1','Alocar e preencher v[i] = i.','Definir uma entrada reproduzível.'],
 ['2','Testar o resultado original e instrumentado.','Impedir que um erro vire dado.'],
 ['3','Preparar novamente o vetor.','Restaurar a entrada após os testes.'],
 ['4','Aquecer e calibrar R.','Obter um lote mensurável.'],
 ['5','Medir M lotes e guardar cada duração.','Quantificar a variação observada.'],
 ['6','Dividir cada lote por R.','Obter o tempo por chamada.'],
 ['7','Contar uma execução separada.','Registrar C e A sem contaminar o tempo.'],
 ['8','Calcular resumo, salvar e liberar memória.','Permitir auditoria e repetição.']],[42,230,223])
h('Repetir a inversão no mesmo vetor funciona?')
p('Sim. Uma inversão o deixa decrescente, a próxima o restaura. A função percorre os mesmos índices em ambos os casos. Antes de cada rodada, você pode preencher novamente fora do cronômetro e aquecer com número par de inversões.')
h('Evite medir código eliminado')
p('Use as funções de src/algoritmos.c compiladas separadamente do auxiliar, sem LTO. Nas buscas, consuma os retornos em uma soma local e use-a depois do lote; se necessário, use um destino volatile no auxiliar. Não torne o vetor inteiro volatile, pois isso altera os acessos medidos. Verifique também o resultado da inversão após o lote.')
p('Mantenha a mesma máquina e as mesmas opções de compilação. Evite executar os quatro experimentos simultaneamente e registre se houve mudança de carga ou energia.','small')

page('15 / Estatística','Média e variação, sem mistério')
p('Se a rodada r durou Bᵣ segundos e contém R chamadas, o valor daquela rodada é <b>tᵣ = Bᵣ/R</b>. Primeiro calcule os tempos por chamada; depois resuma as M rodadas.')
code('media = (t1 + t2 + ... + tM) / M\n\ns = sqrt( soma((tr - media)^2) / (M - 1) )\n\nCV = 100 * s / media  /* quando media > 0 */')
p('O desvio-padrão amostral s descreve a dispersão dos <b>tempos médios por chamada de cada rodada</b>. Ele não mede a variação de cada chamada individual dentro do lote. Dez rodadas seguidas também não garantem independência estatística.')
h('Um exemplo didático')
table(['Rodada','Lote de 10.000 chamadas','Tempo por chamada'],[
 ['1','0,100 s','10 μs'],['2','0,110 s','11 μs'],['3','0,090 s','9 μs']],[75,205,215])
p('A média é 10 μs e o desvio-padrão é 1 μs. Esses números são ilustrativos, não medições deste projeto. Para apresentar em microssegundos, multiplique segundos por 1.000.000.')
note('Como interpretar a diferença','Se duas médias forem próximas em relação à variação, descreva o resultado como inconclusivo para aquela diferença. Não declare uma versão vencedora por uma única rodada. Média ± desvio-padrão não é automaticamente um intervalo de confiança.')

page('16 / Dados','CSV que permite conferir o trabalho')
p('Guarde os dados de cada rodada e um resumo por configuração. O arquivo atual de inversão é um resumo antigo; preserve sua origem se gerar uma nova série com outro método.')
h('Arquivo bruto: uma linha por rodada')
code('algoritmo,n,cenario,rodada,repeticoes,tempo_total_s\ninversao,1000,padrao,1,100000,0.053000000',size=8.5)
p('A linha acima reaproveita valores de escala apenas para ilustrar o formato; não é uma nova rodada medida. Use ponto decimal no arquivo e vírgula para separar campos. Grave segundos com precisão suficiente, como %.12e.')
h('Resumo: uma linha por configuração')
code('algoritmo,n,cenario,rodadas,repeticoes,media_s,dp_s,\ncomparacoes,atribuicoes',size=8.5)
p('O cabeçalho acima foi quebrado visualmente: no CSV real ele ocupa uma só linha. Com quatro algoritmos, cinco tamanhos e um cenário, são 20 linhas de resumo e, com dez rodadas, 200 linhas brutas.')
h('Metadados: registre junto aos CSVs')
p('Data, responsável, processador, memória, sistema operacional, versão do compilador, comando de compilação, temporizador, aquecimento, número de rodadas, critério de calibração, definição da entrada e identificação do commit testado.')
code('gcc --version\ngit rev-parse HEAD\ngit status --short')
p('O commit sozinho não descreve alterações ainda não commitadas. Prefira medir uma versão salva em commit; caso contrário, registre também essa condição. Contagens determinísticas se repetem entre máquinas; tempos não precisam coincidir.')

page('17 / Gráficos','Mostre crescimento, não só números')
table(['Gráfico','Eixos e leitura'],[
 ['Tempo por chamada','x = n; y = média em segundos ou μs. Barras de erro = desvio-padrão entre rodadas, explicitado na legenda.'],
 ['Comparações','x = n; y = C(n) de uma chamada no pior caso. Sobreponha fórmula e contagem observada.'],
 ['Atribuições','x = n; y = A(n), com a mesma convenção para todos.'],
 ['Binárias em detalhe','Separe iterativa e recursiva em um gráfico próprio para que a escala linear não esconda os valores.']],[135,360])
h('Passo a passo em uma planilha')
p('<b>1.</b> Importe o CSV com separador vírgula e ponto como decimal.<br/><b>2.</b> Confira se n e tempo são números, não texto.<br/><b>3.</b> Crie um gráfico de dispersão XY, usando n no eixo x.<br/><b>4.</b> Separe séries por algoritmo e cenário; acrescente unidades.<br/><b>5.</b> Use os valores da coluna dp_s nas barras de erro, se a ferramenta permitir.<br/><b>6.</b> Exporte a figura e salve em graphs/ junto do arquivo ou script que a gera.')
p('Os tamanhos não são igualmente espaçados: 1.000 → 50.000 difere de 500.000 → 1.000.000. Um gráfico de categorias igualmente espaçadas pode distorcer essa leitura. Se usar escala logarítmica, informe isso nos eixos.')
note('O que procurar','Ao dobrar n, contagens lineares aproximadamente dobram. Em buscas binárias, o número máximo de passos aumenta em cerca de uma unidade. Os tempos podem mudar por cache e outros efeitos: a teoria prevê ordem de crescimento, não segundos exatos.')

page('18 / Teoria','Como escrever T(n) com clareza')
p('Neste guia, <b>T(n) = C(n) + A(n)</b>: cada operação contada tem peso unitário. Esse é um modelo declarado, não uma medida completa do processador. Se quiser usar pesos distintos, escreva T(n) = aC(n) + bA(n) e explique a escolha.')
table(['Algoritmo','Melhor caso','Médio¹','Pior caso'],[
 ['Inversão','Θ(n)','Θ(n)','Θ(n)'],
 ['Sequencial','Θ(1)','Θ(n)','Θ(n)'],
 ['Binária iterativa','Θ(1)','Θ(log n)','Θ(log n)'],
 ['Binária recursiva','Θ(1)','Θ(log n)','Θ(log n)']],[160,110,110,115])
p('¹ Para as buscas, supondo sucesso igualmente provável entre n chaves distintas. A distribuição deve ser explicitada. A inversão independe dos valores. Θ descreve uma ordem justa de crescimento; O fornece um limite superior. As classificações em Θ da tabela também satisfazem o O correspondente.','small')
h('Desenvolvendo a recorrência')
p('No caminho máximo de ausência da binária recursiva, para n ≥ 1, o intervalo da direita tem piso(n/2) elementos. Sob nossa contagem: <b>T(n) = T(piso(n/2)) + 4</b>, com <b>T(0) = 1</b>. Após k níveis não vazios: T(n) = 4k + 1, onde k = piso(log₂ n) + 1.')
h('Um parágrafo que cada integrante deve escrever')
p('Explique a entrada de pior caso; conte quantas vezes cada operação ocorre; some os termos; identifique o dominante; conclua a ordem assintótica. Depois compare a fórmula com os CSVs. Para sucesso em n posições uniformes, explique a hipótese ao discutir o caso médio.')
p('Gráficos apoiam a análise, mas não provam a complexidade para todo n. A justificativa vem da lógica do algoritmo e de sua contagem ou recorrência.')

page('19 / GitHub','Primeiro acesso, uma vez por pessoa')
p('Git guarda o histórico local. GitHub hospeda a cópia compartilhada. A branch <b>main</b> será a linha de trabalho da equipe, conforme a decisão do grupo; não é necessário abrir uma branch por tarefa neste fluxo.')
h('1. Prepare o acesso')
p('Instale Git e use um editor de sua preferência. Entre no GitHub e aceite o convite de colaborador, se necessário. Um repositório público permite leitura, mas a permissão de escrita precisa ser concedida. Autentique pelo fluxo oferecido pelo Git Credential Manager ou GitHub Desktop.')
h('2. Configure a autoria no seu computador')
code('git config --global user.name "Seu Nome"\ngit config --global user.email "seu-email-ou-noreply"')
p('Substitua os valores. O e-mail pode ser o endereço noreply disponibilizado na sua conta do GitHub. Essa configuração identifica o autor; não realiza login.')
h('3. Clone o repositório existente')
code('git clone https://github.com/mathbmach/SCC0201-Trabalho-1.git\ncd SCC0201-Trabalho-1\ngit status\ngit branch --show-current',size=8.6)
p('O endereço é o remoto observado na cópia local. Se o grupo tiver renomeado o repositório mostrado na captura, copie o endereço atual em Code → HTTPS. Clone somente uma vez; quem já tem a pasta usa pull para atualizá-la. Não execute git init dentro do projeto clonado. [2]')
note('Alternativa visual','No GitHub Desktop, use File → Clone repository, escolha o projeto ou sua URL e a pasta local. Verifique Current Branch: main. Os conceitos de commit, pull e push são os mesmos. [3]')

page('20 / GitHub','A rotina diária na main')
h('1. Antes de editar: atualize')
code('git status\ngit switch main\ngit pull --ff-only origin main')
p('Comece com a árvore de trabalho limpa. Se houver alterações suas, finalize e salve-as em commit antes de integrar mudanças remotas. Se --ff-only recusar porque os históricos divergiram, siga a próxima página. Combine no grupo qual função e quais arquivos você vai editar. [4]')
h('2. Edite, teste e confira o que mudou')
code('git diff\ngit status\ngit add experiments/auxiliar_inversao.c\ngit diff --staged\ngit commit -m "Adiciona rodadas ao experimento de inversao"')
p('O caminho é um exemplo: adicione apenas os arquivos da sua alteração. add seleciona o conteúdo; commit salva um marco local. Prefira mensagens que expliquem o resultado. Uma alteração no código deve vir acompanhada da validação correspondente.')
h('3. Integre o que chegou e publique')
code('git pull --rebase origin main\ngit push origin main\ngit status')
p('Use rebase aqui para reaplicar seus commits ainda locais sobre as mudanças recebidas. Depois de integrar, compile e teste novamente antes do push. Se alguém publicar enquanto você trabalha, repita pull, resolva o que for necessário e então envie. [4,5]')
p('Confirme no GitHub que o commit e os arquivos apareceram. Avise à equipe o que está pronto. Commit sem push continua apenas na sua máquina.','small')

page('21 / GitHub','Conflitos têm solução')
p('Um conflito significa que o Git não conseguiu juntar automaticamente duas alterações. Ele é comum quando duas pessoas editam as mesmas linhas de algoritmos.c.')
h('Durante pull --rebase')
p('<b>1.</b> Use git status para identificar o arquivo.<br/><b>2.</b> Abra o arquivo e leia os dois trechos entre os marcadores.<br/><b>3.</b> Converse com quem alterou a função e produza uma versão correta.<br/><b>4.</b> Remova todos os marcadores; compile e teste.<br/><b>5.</b> Marque a resolução e continue. Repita se houver mais conflitos. [5]')
code('git add src/algoritmos.c\ngit rebase --continue\n# Se precisar cancelar o rebase em andamento:\ngit rebase --abort')
p('No rebase, os rótulos de “nosso” e “deles” podem confundir. Leia o conteúdo, não escolha um lado automaticamente. --abort volta ao estado anterior ao rebase; não publica nada. Não use push --force para contornar rejeições na main. [5]')
h('Enviei algo errado')
p('Para uma correção simples, edite e faça outro commit. Para desfazer um commit já publicado, use revert: ele cria um novo commit inverso e preserva o histórico. Confira o identificador com git log --oneline; substitua HASH abaixo pelo commit correto. [6]')
code('git revert HASH\n# Teste a versao resultante antes de enviar.\ngit push origin main')
note('Dados e arquivos compartilhados','Escolham uma pessoa para a coleta final. Evitem sobrescrever o mesmo CSV com medições de máquinas diferentes. Versionem fontes, scripts, dados e relatório; executáveis e arquivos temporários devem ficar fora. Se o push for negado por permissão, confiram conta e convite, sem recriar o repositório.')

page('22 / Relatório','Transforme resultados em argumentos')
table(['Parte','O que escrever'],[
 ['Introdução','Objetivo, algoritmos estudados e perguntas da comparação.'],
 ['Metodologia','Entradas, cenários, cinco tamanhos, relógio, R, M, aquecimento, ambiente, flags e convenção de contagem.'],
 ['Cada algoritmo','Como funciona; código relevante; T(n); melhor/médio/pior; tabela e discussão do experimento.'],
 ['Comparação','Crescimento linear versus logarítmico; iterativa versus recursiva; diferenças entre contagem e tempo.'],
 ['Conclusão','O que os dados sustentam, limitações e síntese da relação entre teoria e prática.'],
 ['Reprodução','Comandos, nomes dos arquivos e commit usado para gerar a coleta final.']],[100,395])
h('Como discutir sem exagerar')
p('Em vez de “a binária é sempre melhor”, escreva em quais tamanhos e condições ela teve menor tempo. Mencione que o vetor já estava ordenado e que o custo de ordenação ficou fora da medição.')
p('Se o tempo não crescer como esperado, confira resolução, retorno não utilizado, otimização, máquina, carga e erros de lógica. Não apague medições inconvenientes sem uma regra e justificativa registradas.')
note('Padrão de contribuição individual','Cada membro entrega um texto que explique seu algoritmo, uma derivação de custo, dados rastreáveis e pelo menos uma interpretação. Outra pessoa revisa. Quem organiza o PDF uniformiza estilo e referências, sem precisar reconstruir os experimentos dos colegas.')

page('23 / Fechamento','Checklist para a equipe')
h('Código e integração')
p('□ As quatro funções estão completas e retornam o contrato esperado.<br/>□ As buscas funcionam antes e depois do comando de inversão.<br/>□ Cabeçalho, nomes e parâmetros coincidem entre os arquivos.<br/>□ A saída segue exatamente o enunciado e o programa compila sem avisos relevantes.<br/>□ Alocações, leitura de entrada e limites foram revisados.')
h('Experimentos e análise')
p('□ Os cinco tamanhos e as repetições mínimas foram contemplados.<br/>□ Tempo usa função original; contagem usa instrumentação separada.<br/>□ As fórmulas conferem com casos pequenos e com os CSVs.<br/>□ Rodadas, média, variação e unidades estão identificadas.<br/>□ Metadados, código medido e dados brutos estão preservados.<br/>□ Os gráficos representam os números reais, sem exemplos didáticos misturados.')
h('Entrega')
p('□ Cada membro revisou sua seção e outra seção do relatório.<br/>□ A main está atualizada com a versão final testada.<br/>□ A pasta submission contém exatamente o conjunto solicitado.<br/>□ A versão preparada para envio foi compilada e testada novamente.<br/>□ Nome da disciplina, integrantes, números USP, prazo e formato foram conferidos.<br/>□ O responsável por cada envio confirmou o recebimento na plataforma.')
note('Próximos passos no estado atual','Concluir sequencial e binária iterativa; validar o conjunto; evoluir o auxiliar de inversão; replicar o método nos outros três; fazer a coleta final em uma máquina; consolidar gráficos e relatório. O guia não altera essas implementações automaticamente.')

# Appendix source is split at marked blocks for readable full-code pages.
BENCH_PARTS=[]
page('Apêndice / 1 de 4','Um experimento completo em C')
p('Exemplo proposto para Windows: mede a inversão original e grava dez rodadas por tamanho. Salve a sequência das quatro páginas em <b>experiments/exemplo_inversao.c</b>. Inclua as partes na ordem, sem repetir cabeçalhos de página. O código usa o inverte já existente.')
part=r'''#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "../../src/algoritmos.h"

typedef struct { long long c, a; } Contagem;

static void preencher(int *v, int n) {
    for (int i = 0; i < n; ++i) v[i] = i;
}

static Contagem contar(int *v, int n) {
    Contagem op = {0, 2};
    int i = 0, j = n - 1, temp;
    for (;;) {
        op.c++;
        if (!(i < n / 2)) break;
        temp = v[i]; v[i] = v[j]; v[j] = temp;
        i++; j--;
        op.a += 5;
    }
    return op;
}

static int validar(int *v, int n, int invertido) {
    for (int i = 0; i < n; ++i)
        if (v[i] != (invertido ? n - 1 - i : i))
            return 0;
    return 1;
}'''.replace('../../src/algoritmos.h','../src/algoritmos.h')
BENCH_PARTS.append(part);code(part,8.5)

page('Apêndice / 2 de 4','Relógio e medição do lote')
p('A frequência é consultada uma vez no main. A função lote mede apenas o laço de chamadas e verifica falhas do relógio. Para cada rodada, preparar repõe a entrada e faz 20 inversões de aquecimento, um número par.')
part=r'''static double lote(int *v, int n, int r,
                   LARGE_INTEGER freq) {
    LARGE_INTEGER inicio, fim;
    if (!QueryPerformanceCounter(&inicio)) exit(2);
    for (int i = 0; i < r; ++i) inverte(v, n);
    if (!QueryPerformanceCounter(&fim)) exit(2);
    return (double)(fim.QuadPart - inicio.QuadPart)
           / (double)freq.QuadPart;
}

static void preparar(int *v, int n) {
    preencher(v, n);
    for (int i = 0; i < 20; ++i) inverte(v, n);
}

int main(void) {
    const int ns[] = {1000, 50000, 100000, 500000,
                      1000000};
    const int M = 10;
    LARGE_INTEGER freq;
    if (!QueryPerformanceFrequency(&freq) ||
        freq.QuadPart <= 0) return 2;
    FILE *bruto = fopen("data/exemplo_rodadas.csv", "w");
    FILE *res = fopen("data/exemplo_resumo.csv", "w");
    if (!bruto || !res) {
        if (bruto) fclose(bruto);
        if (res) fclose(res);
        return 3;
    }
    fprintf(bruto, "algoritmo,n,rodada,repeticoes,"
                   "tempo_total_s\n");
    fprintf(res, "algoritmo,n,rodadas,repeticoes,media_s,"
                 "dp_s,comparacoes,atribuicoes\n");'''
BENCH_PARTS.append(part);code(part,8.5)

page('Apêndice / 3 de 4','Validar, calibrar e coletar')
p('O piloto dobra R até 0,05 s e não entra nos CSVs finais. A validação de paridade confirma se um número ímpar de inversões deixou o vetor invertido. A contagem é feita separadamente.')
part=r'''    for (int t = 0; t < 5; ++t) {
        int n = ns[t], R = 100;
        int *v = malloc((size_t)n * sizeof *v);
        if (!v) return 4;
        preencher(v, n);
        inverte(v, n);
        if (!validar(v, n, 1)) return 5;
        preencher(v, n);
        Contagem op = contar(v, n);
        long long m = n / 2;
        if (!validar(v, n, 1) || op.c != m + 1 ||
            op.a != 5 * m + 2) return 6;
        for (;;) {
            preparar(v, n);
            double b = lote(v, n, R, freq);
            if (!validar(v, n, R % 2)) return 7;
            if (b >= 0.05) break;
            if (R > INT_MAX / 2) return 8;
            R *= 2;
        }
        double tempos[10], media = 0.0;
        for (int r = 0; r < M; ++r) {
            preparar(v, n);
            double b = lote(v, n, R, freq);
            if (!validar(v, n, R % 2)) return 7;
            tempos[r] = b / R;
            media += tempos[r];
            fprintf(bruto, "inversao,%d,%d,%d,%.12e\n",
                    n, r + 1, R, b);
        }
        media /= M;'''
BENCH_PARTS.append(part);code(part,8.5)
p('Os retornos 2–10 representam falhas; não use uma coleta interrompida como série completa. Para uma versão final do auxiliar, acrescente mensagens de diagnóstico e centralize a liberação de recursos nos caminhos de erro.','small')

page('Apêndice / 4 de 4','Salvar e adaptar o exemplo')
part=r'''        double soma = 0.0;
        for (int r = 0; r < M; ++r) {
            double d = tempos[r] - media;
            soma += d * d;
        }
        double dp = sqrt(soma / (M - 1));
        fprintf(res, "inversao,%d,%d,%d,%.12e,%.12e,"
                     "%lld,%lld\n",
                n, M, R, media, dp, op.c, op.a);
        free(v);
    }
    int erro = ferror(bruto) || ferror(res);
    if (fclose(bruto) != 0) erro = 1;
    if (fclose(res) != 0) erro = 1;
    return erro ? 10 : 0;
}'''
BENCH_PARTS.append(part);code(part,8.5)
p('Na raiz do repositório, com GCC disponível e data/ existente, compile em uma única linha. Os dois CSVs exemplo serão substituídos a cada execução: copie ou renomeie coletas que quiser preservar.')
code('gcc -std=c11 -O2 -Wall -Wextra\n    experiments/exemplo_inversao.c src/algoritmos.c\n    -o exemplo_inversao.exe -lm\n\n.\\exemplo_inversao.exe',8.5)
p('Una as três primeiras linhas no terminal. Não inclua src/main.c: o auxiliar já possui seu próprio main. Não use -flto nesta proposta.')
h('Adaptação para as buscas')
p('Mantenha a sequência, os tamanhos, o relógio e as estatísticas. Troque inverte pela busca original, use p = n, valide retorno 0, substitua contar pela instrumentação correspondente e mude os nomes dos CSVs. A recursiva é chamada com (v, 0, n-1, p). Consuma os retornos, como explicado na página 16.')

page('Referências','Bases e decisões a confirmar')
p('<b>Material do grupo.</b> Conversa “Divisão detalhada das contribuições”; captura do repositório; README, src/main.c, src/algoritmos.c, src/algoritmos.h, auxiliar_inversao.c e CSV local, consultados em 17/09/2026. As fórmulas foram derivadas das implementações descritas neste guia.')
refs=[
 ('1','Microsoft Learn · Temporização de alta resolução','https://learn.microsoft.com/en-us/windows/win32/sysinfo/acquiring-high-resolution-time-stamps'),
 ('2','GitHub Docs · Receber alterações de um repositório','https://docs.github.com/en/get-started/using-git/getting-changes-from-a-remote-repository'),
 ('3','GitHub Docs · Clonar pelo GitHub Desktop','https://docs.github.com/en/desktop/adding-and-cloning-repositories/cloning-and-forking-repositories-from-github-desktop'),
 ('4','Git · pull','https://git-scm.com/docs/git-pull'),
 ('5','Git · rebase','https://git-scm.com/docs/git-rebase'),
 ('6','Git · revert','https://git-scm.com/docs/git-revert')]
for num,label,url in refs:p(f'[{num}] <link href="{url}" color="#0066cc">{label}</link>','body')
p('Documentação técnica consultada em 17/09/2026. Os títulos acima são links clicáveis. As recomendações de organização e os exemplos didáticos foram preparados para este projeto.','small')
h('Confirmem antes da entrega')
p('<b>Enunciado:</b> entrada, saída, limites de n, formato do envio e prazo.<br/><b>Identificação:</b> SCC0201 ou SCC0502; nomes e números USP.<br/><b>Método:</b> convenção de contagem e distribuição para o caso médio.<br/><b>Coleta:</b> máquina, compilador, commit, responsável e destino dos dados.')
note('O objetivo final','Qualquer integrante deve conseguir explicar o que foi contado, o que foi cronometrado, por que a entrada força o pior caso e como outra pessoa reproduz o resultado. Esse entendimento é a base de um trabalho consistente.')

c.save()
(ROOT/'tmp/pdfs/example_inversao.c').write_text('\n\n'.join(BENCH_PARTS).replace('../src/algoritmos.h','../../src/algoritmos.h'),encoding='utf-8')
(ROOT/'tmp/pdfs/page_index.json').write_text(json.dumps(pages,ensure_ascii=False,indent=2),encoding='utf-8')
print(f'Created {OUT} / {page_no} pages')
