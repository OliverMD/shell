#ifndef SHELL_H
#define SHELL_H

int parse_command(char* command);
void get_parameters(char* input, char** parameters, int bytes_read);
int main();
int execute_program(char** parameters);

#endif
