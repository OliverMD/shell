#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "shell.h"
#include "commands.h"
/*
 * USEFUL INFORMATION
 * ==================
 * Standard Input => 0
 * Standard Output => 1
 * Standard Error => 2
 *
 * SHELL COMMANDS
 * ==============
 * touch name : Creates a file in the current directory with given name
 * insert name text : Pipes text in to a file given by name
 * del name : Deletes file given by name
 * cat name : Prints the content of file given by name.
 */

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

int execute_program(char** parameters){
	int id;
	int return_value = 1;
	char* path = getenv("PATH");
	char** paths = malloc(12 * sizeof(char*));
    get_paths(path, paths);
	char* env[] = {NULL};
	int i = 0;

	id = fork();
	if(id  ==-1){
		perror("fork");
	}
	if(id == 0){
		while(paths[i] != NULL){
			char* full_path = paths[i];
			strcat(full_path, "/");
			strcat(full_path, parameters[1]);
		    //Child process
		     id = execve(full_path, &(parameters[1]), env);
		     if (id != -1){
				 break;
			 }
			 i++;
	    }
		if (id == -1){
			perror("Command Not Found");
		    //exit(1);
		}
	}
	wait(&return_value);
	//free(path);
	// TODO: Free all paths in paths
	//free(paths);
	return return_value;
}

int main(){
	//Do potentially expensive memory operations during initialization phase
	char* input_buffer = (char *) calloc(64, sizeof(char));
	int bytes_read;
	write(1, "$> ", 3);

	while(1){
		bytes_read = read(0, input_buffer, 64);
		char** parameters = (char**) calloc(8, sizeof(char*));
		get_parameters(input_buffer, parameters, bytes_read);

		switch(parse_command(parameters[0])){
		case 0:
				touch(parameters);
				break;
		case 1:
				insert(parameters);
				break;
		case 2:
				del(parameters);
				break;
		case 3:
			    cat(parameters);
				break;
		case 4:
			execute_program(parameters);
			break;
		case 5:
			free(input_buffer);
			free(parameters);
			exit(0);
		default:
				write(1, "Command not recognised\n",24);
				break;
		}

		free(parameters);
		write(1, "$> ", 3);
	}
	exit(0);
}
