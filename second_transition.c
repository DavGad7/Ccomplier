#include "second_transition.h"
/*called if error detected in "secondTransition.c"  */
#define ERROR_IN_SECOND_TRANSITION  {           \
    error_in_second_transition(TRUE, FALSE); \
    return;                                     \
}
/*secondTransition()
 * first of all we update the value of a tag with InstC with the "updateData()" then we move the pointer afterMacSpreding
 * to the ".am" file to the start then we go across and we are checking every line for the line info with the "lineInfoSecondT()"
 * function after we go across the file we complete the tags addresses in the machine code with the function 
 * "codeFromFirstTransition()" then we create the ".ob" ,"ent" and ".ext" file with the "createObj()" ,"createEntry()" 
 * and "createExternal()" functions and if there wasn't any error until now we return FALSE(there wasn't error in second transition)
 * 
*/
boolean secondTransition(int *InstC, int *DataC, FILE *afterMacSpreading, file_name fName, tag *head, instruction inst[], word data[]) {
    char line[MAX_LINE_LENGTH]; 
    int iLine = 1; 

    updateData( InstC,head); 

    fseek(afterMacSpreading, 0, SEEK_SET);
    memset(line, '\0', sizeof(line)); 
    while (fgets(line, MAX_LINE_LENGTH, afterMacSpreading) != NULL) { 
        lineInfoSecondT(iLine,line, fName,head); 
        memset(line, '\0', sizeof(line)); 
        iLine++; 
    }

    codeFromFirstTransition(InstC, inst, fName,head); 
    
    if (error_in_second_transition(FALSE, FALSE)) 
        return TRUE;

    createObj( InstC, DataC,fName, inst, data); 
    createEntry(&fName, head); 
    createExternal( InstC,fName, inst); 
    return FALSE;
}
/*updateData()
 * we go over the linked list and if the tag has the data on we add InstC to the tag value.

*/
void updateData(int *InstC,tag *head) {
    tag *curr = head;

    while (curr->next != NULL) { 
        if (curr->data) 
            curr->val += (*InstC); 
        curr = curr->next;
    }
}
/*lineInfoSecondT()
 * first of all we check if the line is a comment line or empty line then we check if there is tag definition if there is 
 * we skip the tag content and update the var str to the next content then we check if there is an ".entry" definition 
 * if thre is we call the function "entry()".
*/
void lineInfoSecondT(int iLine, char *line, file_name fName,tag *head) {
    char content[MAX_LINE_LENGTH]; 

    line = remove_spaces(line);
    if (line[0] == ';' || line[0] == '\n') { 
        return;
    }

    memset(content, '\0', MAX_LINE_LENGTH);
    line = line_content(line, content); 
    if (content[strlen(content)-1] == ':') { 
        line = remove_spaces(line);
        memset(content, '\0', MAX_LINE_LENGTH);
        line = line_content(line, content); 
    }

    if (!strcmp(content, ".entry")) { 
        entry(iLine,line,fName,head);
    }
}
/*entry()
 * first of all we check if we got any arguments in not we print an error message and call ERROR_IN_SECOND_TRANSITION 
 * then we check if the tag name is valid  if not we print an error message and call ERROR_IN_SECOND_TRANSITION
 * then we check if there were a text after the tag name if there was we print an error message and call ERROR_IN_SECOND_TRANSITION
 * then we search for the node of the tag from the linked list of tags then if we got null then the tag is not in the code
 * and we print an error message and call ERROR_IN_SECOND_TRANSITION then we turn on the entry flag of the node.
*/
void entry(int iLine,char *line,  file_name fName,tag *head) {
    char tName[MAX_LINE_LENGTH]; 
    tag *arg_tag;

    line = remove_spaces(line); 
    if (line[0] == '\n') {
        printf("Error in line %d, \".entry\" must get 1 argument, actually got 0!\n", iLine);
        ERROR_IN_SECOND_TRANSITION
    }

    memset(tName, '\0', MAX_LINE_LENGTH);
    line = line_content(line, tName); 
    if (!macro_name_valid(tName)) { 
        printf("Error in line %d,  \"%s\" is not valid tag name \n", iLine, tName);
        ERROR_IN_SECOND_TRANSITION
    }

    line = remove_spaces(line);
    if (line[0] != '\n') {
        printf("Error in line %d, text after the tag name!\n", iLine);
        ERROR_IN_SECOND_TRANSITION
    }

    arg_tag = tagSearch(head, tName); 
    if (arg_tag == NULL) { 
        printf("Error in line %d, tag \"%s\" is entry, but is not in the code!\n", iLine , tName);
        ERROR_IN_SECOND_TRANSITION
    }

    arg_tag->entry = TRUE; 
}
/* tagSearch()
 * we go over the linked list of tags until it ends or we found the tag that we were looking for.
*/
tag *tagSearch(tag *head, char *tName) {
    tag *curr = head;

    while (curr->next != NULL && strcmp(tName, curr->name) != 0) 
        curr = curr->next;

    return curr->next == NULL? NULL: curr; 
}
/*codeFromFirstTransition()
 * first of all we are going to over the instruction inst then we check if to the instruction has a name if it does so we
 * need to write the tag address in the machine code then we find the node in the linked list of tags that represents the tag
 * address that the machine code need if we didn't found the node we print an error message and call ERROR_IN_SECOND_TRANSITION
 * then we check if the tag is external we change the current insruction value to E(represents that the content of the word 
 * depends on the value of a symbol that is not defined in the current source file) else we write the tag adress at the machine code
*/
void codeFromFirstTransition(int *InstC, instruction inst[], file_name fName,tag *head) {
    int i;
    tag *temp;

    for (i = 0; i < (*InstC); i++) {
        if (strlen(inst[i].tName) != 0) { 
            temp = tagSearch(head, inst[i].tName);
            if (temp == NULL) { 
                printf("Error in line %d, tag \"%s\" is not defined!\n", inst[i].iLine , inst[i].tName);
                ERROR_IN_SECOND_TRANSITION
            }

            if (temp->is_extern) { 
                inst[i].val = E;
            } else {
                inst[i].val = temp->val << 2; 
                inst[i].val |= R;
            }
        }
    }
}
/*createObj()
 * first of all we change the extension of the file to ".ob" then we open the file in write mode and we check if the file 
 * didn'rt open if it didn't we print an error message and exit but if the file did created we write InstC and DataC
 * values in the base 32 to the ".ob" file and then we write the all the instructions to the file in the base 32
 * we write the adress and after that we write the values then we write the data to the ".ob" file in base 32 then after
 * we finished with all of that we close the file.
*/
void createObj(int *InstC, int *DataC, file_name fName,instruction inst[], word data[]) {
    FILE *obj; 
    char in_base_32[MAX_BASE_LENGTH]; 
    int i;
	
    fName.name[fName.len-2] = 'o'; 
    fName.name[fName.len-1] = 'b';
    obj = fopen(fName.name, "w"); 
 
    if (obj == NULL) { 
        printf("Error! Couldn't make file! leave the program.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(obj, "%s ", base_32((*InstC), in_base_32));
    fprintf(obj, "%s\n", base_32((*DataC), in_base_32));

    for (i = 0; i < (*InstC); i++) {
        fprintf(obj, "%s ", base_32(i + LOAD_LOCATION, in_base_32)); 
        fprintf(obj, "%s \n", base_32(inst[i].val, in_base_32)); 
    }

    for (; i < (*DataC) + (*InstC); i++) {
        fprintf(obj, "%s ", base_32(i + LOAD_LOCATION, in_base_32)); 
        fprintf(obj, "%s\n", base_32(data[i-(*InstC)].val, in_base_32)); 
    }

    fclose(obj);
}
/*createEntry()
 * first of all we add the ".ent" extension then we so over the linked list of ttags and check if the entry flag is on 
 * if it the first entry we found we create the file ".ent" in write mode and check if the file didn't create if it didn't 
 * we print an error message and exit but if it wasn't the first one we  write the name of the tag and its value in the file 
 * then we change the extension of the parameter file name to ".ent" extension anf in the end we free the memoty of the 
 * program and in the end if there was at least one entry we close the file of the ".ent" file.
*/
void createEntry(file_name *fName, tag *head) {
    char *entFileName = (char *)malloc((fName->len + 1) * sizeof(char)); 
    tag *curr = head; 
    FILE *ent; 
    boolean min_one_ent = FALSE;
    char in_base_32[MAX_BASE_LENGTH]; 

    MEMORY_ERROR(entFileName)

    memset(entFileName, '\0', fName->len+1);
    strncpy(entFileName, fName->name, fName->len - 2); 
    entFileName[fName->len-2] = 'e';
     entFileName[fName->len-1] = 'n'
     ; entFileName[fName->len] = 't'; 
    entFileName[fName->len+1] = '\0';

    while (curr->next != NULL) { 
        if (curr->entry) { 
            if (!min_one_ent) { 
                ent = fopen(entFileName, "w"); 
                if (ent == NULL) { 
                    printf("Error! Couldn't make file! leave the program.\n");
                    exit(EXIT_FAILURE);
                }
            }
            min_one_ent = TRUE;
            fprintf(ent, "%s %s\n", curr->name, base_32(curr->val, in_base_32)); 
        }
        curr = curr->next;
    }

    fName->len++;
    strcpy(fName->name, entFileName);

    free(entFileName);
    if (min_one_ent) fclose(ent);
}
/*createExternal()
 * first of all we change the extension to ".ext" from ".ent" then we find all the shows of externals tags at the machine code 
 * and if there is an extern flag we check if there was at least one external tag before if not we create the file with tha ".ext" 
 * in write mode then we check if there was an problem with the file creation if there was we print an error message and exit
 * if there were an external flag befrore we write the name of the tag and its value to rhe ".ext" file .
*/
void createExternal(int *InstC, file_name fName, instruction inst[]) {
    char in_base_32[MAX_BASE_LENGTH]; 
    FILE *ext; 
    boolean min_one_ext = FALSE; 
    int i;

    fName.name[fName.len-2] = 'x'; 

    for (i = 0; i < (*InstC); i++) { 
        if ((inst[i].val & E) == E) { 
            if (!min_one_ext) { 
                ext = fopen(fName.name, "w"); 
                if (ext == NULL) { 
                    printf("Error! Couldn't make file! leave program.\n");
                    exit(EXIT_FAILURE);
                }
            }
           min_one_ext = TRUE; 
            fprintf(ext, "%s %s\n", inst[i].tName, base_32(i + LOAD_LOCATION, in_base_32)); 
        }
    }


    if (min_one_ext)
     fclose(ext);
}
/* base_32()
 * we change the num to the base 32 representation of num then we reverse the num and return it  
*/
char *base_32(unsigned int num, char num_in_32[]) {
    static char base_32_chars[] = "!@#$%^&*<>abcdefghijklmnopqrstuv"; 
    int i;
    int len;                                   \
    char temp;  
    memset(num_in_32, '\0', MAX_BASE_LENGTH); 

    if (num == 0) 
        num_in_32[0] = base_32_chars[0];

    i = 0;
    while (num != 0) {
        num_in_32[i] = base_32_chars[num%32];
        num /= 32;
        i++;
    }

    len = strlen((num_in_32));                       \
    for (i = 0; i < len/2; i++) {  \
        temp = (num_in_32)[i];                      \
        (num_in_32)[i] = (num_in_32)[len-i-1];     \
        (num_in_32)[len-i-1] = temp;             \
    }   
    return num_in_32;
}

/*error_in_second_transition()
 * if reset required turn error_in_second_transition off and if the function called because of an error 
 * turn error_in_second_transition on and then return error_in_second_transition
*/
boolean error_in_second_transition(boolean is_error, boolean reset) {
    static boolean error_in_second_transition = FALSE; 

    if (reset) 
        error_in_second_transition = FALSE;

    if (is_error) 
        error_in_second_transition = TRUE; 

    return error_in_second_transition; 
}




