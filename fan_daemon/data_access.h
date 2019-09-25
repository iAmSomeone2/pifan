#ifndef DATA_ACCESS_H
#define DATA_ACCESS_H 1

#define DEFAULT_UPDATE_INTERVAL     10
#define DEFAULT_TARGET_TEMP         50

int cpu_temp();
int get_target_temp();
int get_wait_time();

#endif
