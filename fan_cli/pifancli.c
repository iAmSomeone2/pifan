#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pifanconfig.h>

__uint8_t set_target_temp(int target);

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "An integer value for the new target temp is required\n");
        return 1;
    }
    
    int target_temp = atoi(argv[1]);
    if (target_temp == 0) {
        fprintf(stderr, "A non-zero integer value for the new target temp is required\n");
        return 2;
    }
    
    int success = set_target_temp(target_temp);
    if (!success) {
        // Check if the user is "root". This will be neccessary when running in production.
        uid_t uid = getuid(), euid = geteuid();
        if (uid != euid) {
            fprintf(stderr, "UID and eUID don't match.\nUser can't be trusted!\n");
            return 3;
        } else if (euid != 0) {
            printf("Can't access the config file as this user.\nTry again as root.\n");
            return 3;
        } else if (euid == 0) {
            fprintf(stderr, "Something seems to be wrong with the config file.\nCheck %s\n", CONFIG_FILE_PATH);
            return 3;
        }
    } else {
        printf("New target temp is: %d°C\n", target_temp);
    }
    
    return 0;
}

/*
    Opens the pifan.conf file and replaces the old target temp with
    the new one.
    Returns 1 if the operation was successful. 0, otherwise.
*/
__uint8_t set_target_temp(int target) {
    FILE *config_file = fopen(CONFIG_FILE_PATH, "w+");
    if (config_file == NULL) {
        fprintf(stderr, "Couldn't open %s for writing.\n", CONFIG_FILE_PATH);
        return 0;
    }
    
    // Create new version of the target_temp string
    char *base_str = "target_temp=";
    char num_str[12];
    sprintf(num_str, "%d", target);
    char *output_str = malloc(strlen(base_str) + strlen(num_str) + 1);
    strcpy(output_str, base_str);
    strcat(output_str, num_str);
    
    int write_result = fprintf(config_file, "%s", output_str);
    if (write_result <= 0) {
        fprintf(stderr, "Could not write new target temp to %s\n", CONFIG_FILE_PATH);
        fclose(config_file);
        free(output_str);
        return 0;
    }
    
    fclose(config_file);
    free(output_str);
    return 1;
}
