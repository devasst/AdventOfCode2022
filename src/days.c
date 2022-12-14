
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "../include/days.h"
#include "../include/stack.h"


day_function_cb_name __f_names = {
    "Calorie Counting",
    "Rock-Paper-Scissors",
    "Rucksack Reorganization",
    "Camp Cleanup",
    "Supply Stacks"
};

//General

#define SET_BIT(p, n) ( (p) |= ( (1U) << (n) ) )
#define CLEAR_BIT(p, n) ( (p) &= ~( (1U) << (n) ) )
#define TOGGLE_BIT(p, n) ( (p) ^= ( (1U) << (n) ) )
#define CHECK_BIT(p, n) ( ((p) >> (n)) & (1U) )

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

//Day3
#define ALPHABET_MAYUS_NUMBER 26
#define ALPHABET_MINUS_NUMBER 26
#define ALPHABET_MAYUS_OFFSET 65 // A-Z (0-25)
#define ALPHABET_MINUS_OFFSET (97 - ALPHABET_MAYUS_NUMBER) // a-z (26-51)
#define FOUND_ALPHABET_FIRST_HALF 0x01
#define FOUND_ALPHABET_SECOND_HALF 0x02
#define ALPHABET_MAYUS_PRIORITY 27
#define ALPHABET_MINUS_PRIORITY 1
#define FOUND_BADGE1 0x01
#define FOUND_BADGE2 0x02
#define FOUND_BADGE3 0x04

static int __day3_rucksack_reorganization(void *data){
    size_t items_size = 0;
    char **items = __read_file_char_lines("../data/input3.txt", &items_size);
    uint8_t found_alphabet[ALPHABET_MAYUS_NUMBER+ALPHABET_MINUS_NUMBER] = {0}; // A-Z (0-25) + a-z (26-51)
    int total_item_in_both_compartments_prio = 0, total_badge_prio = 0;
    uint8_t group_member_mask = FOUND_BADGE1;

    //Puzzle 1
    for(int i = 0 ; i < items_size; ++i){
        size_t half_size = strlen(items[i]) / 2;
        char item_in_both_compartments = 0;
        int item_in_both_compartments_prio = 0;
        memset(found_alphabet, 0, ALPHABET_MAYUS_NUMBER+ALPHABET_MINUS_NUMBER);
        //printf("%.*s - %.*s \n", (int)half_size, items[i], (int)half_size, items[i]+half_size);

        for( int j = 0; j < half_size; ++j){
            int found_alphabet_index = 0;
            //First half
            if(items[i][j] >= 'A' && items[i][j] <= 'Z'){
                found_alphabet_index = items[i][j] - ALPHABET_MAYUS_OFFSET;
            }else if (items[i][j] >= 'a' && items[i][j] <= 'z'){
                found_alphabet_index = items[i][j] - ALPHABET_MINUS_OFFSET;
            }

            if(CHECK_BIT(found_alphabet[found_alphabet_index], FOUND_ALPHABET_SECOND_HALF)){
                item_in_both_compartments = items[i][j];
                item_in_both_compartments_prio = found_alphabet_index < ALPHABET_MAYUS_NUMBER 
                                                 ? found_alphabet_index + ALPHABET_MAYUS_PRIORITY 
                                                 : found_alphabet_index + ALPHABET_MINUS_PRIORITY - ALPHABET_MAYUS_NUMBER;
                break;
            }
            if (!CHECK_BIT(found_alphabet[found_alphabet_index], FOUND_ALPHABET_FIRST_HALF)){
                found_alphabet[found_alphabet_index] = SET_BIT(found_alphabet[found_alphabet_index], FOUND_ALPHABET_FIRST_HALF);
            }

            //Second half
            if(items[i][j+half_size] >= 'A' && items[i][j+half_size] <= 'Z'){
                found_alphabet_index = items[i][j+half_size] - ALPHABET_MAYUS_OFFSET;
            }else if (items[i][j+half_size] >= 'a' && items[i][j+half_size] <= 'z'){
                found_alphabet_index = items[i][j+half_size] - ALPHABET_MINUS_OFFSET;
            }

            if(CHECK_BIT(found_alphabet[found_alphabet_index], FOUND_ALPHABET_FIRST_HALF)){
                item_in_both_compartments = items[i][j+half_size];
                item_in_both_compartments_prio = found_alphabet_index < ALPHABET_MAYUS_NUMBER 
                                                 ? found_alphabet_index + ALPHABET_MAYUS_PRIORITY 
                                                 : found_alphabet_index + ALPHABET_MINUS_PRIORITY - ALPHABET_MAYUS_NUMBER;                
                break;
            }
            if (!CHECK_BIT(found_alphabet[found_alphabet_index], FOUND_ALPHABET_SECOND_HALF)){
                found_alphabet[found_alphabet_index] = SET_BIT(found_alphabet[found_alphabet_index], FOUND_ALPHABET_SECOND_HALF);
            }
        }
        //printf("Double item: %c -> %d with priority %u\n", item_in_both_compartments, item_in_both_compartments, item_in_both_compartments_prio);
        total_item_in_both_compartments_prio += item_in_both_compartments_prio;
    }
    printf("PUZZLE1: Double item total priority -> %u\n", total_item_in_both_compartments_prio);

    //Puzzle 2
    for(int i = 0 ; i < items_size; ++i){
        size_t item_size = strlen(items[i]);
        char badge_item = 0;
        int badge_item_prio = 0;
        
        if( i % 3 == 0){
            //reset for each group of 3
            memset(found_alphabet, 0, ALPHABET_MAYUS_NUMBER+ALPHABET_MINUS_NUMBER);
            group_member_mask = FOUND_BADGE1;
        }

        for( int j = 0; j < item_size; ++j){
            int found_alphabet_index = 0;
            if(items[i][j] >= 'A' && items[i][j] <= 'Z'){
                found_alphabet_index = items[i][j] - ALPHABET_MAYUS_OFFSET;
            }else if (items[i][j] >= 'a' && items[i][j] <= 'z'){
                found_alphabet_index = items[i][j] - ALPHABET_MINUS_OFFSET;
            }else{
                continue;
            }

            if(!CHECK_BIT(found_alphabet[found_alphabet_index], group_member_mask)){
                found_alphabet[found_alphabet_index] = SET_BIT(found_alphabet[found_alphabet_index], group_member_mask);
            }

            if( CHECK_BIT(found_alphabet[found_alphabet_index], FOUND_BADGE1) && 
                CHECK_BIT(found_alphabet[found_alphabet_index], FOUND_BADGE2) && 
                CHECK_BIT(found_alphabet[found_alphabet_index], FOUND_BADGE3) ){
                badge_item = items[i][j];
                badge_item_prio =   found_alphabet_index < ALPHABET_MAYUS_NUMBER 
                                    ? found_alphabet_index + ALPHABET_MAYUS_PRIORITY 
                                    : found_alphabet_index + ALPHABET_MINUS_PRIORITY - ALPHABET_MAYUS_NUMBER;
                //printf("Badge item: %c -> %d with priority %u\n", badge_item, badge_item, badge_item_prio);
                total_badge_prio += badge_item_prio;
                break;
            }
        }

        switch (group_member_mask) {
            case FOUND_BADGE1:
                group_member_mask = FOUND_BADGE2;
                break;
            case FOUND_BADGE2:
                group_member_mask = FOUND_BADGE3;
                break;
            case FOUND_BADGE3:
            default:
                group_member_mask = FOUND_BADGE1;
                break;
        }

        free(items[i]); //free to avoid another loop just to destroy
    }
    printf("PUZZLE2: Badge item total priority -> %u\n", total_badge_prio);

    free(items);

    
    return 0;
}

