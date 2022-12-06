
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/days.h"


day_function_cb_name __f_names = {
    "Calorie Counting"
};

static char *__read_file_to_char(const char *filepath){
    FILE *fp = fopen(filepath, "r");
    if(fp == NULL){
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    size_t file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *content = (char *)calloc(file_size, sizeof(char));
    size_t content_size = fread(content, file_size, 1, fp);

    fclose(fp);
    return content;
}

static int *__read_file_number_lines(const char *filepath, size_t *line_number){
    FILE *fp = fopen(filepath, "r");
    if(fp == NULL){
        return NULL;
    }

    int *lines = NULL;
    *line_number = 0;

    char line[1024] = {0};

    while(fgets(line, 1024, fp) != NULL){
        ++(*line_number);
        lines = realloc(lines, (*line_number)*sizeof(int));
        lines[*line_number -1] = strlen(line) > 0 ? atoi(line) : 0;
    }
    
    fclose(fp);
    return lines;
}

static int __day1_calorie_counting(void *data){
    int max_calories[3] = {0}, actual_calories = 0;
    size_t number_lines = 0;
    int *input = __read_file_number_lines("../data/input1.txt", &number_lines);
    for(int i = 0; i < number_lines; ++i){
        actual_calories += input[i];
        if(input[i] == 0 || i == number_lines - 1){
            for(int j = 0; j < 3; ++j){
                if(actual_calories > max_calories[j]){
                    int k = 2;
                    while (k > j){
                        max_calories[k] = max_calories[k-1];
                        --k;
                    }
                    max_calories[j] = actual_calories;
                    break;
                }
            }
            actual_calories = 0;
            continue;
        }
    }
    printf("Max. calories 1 = %d\n", max_calories[0]);
    printf("Max. calories 2 = %d\n", max_calories[1]);
    printf("Max. calories 3 = %d\n", max_calories[2]);
    printf("Max. calories Top3 = %d", max_calories[0] +max_calories[1] +max_calories[2]);

    free(input);
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
