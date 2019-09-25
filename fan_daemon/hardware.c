#include <stdio.h>
#include <unistd.h>
#include <pigpiod_if2.h>

#include "hardware.h"

/*
    Retrieves the current CPU temp from the system.
    To get temp in degrees C, divide by 1000.
*/
int cpu_temp() {
    int temperature = 0;
    FILE *file = fopen(THERMAL_FILE_PATH, "r");
    if (file == NULL) {
        fprintf(stderr, "CPU temp could not be read from: %s\n", THERMAL_FILE_PATH);
        return 0;
    }
    
    int result = fscanf(file, "%d", &temperature);
    if (result == EOF) {
        fprintf(stderr, "%s is in an unrecognized format.\n", THERMAL_FILE_PATH);
        return 0;
    }
    
    fclose(file);
    return temperature; 
}

/*
    Sets whether the fan is on or off.
    Passing a number greater than 0 enables the fan. 0 disables it.
*/
int enable_fan(int pi, __uint8_t enabled) {
    int result = PI_BAD_GPIO;
    if (enabled) {
        result = gpio_write(pi, FAN_ENABLE_PIN, 1);
    } else {
        result = gpio_write(pi, FAN_ENABLE_PIN, 0);
    }
    
    return result;
}

/*
 * Sets up the appropriate pin settings to enable the fan.
 */
int initialize_pins(int pi) {
    int result = 0;
    // Set pin modes for our input and output
    result = set_mode(pi, FAN_ENABLE_PIN, PI_OUTPUT);
    if (result != 0) {
	    fprintf(stderr, "Couldn't set pin mode on pin %d.\n", FAN_ENABLE_PIN);
	    return result;
    }

    result = set_mode(pi, FAN_SPEED_PIN, PI_INPUT);
    if (result != 0) {
	    fprintf(stderr, "Couldn't set pin mode on pin %d.\n", FAN_SPEED_PIN);
	    return result;
    }

    // Set pull-down resistors on all I/O pins
    result = set_pull_up_down(pi, FAN_ENABLE_PIN, PI_PUD_DOWN);
    if (result != 0) {
       fprintf(stderr, "Couldn't set pull-down on pin %d.\n", FAN_ENABLE_PIN);
       return result;
    }

    result = set_pull_up_down(pi, FAN_SPEED_PIN, PI_PUD_DOWN);
    if (result != 0) {
	    fprintf(stderr, "Couldn't set pull-down on pin %d.\n", FAN_SPEED_PIN);
	    return result;
    }

    return result;
}

/*
    Determines whether the fan should run or not based on the current CPU
    temp and the target temp in pifan.conf.
*/
__uint8_t determine_fan_status(int pi) {
    __uint8_t fan_status = 0;
    volatile int target_temp = 0;
    
    FILE *config_file = fopen(CONFIG_FILE_PATH, "r");
    if (config_file == NULL) {
        fprintf(stdout, "Couldn't open %s\nSetting target temp to default of %d°C\n", CONFIG_FILE_PATH, DEFAULT_TARGET_TEMP);
        target_temp = DEFAULT_TARGET_TEMP;
    } else {
        int result = fscanf(config_file, "target_temp=%d", &target_temp);
        if (result == EOF) {
            fprintf(stdout, "Couldn't read the target temp from %s\nSetting target temp to default of %d°C\n", CONFIG_FILE_PATH, DEFAULT_TARGET_TEMP);
            target_temp = DEFAULT_TARGET_TEMP;
        }
        fclose(config_file);
    }
    
    int current_temp = cpu_temp();
    
    fan_status = (current_temp >= (target_temp+TEMP_BUFFER)*1000) || (current_temp <= (target_temp-TEMP_BUFFER)*1000);
    
    return fan_status;
}
