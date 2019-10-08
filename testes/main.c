#include <stdio.h>
#include "../include/cthread.h"
int i = 0;
void* func1(void *arg) {
  i++;
  printf("THREAD%d\n", i);
  return NULL;
}
void* vaiSerEsperada(void *arg) {
  i++;
  printf("THREAD%d ", i);
  printf("[Liberando THREAD]\n");
  return NULL;
}

int main(){

  ccreate(func1, NULL, 0);
  ccreate(func1, NULL, 0);
  ccreate(func1, NULL, 0);
  int seraEsperado = ccreate(vaiSerEsperada, NULL, 0);
  ccreate(func1, NULL, 0);
  ccreate(func1, NULL, 0);
  cjoin(seraEsperado);
  cyield();

  printf("Terminou. i = %d\n", i);
  return 0;
}
