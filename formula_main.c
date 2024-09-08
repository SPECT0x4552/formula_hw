#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int verify_data(FILE *fd, int drivers_num);
FILE* open_file(char file_name[], char *file_mode);


int main(int argc, char* argv[]) {

    srand(time(0));
    char *e = "[-]"; // Indicates error
    char *i = "[*]"; // Indicates additional information

    if(argc <= 1 || argc > 3) {
        printf("%s Usage: ./formula <Number of pilots> <(Optional) Output file>", e);
        return 1;
    }
    FILE* fptr;
    int number_of_laps; 
    int received_drivers;
    int number_of_drivers = atoi(argv[1]);
    char data_name[] = "pilots.dat";
    int driver_index = 0;
    int lap_counter = 0;




    if(number_of_drivers <= 1 || number_of_drivers > 10) {
        printf("%s Invalid number of pilots!\n", e);
        return 1;
    }

    printf("Number of laps: ");
    scanf("%d", &number_of_laps);
    printf("\n");
    
    if(number_of_laps < 5 || number_of_laps > 15) {
        printf("%s Invalid number of laps!\n",e);
        printf("%s Enter a number between 5-15", i);
        return 1;
    }
    
    fptr = open_file(data_name, "r");
   
    if(number_of_drivers != (received_drivers = verify_data(fptr, number_of_drivers))) {
        printf("%s Error reading the data file\n", e);
        return 1;
    }

    fptr = open_file(data_name, "r");

    // Return a pointer to the allocated buffer for the structs
    typedef struct single_driver {
        int index;
        char name[32];
        int lap_times[15];
        int dnf_lap;
        int has_dnf;
        int race_result;

    } driver, *p_driver;


    driver drivers[10];
   
    int step = 1;
    int generate_range = 3333 / number_of_drivers;

    // Populate the structs
    do {
        fscanf(fptr, " %[^' ']s", drivers[driver_index].name);
        drivers[driver_index].has_dnf = 0;
        drivers[driver_index].race_result = 0;
        driver_index++;

    } while(!feof(fptr) && driver_index < number_of_drivers);

    fclose(fptr);



    // Loop where the race begins                    
    while(lap_counter < number_of_laps) {
        int generate_dnf = rand() % 3333+1;
        for(int i = 0; i < number_of_drivers; i++) {
            drivers[i].index = rand() % 100+1;
            if(drivers[i].index <= 3) {
                if(drivers[i].has_dnf != 1) {
                    drivers[i].has_dnf = 1;
                    drivers[i].dnf_lap = lap_counter;
                    drivers[i].race_result = 0;
                }
                drivers[i].lap_times[lap_counter] = 45;
            } else {
                drivers[i].lap_times[lap_counter] = rand() % (125 - 70 + 1) + 70;  
                drivers[i].race_result += drivers[i].lap_times[lap_counter]; 
            }

            
            
        }
        generate_dnf = 0;
        lap_counter++;
    }

    // Loop to print out the results of the race
    for(int i = 0; i < number_of_drivers; i++) {
        printf("   %15s", drivers[i].name);
        printf("  \t");
        for(int j = 0; j < number_of_laps; j++) {
            if(drivers[i].has_dnf == 1 && j >= drivers[i].dnf_lap) {
                printf(" %4c ", 45);
            } else {
                printf(" %4d ", drivers[i].lap_times[j]);
            }
        }

        printf("|   ");        
        if(drivers[i].has_dnf == 1) {
            printf("DNF\n");
        } else { 
            printf("%ds", drivers[i].race_result);
            printf("\n");
        }
        
    }
    printf("\n");

   
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

    fclose(file_descriptor);
    return drivers_count;
}


FILE* open_file(char file_name[], char *file_mode) {
    FILE* fptr;
    if((fptr = fopen(file_name, file_mode)) == NULL) {
        printf("Failed to retrieve a valid file descriptor.\n");
        exit(1);
    }

    return fptr;
}







