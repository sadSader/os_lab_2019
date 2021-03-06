#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

#define FILEPATH "data.txt"
#define PARENTLOG "parent.log"


int main(int argc, char **argv)
{
	int seed = -1;
	int array_size = -1;
	int pnum = -1;
	bool with_files = false;

	while (true) {
		int current_optind = optind ? optind : 1;

		static struct option options[] = { { "seed", required_argument, 0, 0 },
		{ "array_size", required_argument, 0, 0 },
		{ "pnum", required_argument, 0, 0 },
		{ "by_files", no_argument, 0, 'f' },
		{ 0, 0, 0, 0 } };

		int option_index = 0;
		int c = getopt_long(argc, argv, "f", options, &option_index);

		if (c == -1) break;

		switch (c) {
		case 0:
			switch (option_index) {
			case 0:
				seed = atoi(optarg);
				// your code here
				// error handling
				break;
			case 1:
				array_size = atoi(optarg);
				// your code here
				// error handling
				break;
			case 2:
				pnum = atoi(optarg);
				// your code here
				// error handling
				break;
			case 3:
				with_files = true;
				break;

			defalut:
				printf("Index %d is out of options\n", option_index);
			}
			break;
		case 'f':
			with_files = true;
			break;

		case '?':
			break;

		default:
			printf("getopt returned character code 0%o?\n", c);
		}
	}

	if (optind < argc) {
		printf("Has at least one no option argument\n");
		return 1;
	}

	if (seed == -1 || array_size == -1 || pnum == -1) {
		printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
			argv[0]);
		return 1;
	}

	int *array = malloc(sizeof(int) * array_size);
	pid_t *childs = malloc(sizeof(pid_t) * pnum);
	GenerateArray(array, array_size, seed);
	int active_child_processes = 0;

	struct timeval start_time;
	gettimeofday(&start_time, NULL);

	int pipefd[2];

	FILE*  p_log;
	
	remove(PARENTLOG);
    p_log = fopen(PARENTLOG, "a+");
    /*for(int i=0;i<array_size;++i) 
*/
	if (with_files)
	{
		remove(FILEPATH);
		
		// use files here
	}
	else
	{
		// init pipe here

		if (pipe(pipefd) == -1) {
			perror("pipe");
			exit(EXIT_FAILURE);
		}


	}
	for (int i = 0; i < pnum; ++i){
		pid_t child_pid = fork();
		if (child_pid >= 0){
			// successful fork
			if (child_pid > 0)
			{
				(childs[active_child_processes] = child_pid);

				active_child_processes += 1;
			}

			if (child_pid == 0){
				// child process
				int begin = array_size / pnum * (active_child_processes);
				int end = min(array_size / pnum * (active_child_processes+1), array_size);
				struct MinMax minMax = GetMinMax(array, begin, end);
				// parallel somehow

				
				//char* strMin="min";
				//char* strMax="max";

				if (with_files){
					// use files here
                char* intBuffer = malloc(sizeof(char) * 20);
				sprintf(intBuffer, "%d %d \n", minMax.min, minMax.max);
				        
					FILE *fp;
					fp = fopen(FILEPATH, "a+");
					fprintf(fp, "%s", intBuffer);
					fclose(fp);
				}
				else{//using pipes
					close(pipefd[0]);
                  //  printf(" writen %d %d \n", minMax.min, minMax.max);
				    write(pipefd[1], &minMax, sizeof(minMax));
					close(pipefd[1]);

					// use pipe here
				}
				//printf("child exit\n");
				return 0;
			}//child process
			 //child_pid>=0 
		}
		else{
			printf("Fork failed!\n");
			return 1;
		}
		
	}//fork
	
	for(int i=0; i<pnum; ++i){
	   		fprintf(p_log, "child[%i], pid: %i \n", i, childs[i]);
	   		
	   		
   		}


		//parent
	
		for (int i = 0; i<pnum; ++i)//waiting
		{
			waitpid(childs[i], &i, 0);
		}

		/*  while (active_child_processes > 0) {
		// your code here
		printf("wait");
		int unused;

		active_child_processes -= 1;
		printf("wait %i", active_child_processes);
		}*/

		struct MinMax min_max;
		min_max.min = INT_MAX;
		min_max.max = INT_MIN;

		//for (int i = 0; i < pnum; i++){
			int min = INT_MAX;
			int max = INT_MIN;
			int* returned = malloc(sizeof(int) * 2 * pnum);

			if (with_files)
			{
				// read from files

				FILE* file = fopen(FILEPATH, "r");

				for (int i = 0; i<pnum * 2; ++i){
				    fscanf(file, "%i", returned + i);
				    }

			}
			else{
				// read from pipes
				struct MinMax buf;
				//printf("%s ", "reading");//wtf
				int status=read(pipefd[0], &buf, sizeof(&buf));
				for(int i=0; i<pnum;++i){
				    if (status<=0){
				        printf("pipe error");
				        fflush(NULL);
			        }

					//printf("min: %i, max:%i ,childs:%i \n", buf.min, buf.max, active_child_processes);
					returned[2*i]=buf.min;
					returned[2*i+1]=buf.max;
					fflush(NULL);

					status=read(pipefd[0], &buf, 1);
				}
					
					
					
			}
			
			for(int i=0; i<pnum*2;++i)
			     printf("%i \n", returned[i]);
			min_max = GetMinMax(returned, 0, 2 * pnum);
			if (min < min_max.min) min_max.min = min;
			if (max > min_max.max) min_max.max = max;
		//}//for

		struct timeval finish_time;
		gettimeofday(&finish_time, NULL);

		double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
		elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

		free(array);
		free(childs);

        printf("Min: %d\n", min_max.min);
		printf("Max: %d\n", min_max.max);
		printf("Elapsed time: %fms\n", elapsed_time);
		fflush(NULL);
		fclose(p_log);
		return 0;
}
