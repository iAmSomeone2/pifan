#ifndef HARDWARE_H
#define HARDWARE_H 1

#define FAN_ENABLE_PIN              17
#define FAN_SPEED_PIN               27
#define DEFAULT_TARGET_TEMP         50
#define TEMP_BUFFER                 2
#define DEFAULT_UPDATE_INTERVAL     10
#define THERMAL_FILE_PATH           "/sys/class/thermal/thermal_zone0/temp"
#define CONFIG_FILE_PATH            "../pifan.conf"

int initialize_pins(int pi);

int cpu_temp();

int enable_fan(int pi, __uint8_t enabled);

__uint8_t determine_fan_status(int pi);

#endif
