#include "print_to_file.h"

int print_to_file(char *filename,
                  char **dnames,
                  int **lap_times,
                  int total_drivers,
                  int total_laps,
                  int *dnf_status,
                  int *dnf_lap)
{

    FILE *fptr;
    int laptime_sum;
    int counter_columns = 0;
    fd_error(fptr = fopen(filename, "w+"));

    // Write the header names
    fprintf(fptr, "DRIVER NAME,");
    while (counter_columns < total_laps)
    {
        // Write the header names for laps
        fprintf(fptr, "LAP %d,", counter_columns + 1);
        counter_columns++;
    }
    // Write the header name for result
    fprintf(fptr, "RESULT\n");

    for (int i = 0; i < total_drivers; i++)
    {
        laptime_sum = 0;
        fprintf(fptr, "%s,", dnames[i]);
        for (int j = 0; j < total_laps; j++)
        {
            if (dnf_status[i] == 1 && dnf_lap[i] <= j)
            {
                fprintf(fptr, "%c,", DNF_LAPTIME);
            }
            else
            {
                fprintf(fptr, "%d,", lap_times[i][j]);
                laptime_sum += lap_times[i][j];
            }
        }
        if (dnf_status[i])
        {
            fprintf(fptr, "DNF\n");
        }
        else
        {
            fprintf(fptr, "%ds\n", laptime_sum);
        }
    }
    laptime_sum = 0;

    fclose(fptr);
    return 2;
}