
#include <stdlib.h>

#define IMPLEMENTED_ADVENT_DAYS 4
#define MAX_ADVENT_DAYS 25

typedef int (*day_funtion_cb)(void *data);
typedef const char *(day_function_cb_name[MAX_ADVENT_DAYS]);

day_funtion_cb *get_day_functions(day_function_cb_name *f_names, size_t num_days);
void day_funtion_cb_destroy(day_funtion_cb *day_functions);