# BuscaEmGrafo-AED2
Este projeto é trabalho da matéria de AED2 (Algoritmos e Estrutura de Dados 2) de Sistemas de Informação da Universidade de São Paulo. 

### Problema
Dado dois vértices, como por exemplo V1 e V10, em um grafo não-direcionado devemos implementar um algoritmo que encontre o caminho mais curto partindo de V1 a V10. Na modelagem do grafo vértices são inicializados como "fechados" ou "abertos", só é possível acessar o vértice caso o mesmo esteja "aberto", para acessar vértices "fechados" é necessario obter a "chave" que pode ou não estar em algum dos vértices do grafo, caso encontre a chave todos os vértices inicialmente "fechados" devem ficar "abertos" para acesso. Para solução do problema deve implementar uma função "caminho" que recebe o grafo e informações do mesmo (número de arestas, vertices, vértice de partida, vértice destino etc) e deve devolver uma lista ligada do caminho mais curto partindos do vértice de partida até o vértice destino.
### Solução
Para solução foi utilizado o algoritmo de Dijkstra, ideal para procura do caminho mais curto entre dois vértices em um grafo não-dirigido ou dirigido. A implementação foi feita em C e após conclusão transferida para um arquivo em C++.
