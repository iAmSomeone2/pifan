#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include <pigpiod_if2.h>
#include <pifanconfig.h>

#include "hardware.h"
#include "data_access.h"

static volatile __uint8_t active = 1;
static volatile __uint8_t fan_state = 1;

struct tracker_args {
    int         pi;
} ;

/*
    Sets active to 0 when a SIGINT or SIGTERM signal is received.
    This allows the fan daemon to gracefully shutdown.
*/
void interrupt_handler() {
    active = 0;
}


/*
 * Tracks the fan speed using the pigpio lib. Results are written to a
 * temp file that can be accessed by other parts of the program while
 * the daemon is running.
*/
void *fan_tracker(void *arguments) {
    struct tracker_args *args = arguments;
    
    printf("Tracking fan speed for Pi: %d\n", args->pi); 
    /*  
     * Set up shared memory on this thread so that we can share the current
     * fan speed with other programs in this suite. 
    */
    
    unsigned speed_callback = register_speed_callback(args->pi);
    while(active) { 
        if (fan_state == 1) {
            int rpm = get_fan_speed();
            if (rpm == 0) {
                //fprintf(stderr, "Fan reporting %d RPM!\nCheck that it's not obstructed.\n", rpm);
            } else {
                fprintf(stdout, "%d\n", rpm);
            }
        }
        sleep(1);
    }
    
    // Clean up
    
    // Clear up our callback
    callback_cancel(speed_callback);
    // Exit the thread
    pthread_exit(NULL);
}

int main(void) {
    /*
     * Put together all initial states, connect to the pigpio daemon, and spawn a
     * thread for dealing with tracking the fan speed.
    */
     
    // Capturing these lets us gracefully shutdown
    signal(SIGINT, interrupt_handler);
    signal(SIGTERM, interrupt_handler);
    
    int program_status = 0;
    
    int rPi = pigpio_start(NULL, NULL);
    if (rPi < 0 ) {
        fprintf(stderr, "Failed to connect to pigpio daemon.\n");
        exit(1);
    }
    
    fprintf(stdout, "Successfully connected to pigpio daemon.\n");
    int target_temp = get_target_temp();
    fprintf(stdout, "Target system temp: %d°C\n", target_temp);

    int init_result = initialize_pins(rPi);
    if (init_result != 0) {
        exit(init_result);
    }
    
    // Set up thread for reading the fan tach.
    pthread_t tach_thread;
    struct tracker_args args;
    args.pi = rPi;
    
    int rc = pthread_create(&tach_thread, NULL, fan_tracker, (void *)&args);
    if (rc != 0) {
        fprintf(stderr, "Unable to create thread %d\n", rc);
        exit(-1);
    }
    
    // Main loop
    while (active) {
        __uint8_t fan_status = determine_fan_status(rPi);
        if (fan_state != fan_status) {
            // We should only try changing the fan state if it's needed.
            int enable_result = enable_fan(rPi, fan_status);
            if (enable_result != 0) {
                fprintf(stderr, "Encountered a problem changing fan modes.\n");
                program_status = enable_result;
                break;
            } else {
                fan_state = fan_status;
            }
        }
        // Pause loop
        sleep(DEFAULT_UPDATE_INTERVAL);
    }

    // Clean up
    printf("\nTurning off fan control daemon...\n");
    int enable_result = enable_fan(rPi, 0);
    if (enable_result != 0) {
        fprintf(stderr, "Encountered a problem changing fan modes.\n");
        program_status = enable_result;
    }

    pigpio_stop(rPi);
    pthread_join(tach_thread, NULL);
    return program_status;
}
