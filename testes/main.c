#include <stdio.h>
#include "../include/cthread.h"
int i = 1;
void* func1(void *arg) {
  printf("\n ihu %d\n", i);
  i++;
  return NULL;
}

int main(){

  printf("\n[=CREATE 1=]\n");
  ccreate(func1, NULL, 0);
  printf("\n[=YIELD 1=]\n");
  cyield();
  printf("\n[=CREATE 2=]\n");
  ccreate(func1, NULL, 0);
  printf("\n[=YIELD 2=]\n");
  cyield();
  printf("\n[=CREATE 3=]\n");
  ccreate(func1, NULL, 0);
  printf("\n[=YIELD 3=]\n");
  cyield();

  printf("Terminou. i = %d\n", i);
  return 0;
}
