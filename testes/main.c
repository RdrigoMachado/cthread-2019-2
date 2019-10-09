#include <stdio.h>
#include <stdlib.h>
#include "../include/cthread.h"
void func1(int *tids) {
	printf("Eu sou a thread ID %d fazendo join na thread ID %d\n", tids[0], tids[1]);
  cjoin(tids[1]);
	printf("Eu sou a thread ID %d: retorno cjoin(%d): \n",tids[0],tids[1]);
	printf("Eu sou a thread ID %d terminando\n", tids[0]);
}

void func2(int *tids) {
	printf("Eu sou a thread ID %d comecando\n", tids[1]);
	printf("Eu sou a thread ID %d terminando\n", tids[1]);
}

int main(){
	int tids[2];

	tids[0] = ccreate((void *(*)(void*))func1, (void*)&tids, 0);
	printf("main(): id1: %d\n",tids[0]);

	tids[1] = ccreate((void *(*)(void*))func2, (void*)&tids, 0);
	printf("main(): id2: %d\n",tids[1]);

	printf("main(): cjoin(%d)\n",tids[0]);
	printf("main(): retorno cjoin(): %d\n",cjoin(tids[0]));

	csem_t* semaforo = malloc(sizeof(csem_t));
	int recursos = 5;
	csem_init(semaforo, recursos);
		if(semaforo == NULL)
			printf("semaforo nulo\n");
		else
			printf("Sucesso ao criar semaforo\n#recursos: %d\n", semaforo->count);

		cwait(semaforo);


    return 0;
}
