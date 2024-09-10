#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int verify_data(FILE *fd, int drivers_num);
int basic_checks(int args, int names_number);
void fd_error(FILE *fpointer);
void populate_names(char **destination_arr, int num_of_drivers);
void generate_laptime(int **lap_times, int total_drivers, int total_laps, int *dnf_status, int *dnf_lap);
int print_to_file(char *filename, char **dnames, int **lap_times, int total_pilots, int laps_num, int *has_dnf, int *dnfd_lap);


int main(int argc, char* argv[]) {

    srand(time(0));
    char *e = "[-]"; // Indicates error
    char *i = "[*]"; // Indicates additional information
    char *s = "[+]"; // Indicates success
    int number_of_laps;
    int number_of_drivers;
    int received_drivers = 0;
    int maximum_input = 30;
    int wrote_file = 0;
    FILE *fptr;

    typedef struct single_driver {
        int index;
        char name[32];
        int lap_times[15];
        int dnf_lap;
        int has_dnf;
        int race_result;

    } driver, *p_driver;

    driver drivers_arr[10];

    if(!argv[1] || (basic_checks(argc, atoi(argv[1])) != 0))  {
            printf("%s Usage: ./formula <Number of pilots> <(Optional) Output file>\n", e);
            exit(1); 
    }

    if(argv[2]) {
        if(strlen(argv[2]) > maximum_input) {
            printf("%s Output file name is limited to a maximum of %d characters.\n", e, maximum_input);
            printf("Exiting the program...\n");
            exit(1);
        }
    }
    
    number_of_drivers = atoi(argv[1]);
    
    printf("Number of laps: ");
    int read = scanf("%d", &number_of_laps);
    if(number_of_laps < 5 || number_of_laps > 15) {
        printf("%s Invalid number of laps!\n",e);
        printf("%s Enter a number between 5-15\n", i);
        exit(1);
    }
    
    fd_error(fptr= fopen("pilots.dat", "r"));
    
    // Check if there are required amount of drivers available in the given file
    if(number_of_drivers != (received_drivers = verify_data(fptr, number_of_drivers))) {
        printf("%s Not enough drivers - could not verify the integrity of the %s file's data.\n", e, "pilots.dat");
        exit(1);
    } 
   fclose(fptr);

    char **names_array = malloc(number_of_drivers * sizeof(char *));
    int **laps_array = malloc(number_of_drivers * sizeof(int *));
    int *dnf_status = malloc(number_of_drivers * sizeof(int));
    int *dnf_laps = malloc(number_of_drivers*sizeof(int));
    if(names_array == NULL || laps_array == NULL || dnf_status == NULL || dnf_laps == NULL) {
        printf("%s Error allocating memory.\n", e);
    }

    populate_names(names_array, number_of_drivers);
    for(int i = 0; i < number_of_drivers; i++) {
        drivers_arr[i].has_dnf = 0;
        dnf_status[i] = 0;
        laps_array[i] = malloc(sizeof(int) * number_of_laps);
        if(laps_array[i] == NULL) {
            printf("%s Error allocating memory.\n", e);
        }
        strcpy(drivers_arr[i].name, names_array[i]);
        
    }

    // Generate laptimes for each driver
    generate_laptime(laps_array, number_of_drivers, number_of_laps, dnf_status, dnf_laps);

    // If there is a file print the output to the file
    if(argv[2] && sizeof(argv[2]) > 1) {  
        char *filename = argv[2];
        wrote_file = print_to_file(filename, names_array,laps_array, number_of_drivers, number_of_laps, dnf_status, dnf_laps);
    }

    // Generate the output for the race
    printf("\n");
    int sum;
    for(int j = 0; j < number_of_drivers; j++) {
        memcpy(drivers_arr[j].lap_times, laps_array[j], number_of_laps*sizeof(int));
        // Free the memory of the arrays here so that won't be any need for a separate loop
        free(laps_array[j]);
        free(names_array[j]);
        sum = 0;
        drivers_arr[j].has_dnf = dnf_status[j];
        drivers_arr[j].dnf_lap = dnf_laps[j];
        printf("   %12s\t  ", drivers_arr[j].name);
        for(int i = 0; i < number_of_laps; i++) {
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

    

    // CLEANUP
    free(names_array);
    free(laps_array);
    free(dnf_laps);
    free(dnf_status);

    if(wrote_file == 2) {  
        printf("\n%s Results stored in %s in CSV format.\n", s, argv[2]);
        printf("%s For other programs to read the file as CSV, rename the file extension to .csv (e.g. if your output file was results.txt).\n", i);
        return 0;
    }


    return 0;
}

int verify_data(FILE *file_descriptor, int drivers_num) {
    char c;
    int drivers_count = 0;

    while((c = fgetc(file_descriptor)) != EOF && drivers_count != drivers_num) {
        if(c == '.') {
            drivers_count++;
        }
    } 

    return drivers_count;
}

void fd_error(FILE *fpointer) {
    if(fpointer == NULL) {
        printf("[-] Couldn't retreive a valid file handler.\n");
        printf("Exiting the program...\n");
        exit(1);
    }
}


int basic_checks(int arg_count,int total_drivers) {

    if(arg_count <= 1 || arg_count > 3) {
        return 1;
    }

    if(total_drivers < 2 || total_drivers > 10) {
        printf("[*] A minimum of 2 drivers and a maximum of 10 drivers is required for the race results.\n");
        return 1;
    }

    return 0;

}

void populate_names(char **destination_arr, int num_of_drivers) {
    int index = 0;
    FILE *fptr;
    fd_error(fptr = fopen("pilots.dat", "r"));

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


int print_to_file(char *filename, char **dnames, int **lap_times, int total_drivers, int total_laps, int *dnf_status, int *dnf_lap) {
    FILE *fptr;
    int laptime_sum;
    int counter_columns = 0;
    fd_error(fptr = fopen(filename, "w+"));
    
    // Write the header names
    fprintf(fptr, "DRIVER NAME,");
    while(counter_columns < total_laps) {
        // Write the header names for laps
        fprintf(fptr, "LAP %d,", counter_columns+1);
        counter_columns++;
    }
    // Write the header name for result
    fprintf(fptr, "RESULT\n");

    for(int i = 0; i < total_drivers; i++) {
        laptime_sum = 0;
        fprintf(fptr, "%s,", dnames[i]);
        for(int j = 0; j < total_laps; j++) {
            if(dnf_status[i] == 1) {
                    fprintf(fptr, "%c,", 45);
            } else {
                fprintf(fptr, "%d,", lap_times[i][j]);
                laptime_sum += lap_times[i][j];
            }
            
        }
        if(dnf_status[i]) {
            fprintf(fptr,"DNF\n");
        } else {
            fprintf(fptr, "%ds\n",laptime_sum);
        }
        
    }

    
    fclose(fptr);
    return 2;

}




