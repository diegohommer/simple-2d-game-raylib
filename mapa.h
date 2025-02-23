#ifndef MAPA_H
#define MAPA_H

#include <stdio.h>
#include <stdbool.h>
#include "coordenadas.h"
#include "jogador.h"
#include "bau.h"
#include "raylib.h"

#define MAXL 30
#define MAXC 120
#define MAXBAUS 20

/*  TIPO MAPA CRIADO:

    char matriz[MAXL][MAXC] - Representa a matriz de caracteres que representa o mapa do jogo
    Localizacao dimensoes   - Variável do tipo Localizacao que representa as dimensoes X e Y do mapa
    int escada              - Inteiro que representa se o jogador está sobre uma escada ou não (1 se sim, 0 se não)
    int bau                 - Inteiro que representa se o jogador está sobre um baú ou não (1 se sim, 0 se não)
    char porta              - Caracter que representa se o jogador está sobre uma porta ou não (caracter não vazio se sim, caracter vazio se não)
    Jogador player          - Variável do tipo Jogador que representa as informações do jogador

*/

typedef struct
{
    char matriz[MAXL][MAXC];
    Localizacao dimensoes;
    Localizacao posicaoInicialJ;
    Localizacao posicaoSaida;
    int escada;
    int bau, numeroBaus;
    char porta;
    Jogador player;
    Bau baus[MAXBAUS];
    int nivel;
    int saida;
}Mapa;

typedef struct
{
    Texture2D bloco_escada;
    Texture2D bloco_escada_topo;
    Texture2D bloco_bau_aberto;
    Texture2D bloco_bau_fechado;
    Texture2D bloco_bau_queimado;
    Texture2D bloco_porta;
    Texture2D bloco_saida;
    Texture2D plano_de_fundo;
    Texture2D player_idle;
    Texture2D player_movimenta_direita1;
    Texture2D player_movimenta_direita2;
    Texture2D player_movimenta_esquerda1;
    Texture2D player_movimenta_esquerda2;
    Texture2D player_caindo;
    Texture2D player_machucado;
    Texture2D player_bau;
    Texture2D player_bau_aberto;
    Texture2D player_bau_queimado;
    Texture2D player_escada1;
    Texture2D player_escada2;
    Texture2D player_impacto;
    Texture2D teto;
    Texture2D teto_direita;
    Texture2D teto_esquerda;
    Texture2D chao;
    Texture2D chao_esquerda;
    Texture2D chao_direita;
    Texture2D player_porta;
    Texture2D parede_esquerda;
    Texture2D parede_direita;
    Texture2D tijolo;
}Texturas;

void inicializa_baus(Mapa *mapa);
char gera_gema(Mapa *mapa, int gema);
void abre_bau(Mapa *mapa);
void mapa_carrega(Mapa *mapa, int nivel);
void mapa_muda_nivel(Mapa *mapa, int *nivel);
void mapa_reseta_player(Mapa *mapa);
void mapa_diminui_vida(Mapa *mapa);
void mapa_movimenta_vertical(Mapa *mapa, int direcao);
void mapa_localiza_jogador(Mapa *mapa);
void mapa_busca_porta(Mapa mapa, char porta, Localizacao *local_porta);
void mapa_imprime(Mapa mapa, Texturas texturas);
void mapa_imprime_info(Mapa mapa);
void mapa_muda_movimento(Mapa *mapa, bool movendo);
void mapa_muda_movimento_escada(Mapa *mapa);
void mapa_inicializa_texturas(Texturas *texturas);

#endif // MAPA_H
