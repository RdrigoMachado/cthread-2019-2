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

void* voltaPraMain(void* arg){
	setcontext(&main_tcb->context);
	return NULL;
}

ucontext_t link;
void init(){
	inicializarListaDePrioridades();
	main_tcb = malloc(sizeof(TCB_t));
	getcontext(&main_tcb->context);
	(main_tcb->context).uc_link = NULL;
	(main_tcb->context).uc_stack.ss_sp   = malloc(SIGSTKSZ);
	(main_tcb->context).uc_stack.ss_size = SIGSTKSZ;
	executando = main_tcb;

	getcontext(&link);
	link.uc_link = NULL;
	link.uc_stack.ss_sp   = malloc(SIGSTKSZ);
	link.uc_stack.ss_size = SIGSTKSZ;
	makecontext(&link, (void*)voltaPraMain, 0);
}

TCB_t* nova1;
TCB_t* nova2;
int aux = 0;

int ccreate (void* (*start)(void*), void *arg, int prio) {
	if(aux==0)
		init();
	aux++;
	TCB_t* novaThread = malloc(sizeof(TCB_t));
	getcontext(&novaThread->context);
	(novaThread->context).uc_link = &link;
	(novaThread->context).uc_stack.ss_sp   = malloc(SIGSTKSZ);
	(novaThread->context).uc_stack.ss_size = SIGSTKSZ;
	makecontext(&novaThread->context, (void*)start, 1, arg);
	swapcontext(&executando->context, &novaThread->context);
	return -1;
}
int cyield(void) {
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
