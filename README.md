# Árvores Balanceadas: AVL e Rubro-Negra

Trabalho da Unidade II de IMD0039 – Estruturas de Dados Básicas II.
Autora: Radla Oliveira
## Dependências

- Compilador C++17 (g++; no Windows, MinGW-w64). Só é necessário para recompilar.
- Graphviz (opcional): usado apenas para gerar imagens das árvores. Sem ele, o arquivo
  `.dot` é salvo em `saida/` e pode ser aberto em <https://dreampuf.github.io/GraphvizOnline>.

## Como executar

A versão compilada está em `bin/`:

```bash
.\bin\arvores.exe    # Windows
./bin/arvores        # Linux
```

Testes automatizados:

```bash
.\bin\testes.exe     # Windows
./bin/testes         # Linux
```

Para recompilar: `make`. Sem `make`:

```bash
g++ -std=c++17 -Iinclude src/main.cpp src/avl.cpp src/rn.cpp src/imagem.cpp src/dados.cpp -o bin/arvores.exe -static
g++ -std=c++17 -Iinclude tests/testes.cpp src/avl.cpp src/rn.cpp src/imagem.cpp src/dados.cpp -o bin/testes.exe -static
```

## Funcionalidades

Ao iniciar, o usuário escolhe a árvore AVL ou a Rubro-Negra. Para as duas:

- inserção, remoção e busca de elementos;
- percurso em ordem (elementos em ordem crescente);
- impressão da estrutura no terminal (na AVL com altura e fator de balanceamento; na
  Rubro-Negra com a cor de cada nó, `(R)` vermelho e `(B)` preto);
- exportação da árvore como imagem (Graphviz);
- estatísticas: altura, rotações, recolorações e verificação das propriedades;
- carregamento de elementos a partir de um arquivo (um número por linha).

## Exemplo de uso

```
Qual árvore deseja usar?
  1 - Árvore AVL
  2 - Árvore Rubro-Negra
Opção: 2
...
Opção: 1
Valor a inserir: 10
Valor 10 inserido.
...
Opção: 5
        30 (B)
                25 (R)
20 (B)
                10 (B)
        5 (R)
                        2 (R)
                1 (B)
```

## Funcionamento

- **AVL:** cada nó guarda sua altura. Depois de cada inserção ou remoção, a recursão volta
  pelo caminho atualizando as alturas e, se o fator de balanceamento de um nó chegar a +2
  ou -2, aplica uma rotação simples (sinais iguais) ou dupla (sinais diferentes).
- **Rubro-Negra:** cada nó tem uma cor (vermelho ou preto) e um ponteiro para o pai.
  Todo nó novo nasce vermelho. Se o pai também for vermelho, a correção depende do tio:
  tio vermelho → troca de cores (pai e tio ficam pretos, avô fica vermelho, e a verificação
  sobe para o avô); tio preto → rotação no avô (casos LL e RR) ou rotação dupla (LR e RL),
  com troca das cores. Na remoção de um nó preto, a correção depende do irmão do nó que
  ocupou o lugar do removido (4 casos, com rotações e trocas de cor).

O código em `src/` está comentado com mais detalhes.

## Decisões de implementação

- **Valor repetido:** não é inserido; o programa avisa o usuário.
- **Busca ou remoção de elemento inexistente:** a árvore não é alterada e o programa avisa.
- **Remoção de nó com dois filhos:** o nó recebe o valor do sucessor (menor da
  subárvore direita), que é então removido.
- **Tipo opaco,:** o programa principal não acessa os campos dos nós
  diretamente, só as funções de cada árvore.
- **Testes:** as duas árvores usam os mesmos arquivos de dados (`tests/dados`), e as
  propriedades são verificadas após cada inserção e remoção.