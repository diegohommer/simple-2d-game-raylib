#ifndef JOGADOR_H
#define JOGADOR_H

#include "coordenadas.h"
#define MAXLINHAS 40

/*  TIPO JOGADOR CRIADO:

    char nome[MAXLINHAS] - String que representa o nome do jogador
    Localizacao posicaoJ - Variável do tipo Localizacao que representa a posição do jogador no mapa
    int vidas            - Inteiro que representa o número de vidas do jogador
    int pontuacao        - Inteiro que representa a pontuação total do jogador

*/

typedef struct
{
    char nome[MAXLINHAS];
    Localizacao posicaoJ;
    int vidas, pontuacao;
    int bomba;
    int chave;
    int dano;
    int movimento;
    int direcao;
    int subindo;
    int sentido;
    int queda;
    int impacto;
}Jogador;

Jogador jog_inicializa(int x, int y);
void jog_aumenta_pontuacao(Jogador *jogador, int pontos);
void jog_le_nome(Jogador *jogador);
void jog_print_info(Jogador jogador);

#endif // JOGADOR_H
