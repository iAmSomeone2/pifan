#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <pigpiod_if2.h>

#include "hardware.h"
#include "data_access.h"

static volatile __uint8_t active = 1;

/*
    Sets active to 0 when a SIGINT or SIGTERM signal is received.
    This allows the fan daemon to gracefully shutdown.
*/
void interrupt_handler() {
	active = 0;
}

int main(void) {
    int programStatus = 0;
    int rPi = pigpio_start(NULL, NULL);
    if (rPi < 0 ) {
	    fprintf(stderr, "Failed to connect to pigpio daemon.\n");
	    return 1;
    }
    fprintf(stdout, "Successfully connected to pigpio daemon.\n");

    int init_result = initialize_pins(rPi);
    if (init_result != 0) {
	    return init_result;
    }

    signal(SIGINT, interrupt_handler);
    signal(SIGTERM, interrupt_handler);
    
    while (active) {
        __uint8_t fan_status = determine_fan_status(rPi);
        int enable_result = enable_fan(rPi, fan_status);
        if (enable_result != 0) {
            fprintf(stderr, "Encountered a problem changing fan modes.\n");
            programStatus = enable_result;
            break;
        }
        // Pause loop
        int wait_time = get_wait_time();
        sleep(wait_time);
    }

    printf("\nTurning off fan control daemon...\n");
    int enable_result = enable_fan(rPi, 0);
    if (enable_result != 0) {
        fprintf(stderr, "Encountered a problem changing fan modes.\n");
	    programStatus = enable_result;
	}

    pigpio_stop(rPi);
    return programStatus;
}
