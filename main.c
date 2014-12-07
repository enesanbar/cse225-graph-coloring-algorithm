/*  CSE225 - Data Structures Project 3
    Enes ANBAR - 150113842              */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

#define MAX_COURSES 32
#define MAX_LENGTH 8

// Function prototypes
void readCourses(char courseNames[][MAX_LENGTH], int *numberOfCourses);
void sortCourses(char courseNames[][MAX_LENGTH], int numberOfCourses);
GraphPtr createGraph(int numberOfVertices, GraphType type);
void addEdge(Graph *graph, int src, int dest);
void displayGraph(GraphPtr graph);
void destroyGraph(GraphPtr graph);

char *strstrip(char *s);
void toLowerCase(char *string);

int main(void) {

    // Counter
    int i = 0;

    // Course names. There can be max 32 courses with the length of 8
    char courseNames[MAX_COURSES][MAX_LENGTH];
    int numberOfCourses = 0;

    /* Read the courses from the input file and sort them */
    readCourses(courseNames, &numberOfCourses);
    sortCourses(courseNames, numberOfCourses);

    /* Create a course graph */
    GraphPtr courseGraph = createGraph(numberOfCourses, UNDIRECTED);

    printf("\nUNDIRECTED GRAPH");
    displayGraph(courseGraph);
    destroyGraph(courseGraph);

    return 0;
}

/* While reading from file, checks if the course has been read yet
   Note: The compiler need to know the 2nd dimension of the two dimensional array */
int doesCourseExist(char courseNames[][MAX_LENGTH], char courseName[], int numberOfCourses){
    if(numberOfCourses == 0) return 0;

    int i;
    for(i = 0; i < numberOfCourses; i++){
        if(strcmp(courseNames[i], courseName) == 0){
            return 1;
        }
    }

    return 0;
}

void readCourses(char courseNames[][MAX_LENGTH], int *numberOfCourses){
    // Filename and reading mode
    char *filename = "input.txt";
    char *readingMode = "r";

    // Open the input file for reading
    FILE *inputFile = fopen(filename, readingMode);

    // If the file cannot be opened, give an error and exit the program
    if(inputFile == NULL){
        printf("Cannot open the input file: %s\n\n", filename);
        exit(1);
    }

    // Read the file line by line and store the course names in the 2D courseNames array.
    char buffer[128];
    char *token;
    while (fgets (buffer, sizeof(buffer), inputFile)) {
        // Get the part of the line after the color ':'
        char *courses = strstr(buffer, ":") + 1;
        courses = strstrip(courses);  // trim the whitespaces

        /* extract the course names off the line. get the first token */
        token = strtok(courses, ",");

        /* walk through other tokens */
        while( token != NULL ) {
            token = strstrip(token);

            // If it doesn't exist, add it to the course list.
            if(!doesCourseExist(courseNames, token, *numberOfCourses)){
                strcpy(courseNames[*numberOfCourses], token);
                (*numberOfCourses)++;
            }

            token = strtok(NULL, ",");
        }
    }
}

// Sort the course names. There're a few courses, bubble sort seems right.
void sortCourses(char courseNames[][MAX_LENGTH], int numberOfCourses){
    int i, k;
    int needNextPass = 1;

    for(k = 1; k < numberOfCourses && needNextPass; k++){
        needNextPass = 0;

        for(i = 0; i < numberOfCourses - k; i++){
            if(strcmp(courseNames[i], courseNames[i + 1]) > 0){
                // Swap courseNames[i] and courseNames[i + 1]
                char temp[MAX_LENGTH];
                strcpy(temp, courseNames[i]);
                strcpy(courseNames[i], courseNames[i + 1]);
                strcpy(courseNames[i + 1], temp);

                needNextPass = 1;
            }
        }
    }
}

