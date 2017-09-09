#ifndef PROCESS_RING_H
#define PROCESS_RING_H

void message_process(pid32 *nextpid, int curr, pid32 *prev_pid, int32 count_val, int32 pro_cre);
void polling_process(int *curr_ptr, int *next_ptr, int32 *count, int32 *total_rounds, int curr);


#endif
