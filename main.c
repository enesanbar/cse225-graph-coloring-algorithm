/*  CSE225 - Data Structures Project 3
    Enes ANBAR - 150113842              */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COURSES 32
#define MAX_LENGTH 8

// Function prototypes
void readCourses(char courseNames[][MAX_LENGTH], int *numberOfCourses);
char *strstrip(char *s);
void toLowerCase(char *string);

int main(void) {

    // Counter
    int i = 0;

    // Course names. There can be max 32 courses with the length of 8
    char courseNames[MAX_COURSES][MAX_LENGTH];
    int numberOfCourses = 0;
    readCourses(courseNames, &numberOfCourses);
    //sortCourses(courseNames);



    for(i = 0; i < numberOfCourses; i++){
        printf("%s\n", courseNames[i]);
    }



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
        strstrip(courses);  // trim the whitespaces

        /* extract the course names off the line. get the first token */
        token = strtok(courses, ",");

        /* walk through other tokens */
        while( token != NULL ) {
            strstrip(token);

            // If it doesn't exist, add it to the course list.
            if(!doesCourseExist(courseNames, token, *numberOfCourses)){
                strcpy(courseNames[*numberOfCourses], token);
                (*numberOfCourses)++;
            }

            token = strtok(NULL, ",");
        }
    }

}

char *strstrip(char *str)
{
    size_t len = 0;
    char *frontp = str;
    char *endp = NULL;

    if( str == NULL ) { return NULL; }
    if( str[0] == '\0' ) { return str; }

    len = strlen(str);
    endp = str + len;

    /* Move the front and back pointers to address the first non-whitespace
     * characters from each end.
     */
    while( isspace(*frontp) ) { ++frontp; }
    if( endp != frontp )
    {
        while( isspace(*(--endp)) && endp != frontp ) {}
    }

    if( str + len - 1 != endp )
            *(endp + 1) = '\0';
    else if( frontp != str &&  endp == frontp )
            *str = '\0';

    /* Shift the string so that it starts at str so that if it's dynamically
     * allocated, we can still free it on the returned pointer.  Note the reuse
     * of endp to mean the front of the string buffer now.
     */
    endp = str;
    if( frontp != str )
    {
            while( *frontp ) { *endp++ = *frontp++; }
            *endp = '\0';
    }


    return str;
}

/* To perform case-insensitive comparison,
   make character lowercase */
void toLowerCase(char *string){
    int i = 0;
    while(string[i] != '\0'){
        string[i] = (char)tolower(string[i]);
        i++;

    }
}
