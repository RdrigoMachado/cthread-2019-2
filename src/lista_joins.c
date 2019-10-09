#include "../include/support.h"
#include "../include/cdata.h"
#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

PFILA2 joins;

int inicializarListaDeJoins(){
	joins =  malloc(sizeof(PFILA2));
	if(CreateFila2(joins) == 0)
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

int inserirJoinNaFila(JOIN* join){
  if(joins == NULL)
    inicializarListaDeJoins();
  return AppendFila2(joins, join);
}

JOIN* retornaERemoveJoinComTIDEsperado(int tid){
	if(joins == NULL)
		return NULL;
	FirstFila2(joins);
	if(joins->first == NULL)
		return NULL;

	do{
		JOIN* joinAtual = GetAtIteratorFila2(joins);
		if (joinAtual->tidDoTCBSendoEsperado == tid)
			return joinAtual;
	} while(proximoNoListaJOIN() == TRUE);
	return NULL;
}

int tidSendoEsperado(int tid){
	if(joins == NULL)
		return FALSE;

	FirstFila2(joins);
	if(joins->first == NULL)
		return FALSE;

	do{
			JOIN* atual = GetAtIteratorFila2(joins);
			if(atual->tidDoTCBSendoEsperado == tid)
				return TRUE;
	}while(proximoNoListaJOIN() == TRUE);
	return FALSE;
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
