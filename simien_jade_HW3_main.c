/**************************************************************
* Description: A C program that reads and parses lines of user
*              buffer from the command line and executes commands
*              through the usage of fork() and execvp(). The parent
*              waits for the child process to complete before
*              displaying the child process ID as well as the return
*              result from the child process. Through the executable
*              path given, the user is able to provide commands to
*              execute.
**************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char *argv[]){

    /** The buffer is intended to hold up to 180 bytes of characters. Space is allocated to
     *  both the input buffer and the prompt character pointers. Error messages are provided
     *  if the allocation has any issues.
     */
    char *buffer;
    size_t bufferSize = 180;
    buffer = (char *)malloc(bufferSize * sizeof(char));
    char *prompt = (char *)malloc(bufferSize * sizeof(char));

    if(buffer == NULL){
        perror("Buffer allocation error.");
        exit(EXIT_FAILURE);
    }
    if(prompt == NULL){
        perror("Prompt allocation error.");
        exit(EXIT_FAILURE);
    }
    
    /** Editing RUNOPTIONS/Command Line allows personalization of the prompt however, if the 
     * first argument of the argument vector is NULL, a default prompt will be provided.
     */
    if(argv[1] == NULL){
        strcpy(prompt, "> ");
    } else {
        strcpy(prompt, argv[1]);
    }

    while(1){
        printf("%s", prompt);


        /** If-statement handles both fetching user input and checking if there's an error 
         *  reading it. 
         * */
        if(fgets(buffer, bufferSize, stdin) == NULL){
            printf("Error while reading a line of input.");
            exit(1);
        }else{
            printf("%s", buffer);
        }

        /** Pressing 'enter" results in a newline at the end of user input. So to ensure this
         *  space is not counted in the impending parsing, the white space is removed.
         */
        buffer[strcspn(buffer, "\n")] = 0;
        
        /** Before a non-null buffer is parsed, it is checked for the "exit" argument so that
         *  the shell can terminate or if the buffer is blank.
         */
        if(buffer != NULL){
            if(strcmp(buffer, "exit") == 0){
                // printf("exiting\n");
                exit(EXIT_SUCCESS);
            }
            /** If the user still doesn't provide an argument for the current iteration, the
             *  continue keyword can start the next iteration and provide the error message
             * accordingly.
             */
            if((strcmp(buffer, "") == 0)){
                printf("Provide an argument\n");
                continue;
            } 
        } else {
            exit(EXIT_SUCCESS);
        }

        /** Since the maximum number of characters the user can provide is 180, the maximum
         *  amount of characters between spaces is half of that; 90. The execvp() function
         *  needs an array passed to have a NULL pointer at the end hence, an extra space was allocated
         *  to the arg character array to have a total size of 91.
         *  An index is created to increment with every space token parsed since the position
         *  of where the NULL pointer should be depends on the amount of substrings found in
         *  the input buffer.
         */
        char *arg[(bufferSize/2) + 1];
        int i = 0;
        arg[0] = strtok(buffer, " ");
        while(arg[i]){
            // printf("%d    %s\n", i, arg[i]);
            i++;
            arg[i] = strtok(NULL, " ");
        }
        arg[i] = NULL;

        /** Using the fork() function, a new process can be created and depending on the integer
         *  value returned by the function, the shell can either exit in failure or proceed with
         *  calling execvp().
         */
        int newProcess = fork();
        int cpid;
        int result;
        int status;
        
        if(newProcess == -1){
            printf("Error");
            free(buffer);
            exit(EXIT_FAILURE);
        } else if(newProcess == 0){
            /** We're in a child process if the fork is successful. */
            result = execvp(arg[0], arg); 
            if(result == -1){
                printf("Execvp() error.\n");
                exit(EXIT_FAILURE);
            }               
        }
        /** Outside of the if-statements, we're running in the parent process. Using wait() gets
         *  information of the child process.
         */
        cpid = wait(&status);
        printf("Child %d, exited with %d\n", cpid, WEXITSTATUS(status));
        
    }
    exit(EXIT_SUCCESS);
}
