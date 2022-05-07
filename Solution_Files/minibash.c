#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stddef.h>
#include <sys/types.h>

#define PRMTSIZ 255
#define MAXARGS 63
#define EXITCMD "exit"

char singleCommands[21][100];
int j=0;
int y=0;
char redirection[21][100];




void execute_command(char* command) {
    int i = 0;
    int argument_count = 0;    
        
    /* Strip white spaces */
    while (command[i] == ' ') {
        i++;
    }
    command = command + i;
    
    i = 0;
        
    /* Count the number of arguments to the command */
    while (command[i] != '\0') {
         if (command[i] == ' ')
             argument_count++;
         i++;
     }
     
    char** argv = calloc(argument_count + 2, sizeof(char*));
    char* argument = NULL;
    i = 0;        
    while ((argument = strsep(&command, " ")) != NULL) {
       if (strlen(argument) != 0) {
           argv[i] = calloc(strlen(argument) + 1, sizeof(char));
           strncpy(argv[i], argument, strlen(argument));
       }
       i++; 
    }
    argv[i] = NULL;
            
    if (execvp(argv[0], argv) != 0) {
        fprintf(stderr, "Error creating pipe. %s", strerror(errno));
    }
}

int handleString(char* commandInput){
    //
	int init_size = strlen(commandInput);
	char delim[] = ";";

	char *ptr = strtok(commandInput, delim);
    int flag=0;
    int l=0;
	while(ptr != NULL)
	{
        l=0;
        for(int i=0;i<strlen(ptr);i++)
        {
            singleCommands[j][l] = ptr[i];
              ++l;
        }
        singleCommands[j][l] = '\0';
        // printf("%s\n",singleCommands[j]);
        // printf("Hello\n");
        j=j+1;
		ptr = strtok(NULL, delim);
	}
    

	return 0;
}

void pipeimple(char* argv)
    {
    int* fd = calloc(2, sizeof(int));      
    if (pipe(fd) != 0) {
        printf ("Error creating pipe. %s", strerror(errno));
        exit(errno);
    }
    char* command = argv;
    int prev_commands_length = 0;
    int i = 0;
    int quote_begin = 0;
    while (1) {
        if (command[i] == '|') {
            int pid = fork();
            if (pid == -1) {
                printf("Error creating pipe. %s", strerror(errno));
                    exit(errno);
             } else if (pid > 0) {
                  dup2(fd[1], 1);
                  close(fd[0]);
                  close(fd[1]);        
                  char* current_command = calloc(i + 1 - prev_commands_length, sizeof(char));
                  strncpy(current_command, command + prev_commands_length, i - prev_commands_length);
                  execute_command(current_command);
              } else  {
                  dup2(fd[0], 0);
                  close(fd[1]);
                  close(fd[0]);
                   fd = calloc(2, sizeof(int));
                   pipe(fd);
              }
                    prev_commands_length = i + 1;
          } else if (command[i] == '\0') {
              char* current_command = calloc(i + 1 - prev_commands_length, sizeof(char));
              strncpy(current_command, command + prev_commands_length, i - prev_commands_length);
              execute_command(current_command);
          }
          i++;                
     }
 }


int main(int argc, char *argv[]){
    
    while(1)
    {
    fflush(stdout);
    char commandInput[2000]="";
    printf("miniBash>");
    gets(commandInput);
   // printf("Enter command: ");
    printf("%s", commandInput);
    printf("\n");
    handleString(commandInput);
    int t=0;
    while(t<j)
    {
        char* command = singleCommands[t]; 
        int i=0;
        int pipe_present=0;
        int redirection_present=0;
        while (command[i] != '\0')
        {
             if(command[i] == '|')
             {
                 pipe_present++;
             }
             if(command[i] == '>' || command[i] == '<')
             {
                redirection_present++;
                int init_size = strlen(command);
	            char delim[] = ">";

	            char *ptr = strtok(command, delim);
                int flag=0;
                int l=0;
	            while(ptr != NULL)
	            {
                    l=0;
                    for(int i=0;i<strlen(ptr);i++)
                    {
                        redirection[y][l] = ptr[i];
                        ++l;
                    }
                    redirection[y][l] = '\0';
                    printf("%s\n",redirection[y]);
                    y=y+1;
		            ptr = strtok(NULL, delim);
	            }
             }
             i++;

        }
        if(pipe_present==0) //semicolon implementation
        {
            int prev_commands_length=0;
            int pid = fork();
            if(pid==0){
                execute_command(command);
            }
            else if(pid>0) {
                wait(NULL);
            }
        }
        if(redirection_present>0) // redirection implementation
        {
            int fd;
            int ret;
            char* filename = redirection[1];
            fd = open(filename, O_CREAT | O_APPEND | O_WRONLY);
            if(fd < 0)
            {
                perror("open");
            }
            ret = dup2(fd,1);

            if(ret < 0)
            {
                perror("dup2");
            }
            int prev_commands_length = 0;
            int u=0;
            char* command1 = redirection[0];
            while (command1[u] != '\0'){
             u++;
            }
            int pid = fork();
            if(pid==0){
                // char* current_command = calloc(i + 1 - prev_commands_length, sizeof(char));
                // strncpy(current_command, command1 + prev_commands_length, i - prev_commands_length);
                execute_command(command1);

            }
            else if(pid>0) {
                wait(NULL);
            }
            close(fd);
        }
        if(pipe_present>0) { // pipe implementation
            int pid = fork();
            if(pid==0){
                pipeimple(command);
            }
            else if(pid>0) {
                wait(NULL);
            }
        }
        t++;       
    }
    
    }
}