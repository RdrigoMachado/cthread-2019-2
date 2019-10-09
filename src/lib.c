#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../include/lista_tcbs.h"
#include "../include/lista_joins.h"
#include "../include/lista_bloqueados.h"
#include <signal.h>

#define TRUE 1
#define FALSE 0
#define SUCESSO 0
#define ERRO -1

int iniciada = FALSE;
int tid;
TCB_t* main_tcb;
TCB_t* executando;
ucontext_t* termino;


int novoTID(){
	tid++;
	return tid;
}

void* terminarThreadEChamarProxima(void* arg){

	JOIN* join = retornaERemoveJoinComTIDEsperado(executando->tid);
	if(join != NULL){
		inserirTCBNaFila(join->esperando);
	}

	free(executando);
	executando = devolverERetirarTCBDeMaiorPrioridadeDaFila();
	startTimer();
	setcontext(&executando->context);
	return NULL;
}

void init(){
	if(iniciada == TRUE)
		return;
	iniciada = TRUE;

	startTimer();
	tid=-1;
	main_tcb = malloc(sizeof(TCB_t));
	getcontext(&main_tcb->context);
	main_tcb->tid =novoTID();
	(main_tcb->context).uc_link = NULL;
	(main_tcb->context).uc_stack.ss_sp   = malloc(SIGSTKSZ);
	(main_tcb->context).uc_stack.ss_size = SIGSTKSZ;
	executando = main_tcb;

	termino = malloc(sizeof(ucontext_t));

	getcontext(termino);
	termino->uc_link = NULL;
	termino->uc_stack.ss_sp   = malloc(SIGSTKSZ);
	termino->uc_stack.ss_size = SIGSTKSZ;
	makecontext(termino, (void*)terminarThreadEChamarProxima, 0);
}

int ccreate (void* (*start)(void*), void *arg, int prio) {
	init();

	TCB_t* novaThread = malloc(sizeof(TCB_t));
	if(novaThread==NULL)
		return ERRO;

	novaThread->prio = 0;
	novaThread->tid = novoTID();
	getcontext(&novaThread->context);
	(novaThread->context).uc_link = termino;
	(novaThread->context).uc_stack.ss_sp   = malloc(SIGSTKSZ);
	(novaThread->context).uc_stack.ss_size = SIGSTKSZ;
	makecontext(&novaThread->context, (void*)start, 1, arg);

	if(inserirTCBNaFila(novaThread) != SUCESSO)
		return ERRO;
	else
		return tid;
}

int cyield(void) {
	init();

	TCB_t* atual = executando;
	atual->prio = stopTimer();
	inserirTCBNaFila(atual);
	TCB_t* proximo = devolverERetirarTCBDeMaiorPrioridadeDaFila();
	if(proximo == NULL)
		return ERRO;

	executando = proximo;


	startTimer();
	swapcontext(&atual->context, &proximo->context);
	return SUCESSO;
}

int cjoin(int tid) {
	init();
	if(tidExisteNaListaDeTCBs(tid) == FALSE)
		return ERRO;
	if(tidSendoEsperado(tid) == TRUE)
		return ERRO;


	executando->prio = stopTimer();
	JOIN* join = malloc(sizeof(JOIN));
	join->tidDoTCBSendoEsperado = tid;
	join->esperando = executando;
	inserirJoinNaFila(join);

	TCB_t* atual = executando;
	TCB_t* proximo = devolverERetirarTCBDeMaiorPrioridadeDaFila();
	executando = proximo;

	startTimer();
	swapcontext(&atual->context, &proximo->context);
	return SUCESSO;
}

int csem_init(csem_t *sem, int count) {
	init();

	if(sem == NULL)
		return ERRO;

	sem->fila = malloc(sizeof(PFILA2));
	if(sem->fila == NULL)
		return ERRO;

	sem->count = count;
	TCB_t* teste = malloc(sizeof(TCB_t));
	teste->prio = 1;
	teste->tid = 11;
	if(CreateFila2(sem->fila) == 0){
		AppendFila2(sem->fila, teste);
		return SUCESSO;
	}
	return ERRO;
}

int cwait(csem_t *sem) {
	init();

	if(sem->count <= 0){
		sem->count = sem->count-1;
		TCB_t* atual = executando;
		inserirTCBNaFilaBloqueados(atual);
		atual->prio = stopTimer();
		executando = devolverERetirarTCBDeMaiorPrioridadeDaFila();
		startTimer();
		swapcontext(&atual->context, &executando->context);
	}

	return SUCESSO;
}

int csignal(csem_t *sem) {
	init();

	return -1;
}

int cidentify (char *name, int size) {
	init();

	strncpy (name, "Sergio Cechin - 2019/2 - Teste de compilacao.", size);
	return 0;
}
