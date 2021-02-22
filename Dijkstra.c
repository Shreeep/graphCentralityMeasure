// Dijkstra ADT implementation
// COMP2521 Assignment 2
// Implemented by Shree Nath, z5209699 on 15/11/2020
// showShortestPaths function uses code from the provided testDijkstra.c file


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#include "Dijkstra.h"
#include "PQ.h"

//prototypes
static PredNode *insertPredNode(PredNode *predList, Vertex v);
static PredNode *deletePredList(PredNode *predList);
static PredNode *newPredNode(Vertex v);

ShortestPaths dijkstra(Graph g, Vertex src) {
    //the paths object to be returned
    ShortestPaths sps;

    int nV = GraphNumVertices(g);

    sps.numNodes = nV;
    sps.src = src;
    sps.dist = malloc(nV * sizeof(int));
    sps.pred = malloc(nV * sizeof(PredNode));

    PQ vSet = PQNew();

    //initialise dist and pred arrays
    //note: using INT_MAX to represent INFINITY
    for (Vertex i = 0; i < nV; i++) {
        sps.dist[i] = INT_MAX;
        sps.pred[i] = NULL;
    }

    //insert source into the priority queue and intialise as 0
    PQInsert(vSet, src, 0);
    //initialise the distance to src array as 0
    sps.dist[src] = 0;

    while (!PQIsEmpty(vSet)) {

        //get v with min dist[v]
        Vertex v = PQDequeue(vSet);
        //djikstras algorithm loop
        AdjList AdjOutVertices = GraphOutIncident(g, v);
        while (AdjOutVertices != NULL) {
            Vertex w = AdjOutVertices->v;
            int edgeWeight = AdjOutVertices->weight;
            //relax along (v,w,weight)
            if (sps.dist[w] > sps.dist[v] + edgeWeight) {
                
                //if pred list is empty, insert pred node
                if (sps.pred[w] == NULL) {
                    sps.pred[w] = insertPredNode(sps.pred[w], v);
                //if pred list is not empty, delete and replace list
                } else {
                    sps.pred[w] = deletePredList(sps.pred[w]);
                    sps.pred[w] = insertPredNode(sps.pred[w], v); 
                }
                //update the dist array
                sps.dist[w] = sps.dist[v] + edgeWeight;
                //insert w into PQ
                PQInsert(vSet, w, sps.dist[w]);
            } else if (sps.dist[w] == sps.dist[v] + edgeWeight) {
                //add another predecessor to the predList
                sps.pred[w] = insertPredNode(sps.pred[w], v); 
            }
            AdjOutVertices = AdjOutVertices->next;
        }
    }

    //set all distances still set to INT_MAX as 0, because
    //these nodes are not connected to the src node
    for (int i = 0; i < nV; i++) {
        if (sps.dist[i] == INT_MAX) {
            sps.dist[i] = 0;
        }
    }
    //showShortestPaths(sps);
    
    return sps;
}

//code taken from testDijkstra.c as provided in Ass2_testing
void showShortestPaths(ShortestPaths sps) {
    printf("numNodes: %d\n", sps.numNodes);
    printf("source node: %d\n", sps.src);
    printf("dist array: \n");

    for (int i = 0; i < sps.numNodes; i++) {
        printf("\t%d\n", sps.dist[i]);
    }
    printf("pred array: \n");
    for (int i = 0; i < sps.numNodes; i++) {
		int numPreds = 0;
		int preds[sps.numNodes];
		printf("    %d : ",i);
		PredNode *curr = sps.pred[i];
		while (curr != NULL && numPreds < sps.numNodes) {
			preds[numPreds++] = curr->v;
			curr = curr->next;
		}
		
		// Insertion sort
		for (int j = 1; j < numPreds; j++) {
			int temp = preds[j];
			int k = j;
			while (k > 0 && preds[k - 1] > temp) {
				preds[k] = preds[k - 1];
				k--;
			}
			preds[k] = temp;
		}
		
		for (int j = 0; j < numPreds; j++) {
			printf("[%d]->", preds[j]);
		}
		printf("NULL\n");
	}
}

void freeShortestPaths(ShortestPaths sps) {
    
    //free all predLists, except for head which is NULL
    for (int i = 0; i < sps.numNodes; i++) {
        deletePredList(sps.pred[i]);
    }
    //free pred array of lists
    free(sps.pred);
    //free dist array
    free(sps.dist);
}

/*
==========================================================================================
//HELPER FUNCTIONS//
==========================================================================================
*/

static PredNode *insertPredNode(PredNode *predList, Vertex v) {
    //insert the value v in the linked list at index w
    PredNode *node = newPredNode(v);

    //if list is empty, insert predList
    if (predList == NULL) {
        predList = node;
    //else search for the end of the list and insert
    } else {
        PredNode *curr = predList;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = node;
    }
    return predList;
    
}

static PredNode *deletePredList(PredNode *predList) {
    //predList is already empty, do nothinf
    if (predList == NULL) {
        return NULL;
    }
    //iterate through the predList and free the nodes
    while (predList->next != NULL) {
        PredNode *temp = predList;
        free(temp);
        predList = predList->next;
    }
    return NULL;
}

static PredNode *newPredNode(Vertex v) {
    PredNode *newPred = malloc(sizeof(*newPred));
    if (newPred == NULL) {
        fprintf(stderr, "Couldn't allocate new pred node");
        exit(EXIT_FAILURE);
    }

    newPred->v = v;
    newPred->next = NULL;
    return newPred;
}

