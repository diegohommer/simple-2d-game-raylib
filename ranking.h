#ifndef ARQUIVO_H
#define ARQUIVO_H
#define NOMEMAX 30
#define VETORMAX 5

#include <stdio.h>
#include "mapa.h"

typedef struct
{
     char nome[NOMEMAX];
     int pontuacao;
}Ranking;

Ranking cria_ranking(char nome[NOMEMAX], int pontuacao);
void salva_rankings(Ranking entradas[VETORMAX], int ocupacao);
void carrega_rankings(Ranking entradas[VETORMAX], int *ocupacao);
void imprime_rankings(Ranking entradas[VETORMAX], int ocupacao);
void adiciona_ranking(Ranking entradas[VETORMAX], int *ocupacao, Ranking novo_jogador);
void recebe_nome(Mapa *mapa);

#endif // ARQUIVO_H

