#include "commands.h"
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>



void touch(char** parameters){
        int fd = creat(parameters[1], 0644);
        if(fd < 0){
                perror("Touch");
        }
        close(fd);
}

void insert(char** parameters){
        int fd = open(parameters[1], O_WRONLY | O_APPEND);
        if (fd < 0){
                perror("Pipe");
        } else {
                write(fd, parameters[2], 32);
        }
        close(fd);
}

void del(char** parameters){
        unlink(parameters[1]);
}

void cat(char** parameters){
        int fd = open(parameters[1], O_RDONLY);
        if (fd < 0){
                perror("Cat");
        } else {
                char* file_data = (char*)malloc(32 * sizeof(char));
                int bytes_read = read(fd, file_data, 32);
                write(1, file_data, bytes_read);
                while(bytes_read >= 32){
                        bytes_read = read(fd, file_data, 32);
                        write(1, file_data, bytes_read);
                }
                write(1, "\n", 1);
                free(file_data);
        }
        close(fd);
}
