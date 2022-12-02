

#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include "days.h"

static bool _running = true;

void sigint_handler() { _running = false; }

int main(int argc, char *argv[]) {

    signal(SIGINT, sigint_handler);
    
    while (_running) {
        printf("Testing Advent2022\n");
    }

    printf("Ending...\n");
    return 0;
}