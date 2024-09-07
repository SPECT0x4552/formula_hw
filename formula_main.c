#include <stdio.h>
#include <stdlib.h>

int verify_data(FILE *fd, int drivers_num);
FILE* open_file(char file_name[], char *file_mode);
void read_names(char destination[], char *f_mode, int d_num);



int main(int argc, char* argv[]) {

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
    char names_of_drivers[320];



    if(number_of_drivers <= 1 || number_of_drivers > 10) {
        printf("%s Invalid number of pilots!\n", e);
        return 1;
    }

    printf("Number of laps: ");
    scanf("%d", &number_of_laps);
    
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

    read_names(names_of_drivers, fptr, number_of_drivers);
    


    // Return a pointer to the allocated buffer for the structs
    typedef struct single_driver {
        int index;
        char name[32];
        int number_of_laps;
        int lap_times;
        int has_dnf;

    } driver, *p_driver;

    p_driver* drivers_structs_array;
    drivers_structs_array = (p_driver *)malloc(sizeof(driver) * number_of_drivers);

    


    
    printf("Done\n");
   
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



int generate_laptime(int lap_number) {
    // Run this before each new lap
}



FILE* open_file(char file_name[], char *file_mode) {
    FILE* fptr;
    if((fptr = fopen(file_name, file_mode)) == NULL) {
        printf("Failed to retrieve a valid file descriptor.\n");
        exit(1);
    }

    return fptr;
}

void read_names(char destination[], FILE* file_pointer, int d_num) {
    char c;
    while(fscanf()) {

    }
}






