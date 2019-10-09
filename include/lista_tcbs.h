#ifndef __listatcbs__
#define __listatcbs__

TCB_t* devolverERetirarTCBDeMaiorPrioridadeDaFila();
int inserirTCBNaFila(TCB_t* tcb);
int tidExisteNaListaDeTCBs(int tid);
void listarTCBs();

#endif
