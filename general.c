#include "general.h"
/* macro_name_valid(char *name)
 * the function first of all checks if the first character is a letter if not it turns the flag "valid" to false
 * then checks if in the name the character is a letter or a nummber if not it turns the flag "valid" to false
 * then checks if the length of the macro name is over 30 characters if it is over 30 turns the flag "valid" to false
 * then checks if it is register name or an opcode if it does turns the flag "valid" to false
 * after that returns the flag "valid"
*/
boolean macro_name_valid(char *name) {
    int i;
    boolean valid = TRUE; 
    if (!((name[0] >= 'a' && name[0] <= 'z')
          || (name[0] >= 'A' && name[0] <= 'Z')))
        valid = FALSE;

    for (i = 1; i < strlen(name); i++)
        if (!((name[i] >= 'a' && name[i] <= 'z')
              || (name[i] >= 'A' && name[i] <= 'Z')
              || (name[i] >= '0' && name[i] <= '9')))
            valid = FALSE;

    if (strlen(name) > MAX_MACRO_NAME_LENGTH)
        valid = FALSE;

    if (register_num(name) != NOT_REGISTER) 
        valid = FALSE;

    for (i = 0; i < 16; i++){
        if (!strcmp(opcode_names()[i], name)){
        valid=FALSE;
        i=16; 
        }
    }
    return valid;
}
/* remove_spaces(char *line)
 * the function checks until what index of a given string is a '\n', ' ' or a '\t' and then returns the string starting from
 * that index 
*/
char *remove_spaces(char *line) {
    int i = 0;
    while (!(line[i] == '\n') && (line[i] == ' ' || line[i] == '\t')) i++; 
    return line + i; 
}
/* line_content(char *line,  char *content)
 * first of all we go over the given line until we get '\n', ' ' or a '\t' every time we dont get character like this
 * we copy the char to content and we return 
*/
char *line_content(char *line,  char *content) {
    int i = 0;

    while ( !(line[i] == ' ' )&& !( line[i] == '\t') && !(line[i] == '\n')) {
        content[i] = line[i]; 
        i++;
    }

    return line + i; /* delete the non-blank characters from the start of the line until blank character */
}
/*register_num(char *str)
 *first of all the function creates an char array of the names of the registers then it checks if the char pointer
  * that was given points to a register name if it does the function returns the register number if it
 *
*/
int register_num(char *str) {
        int i;

    static char *registers_names[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
    for (i = 0; i < 8; i++)
        if (!strcmp(registers_names[i], str)) 
            break;

    return i; 
}
/*  **opcode_names() 
 * creates a static char array of the opCodes name and returns it
*/
char **opcode_names() {
    static char *opcode_names[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "get", "prn", "jsr", "rts", "hlt"};
    return opcode_names; 
}




