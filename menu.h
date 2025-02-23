#include <stdio.h>
#include "mapa.h"
#include "ranking.h"

#define ALTURA 675
#define LARGURA 1200
#define OPCOES 4
#define TAMANHO_FONTE 40
#define TAMANHO_TEXTO 20
#define POSICAO_Y_MENU 150
#define DESLOCAMENTO_Y_MENU 100

void DesenhaMenu(int opcao);
void menuJogo(Mapa *mapa, int *nivel, int *flag, Ranking ranking[], int *posicoes);
void gameOver(Mapa *mapa, int *menu);
void pausa_jogo(Mapa mapa, int *pause);
