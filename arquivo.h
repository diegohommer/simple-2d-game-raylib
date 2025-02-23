#include <stdio.h>
#include "mapa.h"

void arq_salva_jogo(Mapa mapa);
void arq_recupera_jogo(Mapa *mapa);
void arq_modifica_jogador(Mapa mapa, char novo_c);
void arq_carrega_mapa(Mapa *mapa, char *nomeArq);
