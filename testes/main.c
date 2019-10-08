#include <stdio.h>
#include "../include/cthread.h"
int i = 1;
void* func1(void *arg) {
  printf("\n ihu %d\n", i);
  i++;
  return NULL;
}

int main(){

  ccreate(func1, NULL, 0);
  ccreate(func1, NULL, 0);
  printf("Terminou. i = %d\n", i);
  return 0;
}
