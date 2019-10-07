
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
#define SUCESSO 0
#define ERRO -1


int iniciada = FALSE;
int tid;
TCB_t* main_tcb;
PFILA2 listaDePrioridades;
TCB_t* executando;

int novoTID(){
	tid++;
	return tid;
}

void inicializar(){
		if(iniciada == FALSE){
			iniciada = TRUE;
			tid = -1;
			listaDePrioridades = NULL;
			executando = NULL;
			startTimer();
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

void despachante(){
		executando = retirarThreadDeMaiorPrioridade();
		executando->state = PROCST_EXEC;
		setcontext(&(executando->context));
		startTimer();
}

void escalonador(){
	if(executando == NULL){
		despachante();
	}

	//salva contexto de executando
	//faz swapcontext com o contexto do tcb de maior prioridade
	//adiciona tcb de executando na lista
	//executando = tcb de maior prioridade
	executando = retirarThreadDeMaiorPrioridade();
}

void finalizaThreadEChamaEscalonador(){
		free(executando);
		TCB_t* threadAtual = executando;
		threadAtual->state = PROCST_TERMINO;
		//join
		executando = NULL;
		despachante();
}

ucontext_t* adicionarLinkFinalizacaoThread(){
	ucontext_t* contextoDeFinalizacao = malloc(sizeof(ucontext_t));
	getcontext(contextoDeFinalizacao);
	contextoDeFinalizacao->uc_stack.ss_sp = malloc(STACK_SIZE);
	contextoDeFinalizacao->uc_stack.ss_size = STACK_SIZE;
	contextoDeFinalizacao->uc_stack.ss_flags = 0;
	makecontext(contextoDeFinalizacao, (void*)start, 1 , arg);
}
TCB_t* criarTCB(int prioridade, void* (*start)(void*), void* arg){
	TCB_t* tcb = malloc(sizeof(TCB_t));
	tcb->tid = novoTID();
	tcb->context = *contextoAtual;
	tcb->state = PROCST_APTO;
	tcb->prio = prioridade;

	ucontext_t* contextoAtual = malloc(sizeof(ucontext_t));
	getcontext(contextoAtual);
	contextoAtual->uc_stack.ss_sp = malloc(STACK_SIZE);
	contextoAtual->uc_stack.ss_size = STACK_SIZE;
	contextoAtual->uc_stack.ss_flags = 0;
	makecontext(contextoAtual, (void*)start, 1 , arg);
	contextoAtual->uc_link = adicionarLinkFinalizacaoThread();
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
	TCB_t* novoTCB =  criarTCB(0, void* (*start), arg);
	if(adicionarTCBNaListaDePrioridades(novoTCB) == TRUE)
		return tid;
	else
		return ERRO;
}


int cyield(void) {
	inicializar();
	TCB_t* tcbAtual = executando;
	getcontext(&(cbAtual->context));
	tcbAtual->state = PROCST_APTO;
	unsigned int novaPrioridade = stopTimer();
	tcbAtual->prio = novaPrioridade;
	adicionarTCBNaListaDePrioridades(tcbAtual);
	executando = retirarThreadDeMaiorPrioridade();
	executando->state = PROCST_EXEC;
	setcontext(&(executando->context));
	return SUCESSO;
}

int cjoin(int tid) {
	inicializar();

	return SUCESSO;
}

int csem_init(csem_t *sem, int count) {
	inicializar();
	return -1;
}

int cwait(csem_t *sem) {
	inicializar();
	return -1;
}

int csignal(csem_t *sem) {
	inicializar();
	return -1;
}

int cidentify (char *name, int size) {
	inicializar();
	strncpy (name, "Sergio Cechin - 2019/2 - Teste de compilacao.", size);
	return 0;
}