//Day4
typedef struct __day4_clean_range_s{
    uint8_t from;
    uint8_t to;
    uint8_t overlapped;
} __day4_clean_range_t;

static int __day4_camp_cleanup(void *data){
    size_t items_number = 0;
    char **items = __read_file_char_lines("../data/input4.txt", &items_number);
    int overlapped_ranges = 0;

    for(int i = 0; i < items_number; ++i){
        __day4_clean_range_t member1, member2;
        sscanf(items[i], "%hhu-%hhu,%hhu-%hhu", &member1.from, &member1.to, &member2.from, &member2.to);
        if( (member1.from >= member2.from && member1.to <= member2.to) || 
            (member2.from >= member1.from && member2.to <= member1.to) ){
            overlapped_ranges++;
        }
    }
    printf("PUZZLE1: Pair Overlapped ranges -> %d\n", overlapped_ranges);


    overlapped_ranges = 0;
    for(int i = 0; i < items_number; ++i){
        __day4_clean_range_t member1, member2, member3, member4;

        sscanf(items[i], "%hhu-%hhu,%hhu-%hhu", &member1.from, &member1.to, &member2.from, &member2.to);

        if( (member1.from >= member2.from && member1.from <= member2.to) || 
            (member1.to >= member2.from && member1.to <= member2.to) || 
            (member2.from >= member1.from && member2.from <= member1.to) ||
            (member2.to >= member1.from && member2.to <= member1.to) ){
            overlapped_ranges++;
        }
        free(items[i]);
    }
    free(items);

    printf("PUZZLE2: Total Overlapped ranges -> %u\n", overlapped_ranges);
    return 0;
}

//Day5
#define CrateMover9000 0
#define CrateMover9001 1

static void __day5_stack_add_crate(stack_t *stack, char crate_ID){
    if(stack == NULL){
        return;
    }
    char *ID = (char*)calloc(2, sizeof(char));
    ID[0] = crate_ID;
    stack_push_node(stack, ID);
}

static char __day5_stack_pop_crate(stack_t *stack){
    if(stack == NULL){
        return '\0';
    }
    stack_node_t *node = stack_pop_node(stack);
    char ret = *((char*)stack_node_get_value(node));
    stack_destroy_node(stack, node);
    return ret;
}

static void __day5_stack_move_crate_CrateMover9000(stack_t *stack_from, stack_t *stack_to){
    char crate_to_move = __day5_stack_pop_crate(stack_from);
    __day5_stack_add_crate(stack_to, crate_to_move);
}

