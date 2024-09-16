#pragma once
#include <stdio.h>
#include <stdlib.h>
#define MIN_DRIVERS 2
#define MAX_DRIVERS 10

int basic_checks(int args, int names_number);
int verify_data(FILE *fd, int drivers_num);
void fd_error(FILE *fpointer);

