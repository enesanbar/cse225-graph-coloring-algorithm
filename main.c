/*  CSE225 - Data Structures Project 3
    Enes ANBAR - 150113842              */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

// Function prototypes
void readCourses();
void sortCourses();
GraphPtr createGraph(int numberOfVertices, GraphType type);
void addEdges(Graph *graph);
void displayGraph(GraphPtr graph);
void destroyGraph(GraphPtr graph);
void colorWithBFS(Graph *graph);
void printFinalSchedule(Graph *graph);

char *strstrip(char *s);

// Course names. There can be max 32 courses with the length of 8
char courseNames[MAX_COURSES][MAX_LENGTH];
char colors[3][8] = {"black", "white", "gray"};
int numberOfCourses = 0;

int main(void) {
    /* Read the courses from the input file and sort them */
    readCourses();
    sortCourses();

    /* Create a course graph */
    GraphPtr courseGraph = createGraph(numberOfCourses, UNDIRECTED);

    /* Add edges to the graph*/
    addEdges(courseGraph);

    /* Display the adjacency list */
    displayGraph(courseGraph);

    /* Apply coloring algorithn to the graph */
    colorWithBFS(courseGraph);

    /* print the final schedule */
    printFinalSchedule(courseGraph);

    destroyGraph(courseGraph);

    return 0;
}

/* While reading from file, checks if the course has been read yet */
int doesCourseExist(char courseName[]){
    if(numberOfCourses == 0) return 0;

    int i;
    for(i = 0; i < numberOfCourses; i++){
        if(strcmp(courseNames[i], courseName) == 0){
            return 1;
        }
    }

    return 0;
}

/* Read the courses from the input file */
void readCourses(){
    char *readingMode = "r";

    // Open the input file for reading
    FILE *inputFile = fopen(FILENAME, readingMode);

    // If the file cannot be opened, give an error and exit the program
    if(inputFile == NULL){
        printf("Cannot open the input file: %s\n\n", FILENAME);
        exit(1);
    }

    // Read the file line by line and store the course names in the 2D courseNames array.
    char buffer[128];
    char *token;
    while (fgets (buffer, sizeof(buffer), inputFile)) {
        // Get the part of the line after the colon ':'
        char *courses = strstr(buffer, ":") + 1;
        courses = strstrip(courses);  // trim the whitespaces

        /* extract the course names off the line. get the first token */
        token = strtok(courses, ",");

        /* walk through other tokens */
        while( token != NULL ) {
            /* Remove the whitespaces */
            token = strstrip(token);

            // If it doesn't exist, add it to the global course list.
            if(!doesCourseExist(token)){
                strcpy(courseNames[numberOfCourses], token);
                numberOfCourses++;
            }

            /* get the next course name */
            token = strtok(NULL, ",");
        }
    }

    /* close the file */
    fclose(inputFile);
}

// Sort the course names. There're a few courses, bubble sort seems appropriate.
void sortCourses(){
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

    int i;
    for(i = 0; i < numberOfVertices; i++){
        graph->adjListArray[i].head = NULL;
        graph->adjListArray[i].num_members = 0;
        graph->adjListArray[i].visited = 0;
        graph->adjListArray[i].courseName = courseNames[i];
        graph->adjListArray[i].color = "unvisited";
    }

    return graph;
}

/* Function to create an adjacency list node*/
AdjListNodePtr createNode(char *vertex){
    AdjListNodePtr newNode = (AdjListNodePtr)malloc(sizeof(AdjListNode));
    if(!newNode)
        err_exit("Unable to allocate memory for new node");

    strcpy(newNode->vertex, vertex);
    newNode->next = NULL;

    return newNode;
}

/* check if an edge already exists */
int hasEdge(GraphPtr graph, char vertex1[], char vertex2[]){
    int i;
    for (i = 0; i < graph->numberOfVertices; i++) {
        AdjListNodePtr adjListPtr = graph->adjListArray[i].head;
        if(strcmp(courseNames[i], vertex1) == 0){
            while (adjListPtr) {
                if(strcmp(adjListPtr->vertex, vertex2) == 0) return 1;
                adjListPtr = adjListPtr->next;
            }
        }
    }

    return 0;
}

/* The courses are stored by id (array index)
   get the index of the course name */
int getIndex(char *courseName){
    int i;
    for(i = 0; i < numberOfCourses; i++){
        if(strcmp(courseNames[i], courseName) == 0) return i;
    }

    return -1;
}

