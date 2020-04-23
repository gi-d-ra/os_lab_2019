#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define SEQUENTIAL "sequential_min_max"

int main(int argc, char* argv[]) {
	if (access(SEQUENTIAL, F_OK) == -1) {
		printf("Error: " SEQUENTIAL " not found\n");
		return -1;
	}


	pid_t pid;
	
  	int rv;
  switch(pid=fork()) {
  case -1:
          perror("fork"); /* произошла ошибка */
          exit(1); /*выход из родительского процесса*/
  case 0:
          printf(" CHILD: Это процесс-потомок!\n");
          printf(" CHILD: Мой PID -- %d\n", getpid());
          printf(" CHILD: PID моего родителя -- %d\n",
              getppid());
          execvp("./" SEQUENTIAL, argv);
  default:;
          
          
        
  }

}