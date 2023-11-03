#ifndef FIRST_TRANSITION_H
#define FIRST_TRANSITION_H
#include "general.h"
#define DIRECT_ADDRESSING 1 /*value of the direct addressing */
#define STRUCT_ACCESS 2 /* value of the access to struct addressing */
#define DIRECT_REGISTER 3 /* value of the direct register addressing */
#define MAX_LINE_LENGTH 81
#define MAX_NAME_LENGTH 30
#define TARGET_SHIFT 2 /* the amount to shift if the operand is target operand */
#define SOURCE_SHIFT 4 /* the amount to shift if the operand is source operand */
#define ADDRESSING_MIN (-128) /*min value that the code can use when using immediate addressing */
#define ADDRESSING_MAX 127 /*max value that the code can use when using immediate addressing */


/* Function description: 
 * -The function runs the first transition of the project
 *
 * Parameters description:
 * -InstC: instruction counter, data type: (integer pointer).
 * -DataC: data counter, data type: (integer pointer).
 * -afterMacSpreading: pointer to the ".am" file, data type: (FILE pointer).
 * -fName: file with the ".am" extension, data type: (file_name).
 * -inst: array of instructions (from the ".am" file), data type: (instruction).
 * -data: array of the data (from the ".am" file), data type: (word).
 * -head: pointer to the head of the linked list of tags, data type: (tag pointer).
 * 
 * Return:
 * -FALSE if there was no error during the first transition.
 * -TRUE if there was an error during the first transition.
 */
boolean firstTransition(int *InstC, int *DataC, FILE *afterMacSpreading , file_name fName , instruction inst[], word data[], tag *head);

/* Function description:
 * -The function deals with a line (from the ".am" file) by the second transition algorithm.
 *
 * Parameters description:
 * -InstC: instruction counter, data type: (integer pointer).
 * -DataC: data counter, data type: (integer pointer).
 * -iLine: the line index (from ".am" file) of the parameter "line", data type: (integer).
 * -line: a line (from the ".am" file), data type: (char pointer).
 * -fName: file with the ".am" extension, data type: (file_name).
 * -inst: array of instructions (from the ".am" file), data type: (instruction).
 * -data: array of the data (from the ".am" file), data type: (word).
 * -head: pointer to the head of the linked list of tags, data type: (tag pointer).
 * 
 * Return: no return (void function).
*/
void lineInfoFirstT(int *InstC, int *DataC, int iLine, char *line ,file_name fName , instruction inst[], word data[] , tag *head);

/* Function description:
 * The function adds new tag to the linked list of tags if it a valid tag.
 *
 * Parameters description:
 * -iLine: the line index (from ".am" file) of the parameter "line", data type: (integer).
 * -value: value of the tag, data type: (integer).
 * -name: name of the tag, data type: (char pointer).
 * -data: if the is data sentence, data type: (boolean).
 * -is_extern: if tag is extern, data type: (boolean).
 * -entry: if tag is entry, data type: (boolean).
 * -opCode: if tag is command sentence, data type: (boolean).
 * -fName: file with the ".am" extension, data type: (file_name).
 * -head: pointer to the head of the linked list of tags, data type: (tag pointer).
 *
 * Return: no return (void function).
 */
void addTag(int iLine, int value, char *name, boolean data, boolean is_extern, boolean entry, boolean opCode, file_name fName , tag *head);

/* Function description:
 * The function deals with commands.
 *
 * Parameters description:
 * -iLine: the line index (from ".am" file) of the parameter "line", data type: (integer).
 * -InstC: instruction counter, data type: (integer pointer).
 * -opCodeName: name of the operating code, data type: (char pointer).
 * -line: a line (from the ".am" file), data type: (char pointer).
 * -fName: file with the ".am" extension, data type: (file_name).
 * -inst: array of instructions (from the ".am" file), data type: (instruction).
 *
 * Return: no return (void function).
 */
void opcode(int iLine, int *InstC,char *opCodeName, char *line, file_name fName, instruction inst[]);