/* Function to create a graph with n vertices; Creates both directed and undirected graphs */
GraphPtr createGraph(int numberOfVertices, GraphType type){
    GraphPtr graph = (GraphPtr)malloc(sizeof(Graph));
    if(graph == NULL)
        err_exit("Unable to allocate memory for graph");

    graph->numberOfVertices = numberOfVertices;
    graph->type = type;

    /* Create an array of adjacency lists*/
    graph->adjListArr = (AdjListPtr)malloc(numberOfVertices * sizeof(AdjList));
    if(!graph->adjListArr)
        err_exit("Unable to allocate memory for adjacency list array");

    int i;
    for(i = 0; i < numberOfVertices; i++){
        graph->adjListArr[i].head = NULL;
        graph->adjListArr[i].num_members = 0;
    }

    return graph;
}

/* Function to create an adjacency list node*/
AdjListNodePtr createNode(int v){
    AdjListNodePtr newNode = (AdjListNodePtr)malloc(sizeof(AdjListNode));
    if(!newNode)
        err_exit("Unable to allocate memory for new node");

    newNode->vertex = v;
    newNode->next = NULL;

    return newNode;
}

/* Adds an edge to a graph*/
void addEdge(Graph *graph, int src, int dest){
    /* Add an edge from src to dst in the adjacency list*/
    AdjListNodePtr newNode = createNode(dest);
    newNode->next = graph->adjListArr[src].head;
    graph->adjListArr[src].head = newNode;
    graph->adjListArr[src].num_members++;

    if(graph->type == UNDIRECTED){
        /* Add an edge from dest to src also*/
        newNode = createNode(src);
        newNode->next = graph->adjListArr[dest].head;
        graph->adjListArr[dest].head = newNode;
        graph->adjListArr[dest].num_members++;
    }
}

/* Function to print the adjacency list of graph*/
void displayGraph(GraphPtr graph){
    int i;
    for (i = 0; i < graph->numberOfVertices; i++) {
        AdjListNodePtr adjListPtr = graph->adjListArr[i].head;
        printf("\n%d: ", i);
        while (adjListPtr) {
            printf("%d->", adjListPtr->vertex);
            adjListPtr = adjListPtr->next;
        }
        printf("NULL\n");
    }
}

/*Destroys the graph*/
void destroyGraph(GraphPtr graph){
    if(graph){
        if(graph->adjListArr){
            int v;
            /*Free up the nodes*/
            for (v = 0; v < graph->numberOfVertices; v++) {
                AdjListNodePtr adjListPtr = graph->adjListArr[v].head;
                while (adjListPtr){
                    AdjListNodePtr tmp = adjListPtr;
                    adjListPtr = adjListPtr->next;
                    free(tmp);
                }
            }
            /*Free the adjacency list array*/
            free(graph->adjListArr);
        }
        /*Free the graph*/
        free(graph);
    }
}

// Trim the space characters from the string.
char *strstrip(char *str){
    size_t len = 0;
    char *frontp = str;
    char *endp = NULL;

    if( str == NULL ) { return NULL; }
    if( str[0] == '\0' ) { return str; }

    len = strlen(str);
    endp = str + len;

    /* Move the front and back pointers to address the first non-whitespace
     * characters from each end. */
    while( isspace(*frontp) ) { ++frontp; }
    if( endp != frontp ){
        while( isspace(*(--endp)) && endp != frontp ) {}
    }

    if( str + len - 1 != endp ) *(endp + 1) = '\0';
    else if( frontp != str &&  endp == frontp ) *str = '\0';

    /* Shift the string so that it starts at str so that if it's dynamically
     * allocated, we can still free it on the returned pointer */
    endp = str;
    if( frontp != str )    {
        while( *frontp ) { *endp++ = *frontp++; }
        *endp = '\0';
    }

    return str;
}

/* To perform case-insensitive comparison, make characters lowercase */
void toLowerCase(char *string){
    int i = 0;
    while(string[i] != '\0'){
        string[i] = (char)tolower(string[i]);
        i++;
    }
}
