/*
 * timestamp.h
 */

#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_


void timestamp_set(u32 timestamp);
u32 now(void);
bool is_current_time_valid(void);
u32 uptime(void);
void timestamp_tick(void);
int timestamp_to_hour(u32 timestamp, int gmt_offset);

#endif /* TIMESTAMP_H_ */
