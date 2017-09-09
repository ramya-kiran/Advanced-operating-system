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
  char implementation[] = "poll";
  char *pont_imp = implementation;
  //char temp_value[];
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
              else if (!(0 <= p && p <= 64)) {
                /* The number is out of range */
                  print_error();
                  printf("-p flag expected a number between 0 - 64\n");
                  return SHELL_ERROR;
              } 
              printf("argument  is %d\n", p);}

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
              printf("argument  is %d\n", r);}


      // Handling i parameter        
      else if (0 == strncmp("-i", args[i], 3)){

                printf("Inside string sthingy\n");
                if(0 == strcmp(args[i+1],"poll")){
          	// call poliing stuff here
                  //temp_value = args[i+1];
                pont_imp = args[i+1];
                printf("The implementation type is %s\n", args[i+1]);}
                else if(0 == strcmp(args[i+1],"sync")){

                  //temp_value = args[i+1];
                  pont_imp = args[i+1];
                  printf("The implementation type is %s\n", args[i+1]);
          	// call message_passing here 
          }
          else {
    	print_error();
    	printf("Please provide a valid implementation option\n");
          }
        
        }else if (0 == strncmp("--h", args[i], 3)){
          
            printf("Example run: process_ring -p int_val -r uint32 -i string\n");
            printf("-p represents number of process beween 0 - 64, by default 2\n");
            printf("-r represents number of rounds which is a uint32 value, by default 3\n");
            printf("-i represents implementation type, expecting either poll or sync, by default poll\n");
        
      }
    
        
        i += 1; /* Skip pass the numeric argument */
      }
      printf("The Value for p is %d\n", p);
      printf("The Value for r is %d\n", r);
      printf("The Value for i is %s\n", pont_imp);

      if(0 == strcmp(pont_imp, "poll")){
          printf("Correctly inside if block of poll\n");                              
    /* This process has a lower priority than the parent process */                         
    // int procs = 10;                                                                         
    // int rounds = 2;                                                                         
    // int total_number_pnt = (procs * rounds ) -1;  
                  int total_number_pnt = (p * r) -1;                                          
                  volatile int inbox_arr[p];                                                          
                  volatile int32 count = -1;                                                              
                  //volatile int count_arr[2];                                                            
                  int q_val;                                                                                  
                  for(q_val =1; q_val< p; q_val++){                                                               
                    inbox_arr[q_val] = -1;                                                                    
                  }                                                                                       
                  inbox_arr[0] = total_number_pnt;                                                        
                  int y;                                                                                  
                  //printf("count here %d\n", &count);                                                    
                  for (y = 0; y < p-1 ; y++){                                                         
                    resume(create(polling_process, 1024, 20 , "val" , 5, &inbox_arr[y], &inbox_arr[y+1], &count, &r, y));                                                                              
                  }                                                                                       
                  resume(create(polling_process, 1024, 20 , "val" , 5, &inbox_arr[p-1], &inbox_arr[0], &count, &r, p-1));                                                                      
                                                                                                          
                  while(count < total_number_pnt);//{printf("count value %d \n",count);}                  
                  return SHELL_OK;                                                                                                                                                                
      }  
      else if(0 == strcmp(pont_imp, "sync")){
                  pid32 parent_pid  = getpid();                                                                                                        
                  // int procs = 10;                                                                                                                      
                  // int rounds = 2;                                                                                                                      
                  // int total_count = (procs * rounds) - 1;    
                  int total_count = (p * r) - 1;                                                                                            
                  int index = 0;                                                                                                                               
                  pid32 procs_array[p];                                                                                                            
                  procs_array[0] = create(message_process, 1024, 20, "consumer", 5, &procs_array[1], index, &parent_pid, r, p);                      
                  for(index = 1; index< p; index++){                                                                                                           
                    procs_array[index] = create(message_process, 1024, 20, "consumer", 5, &procs_array[(index+1)%p], index, &procs_array[index-1], r, p);    
                  }                                                                                                                                    
                                                                                                                                                       
                  int j;                                                                                                                               
                  for(j = 0; j< p; j++){                                                                                                           
                    resume(procs_array[j]);                                                                                                            
                  }                                                                                                                                    
                  send(procs_array[0], total_count);                                                                                                   
                                                                                                                                                       
                  receive();                                                                                                                           
                  return SHELL_OK;                                                                                                                     
    }

    return SHELL_OK;
    }                
      

    //   return SHELL_OK;
    // }
