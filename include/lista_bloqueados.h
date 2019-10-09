#ifndef __listabloqueados__
#define __listabloqueados__

TCB_t* devolverERetirarTCBDeMaiorPrioridadeDaFilaBloqueados();
int inserirTCBNaFilaBloqueados(TCB_t* tcb);
void listarTCBsBloqueados();

#endif
