#ifndef __ALARM_H__
#define __ALARM_H__

int alarm(char* filename, int duration);
int alarm_report(char* ipaddr, int port, int host_index);

int alloc_alarm_ctxt(void);
void free_alarm_ctxt(void);
void init_alarm_ctxt(void);
int check_alarm_timeout(int duration);


#endif
