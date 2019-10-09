#
# Makefile de EXEMPLOinserirTCBNaFila
#
# OBRIGATÓRIO ter uma regra "all" para geração da biblioteca e de uma
# regra "clean" para remover todos os objetos gerados.
#
# É NECESSARIO ADAPTAR ESSE ARQUIVO de makefile para suas necessidades.
#  1. Cuidado com a regra "clean" para não apagar o "support.o"
#
# OBSERVAR que as variáveis de ambiente consideram que o Makefile está no diretótio "cthread"
#

CC=gcc
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src

all: bloqueados tcbs joins cthread
	mkdir -p lib
	ar crs $(LIB_DIR)/libcthread.a $(BIN_DIR)/lib.o $(BIN_DIR)/support.o $(BIN_DIR)/lista_bloqueados.o $(BIN_DIR)/lista_joins.o $(BIN_DIR)/lista_tcbs.o

cthread:
	$(CC) -m32 -c $(SRC_DIR)/lib.c -o $(BIN_DIR)/lib.o -Wall
bloqueados:
	$(CC) -m32 -c $(SRC_DIR)/lista_bloqueados.c -o $(BIN_DIR)/lista_bloqueados.o -Wall
joins:
	$(CC) -m32 -c $(SRC_DIR)/lista_joins.c -o $(BIN_DIR)/lista_joins.o -Wall
tcbs:
	$(CC) -m32 -c $(SRC_DIR)/lista_tcbs.c -o $(BIN_DIR)/lista_tcbs.o -Wall

clean:
	rm -rf $(LIB_DIR)/*.a $(BIN_DIR)/lib.o $(BIN_DIR)/lista_bloqueados.o $(BIN_DIR)/lista_joins.o $(BIN_DIR)/lista_tcbs.o $(SRC_DIR)/*~ $(INC_DIR)/*~ *~
