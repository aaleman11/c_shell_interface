//
//  main.c
//  shellint
//
//  Created by Phillip Gulegin on 2/23/17.
//  Copyright © 2017 Phillip Gulegin. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


// maximum length of a command
#define MAXLINE 80



#define TOKEN_BUFFER_SIZE 80
#define TOKEN_DELIMITER "\n"

char **arg_parser(char *input)
{
    int bufferSize = TOKEN_BUFFER_SIZE, position = 0;

    char **args = malloc(bufferSize * sizeof(char*));
    char *arg;

    arg = strtok(input, TOKEN_DELIMITER);
    while (arg != NULL) {
        args[position] = arg;
        position++;

        if (position >= bufferSize) {
            bufferSize += TOKEN_BUFFER_SIZE;
            args = realloc(arg, bufferSize * sizeof(char*));
        }

        arg = strtok(NULL, TOKEN_DELIMITER);
    }
    args[position] = NULL;
    return args;
}


// checks if input contains &
int containsAmp(char input[]) {
    
    int index = 0;
    while (input[index] != '\n') {
        if (input[index] == '&') {
            return 1;
        }
        index++;
    }
    
    return 0;
}



/*****************************************************
//Parse user input

int analyzeInput(char input[], char *args[]) {
    
    int index = -1;
    int count = 0;
    
    unsigned long length = strlen(input);
    
//    printf("%lu", length);
    
    for(int i = 0; i < length; i++) {
        
        switch(input[i]){
            case ' ':
                break;
            case '\n':
                if(index != -1){
                    args[count] = &input[index];
                    count++;
                }
                input[i] = '\0';
//                args[count] = NULL;
                break;
            default:
//                if(index == -1){
                    index = i;
//                }
                
//                if(input[i] == '&'){
//                    input[i] = '\0';
//                }
        }
    }
    
    
    
    return 0;
    
}
************************************************************/



int main(void) {
    
    char input[MAXLINE];            // inital input
    char **args;                    // command line arguments
    int should_run = 1;             // flag which quits the program
    pid_t pid = 0;                  // process id
    
    while (should_run) {
        
        // prints the input command
        printf("osh>");
        
        // flushes all output
        fflush(stdout);
        
        // steps from CSNS
        /** * After reading user input, the steps are:
         
         * (1) fork a child process using fork()
         
         * (2) the child process will invoke execvp()
         
         * (3) if command included &, parent will invoke wait()
         
         */


        // grabs in the input and the length of the input
        fgets(input, MAXLINE, stdin);


        /**************************
        // analyze the input first
        analyzeInput(input, args);

         ***********************/

        args = arg_parser(input);


        // create a new child process
        pid = fork();
        
        
        // true: then we want child and parent to execute
        // false: then we want the parent to wait for the child to execute
        if (containsAmp(input)) {
            
            // executes if we're the child process
            if (pid == 0) {
                printf("1\n");
                execvp(args[0], args);
                exit(0);
            }
            // executes if we're the parent process
            else if (pid > 0) {
                printf("2\n");
            }
            // executes if we're the parent process, but child couldn't be created
            else {
                printf("3\n");
            }
        }
        else {
            
            // executes if we're the child process
            if (pid == 0) {
                printf("4\n");
                execvp(args[0], args);
                exit(0);
            }
            // executes if we're the parent process
            else if (pid > 0) {
                wait(NULL);
                printf("5\n");
            }
            // executes if we're the parent process, but child couldn't be created
            else {
                printf("6\n");
            }
        }
    }
    
    return 0;
}

