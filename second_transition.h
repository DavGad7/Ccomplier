#ifndef SECOND_TRANSITION_H
#define SECOND_TRANSITION_H
#include "general.h"

#define E 1 /* the value of the E bit from the A,R,E bits */
#define R 2 /* the value of the R bit from the A,R,E bits */

/* the max length that the representation of number in binary of max of 10 bits (unsigned), can be in base 32.
 * the max number in binary of 10 bits (unsigned) is 1111111111 which is "vv" (without the quotation marks) in base 32.
 * that mean the max length of base 32 representation in this program is 2.
 */
#define MAX_BASE_LENGTH 2
/* reverse a string */

/* Function description:
 * -The function runs the second transition of the project
 *
 * Parameters description:
 * -InstC: instruction counter, data type: (integer pointer).
 * -DataC: data counter, data type: (integer pointer).
 * -afterMacSpreading: pointer to ".am" file (read mode), data type: (FILE pointer).
 * -fName: the name of the ".am" file, data type: (file_name pointer).
 * -head: pointer to the head of the linked list of tags, data type: (tag pointer).
 * -inst: array of instructions (from the ".am" file), data type: (instruction).
 * -data: array of the data (from the ".am" file), data type: (word).
 *
 * Return:
 * -TRUE if there was an error while the second transition process.
 * -FALSE if there was not an error while the second transition process.
 */
boolean secondTransition(int *InstC, int *DataC, FILE *afterMacSpreading, file_name fName, tag *head, instruction inst[], word data[]);

/* Function description:
 * the function updates the value of a tag that has data with InstC.
 *
 * Parameters description:
 * -InstC: instruction counter, data type: (integer pointer).
 * -head: pointer to the head of the linked list of tags, data type: (tag pointer).
 *
 * Return: no return (void function).
 */
void updateData( int *InstC,tag *head);

/* Function description:
 * the function handle single line from the ".am" file according to the second transition algorithm in the maman.
 *
 * Parameters description:
 * -iLine: the index of the parameter "line" at the ".as" file, data type: (integer).
 * -line: single line from the ".as" file, data type: (char pointer).
 * -fName: the name of the ".am" file, data type: (file_name pointer).
 * -head: pointer to the head of the linked list of tags, data type: (tag pointer).
 *
 * Return: no return (void function).
 */
void lineInfoSecondT(int iLine, char *line, file_name fName,tag *head);

/* Function description:
 * the function deals with a ".entry".
 *
 * Parameters description:
 * -iLine: the index of the parameter "line" at the ".as" file, data type: (integer).
 * -line: single line from the ".as" file, data type: (char pointer).
 * -fName: the name of the ".am" file, data type: (file_name pointer).
 * -head: pointer to the head of the linked list of tags, data type: (tag pointer).
 * 
 * Return: no return (void function).
 */
void entry( int iLine,char *line,  file_name fName,tag *head);

/* Function description: 
 * the function does a search for a specific tag in the linked list of tags.
 *
 * Parameters description:
 * -head: pointer to the head of the linked list of tags, data type: (tag pointer).
 * -tName: name of the tag that the function search for, data type: (char pointer).
 *
 * Return:
 * -returns pointer to the tag "tName" that you activated the search for if the tag was found in the linkes list of tags.
 * -if the tag "tName" wasn't found in the linked list of tags so returns null.
 */
tag *tagSearch(tag *head, char *tName);


/* Function description:
 * the function completes the process of the transition to the machine code that first transition didn't do.
 *
 * Parameters description:
 * -InstC: instruction counter, data type: (integer pointer).
 * -inst: array of instructions (from the ".am" file), data type: (instruction).
 * -fName: the name of the ".am" file, data type: (file_name pointer).
 * -head: pointer to the head of the linked list of tags, data type: (tag pointer).
 *
 * Return: no return (void function).
 */
void codeFromFirstTransition( int *InstC, instruction inst[], file_name fName,tag *head);

/* Function description:
 * the function create and write the object file (".ob") file.
 *
 * Parameters description:
 * -InstC: instruction counter, data type: (integer pointer).
 * -DataC: data counter, data type: (integer pointer).
 * -fName: the name of the ".am" file, data type: (file_name pointer).
 * -inst: array of instructions (from the ".am" file), data type: (instruction).
 * -data: array of the data (from the ".am" file), data type: (word).
 *
 * Return: no return (void function).
 */
void createObj( int *InstC, int *DataC, file_name fName,instruction inst[], word data[]);

/* Function description:
 * the function create and write the entry file (".rnt") file.
 *
 * Parameters description:
 * -fName: the name of the ".am" file, data type: (file_name pointer).
 * -head: pointer to the head of the linked list of tags, data type: (tag pointer).
 *
 * Return: no return (void function).
 */
void createEntry(file_name *fName, tag *head);

/* Function description: 
 * this function create and fill the ".ob" file.
 *
 * Parameters description:
 * -InstC: instruction counter, data type: (integer pointer).
 * -fName: the name of the ".am" file, data type: (file_name pointer).
 * -inst: array of instructions (from the ".am" file), data type: (instruction).
 *
 * Return: no return (void function).
 */
void createExternal(int *InstC, file_name fName, instruction inst[]);

/* Function description:
 * the function "translate" decimal number from decimal to base 32.
 *
 * Parameters description:
 * -num: the number that we want to convert to base 32, data type: (unsigned integer).
 * -num_in_32: the parameter x in the special base 32 representation, data type: (char).
 * 
 * Return:
 * -num in base 32.
 */
char *base_32(unsigned int num, char num_in_32[]);

/* Function description:
 * The function returns if there was an error in the second transition process.
 *
 * Parameters description:
 * -is_error: represents if there was an error or not, data type: (boolean).
 * -reset: represents if there was any change to the error or not, data type: (boolean).
 *
 * Return:
 * TRUE if there was an error in the second transition process, 
 * FALSE if there wasn't an error in the second transition process.
 */
boolean error_in_second_transition(boolean is_error, boolean reset);
#endif

