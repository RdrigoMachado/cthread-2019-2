#include "../include/support.h"
#include "../include/cdata.h"
#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

PFILA2 bloqueados;

int inicializarListaBloqueadosPorPrioridades(){
	bloqueados =  malloc(sizeof(PFILA2));
	if(CreateFila2(bloqueados) == 0)
		return TRUE;
	else
		return FALSE;
}

int proximoNoListaBloquados(){
	if((NextFila2(bloqueados)) == 0)
		return TRUE;
	else
		return FALSE;
}

TCB_t* devolverERetirarTCBDeMaiorPrioridadeDaFilaBloqueados(){
	if(bloqueados == NULL)
		return NULL;
	FirstFila2(bloqueados);
	if(bloqueados->first == NULL)
		return NULL;
  TCB_t* tcb_maior_prioridade = GetAtIteratorFila2(bloqueados);
  DeleteAtIteratorFila2(bloqueados);
  return tcb_maior_prioridade;
}

int inserirTCBNaFilaBloqueados(TCB_t* tcb){
  if(bloqueados == NULL)
    inicializarListaBloqueadosPorPrioridades();
  FirstFila2(bloqueados);
  if(bloqueados->first == NULL)
			return AppendFila2(bloqueados, tcb);

  do{
    TCB_t* tcbAtual = GetAtIteratorFila2(bloqueados);
    if((tcbAtual->prio) > (tcb->prio))
      return InsertBeforeIteratorFila2(bloqueados, tcb);
  } while(proximoNoListaBloquados() == TRUE);

  return AppendFila2(bloqueados, tcb);
}

void listarTCBsBloqueados(){
  if(bloqueados == NULL)
    return;
  FirstFila2(bloqueados);
  if(bloqueados->first == NULL)
    return;

  do{
    TCB_t* tcbAtual = GetAtIteratorFila2(bloqueados);
    printf("TID: %d, PRIO: %d\n", tcbAtual->tid, tcbAtual->prio);
  } while(proximoNoListaBloquados() == TRUE);
  printf("----------------\n" );
}
