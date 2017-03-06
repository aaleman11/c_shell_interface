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

void history(char **hist, int current){
    //    int i = current;
    //    int histNum = 1;
    
    //char *histA[MAXLINE];
    //    free(histA);
    
    //    for(int temp = 0; temp < MAXLINE; temp++){
    //      strcpy(histA, hist[1]);
    //    }
    //    while(1){
    //        if(&hist){
    //            printf("%d %s\n", histNum, &hist);
    //            histNum++;
    //        }
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

// removes '&' from input
// !!!!!!!!!!!!!!!!!!! this does not work correctly yet for some reason
void removeAmp(char* str) {
    
    //int currArrLen = sizeof(str) / sizeof(str[0]);
    unsigned long len = strlen(str);
    int indexToDelete;
    int i = 0;
    
    for(i = 0; i < len; i++) {
        if(str[i] == '&') {
            indexToDelete = i;
            break;
        }
    }
    
    memmove(&str[indexToDelete], &str[indexToDelete + 1], strlen(str) - indexToDelete);
    
    printf("%s", str);

}

// main function
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
        
        // gets the input
        fgets(input, MAXLINE, stdin);
        
        // create a new child process
        pid = fork();
        
        // true: then we want child and parent to execute
        // false: then we want the parent to wait for the child to execute
        if (containsAmp(input)) {
            
            // removes the '&' from the input
            removeAmp(input);
            
            // returns a set of arguments
            args = arg_parser(input);
        
            // executes if we're the child process
            if (pid == 0) {
//                execvp(args[0], args);
                if (execvp(args[0], args) == -1)
                {
                    printf("Command not executed correctly.\n");
                    exit(0);
                }

            }
            // executes if we're the parent process
            else if (pid > 0) {
                // do nothing with the parent because we want them to run together
            }
            // executes if we're the parent process, but child couldn't be created
            else {
                printf("Something went wrong.\n");
            }
        }
        else {
            
            // returns a set of arguments
            args = arg_parser(input);
        
            // executes if we're the child process
            if (pid == 0) {
                execvp(args[0], args);
            }
            // executes if we're the parent process
            else if (pid > 0) {
                wait(NULL);
            }
            // executes if we're the parent process, but child couldn't be created
            else {
                printf("Something went wrong.\n");
            }
        }
    }
    
    return 0;
}

