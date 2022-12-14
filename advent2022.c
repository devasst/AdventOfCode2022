

#include <stddef.h>
#include <stdio.h>

#include "include/days.h"

static int _menu(day_funtion_cb *day_functions, char **day_functions_names, size_t num_days){
    int opt = 0;

    printf("\n--------------------------------------\n");
    printf("-------- Advent Of Code 2022 ---------\n");
    printf("--------------------------------------\n");
    printf("0. Exit\n");

    for(int i = 0; i < num_days; ++i){
        printf("%u. %s\n", i+1, day_functions_names[i]);
    }
    printf("Option:");

    scanf("%d", &opt);
    return opt;
}

int main(int argc, char *argv[]) {

    day_function_cb_name day_functions_names;
    day_funtion_cb *day_functions = get_day_functions(&day_functions_names, IMPLEMENTED_ADVENT_DAYS);

    int opt = -1;
    while (opt != 0) {
        opt = _menu(day_functions, (char**)day_functions_names, IMPLEMENTED_ADVENT_DAYS);
        if(opt > 0 && opt <= IMPLEMENTED_ADVENT_DAYS){
            day_functions[opt-1](NULL);
        }
    }

    day_funtion_cb_destroy(day_functions);
    return 0;
}