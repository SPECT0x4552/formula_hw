#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void populate_names(char **destination_arr, int num_of_drivers);
void generate_laptime(int **lap_times, int total_drivers, int total_laps, int *dnf_status, int *dnf_lap); 

int main(void) {
    srand(time(0));

    int drivers = 3;
    int total_laps = 5;


    typedef struct single_driver {
        int index;
        char name[32];
        int lap_times[15];
        int dnf_lap;
        int has_dnf;
        int race_result;

    } driver, *p_driver;

    driver drivers_arr[10];

    char **names_array = malloc(drivers * sizeof(char *));
    if(names_array == NULL) {
        printf("Error allocating memory.\n");
        return 1;
    }

    int **laps_array = malloc(drivers * sizeof(int *));
    int *dnf_status = malloc(drivers * sizeof(int));
    int *dnf_laps = malloc(drivers*sizeof(int));

    populate_names(names_array, drivers);
    for(int i = 0; i < drivers; i++) {
        drivers_arr[i].has_dnf = 0;
        dnf_status[i] = 0;
        laps_array[i] = malloc(sizeof(int) * total_laps);
        strcpy(drivers_arr[i].name, names_array[i]);
        free(names_array[i]);
    }

    //Cleanup
    free(names_array);

    // Generate laptimes for each driver
    generate_laptime(laps_array, drivers, total_laps, dnf_status, dnf_laps);

    // Generate the output
    printf("\n");
    int sum;
    for(int j = 0; j < drivers; j++) {
        sum = 0;
        memcpy(drivers_arr[j].lap_times, laps_array[j], 15*sizeof(int));
        free(laps_array[j]);
        drivers_arr[j].has_dnf = dnf_status[j];
        drivers_arr[j].dnf_lap = dnf_laps[j];
        printf("   %12s\t  ", drivers_arr[j].name);
        for(int i = 0; i < total_laps; i++) {
            if(drivers_arr[j].has_dnf == 1 && drivers_arr[j].dnf_lap <= i) {
                printf(" %3c ",'-');
                sum = 0;
            } else {
                printf(" %3d ", drivers_arr[j].lap_times[i]);
                sum += drivers_arr[j].lap_times[i]; 
            }
            
        }
        printf("  | ");
        if(sum == 0) {
            printf("  %s", "DNF");
        } else {
            printf("  %ds", sum);
        }
        
        printf("\n");
    }

    printf("\n");
    free(laps_array);
    free(dnf_status);
    free(dnf_laps);

    

    return 0;

    }


void populate_names(char **destination_arr, int num_of_drivers) {
    int index = 0;
    FILE *fptr;
    fptr = fopen("pilots.dat", "r");
    if(fptr == NULL) {
        printf("Error handling the file.\n");
        exit(1);
    }

    do {
        destination_arr[index] = malloc(32);
        fscanf(fptr, " %[^' ']s", destination_arr[index]);
        index++;
    } while(!feof(fptr) && index < num_of_drivers);

    fclose(fptr);
}

void generate_laptime(int **lap_times, int total_drivers, int total_laps, int *dnf_status, int *dnf_lap) {
    int index;
    for(int i = 0; i < total_laps; i++) {
        int random = rand() % 3333+1;
        for(int j = 0; j < total_drivers; j++) {
            index = rand() % 100+1;
            if(index <= 3) {
                if(dnf_status[j] != 1) {
                    dnf_lap[j] = i;
                    dnf_status[j] = 1;
                    lap_times[j][i] = 45;
                }
            } else {
                lap_times[j][i] = rand() % (125-70+1)+70;
            }    
            
        }
    }
}
