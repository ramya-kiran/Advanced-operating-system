/* xsh_hello.c - xsh_hello */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------                                                            
 * xsh_hello - takes an single string argument and outputs it.                                                                  
 *------------------------------------------------------------------------                                                            
 */ 

shellcmd xsh_hello(int nargs, char *args[]) {
  // Checking if the number of arguments are greater than 2.
  if (nargs > 2){
    fprintf(stderr, "too many arguments, expecting hello <string> as input\n");
    return 1;
  }
  // Checking if the number of arguments are lesser than 2.
  if (nargs < 2){
    fprintf(stderr, "too few arguments, expecting hello <string> as input\n");
    return 1;
  }

  // greeting the string sent by the command line.
  if (nargs == 2){
    printf("Hello %s, Welcome to the world of Xinu!!\n", args[1]);
    return 0;
  }
  return 0;
}
