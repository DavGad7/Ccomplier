#include "macro_spreading.h"
/*freeMemory
 * We go over the linked list of macros and free every macro memory
*/
void freeMemory(FILE *as, FILE *am, macro *head) {
    macro *mac;
    while (head != NULL) {
        mac = head;
        head = head->next;
        free(mac);
    }
    fclose(as);
    fclose(am);
}
/*macroSpreding()
 * First of all we declare and reset the values of the linked list of macros and then we allocate 
 * memory for it and we check if the memory allocation was valid then we create the ".am" file
 * and we check if there was an problem wih the file creation if there were we print an error message and exit with EXIT_FALIURE
 * protocol then we go over the file and for every line we check the code if it include a macro with the "addMavToFile()" function
 * then we write the line that went through the addMacToFile fuction to the ".am" file
 * then we free the allocated memory of the file pointer am file and the macro pointer and in the end we return 
 * if there was an error in the macro spreading process or not with the "error_in_macro" function
*/
boolean macroSpreading(FILE *fp, file_name fName) {
    macro *head; 
    char line[MAX_LINE_LENGTH]; 
    int iLine = 1; 
    FILE *am; 

    head = (macro *)malloc(sizeof(macro)); 
    MEMORY_ERROR(head) 
    memset(head->mContent, '\0', MAX_LINE_LENGTH * MAX_LINES_MACRO);
    head->next=NULL;
    fName.name[fName.len-1] = AM; 
    am = fopen(fName.name, "w"); 
    if (am == NULL) { 
        printf("Error!! Failed to make the required file!.\n");
        exit(EXIT_FAILURE);
    }

    memset(line, '\0', sizeof(line)); 
    while (readFile(iLine, line,fp,fName) != NULL) { 
        addMacLineToFile(iLine, line, am, fName,head); 
        memset(line, '\0', sizeof(line)); 
        iLine++;
    }

    fprintf(am, "\n"); 
    freeMemory(fp, am, head); 

    return error_in_macro(FALSE, FALSE);
}
/* readFile()
 * first of all we get the line with fgets then we check if line is not null(if the file didn't end yet)
 * then we check if the line at the is over than 81 chars including '\n' if it is not 
 * we change the file extension to ".as" and prints an error file then we change the extension to ".am" and call
 * "error_in_macro()" with is_error on but if the line is not over than 81 chars we just return the line that we got. 
*/
char *readFile(int iLine,char *line, FILE *fp,  file_name fName) {
    char *temp = fgets(line, MAX_LINE_LENGTH, fp);

    if (temp != NULL) { 
        if (strlen(line) == MAX_LINE_LENGTH - 1 && line[MAX_LINE_LENGTH-1] != '\n') { 
            fName.name[fName.len-1] = AS; 
            printf("Error in line %d, line over 80 characters!\n", iLine);
            fName.name[fName.len-1] = AM;
            error_in_macro(TRUE, FALSE);
        }
    }

    return temp;
}
/*addMacToFile()
 * first of all we check if the flag in_mac is on if it does we check if we detected "endmacro" in the line if did we 
 * turn the flag in_mac off but if "endmacro" didn't detected we add the line to the last macro content 
 * and if the flag in_mac is off we remove the blank characters from the line and with "remove_spaces()" and "line_content()"
 * functions then if a macro name detected we write the macro content to the ".am" file else if the lne content is equal to
 * START_OF_MACRO if it does macro start definition detected and we turn the in_mac flag on else we copy the line to 
 * the ".am" file without adding something.
*/
void addMacLineToFile(int iLine, char *line, FILE *am, file_name fName, macro *head) {
    char content[MAX_LINE_LENGTH]; 
    char line_dup[MAX_LINE_LENGTH]; 
    macro *temp; 
    static boolean in_mac = FALSE; 
    strcpy(line_dup, line);
    memset(content, '\0', MAX_LINE_LENGTH); 
    
    if (in_mac) {
        if (macroEnd(line)) 
            in_mac = FALSE; 
        else 
            strcat((lastMacro(head))->mContent, line_dup); 
    } else {
        line = remove_spaces(line);
        line = line_content(line, content);
        if ((temp = searchMacro(content,head)) != NULL) { 
            fprintf(am, "%s", temp->mContent); 
        } else if (!strcmp(START_OF_MACRO, content)) {
            in_mac = TRUE; 
            addMacro(iLine, line,fName,head); 
        } else { 
            fprintf(am, "%s", line_dup);
    }
}}
/*addMacro()
 * first of all we gt the macro name to th mac_name varible then we check if the name is valid or not if 
 * not we cange the extension to ".as" and print an error message then we change the extension to ".am"
 * and call "error_in_macro()" with error sign on then we check if the macro name is already is in the linked 
 * list of macros if it does we are doing the same protocol and same thing for the check if the macro name is 
 * a register name then we get to the tail of the macro list and put the name of the macro in the macro node then 
 * we create a new node for the next macro
*/
void addMacro(int iLine,char *line, file_name fName,macro *head) {
    char mac_name[MAX_MACRO_NAME_LENGTH]; 
    macro *mac = head; 
    memset(mac_name, '\0', MAX_MACRO_NAME_LENGTH);

    line = remove_spaces(line);
    line_content(line, mac_name); 

    if (!macro_name_valid(mac_name)) {
        fName.name[fName.len-1]  = AS; 
        printf("Error in line %d, macro \"%s\" has an invalid name!\n", iLine , mac_name);
        fName.name[fName.len-1] = AM; 
        error_in_macro(TRUE, FALSE); 
    }

    if (searchMacro(mac_name,head) != NULL) {
        fName.name[fName.len-1] = AS; 
        printf("Error in line %d, macro \"%s\" already defined!\n", iLine , mac_name);
        fName.name[fName.len-1] = AM; 
        error_in_macro(TRUE, FALSE); 
    }

    if (register_num(mac_name) != NOT_REGISTER) {
        fName.name[fName.len-1] = AS; 
        printf("Error in line %d, macro \"%s\" cant be a register name!\n", iLine , mac_name);
        fName.name[fName.len-1] = AM; 
    }

    while (mac->next != NULL) 
        mac = mac->next; 
    strcpy(mac->mName, mac_name); 

    mac->next = (macro *)malloc(sizeof(macro));
    MEMORY_ERROR(mac->next) 
    memset((mac->next)->mContent, '\0', MAX_LINE_LENGTH * MAX_LINES_MACRO);
    (mac->next)->next = NULL;
}
/*searchMacro()
 * we go over all the linked list of macros and cheks if mac_name is a name of a macro in the list if it is 
 * we return the current macro if it didn't return a macro in the while loop we return null 
*/
macro *searchMacro(char *mac_name,macro *head ) {
    macro *mac = head; 
    if (strlen(mac_name) > 0){ 
        while (mac != NULL) { 
            if (!strcmp(mac->mName, mac_name)) 
                return mac;
            mac = mac->next; 
        }
    }
    return NULL;
}
/*lastMacro()
 * we go over the linked list of macros until the last one
*/
macro *lastMacro(macro *head) {
    macro *mac = head;
    while ((mac->next)->next != NULL)
        mac = mac->next;
    return mac;
}
/*macroEnd()
 * we check if in the line we got an END_OF_MACRO definition
*/
boolean macroEnd(char *line) {
    char content[MAX_LINE_LENGTH];
    memset(content, '\0', MAX_LINE_LENGTH);
    line = remove_spaces(line); 
    line_content(line, content); 

    return !strcmp(content, END_OF_MACRO); 
}
/*error_in_macro()
 * if reset required turn error_in_macro off and if the function called because of an error 
 * turn error_in_macro on and then return error_in_macro
*/
boolean error_in_macro(boolean is_error, boolean reset) {
    static boolean error_in_macro = FALSE; 
    if (reset) 
        error_in_macro = FALSE; 
    if (is_error) 
        error_in_macro = TRUE; 
    return error_in_macro; 
}

