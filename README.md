# CSE225 Data Structures Assignment

In the assignment, Graph Coloring Algorithm is used to generate final exam schedule,
such that each course of a student should be on different time slots in order to make a more fair exam schedule for each student.
Find the instructions in the pdf document.

# Building
```sh
$ make
```

# Running
```sh
$ ./main

The Adjacency List

BLAW203 : MATH101 
CSE100  : MATH101 - MATH259 
HIST111 : MATH259 - STAT253 
MATH101 : BLAW203 - CSE100  - MATH259 
MATH259 : CSE100  - HIST111 - MATH101 - STAT253 
STAT253 : HIST111 - MATH259 

Final Exam Period 1 => BLAW203 CSE100 HIST111 

Final Exam Period 2 => MATH101 STAT253 

Final Exam Period 3 => MATH259

```

