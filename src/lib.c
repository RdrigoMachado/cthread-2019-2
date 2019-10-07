
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"
#define STACK_SIZE SIGSTKSZ
#include <signal.h>
#define TRUE 1
#define FALSE 0


int iniciada = FALSE;
TCB_t* main_tcb;
PFILA2 listaDePrioridades = NULL;
int tid;

int novoTID(){
	tid++;
	return tid;
}

int inicializar(){
		iniciada = TRUE;
		tid = -1;
		return TRUE;
}

//==================Adicionar TCB's nas Listas/Filas============================
TCB_t* criarTCB(){
	TCB_t* tcb = malloc(sizeof(TCB_t));
	ucontext_t* contextoAtual = malloc(sizeof(ucontext_t));
	getcontext(contextoAtual);
	contextoAtual->uc_stack.ss_sp = malloc(STACK_SIZE);
	contextoAtual->uc_stack.ss_size = STACK_SIZE;
	contextoAtual->uc_stack.ss_flags = 0;
	tcb->tid = novoTID();
	tcb->context = *contextoAtual;
	tcb->state = PROCST_APTO;
	tcb->prio = 0;
	return tcb;
}

int inicializarListaDePrioridades(){
	listaDePrioridades =  malloc(sizeof(PFILA2));
	if(CreateFila2(listaDePrioridades) == 0)
		return TRUE;
	else
		return FALSE;
}

NoPrioridade* inicializarNovoNoPrioridade(int prioridade){
	NoPrioridade* novoNoPrioridade = malloc(sizeof(NoPrioridade));
	novoNoPrioridade->prioridade = prioridade;
	novoNoPrioridade->filaFCFS = malloc(sizeof(PFILA2));
	CreateFila2(novoNoPrioridade->filaFCFS);
	return novoNoPrioridade;
}

NoPrioridade* adicionarNovaPrioridadeNoFimDaLista(int prioridade){
	NoPrioridade* novoNoPrioridade = inicializarNovoNoPrioridade(prioridade);
	AppendFila2(listaDePrioridades, novoNoPrioridade);
	return novoNoPrioridade;
}

int proximoNoLista(){
	if((NextFila2(listaDePrioridades)) == 0)
		return TRUE;
	else
		return FALSE;
}

NoPrioridade* retornaFilaPrioridade(int prioridade){
	if(listaDePrioridades == NULL)
		inicializarListaDePrioridades();
	FirstFila2(listaDePrioridades);
	if(listaDePrioridades->it == NULL){
			return adicionarNovaPrioridadeNoFimDaLista(prioridade);
	}
	NoPrioridade* noAtual = (listaDePrioridades->it)->node;
	int continua = TRUE;
	if(noAtual != NULL){
		while(continua == TRUE){
			if((noAtual->prioridade) > prioridade){
				NoPrioridade* novaPrioridade = inicializarNovoNoPrioridade(prioridade);
				InsertBeforeIteratorFila2(listaDePrioridades, novaPrioridade);
				return novaPrioridade;
		 	} else if(noAtual->prioridade == prioridade){
				return noAtual;
		 	}
			continua = proximoNoLista();
			if(continua == TRUE){
				noAtual = (listaDePrioridades->it)->node;
			}
		}

	}
	return adicionarNovaPrioridadeNoFimDaLista(prioridade);
}

int adicionarTCBNaListaDePrioridades(TCB_t* tcb){
	int prioridade = tcb->prio;
	NoPrioridade* noPrioridade = retornaFilaPrioridade(prioridade);
	if (AppendFila2((noPrioridade->filaFCFS), tcb) == 0)
		return TRUE;
	else
		return FALSE;
}
//==========================Listagem - Para Debug===============================
void listarPilha(PFILA2 lista){
	FirstFila2(lista);
	while(lista->it !=NULL){
		PNODE2 no = (lista->it);
		TCB_t* tcb = no->node;
		printf("TID: %d, ",tcb->tid);
		if((NextFila2(lista)) != 0)
			return;
	}
}
void listarFila(){
	FirstFila2(listaDePrioridades);
	while(listaDePrioridades->it !=NULL){
		NoPrioridade* no = listaDePrioridades->it->node;
		printf("\nPrioridade %d:\n", no->prioridade);
		listarPilha(no->filaFCFS);
		if((NextFila2(listaDePrioridades)) != 0)
			return;
	}
}

void teste(){
	TCB_t* novoTCB1 = criarTCB();
	TCB_t* novoTCB2 = criarTCB();
	TCB_t* novoTCB3 = criarTCB();
	novoTCB3->prio = 4;
	TCB_t* novoTCB4 = criarTCB();
	novoTCB4->prio = 2;
	TCB_t* novoTCB5 = criarTCB();
	novoTCB5->prio = 4;

	inicializarListaDePrioridades();
	adicionarTCBNaListaDePrioridades(novoTCB1);
	adicionarTCBNaListaDePrioridades(novoTCB2);
	adicionarTCBNaListaDePrioridades(novoTCB3);
	adicionarTCBNaListaDePrioridades(novoTCB4);
	adicionarTCBNaListaDePrioridades(novoTCB5);
	printf("LISTAR:\n\n");
	listarFila();

}
//==============================================================================

int ccreate (void* (*start)(void*), void *arg, int prio) {
	if(iniciada == FALSE){
		if (inicializar() == TRUE)
			printf("Sucesso\n");
		else
			printf("Erro\n");
	}
	teste();


	return -1;
}

int cyield(void) {
	return -1;
}

int cjoin(int tid) {
	return -1;
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
