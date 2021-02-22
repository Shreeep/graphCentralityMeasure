// Centrality Measures ADT implementation
// COMP2521 Assignment 2
// Implemented by Shree Nath, z5209699 on 21/11/2020



#include <stdio.h>
#include <stdlib.h>

#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"
#include "Graph.h"

//prototypes
static void calcNumPaths(Vertex t, Vertex s,PredNode **pred, int *numPaths);
static int countPathVertices(Vertex t, Vertex s,PredNode **pred, int *numPathsWithV);
static void setArrToZero(int *arr, int numNodes);


NodeValues closenessCentrality(Graph g) {

	NodeValues nvs;
    nvs.numNodes = GraphNumVertices(g);
    nvs.values = malloc (nvs.numNodes * sizeof(double));

    //calculate closeness for each node
    for (Vertex i = 0; i < nvs.numNodes; i++) {
        ShortestPaths sps = dijkstra(g, i);
        int n_reachable = 0;
        int sum_dist = 0;
        //calculate num reachable nodes, and the sum of distances together
        for (Vertex j = 0; j < sps.numNodes; j++) {
            if (sps.dist[j] != 0) {
                sum_dist += sps.dist[j];
                n_reachable++;
            }
        }

        //check if node can reach itself
        n_reachable++;

        //typcaste to double, and simplify variable names
        double n = n_reachable;
        double N = nvs.numNodes;

        //isolated nodes have a closeness of 0
        if (sum_dist == 0) {
            nvs.values[i] = 0;
        //calculate closeness of non-isolated node
        } else {
            nvs.values[i] = ((n - 1)/(N - 1)) * ((n-1)/((double) sum_dist));
        }
        freeShortestPaths(sps);   
    }
	return nvs;
}

NodeValues betweennessCentrality(Graph g) {
	NodeValues nvs;
    nvs.numNodes = GraphNumVertices(g);
    double *betweennessArr = malloc(nvs.numNodes * sizeof(double));
    
    //nested for loops to calculate paths betwen s and t nodes
    for (Vertex s = 0; s < nvs.numNodes; s++) {
        //calculate shortest paths from s to every other node, t
        ShortestPaths sps = dijkstra(g, s);

        //intialise an array which stores the number of shortest paths between s, t
        int *numPaths;
        numPaths = malloc(sps.numNodes * sizeof(int));
        setArrToZero(numPaths, sps.numNodes);

        //array to count the nodes in a given path between s, t
        int *numPathsWithV;
        numPathsWithV = malloc(sps.numNodes * sizeof(int));
        setArrToZero(numPathsWithV, sps.numNodes);


        for (Vertex t = 0; t < nvs.numNodes; t++) {
            //don't calculate betweeness when s and t are disconnected
            if (!sps.dist[t]) {
                continue;   
            }
            //a separate pred array is generated because calcNumPaths destroys sps.pred
            ShortestPaths sps2 = dijkstra(g, s);
            setArrToZero(numPathsWithV, sps2.numNodes);

            //calculate paths between s & t
            calcNumPaths(t, s, sps.pred, numPaths);
            //count appearances of vertices in paths between s & t
            countPathVertices(t, s, sps2.pred, numPathsWithV);

            //insert betweeness into array and reset numPathsWithV array to 0;
            for (Vertex v = 0; v < nvs.numNodes; v++) {
                betweennessArr[v] += ((double) numPathsWithV[v] / (double) numPaths[t]);
            }
            freeShortestPaths(sps2);
        }
        freeShortestPaths(sps);
        free(numPaths);
        free(numPathsWithV);
    }
    nvs.values = betweennessArr;
	return nvs;
}

NodeValues betweennessCentralityNormalised(Graph g) {
	NodeValues nvs = betweennessCentrality(g);
    int n = GraphNumVertices(g);
    for (int i = 0; i < n; i++) {
        nvs.values[i] = ((nvs.values[i]) / (((double) n - 1 )*((double) n - 2)));
    }
	return nvs;
}

void showNodeValues(NodeValues nvs) {
    for (int i = 0; i < nvs.numNodes; i++) {
        printf("%d: %f\n", i, nvs.values[i]);
    }
}

void freeNodeValues(NodeValues nvs) {
    free(nvs.values);
}


/*
==========================================================================================
//HELPER FUNCTIONS//
==========================================================================================
*/

//calculates num of paths between source s and dest t
static void calcNumPaths(Vertex t, Vertex s,PredNode **pred, int *numPaths) {

    //return from search if we reach the src vertex
    if (pred[t] == NULL) {
        return;
    }

    //initalise a vertex which has pred[s] as 1
    if (pred[t]->v == s) {
        numPaths[t] += 1;
    }

    //search through the predecessors and recursively count paths
    while (pred[t] != NULL) {
        calcNumPaths(pred[t]->v, s, pred, numPaths);
        numPaths[t] += numPaths[pred[t]->v];
        pred[t] = pred[t]->next;
    }
    return;
}

//count how many times each intermediary node appears
static int countPathVertices(Vertex t, Vertex s, PredNode **pred, int *numPathsWithV) {

    //get pred of t
    PredNode *curr = pred[t];

    //reset the running total for each pred vertex (built up recursively for each vertex)
    int runningPathsTotal = 0;

    //return from search if we reach the src vertex
    if (curr == NULL) {
        return 0;
    }

    //add a path to the running total if we reach a node adj to the src
    if (curr->v == s) {
        runningPathsTotal += 1;
    }

    //recusively count how many shortest paths travel through each pred vertex
    while (curr != NULL) {
        int numPathsToCurr = countPathVertices(curr->v, s, pred, numPathsWithV);

        //store the paths travelling through curr->v and add to the running total
        numPathsWithV[curr->v] += numPathsToCurr;
        runningPathsTotal += numPathsToCurr;
        curr = curr->next;
    }
    return runningPathsTotal;
}

//reset an array to zero
static void setArrToZero(int *arr, int numNodes) {
    for (int i = 0; i < numNodes; i++) {
        arr[i] = 0;
    }
}