/* Function description:
 * The function deals with ".string".
 *
 * Parameters description:
 * -DataC: data counter, data type: (integer pointer).
 * -iLine: the line index (from ".am" file) of the parameter "line", data type: (integer).
 * -line: a line (from the ".am" file), data type: (char pointer).
 * -fName: file with the ".am" extension, data type: (file_name).
 * -data: data: array of the data (from the ".am" file), data type: (word).
 * -fromStruct: if function called from struct_def, data type: (boolean).

 * Return: no return (void function).
 */
void string(int *DataC, int iLine, char *line, file_name fName, word data[], boolean fromStruct);

/* Function description:
 * The function deals with ".data".
 *
 * Parameters description:
 * -iLine: the line index (from ".am" file) of the parameter "line", data type: (integer).
 * -DataC: data counter, data type: (integer pointer).
 * -line: a line (from the ".am" file), data type: (char pointer).
 * -fName: file with the ".am" extension, data type: (file_name).
 * -data: data: array of the data (from the ".am" file), data type: (word).
 *
 * Return: no return (void function).
 */
void data_def(int iLine,  int *DataC , char *line ,file_name fName, word data[]);

/*Function description:
 * The function deals with ".struct".
 *
 * Parameters description:
 * -DataC: data counter, data type: (integer pointer).
 * -iLine: the line index (from ".am" file) of the parameter "line", data type: (integer).
 * -line: a line (from the ".am" file), data type: (char pointer).
 * -fName: file with the ".am" extension, data type: (file_name).
 * -data: data: array of the data (from the ".am" file), data type: (word).
 *
 * Return: no return (void function).
 */
void struct_def(int *DataC, int iLine, char *line, file_name fName, word data[]);

/* Function description:
 * The function deals with ".extern".
 *
 * Parameters description:
 * -iLine: the line index (from ".am" file) of the parameter "line", data type: (integer).
 * -line: a line (from the ".am" file), data type: (char pointer).
 * -fName: file with the ".am" extension, data type: (file_name).
 * -head: pointer to the head of the linked list of tags, data type: (tag pointer).
 *
 * Return: no return (void function).
 */
void extern_def(int iLine, char *line, file_name fName ,tag *head);

/* Function description:
 * The function deals with commands with opcode that get onlt one operand.
 *
 * Parameters description:
 * -opcode:index of opcode, data type: (integer).
 * -iLine: the line index (from ".am" file) of the parameter "line", data type: (integer).
 * -InstC: instruction counter, data type: (integer pointer).
 * -shift: how much to (<<), the value determined by if operand is source or a target, data type: (integer).
 * -line: a line (from the ".am" file), data type: (char pointer).
 * -opCodeOrigin: original word the other words are derived from, data type: (instruction).
 * -inst: array of instructions (from the ".am" file), data type: (instruction).
 * -fName: file with the ".am" extension, data type: (file_name).
 * 
 * Return: no return (void function).
 */
void one_operand(int opcode,int iLine,  int *InstC ,  int shift ,  char *line , instruction *opCodeOrigin, instruction inst[], file_name fName);

/* Function description:
 * The function deals with commands with opcode that get only two operand.
 *
 * Parameters description:
 * -opcode:index of opcode, data type: (integer).
 * -iLine: the line index (from ".am" file) of the parameter "line", data type: (integer).
 * -InstC: instruction counter, data type: (integer pointer).
 * -line: a line (from the ".am" file), data type: (char pointer).
 * -opCodeOrigin: original word the other words are derived from, data type: (instruction).
 * -inst: array of instructions (from the ".am" file), data type: (instruction).
 * -fName: file with the ".am" extension, data type: (file_name).

 *
 * Return: no return (void function).
 */
void two_operands(int opcode,int iLine,  int *InstC ,  char *line , instruction *opCodeOrigin, instruction inst[], file_name fName);

/* Function description:
 * The function returns if there was an error in the first transition process.
 *
 * Parameters description:
 * -is_error: represents if there was an error or not, data type: (boolean).
 * -reset: represents if there was any change to the error or not, data type: (boolean).
 *
 * Return:
 * TRUE if there was an error in the first transition process, 
 * FALSE if there wasn't an error in the first transition process.
 */
boolean error_in_first_transition(boolean is_error, boolean reset);
#endif