static void __day5_stack_move_crate_CrateMover9001(stack_t *stack_from, stack_t *stack_to, int num_crates_to_move){
    char *crates_to_move = (char*)calloc(num_crates_to_move, sizeof(char));
    for(int i = num_crates_to_move-1; i >= 0; --i){
        char crate_to_move = __day5_stack_pop_crate(stack_from);
        crates_to_move[i] = crate_to_move;
    }
    for(int i = 0; i < num_crates_to_move; ++i){
        __day5_stack_add_crate(stack_to, crates_to_move[i]);
    }
    free(crates_to_move);
}

static int __day5_supply_stacks(void *data){
    size_t lines_number = 0;
    char **content_lines = __read_file_char_lines("../data/input5.txt", &lines_number);

    int stacks_number = 0, i = 0;

    //FIXME: define which CrateMover is used
    // - CrateMover9000: one crate by one
    // - CrateMover9001: multiple crates at once
    int model_CrateMover = CrateMover9001;

    //Get initial stacks and crate positions
    for(i = 0; i < lines_number; ++i){
        if(strstr(content_lines[i], "[") != NULL){
            continue;
        }
        
        char *line = strdup(content_lines[i]), *p_line = line, *stack;
        while( (stack = strtok_r(line, " ", &line)) != NULL){
            int stack_num = atoi(stack);
            if(stack_num > stacks_number){
                stacks_number = stack_num;
            }
        }
        free(p_line);
        break;
    }
    printf("Number of Stacks: %d\n", stacks_number);
    stack_t **stacks = (stack_t**)calloc(stacks_number, sizeof(stack_t*));
    for(int j = 0; j < stacks_number; ++j){
        stacks[j] = stack_new(free);
    }
    for(int j = i - 1; j >= 0; --j){
        int cID_offset = 1; // '['
        int cIDX = 0;
        //printf("Row %d of crates: %s\n", j, content_lines[j]);
        while(cID_offset < strlen(content_lines[j])){
            if(content_lines[j][cID_offset] >= 'A' && content_lines[j][cID_offset] <= 'Z'){
                __day5_stack_add_crate(stacks[cIDX], content_lines[j][cID_offset]);
                //printf("Added crate %c to stack %d\n", content_lines[j][cID_offset], cIDX);
            }
            cID_offset += 4; // 'ID' + ']' + ' ' + '['
            ++cIDX;
        }
    }

    //Follow instructions and move crates
    size_t lenght_min = strlen("move X from Y to Z");
    int instruction_number = 1;
    for(int k = i+1; k < lines_number; ++k){
        if(strlen(content_lines[k]) < lenght_min){
            continue;
        }
        //printf("Instruction %d -> %s", instruction_number++, content_lines[k]);
        int crates_to_move = 0, stack_from = -1, stack_to = -1;
        sscanf(content_lines[k], "move %d from %d to %d\n", &crates_to_move, &stack_from, &stack_to);
        if(stack_from != -1 && stack_to != -1 && crates_to_move > 0){
            if(model_CrateMover == CrateMover9000){
                for(int crate = 0; crate < crates_to_move ; ++crate){
                    __day5_stack_move_crate_CrateMover9000(stacks[stack_from-1], stacks[stack_to-1]);
                    //printf("Moved 1 from %d to %d\n", stack_from, stack_to);
                }
            }else if(model_CrateMover == CrateMover9001){
                __day5_stack_move_crate_CrateMover9001(stacks[stack_from-1], stacks[stack_to-1], crates_to_move);
                //printf("Moved %d from %d to %d\n", crates_to_move, stack_from, stack_to);
            }
        }
    }

    //Print upper crates
    char *upper_crates_ID = (char*)calloc(stacks_number+1, sizeof(char));
    for(int j = 0; j < stacks_number; ++j){
        upper_crates_ID[j] = __day5_stack_pop_crate(stacks[j]);
    }
    printf("%s: Upper Crates IDs -> %s\n", model_CrateMover == CrateMover9000 ? "PUZZLE1" : "PUZZLE2", upper_crates_ID);
    free(upper_crates_ID);

    //Clean and destroy
    for(int j = 0; j < stacks_number; ++j){
        stack_destroy(stacks[j]);
    }
    free(stacks);
    for(i = 0; i < lines_number; ++i){
        free(content_lines[i]);
    }
    free(content_lines);

    return 0;
}

//TODO: think about [char*] of [const char*]
day_funtion_cb *get_day_functions(day_function_cb_name *f_names, size_t num_days){
     // Day function callback names
     for(int i = 0; i < num_days; ++i){
        (*f_names)[i] = __f_names[i];
     }

    // Day function callback functions
    day_funtion_cb *day_functions = calloc(num_days, sizeof(day_funtion_cb));
    day_functions[0] = __day1_calorie_counting;
    day_functions[1] = __day2_rock_paper_scissors;
    day_functions[2] = __day3_rucksack_reorganization;
    day_functions[3] = __day4_camp_cleanup;
    day_functions[4] = __day5_supply_stacks;
    return day_functions;
}

void day_funtion_cb_destroy(day_funtion_cb *day_functions){
    free(day_functions);
}
