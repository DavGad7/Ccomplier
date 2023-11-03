
#ifndef GENERAL_H
#define GENERAL_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* boolean */
typedef int boolean;
#define TRUE 1
#define FALSE 0

/* Maximum line length: 80, and if you include the '\n' +1 = 81 */
#define MAX_LINE_LENGTH 81

/*Maximum length of tag or a macro name */
#define MAX_MACRO_NAME_LENGTH 30

/* Buffer for data and the instruction arrays */
#define BUFFER 2000

/* Load location of the program at the memory */
#define LOAD_LOCATION 100

/*length of a word in the memory */
#define WORD_LENGTH 10

/* Minimum value that number can be in the code, calculate in this way: there are 10 bits and the most left bit is only for positive/negetive represention.
 * then we will take the most left bit as 1, and the rest will be 0's, we got the number 1000000000 in binary, and this equal to -512 in decimal */
#define MIN (-512)

/* the maximum value that number can be in the code, calculate in this way: there are 10 bits and the most left bit is only for positive/negetive represention.
 * then we will take the most left bit as 0, and the rest will be 1's, we got the number 0111111111 in binary, and this equal to 511 in decimal */
#define MAX 511

/* constant that store the case of return to string that is not register name in the function "register_index" in "general.c". */
#define NOT_REGISTER 8

/* checks if there was an memory error or not if there was prints an error message  */
#define MEMORY_ERROR(ptr) {                 \
    if ((ptr) == NULL) {                             \
        printf("Memory error! Exit the program.\n"); \
        exit(EXIT_FAILURE);                          \
    }                                                \
}
typedef struct {
    char *name; /* the file name */
    int len; /* the length of the file name */
} file_name;

/* linked list of tags */
typedef struct tag {
    int val; /* value of the tag - at memory */
    char name[MAX_MACRO_NAME_LENGTH]; /* name of the tag */
    boolean opCode; /* TRUE if the tag is including opcode (in other words, instructional sentence), FALSE otherwise */
    boolean data; /* TRUE if the sentence is directive sentence, FALSE otherwise */
    boolean is_extern; /* TRUE if the tag is with extern guidance, FALSE otherwise */
    boolean entry; /* TRUE if the tag is with entry guidance, FALSE otherwise */
    struct tag *next; /* the next node in the linked list */
} tag;

/* represents a word at the memory - 10 bits */
typedef struct {
    unsigned int val: WORD_LENGTH; /* the value of the word - binary */
} word;

/* represents a word at the memory - 10 bits */
typedef struct {
    int iLine; /* the index of the line that the current word created from */
    unsigned int val: WORD_LENGTH; /* the value of the word - binary */
    char tName[MAX_MACRO_NAME_LENGTH]; /* if the current word require an address of tag, this field save the name of the tag. else, the field will be "". */
    boolean is_register; /* if the current word is register addressing */
} instruction;

/* Function description: 
 * The function checks if the macro or tag name is valid.
 *
 * Parameters description:
 * -name: name of the macro or tag, data type: (char).
 *
 * Return:
 * True if the macro or tag name is valid
 * False if the macro or tag name is unValid
 */
boolean macro_name_valid(char *name);

/* Function description: 
 * the function removes the blank characters from the start of the string until first non-blank character.
 *
 * Parameters description::
 * -line: the string that you want to clean from the blank characters, data type: (char).
 *
 * Return:
 * returns a copy of "line" but without the blank characters in the start
 */
char *remove_spaces(char *line);

/* Function description: 
 * The function extract the content without the blank characters from "line" and fills the parameter content with the non-blank characters
 *
 * Parameters description:
 * -line: The string that we want to "clear" from blank characters, data type: (char).
 * -content: The string that we want to put the "pure-line"(without the blank charaacters) in, data type: (char).
 *
 * Return:
 * returns a copy of "line" without the blank characters. 
 * 
 */
char *line_content(char *line,  char *content);

/* Function description: 
 * The function checks if a string is register.
 *
 * Parameters description:
 * -str: String that checked if it is register name, data type: (char).
 *
 * Return:
 * returns the number of the index of the register.
 * if it is not a register name, returns -1. 
 */
int register_num(char *str);

/* Function description: 
 * this function return array of strings that each string is opcode name.
 *
 * Parameters description:
 * -No parameters.
 *
 * Return:
 * The array of strings that each string is opcode name.
 */
char **opcode_names();
#endif
