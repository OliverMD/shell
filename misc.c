#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "misc.h"

int parse_command(char* command){
    //Each command can be differentiated by it's first letter so this is all
    //we have to check.
    switch(command[0]){
    case 't': return 0;
    case 'i': return 1;
    case 'd': return 2;
    case 'c': return 3;
    case 'e': return 4;
    case 'q': return 5;
    default: return -1;
    }
}

void get_paths(char* input, char** paths){
    int i = 0;
    int k = 0;
    int input_len = strlen(input);
    while(input[i] != '\0' && i < input_len){
        int start = i;
        int end;
        for(end = start +1; input[end] != ':' && input[end] != '\0'; end++);
        char* path = (char*) malloc(64*sizeof(char));
        int j;
        for(j = 0; j < end - start; j++){
            path[j] = input[start + j];
        }
        path[j] = '\0';

        paths[k] = path;
        k++;
        i = end+1;
    }
    paths[k] = NULL;
}

void get_parameters(char* input, char** parameters, int bytes_read){
    int i = 0;
    int k = 0;
    //char** parameters = (char**) calloc(8, sizeof(char*));
    //Loops over the input string, when it hits a " or a space it
    //adds a new word to parameters.
    while(input[i] != '\0' && i < bytes_read){
        int start = i;
        int end;
        if(input[i] == '"'){
            for(end=start+1; input[end] != '"'; end++);
            start++;
        }else{
            for(end = start; input[end] != ' '
                       && input[end] != '\n'
                    && input[end] != '\0'; end++);
        }

        int j;
        char* param = (char*) calloc(32, sizeof(char));

        for(j=0; j < end-start; j++){
            param[j] = input[start+j];
        }
        param[j] = '\0';

        parameters[k] = param;
        k++;
        if(input[end] == '\0') break;
        i = end + 1;
    }
    parameters[k] = NULL;
}