/* add a single edge to the graph*/
void addEdge(Graph *graph, char *src, char *dest){
    /* Linked list sorted insertion algorithm */
    int srcIndex = getIndex(src);
    AdjListNodePtr newNode = createNode(dest);                          /* pointer to new node */
    AdjListNodePtr previousNode = NULL;                                 /* pointer to previous node in list */
    AdjListNodePtr currentNode = graph->adjListArray[srcIndex].head;    /* pointer to current node in list */

    /* loop to find the correct location in the list */
    while ( currentNode != NULL && strcmp(dest, currentNode->vertex) > 0 ) {
        previousNode = currentNode;
        currentNode = currentNode->next;
    }

    /* insert new node at beginning of list */
    if ( previousNode == NULL ) {
        newNode->next = graph->adjListArray[srcIndex].head;
        graph->adjListArray[srcIndex].head = newNode;
    }
    /* insert new node between previousNode and currentNode */
    else {
        previousNode->next = newNode;
        newNode->next = currentNode;
    }

    graph->adjListArray[srcIndex].num_members++;

    /* Add an edge from dest to src also*/
    if(graph->type == UNDIRECTED){

        /* Linked list sorted insertion algorithm */
        int destIndex = getIndex(dest);
        AdjListNodePtr newNode2 = createNode(src);                          /* pointer to new node */
        AdjListNodePtr previousNode2 = NULL;                                 /* pointer to previous node in list */
        AdjListNodePtr currentNode2 = graph->adjListArray[destIndex].head;    /* pointer to current node in list */

        /* loop to find the correct location in the list */
        while ( currentNode2 != NULL && strcmp(src, currentNode2->vertex) > 0 ) {
            previousNode2 = currentNode2;
            currentNode2 = currentNode2->next;
        }

        /* insert new node at beginning of list */
        if (previousNode2 == NULL) {
            newNode2->next = graph->adjListArray[destIndex].head;
            graph->adjListArray[destIndex].head = newNode2;
        }
        /* insert new node between previousNode2 and currentNode2 */
        else {
            previousNode2->next = newNode2;
            newNode2->next = currentNode2;
        }

        graph->adjListArray[destIndex].num_members++;
    }
}

/* Read each students exams and create adjaceny lists */
void addEdges(Graph *graph){
    char *readingMode = "r";

    // Open the input file for reading
    FILE *inputFile = fopen(FILENAME, readingMode);

    // If the file cannot be opened, give an error and exit the program
    if(inputFile == NULL){
        printf("Cannot open the input file: %s\n\n", FILENAME);
        exit(1);
    }

    // Read the file line by line and store the course names in the 2D courseNames array.
    char buffer[128];
    char *previousToken, *token;
    while (fgets (buffer, sizeof(buffer), inputFile)){
        char *courses = strstr(buffer, ":") + 1;
        courses = strstrip(courses);

        char tempArray[MAX_COURSES][MAX_LENGTH];
        int numberOfCoursesInLine = 0;

        /* extract the course names off the line. get the first token */
        token = strtok(courses, ",");

        /* create an array that contains the courses in a single line. */
        while( token != NULL ) {
            /* Remove the whitespaces */
            token = strstrip(token);
            strcpy(tempArray[numberOfCoursesInLine++], token);

            /* get the next course name */
            token = strtok(NULL, ",");
        }

        int i, j;
        for(i = 0; i < numberOfCoursesInLine - 1; i++){
            for(j = i + 1; j < numberOfCoursesInLine; j++){
                /* Add an edge if this edge has not been added before */
                if(!hasEdge(graph, tempArray[i], tempArray[j]))
                    addEdge(graph, tempArray[i], tempArray[j]);
            }
        }
    }
}

/* Function to print the adjacency list of graph*/
void displayGraph(GraphPtr graph){
    int i;
    printf("The Adjacency List\n");

    for (i = 0; i < graph->numberOfVertices; i++) {
        AdjListNodePtr adjListPtr = graph->adjListArray[i].head;
        printf("\n%-8s: ", courseNames[i]);
        while (adjListPtr) {
            printf("%-8s", adjListPtr->vertex);
            if(adjListPtr->next != NULL) printf("- ");
            adjListPtr = adjListPtr->next;
        }
    }

    printf("\n\n");
}

int isEmpty(QueueNodePtr headPtr){
    return headPtr == NULL;
}

