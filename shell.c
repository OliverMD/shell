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
#include "misc.h"
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
