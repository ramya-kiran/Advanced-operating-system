#include <xinu.h>
#include <stdlib.h>
#include <stdio.h>
#include <process_ring.h>
#include <string.h>



void message_process(pid32 *next_pid, int curr, pid32 *prev_pid, int32 count_val, int32 pro_cre){                                     
  int32 last = 0;                                                                                                                      
  while (last < count_val){                                                                                                            
    int32 temp = receive();                                                                                                            
    printf("Ring Element %d : Round %d : Value : %d\n", curr, last , temp);                                                            
    last += 1;                                                                                                                         
    send(*next_pid, temp-1);                                                                                                           
  }                                                                                                                                    
  if(curr != pro_cre-1) {                                                                                                              
    receive();                                                                                                                         
    send(*prev_pid, OK);                                                                                                               
  }                                                                                                                                    
  else{                                                                                                                                
    send(*prev_pid, OK);                                                                                                               
  }                                                                                                                                    
  return OK;                                                                                                                           
} 


void polling_process(int *curr_ptr, int *next_ptr, int32 *count, int32 *total_rounds, int curr){
  int32 last = 0;
  while (last < *(total_rounds)){
    int32 temp = *(curr_ptr);
    if(temp != -1){
      printf("Ring Element %d : Round %d : Value : %d\n", curr , last, temp);
      last = last + 1;
      *(count) = *(count) + 1;
      *(curr_ptr) = -1;
      *(next_ptr) = temp -1;
    }
  }
  return OK;
}