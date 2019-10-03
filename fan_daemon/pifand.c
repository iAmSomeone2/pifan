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

struct tracker_args {
    int pi;
    FILE *outfile;
};

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
    
    // Capturing these lets us gracefully shutdown
    signal(SIGINT, interrupt_handler);
    signal(SIGTERM, interrupt_handler);
    
    printf("Tracking fan speed for Pi %d\n", args->pi); 
    /*  
     * Set up loop to poll the gpio pin connected to the fan tach.
     * To accurately get the fan speed, we'll have to figure out a
     * reasonable amount of time to let the loop run while still
     * getting all of the tach. pulses.
     */
     unsigned speed_callback = register_speed_callback(args->pi);
     while(active) {
         int rpm = get_fan_speed();
         printf("Current fan speed: %d RPM\n", rpm);
         sleep(1);
     }
     
    callback_cancel(speed_callback);
    pthread_exit(NULL);
}

int main(void) {
    FILE *fan_speed = fopen(FAN_SPEED_FILE, "w+");
    if (fan_speed == NULL) {
        fprintf(stderr, "Could not open %s for writing.\n", FAN_SPEED_FILE);
    }
    
    __uint8_t fan_state = 0;
    int program_status = 0;
    
    int rPi = pigpio_start(NULL, NULL);
    if (rPi < 0 ) {
        fprintf(stderr, "Failed to connect to pigpio daemon.\n");
        exit(1);
    }
    
    /*
     * Only start the thread for getting the fan speed if we can open
     * the output file. 
     */
    pthread_t tach_thread;
    if (fan_speed != NULL) {
        struct tracker_args args;
        args.pi = rPi;
        args.outfile = fan_speed;
        
        // Set up thread for reading the fan tach.
        int rc = pthread_create(&tach_thread, NULL, fan_tracker, (void *)&args);
        if (rc != 0) {
            fprintf(stderr, "Unable to create thread %d\n", rc);
            exit(-1);
        }
    }
    
    fprintf(stdout, "Successfully connected to pigpio daemon.\n");
    int target_temp = get_target_temp();
    fprintf(stdout, "Target system temp: %d°C\n", target_temp);

    int init_result = initialize_pins(rPi);
    if (init_result != 0) {
        exit(init_result);
    }

    // Capturing these lets us gracefully shutdown
    signal(SIGINT, interrupt_handler);
    signal(SIGTERM, interrupt_handler);
    
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

    printf("\nTurning off fan control daemon...\n");
    int enable_result = enable_fan(rPi, 0);
    if (enable_result != 0) {
        fprintf(stderr, "Encountered a problem changing fan modes.\n");
        program_status = enable_result;
    }

    pigpio_stop(rPi);
    if (fan_speed != NULL) {
        pthread_join(tach_thread, NULL);
        fclose(fan_speed);
    }
    return program_status;
}
