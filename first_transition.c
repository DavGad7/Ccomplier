#include "first_transition.h"
/*called if error detected in "secondTransition.c"  */
#define ERROR_IN_FIRST_TRANSITION  {           \
    error_in_first_transition(TRUE, FALSE); \
    return;                                     \
}
/*firstTransition()
 * first of all we are creating 2 variabels one for to store the line from the file and one for counting 
 * in what line in the file we are then we go over the given file we are checking every line for
 * the line info with the "lineInfoFirstT()" function and we reseting the var line array values to '\0'
 * and we are adding +1 to the line index until the files ends then if the function ran wthout an error 
 * from "lineInfoFirstT()" we return that wasnt an error with the "error_in_first_transition()" function
*/
boolean firstTransition(int *InstC, int *DataC, FILE *afterMacSpreading , file_name fName , instruction inst[], word data[], tag *head) {
    char line[MAX_LINE_LENGTH]; 
    int iLine = 1; 

    memset(line, '\0', sizeof(line));
    while (fgets(line, MAX_LINE_LENGTH, afterMacSpreading) != NULL) {
        lineInfoFirstT(InstC, DataC , iLine, line, fName, inst, data ,head); 
        memset(line, '\0', sizeof(line)); 
        iLine++; 
    }

    return error_in_first_transition(FALSE, FALSE);
}
/*lineInfoFirstT()
 * first of all we check if the line is empty or a comment afer that we check for macros
 * then we check for ".data", ".string", ".entry" or ".extern" sentences then if its not
 * any of them we add the tag to the linked list with code sign and we call the function
 * "opcode()"
*/
void lineInfoFirstT(int *InstC, int *DataC, int iLine, char *line ,file_name fName , instruction inst[], word data[] , tag *head) {
    char tName[MAX_NAME_LENGTH]; 
    char content[MAX_LINE_LENGTH]; 
    boolean tag_def = FALSE; 
    line = remove_spaces(line);
    if (line[0] == ';' || line[0] == '\n') { 
        return;
    }
    memset(content, '\0', MAX_LINE_LENGTH);
    line = line_content(line, content); 
    if (content[strlen(content)-1] == ':') {
        tag_def = TRUE;
        content[strlen(content)-1] = '\0'; 
        if (!macro_name_valid(content)) { 
            printf("Error in line %d, \"%s\" is not valid tag name \n", iLine, content);
            ERROR_IN_FIRST_TRANSITION;
        }
        strcpy(tName, content); 
        if (tName[0] == '\0') {
            printf("Error in line %d, tag is blank!\n", iLine);
            ERROR_IN_FIRST_TRANSITION;
        }
        line = remove_spaces(line);
        memset(content, '\0', MAX_LINE_LENGTH);
        line = line_content(line, content);
        if (strlen(content) == 0) { 
            printf("Error in line %d, tag \"%s\" does not contains any directive sentence or commands!\n", iLine, tName);
            ERROR_IN_FIRST_TRANSITION;
        }
    }

    if (content[strlen(content)-1] == ',') {
        printf("Error in line %d, Unnecessary comma!\n", iLine);
        ERROR_IN_FIRST_TRANSITION;
    }

    if (!strcmp(content, ".data")) { 
        if (tag_def) 
            addTag(iLine , *DataC , tName, TRUE, FALSE, FALSE, FALSE, fName, head); 

        data_def(iLine, DataC, line, fName, data);
    } else if (!strcmp(content, ".string")) { 
        if (tag_def) 
            addTag(iLine , *DataC , tName, TRUE, FALSE, FALSE, FALSE, fName, head); 

        string(DataC , iLine , line , fName, data, FALSE);
    } else if (!strcmp(content, ".struct")) { 
        if (tag_def) 
            addTag( iLine , *DataC , tName, TRUE, FALSE, FALSE, FALSE, fName, head); 
        struct_def(DataC, iLine, line, fName,  data);
    } else if (!strcmp(content, ".entry")) { 
        if (tag_def) { 
            if (!macro_name_valid(tName)) { 
                printf("Error in line %d, the tag \"%s\" has invalid name!\n", iLine, tName);
                ERROR_IN_FIRST_TRANSITION;
            }
            printf("Warning in line %d ,  \"%s\" does not have meaning!\n", iLine , tName);
        }
    } else if (!strcmp(content, ".extern")) { 
        if (tag_def) { 
            if (!macro_name_valid(tName)) { 
                printf("Error in line %d, the tag \"%s\" has invalid name!\n", iLine , tName);
                ERROR_IN_FIRST_TRANSITION;
            }
            printf("Warning in line %d ,\"%s\" does not have meaning!\n", iLine , tName);
        }
        extern_def(iLine, line, fName , head);
    } else { 
        if (tag_def) 
            addTag( iLine , *InstC ,tName, FALSE, FALSE, FALSE, TRUE, fName, head); 
        opcode( iLine, InstC , content, line, fName , inst);
    }
}
/*addTag()
 * first we go acroos th linked list of tags to check if the tag was already in the linked list if it is 
 * rerurn ERROR_IN_FIRST_TRANSITION and prints an error message if we didn't got an error
 * we fill out the tag information and we create a new node with defult values
*/
void addTag( int iLine, int value, char *name, boolean data, boolean is_extern, boolean entry, boolean opCode, file_name fName , tag *head) {
    tag *curr = head;
    while (curr->next != NULL) {
        if (!strcmp(curr->name, name)) {
            printf("Error in line %d, the tag \"%s\" was already defined as a tag!\n", iLine , name);
            ERROR_IN_FIRST_TRANSITION;
        }
        curr = curr->next;
    }

    strcpy(curr->name, name);
    curr->entry = entry;
    curr->opCode = opCode;
    curr->data = data;
    curr->is_extern = is_extern;
    curr->val = value + LOAD_LOCATION;
        
    curr->next = (tag *)malloc(sizeof(tag));
    MEMORY_ERROR(curr->next)
    curr = curr->next;
    curr->next = NULL;
    curr->entry = FALSE;
    curr->data = FALSE;
    curr->is_extern = FALSE;
    curr->opCode = FALSE;
    curr->val = 0;
}
/*opcode()
 * first of all we check what opcode is the given opCodeName is if it is not found returns error message
 * then we the fill instruction array with the opcode then if the opcode include 2 operands we 
 * call the "two_operands()" function and if the opcode include a single op we call
 * the "one_operand()" function and if the opcode includes 0 operands we print an error message 
 * and we call ERROR_IN_FIRST_TRANSITION 
*/
void opcode( int iLine, int *InstC,char *opCodeName, char *line, file_name fName, instruction inst[]) {
    int i;

    if (opCodeName[0] == ',') {
        printf("Error in line %d, Unnecessary comma!\n", iLine);
        ERROR_IN_FIRST_TRANSITION;
    }

    for (i = 0; i != 16 && strcmp(opCodeName, opcode_names()[i]) != 0; i++);
    if (i == 16) {
        printf("Error in line %d, opcode doesnt exist!\n", iLine);
        ERROR_IN_FIRST_TRANSITION;
    }

    inst[(*InstC)].val = i << 6;
    inst[(*InstC)].is_register = FALSE;
    (inst[(*InstC)].tName)[0] = '\0';
    inst[(*InstC)].iLine = iLine;
    (*InstC)++;

    line = remove_spaces(line);
    if ((0 <= i && i <= 3) || i == 6) { 
        two_operands(i,iLine,InstC,line,&inst[(*InstC)-1],inst,fName);
    } else if (4 <= i && i <= 13) { 
        one_operand(i,iLine,InstC, TARGET_SHIFT,line,&inst[(*InstC)-1],inst,fName);
    } else { 
        if (line[0] != '\n') {
            printf("Error in line %d, there is an external text in the operand!!\n", iLine);
            ERROR_IN_FIRST_TRANSITION;
        }
    }
}
/*string()
 * first we check if we get an argument if not prints an error messageand call ERROR_IN_FIRST_TRANSITION and then if there
 * is an openng quotation marks if not prints an error message and calls ERROR_IN_FIRST_TRANSITIONthen then 
 * we go over the line until the closing quotation marks
 * and we adding the value of the current char at the string definition and move to the next char when we get to the closing marks
 * we check if there is External text in the quotation marks prints an error message and calls ERROR_IN_FIRST_TRANSITION
 * then adds '\0' to the data when we done going over the line and if there was not a closing quotation marks prints an error message
 * and calls ERROR_IN_FIRST_TRANSITION
*/
void string(int *DataC, int iLine, char *line, file_name fName, word data[], boolean fromStruct) {
    boolean close_quotation = FALSE; 

    line = remove_spaces(line);
    if (line[0] == '\n') { 
        if (fromStruct) {
            printf("Error in line %d, \".struct\" argument didn not get a string argument!\n", iLine);
        } else {
            printf("Error in line %d, \".string\" argument did not get an argument!\n", iLine);
        }
        ERROR_IN_FIRST_TRANSITION;
    }
    if (line[0] != '"') { 
        if (fromStruct) {
            printf("Error in line %d, string field in \".struct\" have to be open by quotation marks!\n", iLine);
        } else {
            printf("Error in line %d, definition of \".string\" have to be open by quotation marks!\n", iLine);
        }
        ERROR_IN_FIRST_TRANSITION;
    }
    line++; 

    while (line[0] != '\n') { 
        if (line[0] == '"') { 
            close_quotation = TRUE;

            line++; 
            line = remove_spaces(line);
            if (line[0] != '\n') { 
                printf("Error in line %d, there is an external text in the operand!\n", iLine);
                ERROR_IN_FIRST_TRANSITION;
            }

            data[(*DataC)].val = '\0'; 
            (*DataC)++;
        } else {
            data[(*DataC)].val = (int)line[0]; 
            (*DataC)++;

            line++; 
        }
    }

    if (!close_quotation) { 
        if (fromStruct) {
            printf("Error in line %d, string field in \".struct\" has to be closed by quotation marks!\n", iLine);
        } else {
            printf("Error in line %d, definition of \".string\" has to be closed by quotation marks!\n", iLine);
        }
        ERROR_IN_FIRST_TRANSITION;
    }
}
/*data_def()
 * first of all we remove the start spaces of the line with the function "remove_spaces()" then we check if there is an argument in the 
 * line if not we print an error message and call ERROR_IN_FIRST_TRANSITION then if there is a comma in the beginning of the line 
 * we print an error message and call ERROR_IN_FIRST_TRANSITION then we go over the line and we check if there is double commas
 * bettwen arguments if there is we print an error message and call ERROR_IN_FIRST_TRANSITION then we check if there is a number
 * in data defniton because he can only get numbers if not we print an error message and call ERROR_IN_FIRST_TRANSITION then we check
 * if the number is in between the min and max numers that he can get if not we print an error message and call ERROR_IN_FIRST_TRANSITION
 * then we check if there is a missing comma if there is we print an error message and call ERROR_IN_FIRST_TRANSITION 
 * then we check if there is an extrnal text if there is we print an error message and call ERROR_IN_FIRST_TRANSITION
 * and we check all that until the end of the line and in the end we check if the flag commaEnd is TRUE if it is 
 * we print an error message and call ERROR_IN_FIRST_TRANSITION because ther is an unnecessary comma in the end
*/
void data_def(int iLine,  int *DataC , char *line ,file_name fName, word data[]) {
    boolean commaEnd = TRUE; 
    int len; 
    int numVal; 
    line = remove_spaces(line);
    if (line[0] == '\n') { 
        printf("Error in line %d, \".data\" argument did not get an argument!\n", iLine);
        ERROR_IN_FIRST_TRANSITION;
    }
    if (line[0] == ',') { 
        printf("Error in line %d, Comma in the beginning of the line!\n", iLine);
        ERROR_IN_FIRST_TRANSITION;
    }
    while (line[0] != '\n') { 
        line = remove_spaces(line);
        len = (int)strlen(line);
        numVal = (int)strtol(line, &line, 10);

        if (line[0] == ',' && len == strlen(line)) { 
            printf("Error in line %d, double commas between two arguments!\n", iLine);
            ERROR_IN_FIRST_TRANSITION;
        }
        if (line[0] != ' ' && line[0] != '\t' && line[0] != ',' && line[0] != '\n') { 
            printf("Error in line %d, invalid argument \".data\" can get only integers!\n", iLine);
            ERROR_IN_FIRST_TRANSITION;
        }

        if (MIN <= numVal && numVal <= MAX) { 
            data[(*DataC)].val = numVal; 
            (*DataC)++;
        } else { 
            printf("Error in line %d, the number: %d is out of boundaries! the number only can be between -512 to 511.\n", iLine, numVal);
            ERROR_IN_FIRST_TRANSITION;
        }

        line = remove_spaces(line);
        if (line[0] != ',' && line[0] != '\n' && isdigit(line[0])) { 
            printf("Error in line %d, needs to be a comma there but it is missing!\n", iLine);
            ERROR_IN_FIRST_TRANSITION;
        } else if (!isdigit(line[0]) && line[0] != '+' && line[0] != '-' && line[0] != ',' && line[0] != '\n') { 
            printf("Error in line %d, there is an external text in the line!\n", iLine);
            ERROR_IN_FIRST_TRANSITION;
        } else if (line[0] != '\n') { 
            line++;
        } else if (line[0] == '\n') { 
            commaEnd = FALSE; 
        }
    }

    if (commaEnd) { 
        printf("Error in line %d, Comma in the end of the line is unnecessary!\n", iLine);
        ERROR_IN_FIRST_TRANSITION;
    }
}
/*struct_def()
 * first of all we remove the start spaces of the line with the function "remove_spaces()" then we check if there is an argument in the 
 * line if not we print an error message and call ERROR_IN_FIRST_TRANSITION then we check if there is a number in the definition
 * if not we print an error message and call ERROR_IN_FIRST_TRANSITION then we check if the first argument of the struct is an integer
 * if not we print an error message and call ERROR_IN_FIRST_TRANSITION then we check if the number is in between the min and max 
 * numers that he can get if not we print an error message and call ERROR_IN_FIRST_TRANSITION then we check if there is a comma 
 * missing if there is we print an error message and call ERROR_IN_FIRST_TRANSITION and we call the function "string()"
*/
void struct_def(int *DataC, int iLine, char *line, file_name fName, word data[]) {
    int len; 
    int numVal; 
    line = remove_spaces(line);
    if (line[0] == '\n') {
        printf("Error in line %d, \".struct\" argument did not get an argument!\n", iLine);
        ERROR_IN_FIRST_TRANSITION;
    }
    len = (int)strlen(line);
    numVal = (int)strtol(line, &line, 10); 
    if (len == strlen(line)) { 
        printf("Error in line %d, \".struct\"  argument did not get an argument!\n", iLine);
        ERROR_IN_FIRST_TRANSITION;
    }
    if (line[0] != ' ' && line[0] != '\t' && line[0] != ',' && line[0] != '\n') { 
        printf("Error in line %d, inValid argument got! first argument of \"struct\" must be an integer!\n", iLine);
        ERROR_IN_FIRST_TRANSITION;
    }
    if (MIN <= numVal && numVal <= MAX) { 
        data[(*DataC)].val = numVal;
        (*DataC)++;
    } else { 
        printf("Error in line %d, the number %d is out of boundaries! the number only can be between -512 to 511.\n", iLine , numVal);
        ERROR_IN_FIRST_TRANSITION;
    }

    line = remove_spaces(line);
    if (line[0] == '"') {
        printf("Error in line %d, needs to be a comma there but it is missing!!\n", iLine);
        ERROR_IN_FIRST_TRANSITION;
    }
    if (line[0] != ',') {
        printf("Error in line %d, invalid text!\n", iLine);
        ERROR_IN_FIRST_TRANSITION;
    }
    line++; 

    string(DataC , iLine , line , fName, data, TRUE); 
}
/*extern_def()
 * first of all we remove the start spaces of the line with the function "remove_spaces()" then we check if there is an argument in the 
 * line if not we print an error message and call ERROR_IN_FIRST_TRANSITION then we check if the tag "head" name is valid if not
 * we print an error message and call ERROR_IN_FIRST_TRANSITION then we check if there is text after the tag name defnition if 
 * there is we print an error message and call ERROR_IN_FIRST_TRANSITION and after that we call the function "add_tag()"
*/
void extern_def( int iLine, char *line, file_name fName ,tag *head) {
    char tName[MAX_LINE_LENGTH]; 
    memset(tName, '\0', MAX_LINE_LENGTH); 

    line = remove_spaces(line);
    if (line[0] == '\n') { 
        printf("Error in line %d, \".extern\" must get 1 argument, actually got 0!\n", iLine);
        ERROR_IN_FIRST_TRANSITION;
    }

    line = line_content(line, tName);
    if (!macro_name_valid(tName)) { 
        printf("Error in line %d, the tag \"%s\" has invalid name!\n", iLine , tName);
        ERROR_IN_FIRST_TRANSITION;
    }

    line = remove_spaces(line);
    if (line[0] != '\n') { 
        printf("Error in line %d, text after tag name!\n", iLine);
        ERROR_IN_FIRST_TRANSITION;
    }

    addTag(iLine, 0 , tName , FALSE, TRUE, FALSE, FALSE, fName , head);
}
/*one_operand()
 * first of all, we check if there is a comma at the beginning of the line if there is we print an error message and call ERROR_IN_FIRST_TRANSITION
 * then, we fill the content with operand with "line_content()" function then we check if the following word after the word that we put in "op" is
 * beginning with a comma if it does we print an error message and call ERROR_IN_FIRST_TRANSITION, then we check if there is
 * an external text in the line if it is we print an error message and call ERROR_IN_FIRST_TRANSITION, then we check
 * if we got an operand if not we print an error message and call ERROR_IN_FIRST_TRANSITION, then we check if the operand is
 * immediate addressing if it is we extract the value of the number to "numVal" then we check if the argument is not an integer number
 * if it is not we print an error message and call ERROR_IN_FIRST_TRANSITION, then we check if
 * the number is out of boundaries of -128 to 127 if it is we print an error message and call ERROR_IN_FIRST_TRANSITION,
 * then we check if the command can get an immediate addressing operand if it can, we create a word in the instruction
 * that stores the value of the number, and we fill out all the other instruction fields else we print an error message
 * and call ERROR_IN_FIRST_TRANSITION, but if it is not immediate addressing we check if the operand is a direct register addressing
 * if it is, we check if the command can get a direct register addressing operand if not we print an error message and call
 * ERROR_IN_FIRST_TRANSITION, but if it can, we check if the previous word is a register addressing if it is
 * we add the value of the original opcode word to the direct register addressing and merge to the previous word the value of the current
 * register else we add to the value of the original opcode word the direct register addressing, and we fill the instruction data
 * according to the data but if it is not a direct register addressing either we check if the operand is access to struct addressing
 * then we check if the command can get access to struct addressing, if not we print an error message and call
 * ERROR_IN_FIRST_TRANSITION, but if it can, we fill the instruction data according to the data and check if the tag name is valid
 * if not we print an error message and call ERROR_IN_FIRST_TRANSITION, then if the field that is written is 1 or 2
 * and if there is more text in the field than written or there is an external text in the operand we print an error
 * message and call ERROR_IN_FIRST_TRANSITION and if it is not direct register addressing then we check for direct addressing and
 * fill the instruction data according to the data and check if the tag name is valid, if not we print an error message and
 * call ERROR_IN_FIRST_TRANSITION if it is not direct addressing either, we print an error message and call ERROR_IN_FIRST_TRANSITION.
*/
void one_operand(int opcode,int iLine,  int *InstC ,  int shift ,  char *line , instruction *opCodeOrigin, instruction inst[], file_name fName) {
    int numVal; 
    int reg; 
    char *textAfterDot; 
    char op[MAX_LINE_LENGTH]; 
    char *lineContent; 


    if (line[0] == ',') {
        printf("Error in line %d, Comma in the beginning of the line!\n", iLine);
        ERROR_IN_FIRST_TRANSITION;
    }

    memset(op, '\0', MAX_LINE_LENGTH);
    line = line_content(line, op); 

    line = remove_spaces(line);
    if (line[0] == ',') {
        printf("Error in line %d, misplaced comma!\n", iLine);
        ERROR_IN_FIRST_TRANSITION;
    }
    if (line[0] != '\n') {
        printf("Error in line %d, there is an external text in the line!\n", iLine);
        ERROR_IN_FIRST_TRANSITION;
    }
    if (strlen(op) == 0) {
        printf("Error in line %d, \"%s\" has to get one operand but in reality got 0!\n", iLine , opcode_names()[opcode]);
        ERROR_IN_FIRST_TRANSITION;
    }

    if (op[0] == '#') { 
        numVal = (int)strtol(op + 1, &lineContent, 10);

        lineContent = remove_spaces(lineContent);
        if (strlen(lineContent) != 0) { 
            printf("Error in line %d, immediate addressing has to be an integer!\n", iLine);
            ERROR_IN_FIRST_TRANSITION;
        }
        if (!(ADDRESSING_MIN <= numVal && numVal <= ADDRESSING_MAX)) { 
            printf("Error in line %d, the number is out of boundaries! the number only can be between -128 to 127!\n", iLine);
            ERROR_IN_FIRST_TRANSITION;
        }

        if ((shift == SOURCE_SHIFT && (0 <= opcode && opcode <= 3)) || (shift == TARGET_SHIFT && (opcode == 1 || opcode == 12))) { 
            inst[(*InstC)].val = numVal << 2; 
            inst[(*InstC)].is_register = FALSE;
            (inst[(*InstC)].tName)[0] = '\0';
            inst[(*InstC)].iLine = iLine;
            (*InstC)++;
        } else { 
            printf("Error in line %d, invalid immediate addressing!\n", iLine);
            ERROR_IN_FIRST_TRANSITION;
        }
    } else if ((reg = register_num(op)) != NOT_REGISTER) { 
        if ((shift == SOURCE_SHIFT && (0 <= opcode && opcode <= 3)) || (shift == TARGET_SHIFT && (0 <= opcode && opcode <= 13))) {
            if (inst[(*InstC)-1].is_register) { 
                opCodeOrigin->val |= (DIRECT_REGISTER << shift);
                inst[(*InstC)-1].val |= (reg << 2); 
            } else {
                opCodeOrigin->val |= (DIRECT_REGISTER << shift); 
                if (shift == SOURCE_SHIFT) { 
                    inst[(*InstC)].val = reg << 6;
                } else {
                    inst[(*InstC)].val = reg << 2;
                }
                inst[(*InstC)].is_register = TRUE;
                (inst[(*InstC)].tName)[0] = '\0';
                inst[(*InstC)].iLine = iLine;
                (*InstC)++;
            }
        } else { 
            printf("Error in line %d, invalid direct register addressing!\n", iLine);
            ERROR_IN_FIRST_TRANSITION;
        }
    } else if (op[strlen(op)-2] == '.') { 
        textAfterDot = &op[strlen(op)-1]; 
        op[strlen(op)-2] = '\0'; 

        if ((shift == SOURCE_SHIFT && ((0 <= opcode && opcode <= 3) || opcode == 6)) || (shift == TARGET_SHIFT && (0 <= opcode && opcode <= 13))) { /* if the command can get an access to struct addressing op */
            opCodeOrigin->val |= (STRUCT_ACCESS << shift);
            inst[(*InstC)].val = 0;
            inst[(*InstC)].is_register = FALSE;
            strcpy(inst[(*InstC)].tName, op);
            inst[(*InstC)].iLine = iLine;
            (*InstC)++;
            if (!macro_name_valid(op)) { 
                printf("Error in line %d, operand is inValid name of tag!\n", iLine);
                ERROR_IN_FIRST_TRANSITION;
            }
        } else { 
            printf("Error in line %d, invalid access to struct addressing!\n", iLine);
            ERROR_IN_FIRST_TRANSITION;
        }

        if (textAfterDot[0] == '1' || textAfterDot[0] == '2') { 
            inst[(*InstC)].val = (textAfterDot[0] == '1'? 1 : 2) << 2;
            inst[(*InstC)].is_register = FALSE;
            (inst[(*InstC)].tName)[0] = '\0';
            inst[(*InstC)].iLine = iLine;
            (*InstC)++;
            textAfterDot++; 
            if (textAfterDot[0] != '\0' && textAfterDot[0] != '\t' && textAfterDot[0] != ' ') { 
                printf("Error in line %d, inValid number have to be 1 or 2 \n", iLine);
                ERROR_IN_FIRST_TRANSITION;
            }

            textAfterDot = remove_spaces(textAfterDot);
            if (textAfterDot[0] != '\0') {
                printf("Error in line %d, there is an external text in the operand!\n", iLine);
                ERROR_IN_FIRST_TRANSITION;
            }
        } else {
            printf("Error in line %d, inValid number have to be 1 or 2 \n", iLine);
            ERROR_IN_FIRST_TRANSITION;
        }
    } else if ((shift == SOURCE_SHIFT && ((0 <= opcode && opcode <= 3) || opcode == 6)) || (shift == TARGET_SHIFT && (0 <= opcode && opcode <= 13))) { /* if the op is a direct addressing */
        opCodeOrigin->val |= (DIRECT_ADDRESSING << shift);
        inst[(*InstC)].val = 0;
        inst[(*InstC)].is_register = FALSE;
        strcpy(inst[(*InstC)].tName, op);
        inst[(*InstC)].iLine = iLine;
        (*InstC)++;
        if (!macro_name_valid(op)) { 
            printf("Error in line %d, operand is inValid name of tag!\n", iLine);
            ERROR_IN_FIRST_TRANSITION;
        }
    } else { 
        printf("Error in line %d, \"%s\" is illegal operand for the command \"%s\"!\n", iLine, op, opcode_names()[opcode]);
        ERROR_IN_FIRST_TRANSITION;
    }
}
/*two_operands()
 * first of all, we check if we got any operands at all if not we print an error message and call ERROR_IN_FIRST_TRANSITION 
 * then we check if there is a comma in the beginning of the line if there is we print an error message and call
 * ERROR_IN_FIRST_TRANSITION then we fill lineWithSpace with line and add space before and after the comma in it
 * and if there is an missplaced comma we print an error message and call ERROR_IN_FIRST_TRANSITION then we handle the first operand
 * with the "one_operand()" funtion then we check if we got only one operand if we do we print an error message and call 
 * ERROR_IN_FIRST_TRANSITION then we handle the second operand  and then we check if there is an external text in the operand 
 * if there is we print an error message and call ERROR_IN_FIRST_TRANSITION 
*/
void two_operands(int opcode,int iLine,  int *InstC ,  char *line , instruction *opCodeOrigin, instruction inst[], file_name fName) {
    int content_len; 
    int i = 0;
    boolean there_is_comma = FALSE; 
    char lineWithSpace[MAX_LINE_LENGTH + 3]; 
    char *lineWithSpace_ptr; 
    char content[MAX_LINE_LENGTH]; 

    memset(lineWithSpace, '\0', MAX_LINE_LENGTH + 3);
    memset(content, '\0', MAX_LINE_LENGTH);

    if (line[0] == '\n') {
        printf("Error in line %d, \"%s\" needs to get two operands, but got none\n", iLine , opcode_names()[opcode]);
        ERROR_IN_FIRST_TRANSITION;
    }
    if (line[0] == ',') {
        printf("Error in line %d, Comma in the beginning of the line!\n", iLine);
        ERROR_IN_FIRST_TRANSITION;
    }

    while (i < strlen(line)) {
        if (line[i] == ',') {
            if (there_is_comma) {
                printf("Error in line %d, misplaced comma!!\n", iLine);
                ERROR_IN_FIRST_TRANSITION;
            }
            there_is_comma = TRUE;
            lineWithSpace[i] = ' '; lineWithSpace[i+1] = ','; lineWithSpace[i+2] = ' ';
            i++;
            if (line[i] == ',') {
                printf("Error in line %d,  misplaced comma!\n", iLine);
                ERROR_IN_FIRST_TRANSITION;
            }
        }
        lineWithSpace[there_is_comma? i+2 : i] = line[i];
        i++;
    }
    if (!there_is_comma) {
        printf("Error in line %d, missing comma!\n", iLine);
        ERROR_IN_FIRST_TRANSITION;
    }

    lineWithSpace_ptr = lineWithSpace; 

    lineWithSpace_ptr = line_content(lineWithSpace_ptr, content); 
    content_len = (int)strlen(content);
    content[content_len] = '\n'; content[content_len+1] = '\0'; 

    one_operand( opcode, iLine,InstC,SOURCE_SHIFT,content ,opCodeOrigin,inst,fName );
    if (error_in_first_transition(FALSE, FALSE)) {
        return;
    }

    lineWithSpace_ptr = remove_spaces(lineWithSpace_ptr); 
    lineWithSpace_ptr++; 
    lineWithSpace_ptr = remove_spaces(lineWithSpace_ptr); 
    if (lineWithSpace_ptr[0] == '\n') {
        printf("Error in line %d, \"%s\" needs to get two operands, but got only one\n", iLine , opcode_names()[opcode]);
        ERROR_IN_FIRST_TRANSITION;
    }

    memset(content, '\0', MAX_LINE_LENGTH);
    lineWithSpace_ptr = line_content(lineWithSpace_ptr, content); 
    content_len = (int)strlen(content);
    content[content_len] = '\n'; content[content_len+1] = '\0'; 
    

    one_operand(opcode, iLine,InstC, TARGET_SHIFT,content ,opCodeOrigin,inst,fName);
    if (error_in_first_transition(FALSE, FALSE)) {
        return;
    }

    lineWithSpace_ptr = remove_spaces(lineWithSpace_ptr);
    if (lineWithSpace_ptr[0] != '\n') {
        printf("Error in line %d, there is an external text in the operand!\n", iLine);
        ERROR_IN_FIRST_TRANSITION;
    }
}
/*error_in_first_transition()
 * if reset required turn error_in_first_transition off and if the function called because of an error 
 * turn error_in_first_transition on and then return rror_in_first_transition
*/
boolean error_in_first_transition(boolean error, boolean reset) {
    static boolean error_in_first_transition = FALSE; /* store the status of the compiling process of the first pass */

    if (reset) /* if reset required */
        error_in_first_transition = FALSE; /* reset to the initial value */

    if (error) /* if the function called because error */
        error_in_first_transition = TRUE; /* change the status to that there is an error */

    return error_in_first_transition; /* return the status of the compiling process of the first pass */
}

