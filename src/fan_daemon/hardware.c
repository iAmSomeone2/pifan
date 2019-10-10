#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pigpiod_if2.h>

#include <pifanconfig.h>
#include "data_access.h"

#include "hardware.h"

int pulse_count = 0;

/*
    Sets whether the fan is on or off.
    Passing a number greater than 0 enables the fan. 0 disables it.
*/
int enable_fan(int pi, int enabled) {
    int result = PI_BAD_GPIO;
    if (enabled) {
        printf("Turning fan on...\n");
        result = gpio_write(pi, FAN_ENABLE_PIN, 1);
    } else {
        printf("Turning fan off...\n");
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

    result = set_mode(pi, FAN_TACH_PIN, PI_INPUT);
    if (result != 0) {
        fprintf(stderr, "Couldn't set pin mode on pin %d.\n", FAN_TACH_PIN);
        return result;
    }

    // Set pull-down resistors on all I/O pins
    result = set_pull_up_down(pi, FAN_ENABLE_PIN, PI_PUD_DOWN);
    if (result != 0) {
       fprintf(stderr, "Couldn't set pull-down on pin %d.\n", FAN_ENABLE_PIN);
       return result;
    }

    result = set_pull_up_down(pi, FAN_TACH_PIN, PI_PUD_DOWN);
    if (result != 0) {
        fprintf(stderr, "Couldn't set pull-down on pin %d.\n", FAN_TACH_PIN);
        return result;
    }

    return result;
}

/*
    Determines whether the fan should run or not based on the current CPU
    temp and the target temp in pifan.json.
*/
int determine_fan_status() {
    __uint8_t fan_status = 0;
    int target_temp = get_target_temp();
    if (target_temp == 0) {
        fprintf(stdout, "There was a problem reading %s\nDefaulting to %d°C\n", CONFIG_FILE_PATH, DEFAULT_TARGET_TEMP);
    }
    
    int current_temp = cpu_temp();
    
    fan_status = (current_temp >= (target_temp+TEMP_BUFFER)*1000) || (current_temp <= (target_temp-TEMP_BUFFER)*1000);
    
    return fan_status;
}

void increment_pulse_count(){
    pulse_count++;
}

/*
 * Sets up the callback that is used when the fan tach. pulses its GPIO
 * pin. 
 */
int register_speed_callback(int pi) {
    int callback_id = callback(pi, FAN_TACH_PIN, RISING_EDGE, increment_pulse_count);
    
    switch (callback_id) {
        case pigif_bad_malloc:
            fprintf(stderr, "Pigpio encountered a bad malloc\n");
            break;
        case pigif_duplicate_callback:
            fprintf(stderr, "Pigpio encountered a duplicate callback\n");
            break;
        case pigif_bad_callback:
            fprintf(stderr, "Pigpio encountered a bad callback\n");
            break;
        default:
            printf("Successfully registered callback for fan speed.\n");
    }

    return callback_id;
}

/*
 * Computes the current speed of the CPU fan. Returned result is the
 * speed in RPM. 
 */
int get_fan_speed() {
    
    
    // Reset the pulse count
    pulse_count = 0;
    
    // Wait for some pulses to accumulate
    sleep(SPEED_POLL_SECS);
    
    // Calculate RPM;
    int pulse_per_sec = pulse_count / SPEED_POLL_SECS;
    int rpm = (pulse_per_sec / 2) * 60; // There are two fan pulses per rev
    
    return rpm;
}
