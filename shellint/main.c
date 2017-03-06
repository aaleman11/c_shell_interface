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
#define MAXLINE 80
#define TOKEN_BUFFER_SIZE 80
#define TOKEN_DELIMITER "\n"
// keeps track of the last 10 history commands
char history[10][MAXLINE];
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
void addHistory(char *input) {
    
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
// checks if command is a history command
// return 1:  if command is: '!!'
// return 0:  command is: '!N'
// return -1: command is: 'history'
// return -2: command is: 'exit'
// return 99: history is not the command
/* int containsHistOrExit(char input[]) { */
    
/*     if (input[0] == '!' && input[1] == '!') { */
/*         return 1; */
/*     } */
/*     else if (input[0] == '!' && (input[1] >= 48 && input[1] <= 57)) { */
/*         return 0; */
/*     } */
/*     // I know, I know... Lol. */
/*     else if (input[0] == 'h' && */
/*              input[1] == 'i' && */
/*              input[2] == 's' && */
/*              input[3] == 't' && */
/*              input[4] == 'o' && */
/*              input[5] == 'r' && */
/*              input[6] == 'y') { */
/*         return -1; */
/*     } */
/*     else if (input[0] == 'e' && */
/*              input[1] == 'x' && */
/*              input[2] == 'i' && */
/*              input[3] == 't' ) { */
/*         return -2; */
/*     } */
/*     else { */
/*         return 99; */
/*     } */
/* } */
// removes '&' from input
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
    
    
    
    //printf("%s", str);
    
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
        
        // if the command is a history command -- 3 scenarios
        /* switch (containsHistOrExit(input)) { */
                
        /*         // executes if command is: '!!' -> execute most recent command */
        /*     case 1: */
                
        /*         break; */
                
        /*         // executes if command is: '!N' -> execute Nth command */
        /*     case 0: */
                
        /*         break; */
                
        /*         // executes if command is: 'history' -> print recent history to console */
        /*     case -1: */
                
        /*         // return to the beginning of a while loop */
        /*         continue; */
                
        /*         // execute if command is exit */
        /*     case -2: */
        /*         should_run = 0; */
        /*         continue; */
        /* } */
        
        
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
	if(input[strlen(input) - 1] == '\n')
	  input[strlen(input) -1] = '\0';

	if(strcmp(input, "exit") == 0)
	  break;
    }
    
    return 0;
}
