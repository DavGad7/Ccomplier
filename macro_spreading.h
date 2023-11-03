#ifndef MACRO_SPREADING_H
#define MACRO_SPREADING_H
#include "general.h"
#define MAX_MACRO_NAME_LENGTH 30
#define MAX_LINES_MACRO 15 /* each macro can contain maximum of 15 lines */
#define START_OF_MACRO "macro" /* the start of macro definition */
#define END_OF_MACRO "endmacro" /* the end definition of macro */
#define AM 'm'
#define AS 's'
/* linked list of macros */
typedef struct macro {
    char mName[MAX_MACRO_NAME_LENGTH]; /* name of the macro */
    char mContent[MAX_LINES_MACRO * MAX_LINE_LENGTH]; /* content of the macro */
    struct macro *next; /* next node in the linked list */
} macro;

/* Function description:
 * The function frees the allocated memory at "macro_spreading.c".
 *
 * Parameters description:
 * -as: pointer to the ".as" file, data type: (FILE pointer).
 * -am: pointer to the ".am" file, data type: (FILE pointer).
 * -head: the head of the linked list of macros, data type: (macro pointer).
 *
 * Return: no return (void function).
 */
void freeMemory(FILE *as, FILE *am, macro *head);

/* Function description:
 * the function executes the macro spreading process.
 *
 * Parameters description:
 * -fp: pointer to the ".as" file, data type: (FILE pointer).
 * -fName: name of the file (the ".as" file), data type: (file_name pointer).
 *
 * Return:
 * -TRUE if there was an error while macro spreading process.
 * -FALSE if there was not an error while macro spreading process,
 */
boolean macroSpreading(FILE *fp, file_name fName);

/* Function description:
 * the functions read a line from file.
 *
 * Parameters description:
 * -iLine: the index of the parameter "line" at the ".as" file, data type: (integer).
 * -line: single line from the ".as" file, data type: (char pointer).
 * -fp: pointer to the ".as" file, data type: (FILE pointer).
 * -fName: the name of the ".as" file, data type: (file_name pointer).
 *
 *
 * Return: no return (void function).
 */
char *readFile(int iLine,char *line, FILE *fp,  file_name fName);

/* Function description:
 * the function deals with a line from the ".as" file and rewrite the ".am" file .
 *
 * Parameters description:
 * -iLine: the index of the parameter "line" at the ".as" file, data type: (integer).
 * -line: single line from the ".as" file, data type: (char pointer).
 * -am: pointer to the ".am" file, data type: (FILE pointer).
 * -fName: the name of the ".as" file, data type: (file_name pointer).
 * -head: the head of the linked list of macros, data type: (macro pointer).
 *
 * Return: no return (void function).
 */
void addMacLineToFile( int iLine, char *line, FILE *am, file_name fName, macro *head);

/* Function description:
 * the function adds a new macro to the linked list of macros and checks if it is a valid macro.
 *
 * Parameters description:
 * -iLine: the index of the parameter "line" at the ".as" file, data type: (integer).
 * -line: single line from the ".as" file, data type: (char pointer).
 * -fName: the name of the ".as" file, data type: (file_name pointer).
 * -head: the head of the linked list of macros, data type: (macro pointer).
 * 
 * Return: no return (void function).
 */
void addMacro(int iLine,char *line, file_name fName,macro *head);

/* Function description:
 * this function return a pointer to a macro from the linked list of macros with the same name as the parameter macro_name.
 * if there is not a macro with the same name as the parameter macro_name, the function returns NULL.
 *
 * Parameters description:
 * -mac_name: name of macro that we going to search him in the linked list, data type: (char pointer).
 * -head: the head of the linked list of macros, data type: (macro pointer).
 *
 * Return:
 * pointer to the macro if it existed in the linked list of macros, NULL otherwise.
 */
macro *searchMacro(char *mac_name,macro *head );

/* Function description:
 * The function returns last defined macro from the linked list of macros.
 *
 * Parameters description:
 * -head: the head of the linked list of macros, data type: (macro pointer).
 *
 * Return:
 * -last defined macro at the linked list of macros.
 */
macro *lastMacro(macro *head);

/* Function description:
 * The function checks if the line is close to the  macro definition.
 *
 * Parameters description:
 * -line: single line from the ".as" file, data type: (char pointer).
 *
 * Return:
 * -TRUE if line is close to macro def.
 * -FALSE if line not is close to macro def
 */
boolean macroEnd(char *line);

/* Function description:
 * The function returns if there was an error in the macro spreading process.
 *
 * Parameters description:
 * -is_error: represents if there was an error or not, data type: (boolean).
 * -reset: represents if there was any change to the error or not, data type: (boolean).
 *
 * Return:
 * -TRUE if there was an error  while the macro spreding process
 * -FALSE if there was not an error  while the macro spreding process
 */
boolean error_in_macro(boolean is_error, boolean reset);

#endif


