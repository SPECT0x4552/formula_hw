#include "check_verify.h"

int verify_data(FILE *file_descriptor, int drivers_num)
{
    char c;
    int drivers_count = 0;

    while ((c = fgetc(file_descriptor)) != EOF && drivers_count != drivers_num)
    {
        if (c == '.')
        {
            drivers_count++;
        }
    }

    return drivers_count;
}

int basic_checks(int arg_count, int total_drivers)
{
    if (arg_count <= 1 || arg_count > 3)
    {
        return 1;
    }

    if (total_drivers < MIN_DRIVERS || total_drivers > MAX_DRIVERS)
    {
        printf("[*] A minimum of 2 drivers and a maximum of 10 drivers is required for the race results.\n");
        return 1;
    }

    return 0;
}

void fd_error(FILE *fpointer)
{
    if (fpointer == NULL)
    {
        printf("[-] Couldn't retreive a valid file handler.\n");
        printf("[*] Exiting the program...\n");
        exit(1);
    }
}