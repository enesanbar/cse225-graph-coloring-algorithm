#ifndef _GRAPH_H_
#define _GRAPH_H_

typedef enum {UNDIRECTED = 0, DIRECTED} GraphType;

/* Adjacency list node */
typedef struct adjlist_node {
    int vertex;                 // Index to adjacency lisk array
    struct adjlist_node *next;
} AdjListNode, *AdjListNodePtr;

/* Adjacency list */
typedef struct adjlist {
    int num_members;
    AdjListNode *head;
} AdjList, *AdjListPtr;

/*  Graph structure. A graph is an array of adjacency list.
    Size of the array will be the number of vertices in graph */
typedef struct graph {
    GraphType type;         // Directed or undirecteds
    int numberOfVertices;   // Number of vertices
    AdjListPtr adjListArr;  // Adjacency list array
} Graph, *GraphPtr;

/* Exit function to handle the fatal errors */
__inline void err_exit(char *msg){
    printf("[Fatal Error]: %s \nExiting", msg);
    exit(1);
}

#endif
