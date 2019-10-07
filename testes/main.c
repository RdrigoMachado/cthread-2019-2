#include <stdio.h>
#include "../include/cthread.h"
void* func1(void *arg) {
  printf("\n ihu \n");
  return NULL;
}

int main(){


  ccreate (func1, NULL, 0);
  return 0;
}
