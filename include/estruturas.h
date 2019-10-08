#ifndef __estruturas__
#define __estruturas__

TCB_t* devolverERetirarTCBDeMaiorPrioridadeDaFila();
int inserirTCBNaFila(TCB_t* tcb);
int tidExisteNaListaDeTCBs(int tid);
void listarTCBs();


JOIN* retornaERemoveJoinComTIDEsperado(int tid);
int inserirJoinNaFila(JOIN* join);
void listarJoins();
int tidSendoEsperado(int tid);
#endif
