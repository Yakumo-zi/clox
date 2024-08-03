#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

void print_binary(int num, int count) {
  char *str = calloc(count, sizeof(char));
  while (num != 0) {
    if (num & 1) {
      str[count - 1] = '1';
    } else {
      str[count - 1] = '0';
    }
    num >>= 1;
    count -= 1;
  }
  while (count--) {
    str[count] = '0';
  }
  printf("%s\n", str);
}
