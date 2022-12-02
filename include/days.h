
#include <stdlib.h>
#include "day1.h"

typedef int (*day_funtion_cb);

day_funtion_cb *get_days_functions(size_t *num_days);