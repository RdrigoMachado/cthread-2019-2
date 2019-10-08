#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../include/estruturas.h"
#define STACK_SIZE SIGSTKSZ
#include <signal.h>

#define TRUE 1
#define FALSE 0
#define SUCESSO 0
#define ERRO -1


int iniciada = FALSE;
int tid;
TCB_t* main_tcb;
PFILA2 listaDePrioridades;
TCB_t* executando;
ucontext_t escalonador;

int novoTID(){
	tid++;
	return tid;
}

void* despachante(void* arg){
	printf("despachante\n" );
	listar();
	executando = devolverERetirarTCBDeMaiorPrioridadeDaFila();
	setcontext(&executando->context);
	return NULL;
}
int aux = 0;
ucontext_t* link;
void init(){
	printf("init\n");
	tid=-1;
	aux++;
	main_tcb = malloc(sizeof(TCB_t));
	getcontext(&main_tcb->context);
	main_tcb->tid =novoTID();
	main_tcb->prio = 5;
	(main_tcb->context).uc_link = NULL;
	(main_tcb->context).uc_stack.ss_sp   = malloc(SIGSTKSZ);
	(main_tcb->context).uc_stack.ss_size = SIGSTKSZ;
	executando = main_tcb;

	link = malloc(sizeof(ucontext_t));

	getcontext(link);
	link->uc_link = NULL;
	link->uc_stack.ss_sp   = malloc(SIGSTKSZ);
	link->uc_stack.ss_size = SIGSTKSZ;
	makecontext(link, (void*)despachante, 0);
}

int ccreate (void* (*start)(void*), void *arg, int prio) {
	if(aux==0)
		init();



	TCB_t* novaThread = malloc(sizeof(TCB_t));
	novaThread->prio = 0;
	novaThread->tid = novoTID();
	getcontext(&novaThread->context);
	(novaThread->context).uc_link = link;
	(novaThread->context).uc_stack.ss_sp   = malloc(SIGSTKSZ);
	(novaThread->context).uc_stack.ss_size = SIGSTKSZ;
	makecontext(&novaThread->context, (void*)start, 1, arg);
	inserirTCBNaFila(novaThread);
	listar();
	return -1;
}
int cyield(void) {
	listar();
	TCB_t* proximo = devolverERetirarTCBDeMaiorPrioridadeDaFila();
	TCB_t* atual = executando;
	if(proximo == NULL)
		return -1;

	inserirTCBNaFila(atual);
	executando = proximo;
	swapcontext(&atual->context, &proximo->context);
	return SUCESSO;
}

int cjoin(int tid) {
	return SUCESSO;
}

int csem_init(csem_t *sem, int count) {
	return -1;
}

int cwait(csem_t *sem) {
	return -1;
}

int csignal(csem_t *sem) {
	return -1;
}

int cidentify (char *name, int size) {
	strncpy (name, "Sergio Cechin - 2019/2 - Teste de compilacao.", size);
	return 0;
}
