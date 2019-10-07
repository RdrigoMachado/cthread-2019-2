
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

void inicializar(){
		if(iniciada == FALSE){
			iniciada = TRUE;
			tid = -1;
		}
}

//==================Adicionar TCB's nas Listas/Filas============================
TCB_t* retirarThreadDeMaiorPrioridade(){
	if(listaDePrioridades == NULL)
		return NULL;
	FirstFila2(listaDePrioridades);
	if(listaDePrioridades->first == NULL)
		return NULL;

	NoPrioridade* noAtual = GetAtIteratorFila2(listaDePrioridades);
	FirstFila2(noAtual->filaFCFS);
	TCB_t* tcbMaiorPrioridade = GetAtIteratorFila2(noAtual->filaFCFS);
	DeleteAtIteratorFila2(noAtual->filaFCFS);
	if((noAtual->filaFCFS)->first == NULL)
		DeleteAtIteratorFila2(listaDePrioridades);

	return tcbMaiorPrioridade;
}

TCB_t* criarTCB(int prioridade){
	TCB_t* tcb = malloc(sizeof(TCB_t));
	ucontext_t* contextoAtual = malloc(sizeof(ucontext_t));
	getcontext(contextoAtual);
	contextoAtual->uc_stack.ss_sp = malloc(STACK_SIZE);
	contextoAtual->uc_stack.ss_size = STACK_SIZE;
	contextoAtual->uc_stack.ss_flags = 0;
	tcb->tid = novoTID();
	tcb->context = *contextoAtual;
	tcb->state = PROCST_APTO;
	tcb->prio = prioridade;
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
		printf("TID: %d",tcb->tid);
		if((NextFila2(lista)) != 0)
			return;
		printf(", ");
	}
}
void listarFila(){
	FirstFila2(listaDePrioridades);
	while(listaDePrioridades->it !=NULL){
		NoPrioridade* no = listaDePrioridades->it->node;
		printf("Prioridade %d:\n", no->prioridade);
		listarPilha(no->filaFCFS);
		printf("\n");
		if((NextFila2(listaDePrioridades)) != 0)
			return;
	}
}

void teste(){
	//cria tcb's
	TCB_t* novoTCB1 = criarTCB(4);
	TCB_t* novoTCB2 = criarTCB(0);
	TCB_t* novoTCB3 = criarTCB(1);
	TCB_t* novoTCB4 = criarTCB(5);
	TCB_t* novoTCB5 = criarTCB(2);

	//adiciona tcb's na lista de prioridades
	inicializarListaDePrioridades();
	adicionarTCBNaListaDePrioridades(novoTCB1);
	adicionarTCBNaListaDePrioridades(novoTCB2);
	adicionarTCBNaListaDePrioridades(novoTCB3);
	adicionarTCBNaListaDePrioridades(novoTCB4);
	adicionarTCBNaListaDePrioridades(novoTCB5);

	//imprime lista de tcb's
	printf("LISTAR:\n");
	listarFila();
	printf("\n");
	//remove todos os tcb's em ordem de prioridade
	TCB_t* tcbMaiorPrioridade = retirarThreadDeMaiorPrioridade();
	do{
		printf("Removendo TCB de maior prioridade:\n");
		printf("TID do TCB: %d\n",tcbMaiorPrioridade->tid);
		tcbMaiorPrioridade = retirarThreadDeMaiorPrioridade();
	}while(tcbMaiorPrioridade != NULL);


}
//==============================================================================

int ccreate (void* (*start)(void*), void *arg, int prio) {
	inicializar();
	teste();
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
