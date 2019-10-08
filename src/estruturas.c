#include "../include/support.h"
#include "../include/cdata.h"
#include "ucontext.h"
#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0


TCB_t* main_tcb;
PFILA2 listaDePrioridades;
TCB_t* executando;

int inicializarListaDePrioridades(){
	listaDePrioridades =  malloc(sizeof(PFILA2));
	if(CreateFila2(listaDePrioridades) == 0)
		return TRUE;
	else
		return FALSE;
}

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
