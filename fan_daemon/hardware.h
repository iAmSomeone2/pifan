#ifndef HARDWARE_H
#define HARDWARE_H 1

#define FAN_ENABLE_PIN              17
#define FAN_SPEED_PIN               27
#define TEMP_BUFFER                 2

int initialize_pins(int pi);

int enable_fan(int pi, __uint8_t enabled);

__uint8_t determine_fan_status();

#endif
