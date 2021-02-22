# graphCentralityMeasure
BSTree.c and BSTree.h are ADTs provided by UNSW.
Testing files provided by UNSW.

Implementation of Dijkstra's algorithm is in dijkstra.c, which calculates the shortest path between any two nodes in the graph ADT.

CentralityMeasures.c contains implementations of algorithms for calculating both Betweeness Centrality and Closeness centrality.

Closeness centrality: https://en.wikipedia.org/wiki/Centrality#Closeness_centrality
Betweeness centrality: https://en.wikipedia.org/wiki/Centrality#Betweenness_centrality

Further expansion on this program would be to use these centrality measures in order to build heirarchical clusters. 
To do this we can use the Lance William Algorithm equations as described:

https://en.wikipedia.org/wiki/Ward%27s_method#Lance%E2%80%93Williams_algorithms provides the

dijkstra.c, CentralityMeasures.c and LanceWilliamsHAC.c authored by Shree Nath.
