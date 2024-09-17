#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_INPUT 30
#define WRITE_OUTPUT_SUCCESS 2
#define MAX_LAPS 15
#define MIN_LAPS 5

#include "./function_library/check_verify.h"
#include "./function_library/generate_name_laptime.h"
#include "./function_library/print_to_file.h"


int main(int argc, char *argv[])
{

    // Set the seed to be used by rand()
    // Value returned by time is used since it's the most simple value to get and that is usually different between rand() executions
    srand(time(0));

    char *e = "[-]"; // Indicates error
    char *info = "[*]"; // Indicates additional information
    char *s = "[+]"; // Indicates success
    int number_of_laps;
    int number_of_drivers;
    int wrote_file = 0;
    int received_drivers = 0;
    FILE *fptr;

    typedef struct single_driver
    {
        int index;
        char name[32];
        int lap_times[15];
        int dnf_lap;
        int has_dnf;
        int race_result;

    } driver;

    driver drivers_arr[10];

    if (!argv[1] || (basic_checks(argc, atoi(argv[1])) != 0))
    {
        printf("%s Usage: ./formula <Number of pilots> <(Optional) Output file>\n", e);
        exit(1);
    }

    if (argv[2])
    {
        if (strlen(argv[2]) > MAX_INPUT)
        {
            printf("%s Output file name is limited to a maximum of %d characters.\n", e, MAX_INPUT);
            printf("Exiting the program...\n");
            exit(1);
        }
    }

    number_of_drivers = atoi(argv[1]);

    printf("Number of laps: ");
    scanf("%d", &number_of_laps);
    if (number_of_laps < MIN_LAPS || number_of_laps > MAX_LAPS)
    {
        printf("%s Invalid number of laps!\n", e);
        printf("%s Enter a number between 5-15\n", info);
        exit(1);
    }

    fd_error(fptr = fopen("pilots.dat", "r"));

    // Check if there are required amount of drivers available in the given file
    if (number_of_drivers != (received_drivers = verify_data(fptr, number_of_drivers)))
    {
        printf("%s Not enough drivers - could not verify the integrity of the %s file's data.\n", e, "pilots.dat");
        exit(1);
    }
    fclose(fptr);

    char **names_array = malloc(number_of_drivers * sizeof(char *));
    int **laps_array = malloc(number_of_drivers * sizeof(int *));
    int *dnf_status = malloc(number_of_drivers * sizeof(int));
    int *dnf_laps = malloc(number_of_drivers * sizeof(int));
    if (names_array == NULL || laps_array == NULL || dnf_status == NULL || dnf_laps == NULL)
    {
        printf("%s Error allocating memory.\n", e);
        printf("%s Exiting the program...\n", e);
    }

    populate_names(names_array, number_of_drivers);
    for (int i = 0; i < number_of_drivers; i++)
    {
        drivers_arr[i].has_dnf = 0;
        dnf_status[i] = 0;
        if((laps_array[i] = malloc(sizeof(int) * number_of_laps)) == NULL) {
            printf("%s Error allocating memory.\n", s);
            printf("%s Exiting the program..\n", info);
            exit(1);
        }
        strcpy(drivers_arr[i].name, names_array[i]);
    }

    // Generate laptimes for each driver
    generate_laptime(laps_array,
                     number_of_drivers,
                     number_of_laps,
                     dnf_status,
                     dnf_laps);

    // If there is a file print the output to the file
    if (argv[2] && sizeof(argv[2]) > 1)
    {
        char *filename = argv[2];
        wrote_file = print_to_file(filename,
                                   names_array,
                                   laps_array,
                                   number_of_drivers,
                                   number_of_laps,
                                   dnf_status,
                                   dnf_laps);
    }

    // Generate the output for the race
    printf("\n");
    int sum;
    for (int j = 0; j < number_of_drivers; j++)
    {
        memcpy(drivers_arr[j].lap_times, laps_array[j], number_of_laps * sizeof(int));
        // Free the memory of the arrays here so that there won't be any need for a separate loop to free the memory
        free(laps_array[j]);
        free(names_array[j]);
        sum = 0;
        drivers_arr[j].has_dnf = dnf_status[j];
        drivers_arr[j].dnf_lap = dnf_laps[j];
        printf("   %12s\t  ", drivers_arr[j].name);
        for (int i = 0; i < number_of_laps; i++)
        {
            if (drivers_arr[j].has_dnf == 1 && drivers_arr[j].dnf_lap <= i)
            {
                printf(" %3c ", '-');
                sum = 0;
            }
            else
            {
                printf(" %3d ", drivers_arr[j].lap_times[i]);
                sum += drivers_arr[j].lap_times[i];
            }
        }
        printf("  | ");
        if (sum == 0)
        {
            printf("  %s", "DNF");
        }
        else
        {
            printf("  %ds", sum);
            sum = 0;
        }

        printf("\n");
    }

    // CLEANUP
    free(names_array);
    free(laps_array);
    free(dnf_laps);
    free(dnf_status);

    if (wrote_file == WRITE_OUTPUT_SUCCESS)
    {
        printf("\n%s Results stored in %s in CSV format.\n", s, argv[2]);
        printf("%s For other programs to read the file as CSV, rename (or convert) the file extension to .csv (e.g. if your output file was results.txt).\n", info);
        return 0;
    }


    return 0;
}

