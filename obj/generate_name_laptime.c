#include "generate_name_laptime.h"

void populate_names(char **destination_arr, int num_of_drivers)
{
    int index = 0;
    FILE *fptr;
    fd_error(fptr = fopen("pilots.dat", "r"));

    do
    {
        destination_arr[index] = malloc(32);
        fscanf(fptr, " %[^' ']s", destination_arr[index]);
        index++;
    } while (!feof(fptr) && index < num_of_drivers);

    fclose(fptr);
}


void generate_laptime(int **lap_times,
                      int total_drivers,
                      int total_laps,
                      int *dnf_status,
                      int *dnf_lap)
{
    int dnf_random;
    int driver_random;
    for (int i = 0; i < total_laps; i++)
    {
        // Generate a random number and divide it by the number of drivers
        // so that we basically have a specified range for each driver
        dnf_random = (rand() % 444 * (i+1))  / total_drivers;
        for (int j = 0; j < total_drivers; j++)
        {
            // Generate a random number within the specified range for a driver
            driver_random = rand() % dnf_random + 1;
            // If the generated number is within the 3% of that specified range
            // this means that the driver will get disqualified
            if (driver_random <= (int)(dnf_random * 0.03))
            {
                if (dnf_status[j] != 1)
                {
                    dnf_lap[j] = i;
                    dnf_status[j] = 1;
                    lap_times[j][i] = 45;
                }
            }
            else
            {
                lap_times[j][i] = rand() % (MAX_LAPTIME - MIN_LAPTIME + 1) + MIN_LAPTIME;
            }
            driver_random = 0;
        }
        dnf_random = 0;
    }
   

}