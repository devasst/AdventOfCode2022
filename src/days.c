
#include <stdlib.h>
#include <string.h>
#include "../include/days.h"


day_function_cb_name __f_names = {
    "Calorie Counting"
};

static int __day1_calorie_counting(void *data){


    return 0;
}

//TODO: think about [char*] of [const char*]
day_funtion_cb *get_day_functions(day_function_cb_name *f_names, size_t num_days){
     // Day function callback names
    *f_names[0] = __f_names[0];

    // Day function callback functions
    day_funtion_cb *day_functions = calloc(num_days, sizeof(day_funtion_cb));
    day_functions[0] = __day1_calorie_counting;
    return day_functions;
}

void day_funtion_cb_destroy(day_funtion_cb *day_functions){
    free(day_functions);
}
