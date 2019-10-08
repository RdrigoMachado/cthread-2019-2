#include <stdio.h>
#include "../include/cthread.h"
int i = 2;
void* func1(void *arg) {
  i++;
  if(i%2 == 0){
    cyield();
  }
  printf("\n ihu %d\n", i);

  return NULL;
}

int main(){

  ccreate(func1, NULL, 0);
  ccreate(func1, NULL, 0);
  ccreate(func1, NULL, 0);
  ccreate(func1, NULL, 0);
  ccreate(func1, NULL, 0);
  ccreate(func1, NULL, 0);

  cyield();

  printf("Terminou. i = %d\n", i);
  return 0;
}
