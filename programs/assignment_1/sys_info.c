#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main (int nargs, char *args[]) {
  
  // Checking for two arguments to be passed
if(nargs < 2){
    printf("Expecting more arguments\n");
    return(-1);
 }

  int pipefd[2];
  pipe(pipefd); // pipe() process initialized before fork()
  int pid = fork(); // fork process initialization
  char buffer[500];

  if(pid < 0){
    printf("fork() was unsuccessful");
    return -1;
  }
  
  // If return value is 0 then the control is in the child process
  // This part of the code is for the child process 
  // It closes the write file of the pipe() function and reads the data sent by the parent 
  // and performs an exec() function.
  if(pid == 0){
    printf("Child PID = %d\n", getpid());
    close(pipefd[1]);
    int nbytes = read(pipefd[0], buffer, sizeof(buffer));
    buffer[nbytes] = '\0';
    if(strcmp(args[1], "/bin/echo")==0){
      execl("/bin/echo", "hello", "Hello World!",(char*)0);
      return 0;
    }
    execl(buffer, buffer, 0, 0);
    close(pipefd[0]);
    return 0;
  }

  // If return value > 0 then the control is in the parent process
  // This part of the code is for the parent process
  // It closes the read file of the pipe() function and writes the data to be sent to the child process 
  // and waits for the child process to complete.
  if(pid > 0){
    printf("Parent PID = %d\n", getpid());
    close(pipefd[0]);
    write(pipefd[1], args[1], (strlen(args[1])));
    close(pipefd[1]);
    wait(NULL);
    return 0;
  }
  return 0;
}

