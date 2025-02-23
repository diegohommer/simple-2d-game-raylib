#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "raylib.h"
#include "mapa.h"
#include "movimentos.h"
#include "arquivo.h"

#define TAM_FONTE 80
#define TAM_PIXEL 70

void input_jogador(Mapa* mapa, int* pause)
{
    int altura = ((30 + (TAM_PIXEL * mapa->dimensoes.linha)) - TAM_FONTE) / 2;
    int largura = ((TAM_PIXEL * mapa->dimensoes.coluna) - MeasureText("GAME OVER", TAM_FONTE)) / 2;
    mapa->player.impacto = 0;

    // MOVIMENTA플O PARA CIMA
    if(IsKeyDown(KEY_UP))
    {
        movimenta_vertical(mapa, -1);
        mapa_muda_movimento_escada(mapa);
    }

    // MOVIMENTA플O PARA BAIXO
    else if(IsKeyDown(KEY_DOWN))
    {
        movimenta_vertical(mapa, 1);
        mapa_muda_movimento_escada(mapa);
    }

    // MOVIMENTA플O PARA ESQUERDA
    else if(IsKeyDown(KEY_LEFT))
    {
        movimenta_horizontal(mapa, -1);
        mapa->player.dano = 0;
        mapa_muda_movimento(mapa, true);
        mapa->player.sentido = 0;
    }

    // MOVIMENTA플O PARA DIREITA
    else if(IsKeyDown(KEY_RIGHT))
    {
        movimenta_horizontal(mapa, 1);
        mapa->player.dano = 0;
        mapa_muda_movimento(mapa, true);
        mapa->player.sentido = 1;
    }

    else
    {
        mapa_muda_movimento(mapa, false);
    }

    // VOLTAR JOGADOR PARA POSI플O INICIAL
    if(IsKeyPressed(KEY_R))
    {
        mapa_reseta_player(mapa);
    }

    // SALVAR JOGO
    if(IsKeyPressed(KEY_K))
    {
        arq_salva_jogo(*mapa);
    }

    // CARREGAR JOGO
    if(IsKeyPressed(KEY_L))
    {
        arq_recupera_jogo(mapa);
    }

    // PAUSAR JOGO
    if(IsKeyPressed(KEY_P))
    {
        *pause = 1;
    }
}

