#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include "macro_spreading.h"
#include "first_transition.h"
#include "second_transition.h"
#define AM 'm'
#define AS 's'
/* reset the error flag (at the: macro spreading, first transition and second transition) in the program to FALSE */
#define RESET_ERROR_FUNCTIONS {                 \
    error_in_macro(FALSE,TRUE);       \
    error_in_first_transition(FALSE,TRUE);  \
    error_in_second_transition(FALSE,TRUE); \
}

/* Function description:
 * The function frees all the allocated memory .
 *
 * Parameters description:
 * -InstC: the InstC, data type: (integer pointer).
 * -DataC: the DataC, data type: (integer pointer).
 * -afterMacSpreading: pointer to ".am" file (read mode), data type: (FILE pointer).
 * -fName: pointer to ".as" file, data type: (file_name pointer).
 * -head: head of the linked list of tags, data type: (tag pointer).
 * 
 * Return: no return (void function).
 */
void free_memory(int *InstC, int *DataC , FILE *afterMacSpreading , file_name *fName, tag *head);

/* Function description:
 * The function deals with parameters that the program got from the command line.
 *
 * Parameters description:
 * -argV: Value of the parameters from command line, data type: (array char pointer).
 * -argc: Amount of the parameters from command line, data type: (integer).
 *
 * Return: no return (void function).
 */
void extractFilesFromLine(char *argV[] , int argC);

/* Function description:
 * The function handle (compile) a signal file from the command line.
 *
 * Parameters description:
 * -fp: pointer to ".as" file, data type: (FILE pointer).
 * -fName: the name of the file, data type: (file_name pointer).
 *
 * Return: no return (void function).
 */
void compile(FILE *as, file_name *fName);

#endif

