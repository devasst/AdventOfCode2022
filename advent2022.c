

#include <stddef.h>
#include <stdio.h>

#include "include/days.h"

static int _menu(day_funtion_cb *day_functions, char **day_functions_names, size_t num_days){
    int opt = 0;

    for(int i = 0; i < num_days; i++){
        printf("%u. %s\n", i+1, day_functions_names[i]);
    }
    scanf("%u", &opt);
    return opt;
}

int main(int argc, char *argv[]) {

    size_t num_days = 1;
    day_function_cb_name day_functions_names;
    day_funtion_cb *day_functions = get_day_functions(&day_functions_names, num_days);

    int opt = -1;
    while (opt != 0) {
        opt = _menu(day_functions, (char**)day_functions_names, num_days);
        if(opt > 0 && opt < num_days){
            day_functions[opt-1](NULL);
        }
    }

    day_funtion_cb_destroy(day_functions);
    return 0;
}