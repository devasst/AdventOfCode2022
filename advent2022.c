

#include <stdio.h>
#include <stdbool.h>
#include "days.h"

static bool _running = true;

void int_handler() { _running = false; }

int main(int argc, char *argv[]) {

  while (_running) {
    printf("Testing Advent2022\n");
  }
  return 0;
}