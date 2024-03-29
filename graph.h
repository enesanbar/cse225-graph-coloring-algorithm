#ifndef _GRAPH_H_
#define _GRAPH_H_
#define MAX_COURSES 32
#define MAX_LENGTH 8
#define FILENAME "input.txt"

typedef enum {UNDIRECTED = 0, DIRECTED} GraphType;

/* Adjacency list node */
typedef struct adjlist_node {
    char vertex[MAX_LENGTH];                 // Index to adjacency lisk array
    struct adjlist_node *next;
} AdjListNode, *AdjListNodePtr;

/* Adjacency list */
typedef struct adjlist {
    char *courseName;
    char *color;
    int visited;
    int num_members;
    AdjListNode *head;
} AdjList, *AdjListPtr;

/*  Graph structure. A graph is an array of adjacency list.
    Size of the array will be the number of vertices in graph */
typedef struct graph {
    GraphType type;         // Directed or undirecteds
    int numberOfVertices;   // Number of vertices
    AdjList adjListArray[MAX_COURSES];  // Adjacency list array
} Graph, *GraphPtr;

struct queueNode {
    AdjList *adjList;
    struct queueNode *nextNode;
};

typedef struct queueNode QueueNode;
typedef QueueNode *QueueNodePtr;

/* Exit function to handle the fatal errors */
__inline void err_exit(char *msg){
    printf("[Fatal Error]: %s \nExiting", msg);
    exit(1);
}

#endif