/* equeue for the breadth first search algorithm */
void enqueu(QueueNodePtr *headPtr, QueueNodePtr *tailPtr, AdjList *adjList){
    QueueNodePtr newPtr;

    newPtr = malloc(sizeof(QueueNode));
    if(newPtr != NULL){
        newPtr->adjList = adjList;
        newPtr->nextNode = NULL;

        if(isEmpty(*headPtr)) *headPtr = newPtr;
        else (*tailPtr)->nextNode = newPtr;

        *tailPtr = newPtr;
    } else {
        err_exit("The adjaceny list not inserted. No memory available.\n");
    }
}

/* dequeue for the breadth first search algorithm */
AdjListPtr dequeu(QueueNodePtr *headPtr, QueueNodePtr *tailPtr){
    AdjList *adjList;
    QueueNodePtr tempPtr;

    adjList = (*headPtr)->adjList;
    tempPtr = *headPtr;
    *headPtr = (*headPtr)->nextNode;

    /* if queue is empty */
    if(*headPtr == NULL){
        *tailPtr == NULL;
    }

    free(tempPtr);
    return adjList;
}

/* Check if a color has been used in an adjaceny list */
int doesAdjListHaveThisColor(Graph *graph, AdjList *adjList, char *color){
    int i;
    int exist = 0;
    AdjListNode *temp = adjList->head;
    for(i = 0; i < adjList->num_members; i++){
        AdjList *vertex = &(graph->adjListArray[getIndex(temp->vertex)]);
        if(strcmp(vertex->color, color) == 0) exist = 1;
        temp = temp->next;
    }
    return exist;
}

/* Color the graph using breadth first search */
void colorWithBFS(Graph *graph){
    int colorIndex = 0;

    QueueNodePtr headPtr = NULL; /* initialize headPtr */
    QueueNodePtr tailPtr = NULL; /* initialize tailPtr */

    /* Give the first course the first color */
    AdjList *adjList = &(graph->adjListArray[0]);
    adjList->visited = 1;
    adjList->color = colors[colorIndex];

    enqueu(&headPtr, &tailPtr, adjList);

    int i;
    while(headPtr){
        AdjList *u = dequeu(&headPtr, &tailPtr);

        AdjListNode *v = u->head;

        /* Go through all of the adjacent courses and assign color to them */
        for(i = 0; i < u->num_members; i++){
            colorIndex = 0;
            int index = getIndex(v->vertex);
            AdjList *temp = &(graph->adjListArray[index]);

            /* If the node han not been visited yet, visit and do the following */
            if(temp->visited == 0){
                temp->visited = 1;

                /* Find an appropriate color for this node */
                while(doesAdjListHaveThisColor(graph, temp, colors[colorIndex])) colorIndex++;
                temp->color = colors[colorIndex];
                graph->adjListArray[index] = *temp;
                colorIndex = 0;

                /* Add this node to the queue */
                enqueu(&headPtr, &tailPtr, &(graph->adjListArray[index]));
            }
            v = v->next;    // Go to the next adjacent node
            colorIndex = 0; // reset the color index
        }
        colorIndex = 0; // reset the color index

    }
}

void printFinalSchedule(Graph *graph){
    int i;
    printf("Final Exam Period 1 => ");
    for(i = 0; i < graph->numberOfVertices; i++){
        if(strcmp(graph->adjListArray[i].color, "black") == 0){
            printf("%s ", graph->adjListArray[i].courseName);
        }
    }

    printf("\n\nFinal Exam Period 2 => ");
    for(i = 0; i < graph->numberOfVertices; i++){
        if(strcmp(graph->adjListArray[i].color, "white") == 0){
            printf("%s ", graph->adjListArray[i].courseName);
        }
    }

    printf("\n\nFinal Exam Period 3 => ");
    for(i = 0; i < graph->numberOfVertices; i++){
        if(strcmp(graph->adjListArray[i].color, "gray") == 0){
            printf("%s ", graph->adjListArray[i].courseName);
        }
    }

    printf("\n\n");
}

/*Destroys the graph*/
void destroyGraph(GraphPtr graph){
    if(graph){
        if(graph->adjListArray){
            int v;
            /*Free up the nodes*/
            for (v = 0; v < graph->numberOfVertices; v++) {
                AdjListNodePtr adjListPtr = graph->adjListArray[v].head;
                while (adjListPtr){
                    AdjListNodePtr tmp = adjListPtr;
                    adjListPtr = adjListPtr->next;
                    free(tmp);
                }
            }
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
