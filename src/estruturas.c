#include "../include/support.h"
#include "../include/cdata.h"
#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

PFILA2 listaDePrioridades;

int inicializarListaDePrioridades(){
	listaDePrioridades =  malloc(sizeof(PFILA2));
	if(CreateFila2(listaDePrioridades) == 0)
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

void listar(){
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
