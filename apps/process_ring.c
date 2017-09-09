#include <xinu.h>
#include <stdlib.h>
#include <stdio.h>
#include <process_ring.h>
#include <string.h>


// Function to implement message passing 
void message_process(pid32 *next_pid, int curr, pid32 *prev_pid, int32 count_val, int32 pro_cre){                                     
  int32 last = 0;                                                                                                                      
  while (last < count_val){   // checking if the number of rounds have been completed for a process
    // waiting for a message                                                                                                          
    int32 temp = receive();                                                                                                            
    printf("Ring Element %d : Round %d : Value : %d\n", curr, last , temp);                                                            
    last += 1;      
    // sending message to the neighbour process                                                                                                                   
    send(*next_pid, temp-1);                                                                                                           
  }    

  // Passing messages back from the last process to the first which sends OK message to aprent so it stops execution.                                                                                                                                
  if(curr != pro_cre-1) {                                                                                                              
    receive();                                                                                                                         
    send(*prev_pid, OK);                                                                                                               
  }                                                                                                                                    
  else{                                                                                                                                
    send(*prev_pid, OK);                                                                                                               
  }                                                                                                                                    
  return OK;                                                                                                                           
} 


// Function to implement polling
void polling_process(int *curr_ptr, int *next_ptr, int32 *count, int32 *total_rounds, int curr){
  int32 last = 0;
  while (last < *(total_rounds)){// checking if the number of rounds have been completed for a process
    int32 temp = *(curr_ptr);
    // polling operation done here, waits until a non -1 value appears inn the mailbox
    if(temp != -1){
      printf("Ring Element %d : Round %d : Value : %d\n", curr , last, temp);
      last = last + 1;
      // incrementing count  for the parent to know when to stop execution
      *(count) = *(count) + 1;
      *(curr_ptr) = -1;
      // passing message to the neighbour process.
      *(next_ptr) = temp -1;
    }
  }
  return OK;
}