#include "assembler.h"
int main( int argc , char *argv[]) {
    extractFilesFromLine(argv, argc); 
    return EXIT_SUCCESS;
}
/*free_memory()
 * We go over the linked list of tags and free every tag memory then we close the file afterMacSpreading
 * and free InstC and DataC memory and fName
*/
void free_memory( int *InstC, int *DataC , FILE *afterMacSpreading , file_name *fName, tag *head) {
    tag *curr;
    while (head != NULL) {
        curr = head;
        head = head->next;
        free(curr);
    }

    fclose(afterMacSpreading);
    free(fName->name);
    free(InstC);
    free(DataC);
}
/*extractFilesFromLine()
 * first of all we check if no arguments received if not print an error and EXIT_FAILURE we go across
 * the given files that we received then we add to the file name the ".as" extension and we open the ".as"
 * file if the file is null we print an error message then we compile the ".as" file with the "compile()" function 
 * then we call RESET_ERROR_FUNCTIONS.
*/
void extractFilesFromLine(char *argV[] , int argC) {
    int i;
    FILE *as; 
    file_name fName; 

    if (argC <= 1) {
        printf("Did not got arguments!\n");
        exit(EXIT_FAILURE);
    }

    for (i = 1; i < argC; i++) {
        fName.len = (int)strlen(argV[i]) + 3;
        fName.name = (char *)malloc((fName.len + 1) * sizeof(char));

        MEMORY_ERROR(fName.name) 
        memset(fName.name, '\0', fName.len + 1); 
        strcpy(fName.name, argV[i]); 
        fName.name[fName.len-3] = '.'; fName.name[fName.len-2] = 'a'; fName.name[fName.len-1] = 's'; 

        as = fopen(fName.name, "r");
        if (as == NULL) {
            printf("Couldn't find \"%s\" file!\n\n", fName.name);
            free(fName.name);
            continue;
        }

        compile(as, &fName);

        RESET_ERROR_FUNCTIONS
    }
}
/*compile()
 * if the file didn't went through the macro spreading process successfully we change the extension to ".as" and print an 
 * error messages then change the extension to ".am" and delete the ".am" file then free the file then we open the 
 * ".am" file in read mode and create the linked list of tags then we create the InstC and DataC and initialize them to 0
 * and run the first transition process and if there was an error we print an error messages and free the memory of the 
 * program then we run the second transition and check if error occurred in the process if tere were we print an error
 * messages and free the memory of the program if the file went through all the processes and compield successfully
 * we the free the allocated memory of the program and we print success message.
*/
void compile(FILE *as, file_name *fName) {
    word data[BUFFER]; 
    instruction inst[BUFFER]; 
    int *InstC; 
    int *DataC; 
    tag *head; 
    FILE *afterMacSpreading; 

    printf("Compiling \"%s\"...\n", fName->name);

    if (macroSpreading(as, *fName)) {
        fName->name[fName->len-1] = AS; 
        printf("File \"%s\" has error/s, can not create \".am\" file!\n",fName->name);
        printf("Failed to compilie the file \"%s\".\n\n",fName->name);
        fName->name[fName->len-1] = AM; 
        remove(fName->name); 
        free(fName->name);
        return;
    }

    afterMacSpreading = fopen(fName->name, "r"); 

    head = (tag *)malloc(sizeof(tag));
    MEMORY_ERROR(head)
    head->name[0] = '\0';
    head->val = 0;
    head->data = head->is_extern = head->entry = head->opCode = FALSE;
    head->next = NULL;

    InstC = (int *)malloc(sizeof(int));
    MEMORY_ERROR(InstC)
    DataC = (int *)malloc(sizeof(int));
    MEMORY_ERROR(DataC)
    (*InstC) = (*DataC) = 0;

    if (firstTransition(InstC, DataC , afterMacSpreading, *fName, inst, data, head)) {
        printf("File \"%s\" has error/s in first transition, can not create \".ob\", \".ent\", \".ext\"!\n",fName->name);
        printf("Failed to compilie the file \"%s\".\n\n",fName->name);
        free_memory(InstC, DataC, afterMacSpreading , fName , head); 
        return;
    }

    if (secondTransition(InstC, DataC,afterMacSpreading, *fName, head, inst, data)) {
        printf("File \"%s\" has error/s in second transition, can not create \".ob\", \".ent\", \".ext\"!\n",fName->name);
        printf("Failed to compilie the file \"%s\".\n\n",fName->name);
        free_memory(InstC, DataC, afterMacSpreading , fName , head); 
        return;
    }

    printf("File compiled successfully!\n\n");
    free_memory(InstC, DataC, afterMacSpreading , fName , head);
}

