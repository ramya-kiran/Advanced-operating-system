/* xsh_arg_example.c - xsh_arg_example */

/*------------------------------------------------------------------------
* xhs_arg_example - display the value associated with the -p flag
*------------------------------------------------------------------------
*/

#include <xinu.h>
#include <stdlib.h>
#include <stdio.h>
#include <process_ring.h>
#include <string.h>

void print_error(void){
  printf("Usage: ...\n");
}

shellcmd xsh_process_ring(int argc, char *args[])
{
  int32 p = 2; /* specified default */
  int32 r = 3; /* specified default */
  char implementation[] = "poll"; /* specified default */
  char *pont_imp = implementation;
  char *endp;
  char *endr;
  int32 i;

  // for loop for getting the arguments and aprsing them 
  for(i = 1; i < argc; i++){

    // Handling processes
      if (0 == strncmp("-p", args[i], 3)){
          /* parse numeric argument to -p */
              if (! (i + 1 < argc)) {
                  print_error();
                  printf("-p flag expected an argument\n");
                  return SHELL_ERROR;
              }
              p = strtol(args[i + 1], &endp, 10);
              if (endp == args[i + 1]) {
                /* The end pointer hasn't advanced */
                  print_error();
                  printf("-p flag expected an integer\n");
                  return SHELL_ERROR;}
              else if (*endp != '\0'){
               /* There was trailing garbage in the string that
                  wasn't converted to an integer. */
                  print_error();
                  printf("-p recieved invalid integer\n");
                  return SHELL_ERROR;}
              else if (!(0 < p && p <= 64)) {
                /* The number is out of range */
                  print_error();
                  printf("-p flag expected a number between 1 - 64\n");
                  return SHELL_ERROR;
              } 
            }
	      //              printf("argument  is %d\n", p);}

// Handling the number of rounds              
      else  if(0 == strncmp("-r", args[i], 3)) { 
          // Handle other cases
                if (! (i + 1 < argc)) {
                    print_error();
                    printf("-r flag expected an argument\n");
                    return SHELL_ERROR;
                    }
                    r = strtol(args[i + 1], &endr, 10);
                if (endr == args[i + 1]) {
                /* The end pointer hasn't advanced */
                    print_error();
                    printf("-r flag expected an integer\n");
                    return SHELL_ERROR;}
                else if (*endr != '\0'){
               /* There was trailing garbage in the string that
                  wasn't converted to an integer. */
                  print_error();
                  printf("-r recieved invalid integer\n");
                  return SHELL_ERROR;
              }
	       else if (!(0 < r && r <= 100)) {
                /* The number is out of range */
                  print_error();
                  printf("-r flag expected a number between 1 - 100\n");
                  return SHELL_ERROR;
	       }
       }
		//printf("argument  is %d\n", r);}


      // Handling i parameter        
      else if (0 == strncmp("-i", args[i], 3)){
	// Checking whether the user has opted for polling or synchronization method
                if(0 == strcmp(args[i+1],"poll")){
                pont_imp = args[i+1];
                printf("The implementation type is %s\n", args[i+1]);}
                else if(0 == strcmp(args[i+1],"sync")){
                  pont_imp = args[i+1];
          }
          else {
    	print_error();
    	printf("Please provide a valid implementation option\n");
          }
        
		// providing appropriate message for help parameter
        }else if (0 == strncmp("--h", args[i], 3)){
          
            printf("Example run: process_ring -p int_val -r uint32 -i string\n");
            printf("-p represents number of process beween 0 - 64, by default 2\n");
            printf("-r represents number of rounds which is between 0 - 100, by default 3\n");
            printf("-i represents implementation type, expecting either poll or sync, by default poll\n");
        
      }
    
        
        i += 1; /* Skip pass the numeric argument */
      }


      if(0 == strcmp(pont_imp, "poll")){
                  // Implementing the polling function 
                  int total_number_pnt = (p * r) -1;  // total number to count down from                                        
                  volatile int inbox_arr[p];  // declaring the mailbox array                                                        
                  volatile int32 count = -1;     // count variable to check when the child processes all complete execution  
                  uint32 now;
                  int32 retval;
                  retval = gettime(&now);
                  // for loop to initialize the mailbox with -1 values (to help with polling)                                                                                                                
                  int q_val;                                                                                  
                  for(q_val =1; q_val< p; q_val++){                                                               
                    inbox_arr[q_val] = -1;                                                                    
                  }                                                                                       
                  inbox_arr[0] = total_number_pnt;     

                  // Declaring the processes and resuming them to start.                                                   
                  int y;                                                                                                                                   
                  for (y = 0; y < p-1 ; y++){                                                         
                    resume(create(polling_process, 1024, 20 , "val" , 5, &inbox_arr[y], &inbox_arr[y+1], &count, &r, y));                                                                              
                  }                                                                                       
                  resume(create(polling_process, 1024, 20 , "val" , 5, &inbox_arr[p-1], &inbox_arr[0], &count, &r, p-1));                                                                      
                                
                  // condition to check if child processes completed execution                                                                          
                  while(count < total_number_pnt); 
                  uint32 fin; 
                  int32 retval_fin; 
                  retval_fin = gettime(&fin); 
                  printf("Time elapsed in clock ticks is %d\n", fin-now);
                  return SHELL_OK;                                                                                                                                                                
      }  
      else if(0 == strcmp(pont_imp, "sync")){
                  // getting parent process pid to pass message once the last child process completes execution.
                  pid32 parent_pid  = getpid();                                                                                                        
                  int total_count = (p * r) - 1;   // total number to count down from                                                                                          
                  int index = 0; 

                  uint32 now;
                  int32 retval;
                  retval = gettime(&now);

                  pid32 procs_array[p];   // getting an array of pid's to help message passing between processes easily
                  if (p == 1){
                    procs_array[0] = create(message_process, 1024, 20, "consumer", 5, &procs_array[0], index, &parent_pid, r, p); 

                  }else{
                  // creating processes but in suspended state to collect pid's of all processes.                                                                                                         
                  procs_array[0] = create(message_process, 1024, 20, "consumer", 5, &procs_array[1], index, &parent_pid, r, p);                      
                  for(index = 1; index< p; index++){                                                                                                           
                    procs_array[index] = create(message_process, 1024, 20, "consumer", 5, &procs_array[(index+1)%p], index, &procs_array[index-1], r, p);    
                  }                                                                                                                                    
                  }
                   // resuming the processes and starting execution of message passing.                                                                                                                                    
                  int j;                                                                                                                               
                  for(j = 0; j< p; j++){                                                                                                           
                    resume(procs_array[j]);                                                                                                            
                  }                                                                                                                                    
                  send(procs_array[0], total_count);                                                                                                   
                  
                  // condition to check if child processes completed execution                                                                                                                              
                  receive();  
                  uint32 fin; 
                  int32 retval_fin; 
                  retval_fin = gettime(&fin); 
                  printf("Time elapsed in clock ticks is %d\n", fin-now);                                                                                                               
                  return SHELL_OK;                                                                                                                     
    }

    return SHELL_OK;
    }            
      
