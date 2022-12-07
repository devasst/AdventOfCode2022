
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/days.h"


day_function_cb_name __f_names = {
    "Calorie Counting",
    "Rock-Paper-Scissors"
};

//General

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

static char **__read_file_char_lines(const char *filepath, size_t *line_number){
    FILE *fp = fopen(filepath, "r");
    if(fp == NULL){
        return NULL;
    }

    char **lines = NULL;
    *line_number = 0;

    char line[1024] = {0};

    while(fgets(line, 1024, fp) != NULL){
        ++(*line_number);
        lines = realloc(lines, (*line_number)*sizeof(char**));
        lines[*line_number -1] = strdup(line);
    }
    
    fclose(fp);
    return lines;
}

//Day1

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

//Day2

#define ROCK_INDEX 0
#define PAPER_INDEX 1
#define SCISSORS_INDEX 2
int RPS_selection_score[3] = {1, 2, 3}; //{rock, paper, scissors
#define RPS_LOSS_INDEX 0
#define RPS_DRAW_INDEX 1
#define RPS_WIN_INDEX 2
int RPS_result_score[3] = {0, 3, 6}; //{loss, draw, win}


static int __rock_paper_scissors_to_index(char selection){
    switch (selection){
        case 'A':
        case 'X':
            return ROCK_INDEX; 
        case 'B':
        case 'Y':
            return PAPER_INDEX;
        case 'C':
        case 'Z':
            return SCISSORS_INDEX;
        default:
            return -1;
    }
}

static const char *__rock_paper_scissors_to_str(char selection){
    switch (selection){
        case 'A':
        case 'X':
            return "ROCK"; 
        case 'B':
        case 'Y':
            return "PAPER";
        case 'C':
        case 'Z':
            return "SCISSORS";
        default:
            return NULL;
    }
}

static int __rock_paper_scissors_prediction_to_index(char prediction){
    switch (prediction){
        case 'X':
            return RPS_LOSS_INDEX; 
        case 'Y':
            return RPS_DRAW_INDEX;
        case 'Z':
            return RPS_WIN_INDEX;
        default:
            return -1;
    }
}

static int __rock_paper_scissors_result(int elf, int other){
    if( elf == other ){
        return RPS_DRAW_INDEX;
    }

    switch (other){
        case ROCK_INDEX:
            if(elf == SCISSORS_INDEX){
                return RPS_WIN_INDEX;
            }
            break;
        case PAPER_INDEX:
            if(elf == ROCK_INDEX){
                return RPS_WIN_INDEX;
            }
            break;        
        case SCISSORS_INDEX:
            if(elf == PAPER_INDEX){
                return RPS_WIN_INDEX;
            }
            break;
        default:
            break;
    }

    return RPS_LOSS_INDEX;
}

static int __rock_paper_scissors_predict_result(int elf, int prediction){
    if( prediction == RPS_DRAW_INDEX ){
        return elf;
    }

    switch (elf){
        case ROCK_INDEX:
            if(prediction == RPS_WIN_INDEX){
                return PAPER_INDEX;
            }else{
                return SCISSORS_INDEX;
            }
            break;
        case PAPER_INDEX:
            if(prediction == RPS_WIN_INDEX){
                return SCISSORS_INDEX;
            }else{
                return ROCK_INDEX;
            }      
        case SCISSORS_INDEX:
            if(prediction == RPS_WIN_INDEX){
                return ROCK_INDEX;
            }else{
                return PAPER_INDEX;
            }
            break;
        default:
            break;
    }

    return -1;
}

static int __day2_rock_paper_scissors(void *data){
    int global_score = 0;
    size_t char_lines_number = 0;
    char **char_lines = __read_file_char_lines("../data/input2.txt", &char_lines_number);

    for(int i = 0; i < char_lines_number; ++i){
        int elf = __rock_paper_scissors_to_index(char_lines[i][0]);
        int other = __rock_paper_scissors_to_index(char_lines[i][2]);
        int result = __rock_paper_scissors_result(elf, other);
        global_score += RPS_selection_score[other] + RPS_result_score[result];
    }
    printf("PUZZLE1. Total score = %d\n", global_score);

    global_score = 0;
    for(int i = 0; i < char_lines_number; ++i){
        int elf = __rock_paper_scissors_to_index(char_lines[i][0]);
        int prediction = __rock_paper_scissors_prediction_to_index(char_lines[i][2]);
        int other = __rock_paper_scissors_predict_result(elf, prediction);
        int result = __rock_paper_scissors_result(elf, other);
        global_score += RPS_selection_score[other] + RPS_result_score[result];
        free(char_lines[i]); //free to avoid another loop just to destroy
    }
    free(char_lines);

    printf("PUZZLE2. Total score = %d\n", global_score);
    return 0;
}

//TODO: think about [char*] of [const char*]
day_funtion_cb *get_day_functions(day_function_cb_name *f_names, size_t num_days){
     // Day function callback names
    (*f_names)[0] = __f_names[0];
    (*f_names)[1] = __f_names[1];

    // Day function callback functions
    day_funtion_cb *day_functions = calloc(num_days, sizeof(day_funtion_cb));
    day_functions[0] = __day1_calorie_counting;
    day_functions[1] = __day2_rock_paper_scissors;
    return day_functions;
}

void day_funtion_cb_destroy(day_funtion_cb *day_functions){
    free(day_functions);
}
