#include <stdio.h>
#pragma once
#include "check_verify.h"

#define DNF_LAPTIME 45

int print_to_file(char *filename, char **dnames, int **lap_times, int total_pilots, int laps_num, int *has_dnf, int *dnf_lapnum);
