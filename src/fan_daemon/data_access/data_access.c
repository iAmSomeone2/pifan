#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "data_access.h"
#include <pifanconfig.h>

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
    Returns the target temp as specified by the config file.
*/
int get_target_temp() {
    FILE *config_file = fopen(CONFIG_FILE_PATH, "r");
    if (config_file == NULL) {
        fprintf(stderr, "Couldn't open config file to determine target temp.\n");
        return 0;
    }
    
    int target_temp = DEFAULT_TARGET_TEMP;
    int result = 0;
    do {
        result = fscanf(config_file, "target_temp=%d", &target_temp);
    } while (result == 0 && result != EOF);
    
    fclose(config_file);
    return target_temp;
}

/*
 * Returns a pointer to a segment of shared memory. 
*/ 
void *create_shared_memory(size_t size) {
    // We want the buffer to be read and write enabled.
    int protection = PROT_READ | PROT_WRITE;
    
    // Set visibilty to shared so that our other programs can use it.
    int visibility = MAP_SHARED;
    
    return mmap(NULL, size, protection, visibility, -1, 0);
}
