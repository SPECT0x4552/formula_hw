#include <stdio.h>
#include <stdlib.h>
#pragma once
#include "check_verify.h"
#define MAX_LAPTIME 125
#define MIN_LAPTIME 70


void generate_laptime(int **lap_times, int total_drivers, int total_laps, int *dnf_status, int *dnf_lap);
void populate_names(char **destination_arr, int num_of_drivers);
