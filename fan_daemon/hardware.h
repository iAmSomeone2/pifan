#ifndef HARDWARE_H
#define HARDWARE_H 1

#define FAN_ENABLE_PIN              2
#define FAN_SPEED_PIN               3
#define TEMP_BUFFER                 2
#define SPEED_POLL_SECS             1

int initialize_pins(int pi);

int enable_fan(int pi, int enabled);

int determine_fan_status();

int register_speed_callback(int pi);

int get_fan_speed();

#endif
