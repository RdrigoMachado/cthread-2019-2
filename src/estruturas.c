#include "../include/support.h"
#include "../include/cdata.h"
#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0




PFILA2 listaDePrioridades;
PFILA2 joins;

int inicializarListaDePrioridades(){
	listaDePrioridades =  malloc(sizeof(PFILA2));
	if(CreateFila2(listaDePrioridades) == 0)
		return TRUE;
	else
		return FALSE;
}

int inicializarListaDeJoins(){
	joins =  malloc(sizeof(PFILA2));
	if(CreateFila2(joins) == 0)
		return TRUE;
	else
		return FALSE;
}

int proximoNoLista(){
	if((NextFila2(listaDePrioridades)) == 0)
		return TRUE;
	else
		return FALSE;
}

int proximoNoListaJOIN(){
	if((NextFila2(joins)) == 0)
		return TRUE;
	else
		return FALSE;
}

TCB_t* devolverERetirarTCBDeMaiorPrioridadeDaFila(){
	if(listaDePrioridades == NULL)
		return NULL;
	FirstFila2(listaDePrioridades);
	if(listaDePrioridades->first == NULL)
		return NULL;
  TCB_t* tcb_maior_prioridade = GetAtIteratorFila2(listaDePrioridades);
  DeleteAtIteratorFila2(listaDePrioridades);
  return tcb_maior_prioridade;
}


int inserirTCBNaFila(TCB_t* tcb){
  if(listaDePrioridades == NULL)
    inicializarListaDePrioridades();
  FirstFila2(listaDePrioridades);
  if(listaDePrioridades->first == NULL)
			return AppendFila2(listaDePrioridades, tcb);

  do{
    TCB_t* tcbAtual = GetAtIteratorFila2(listaDePrioridades);
    if((tcbAtual->prio) > (tcb->prio))
      return InsertBeforeIteratorFila2(listaDePrioridades, tcb);
  } while(proximoNoLista() == TRUE);

  return AppendFila2(listaDePrioridades, tcb);
}
int inserirJoinNaFila(JOIN* join){
  if(joins == NULL)
    inicializarListaDeJoins();
  return AppendFila2(joins, join);
}

int tidExisteNaListaDeTCBs(int tid){
	if(listaDePrioridades == NULL)
		return FALSE;
	FirstFila2(listaDePrioridades);
	if(listaDePrioridades->first == NULL)
		return FALSE;

	do{
		TCB_t* tcbAtual = GetAtIteratorFila2(listaDePrioridades);
		if (tcbAtual->tid == tid)
			return TRUE;
	} while(proximoNoLista() == TRUE);
	return FALSE;
}

int tidSendoEsperado(int tid){
	if(joins == NULL)
		return FALSE;
	FirstFila2(joins);
	if(joins->first == NULL)
		return FALSE;

	do{
		JOIN* joinAtual = GetAtIteratorFila2(joins);
		if (joinAtual->tidDoTCBSendoEsperado == tid)
			return TRUE;
	} while(proximoNoListaJOIN() == TRUE);
	return FALSE;
}

JOIN* retornaERemoveJoinComTIDEsperado(int tid){
	if(joins == NULL){
		printf("joins eh nulo\n");
		return NULL;
	}
	FirstFila2(joins);
	if(joins->first == NULL){
		printf("joins eh vazio\n");
		return NULL;
	}
	do{
		JOIN* joinAtual = GetAtIteratorFila2(joins);
		if (joinAtual->tidDoTCBSendoEsperado == tid)
			return joinAtual;
	} while(proximoNoListaJOIN() == TRUE);
	printf("nao encontrei o join\n" );
	return NULL;
}

void listarTCBs(){
  if(listaDePrioridades == NULL)
    return;
  FirstFila2(listaDePrioridades);
  if(listaDePrioridades->first == NULL)
    return;

  do{
    TCB_t* tcbAtual = GetAtIteratorFila2(listaDePrioridades);
    printf("TID: %d, PRIO: %d\n", tcbAtual->tid, tcbAtual->prio);
  } while(proximoNoLista() == TRUE);
  printf("----------------\n" );
}

void listarJoins(){
  if(joins == NULL)
    return;
  FirstFila2(joins);
  if(joins->first == NULL)
    return;

  do{
    JOIN* joinAtual = GetAtIteratorFila2(joins);
    printf("TID esperando: %d, TID esperado: %d\n", joinAtual->esperando->tid, joinAtual->tidDoTCBSendoEsperado);
  } while(proximoNoListaJOIN() == TRUE);
  printf("----------------\n" );
}
