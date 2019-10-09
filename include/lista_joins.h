#ifndef __listajoins__
#define __listajoins__

JOIN* retornaERemoveJoinComTIDEsperado(int tid);
int inserirJoinNaFila(JOIN* join);
void listarJoins();
int tidSendoEsperado(int tid);

#endif
