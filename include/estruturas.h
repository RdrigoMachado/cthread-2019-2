#ifndef __estruturas__
#define __estruturas__

int inicializarListaDePrioridades();
TCB_t* retirarThreadDeMaiorPrioridade();
int adicionarTCBNaListaDePrioridades(TCB_t* tcb);

#endif
