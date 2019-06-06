#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define PATH "./sequential_min_max"

int main(int argc, char** argv){
 
 pid_t child_pid=fork();
 if (child_pid >= 0){
			// successful fork
			if (child_pid > 0){
				//parent here printf("end");
				return 0;
			}

			if (child_pid == 0){
			    argv[0]=PATH;
			    if(execvp(PATH,argv)==-1){
			         printf("%i",errno);
			         exit(EXIT_FAILURE);
			         }
		    
		    }
 }
 else{
     printf("fork failed");
    exit(EXIT_FAILURE);
 }
 return 0;
}