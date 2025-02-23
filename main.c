#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#include "mapa.h"
#include "movimentos.h"
#include "inputs_jogador.h"
#include "menu.h"
#include "ranking.h"

#define MAPA_L 30
#define MAPA_C 120
#define ALTURA 675
#define LARGURA 1200
#define PIXELS 70
#define POSICAO_Y 0
#define DESLOCAMENTO_Y 70
#define POSICAO_X 0
#define DESLOCAMENTO_X 70
#define TAM_FONTE 80
#define TAM_PIXEL 70
#define TAM_RANKING 5
#define NUM_TEXTURAS 30

int main(void)
{
    // Initialization
    //-------------------------------------------------------------------------------------
    Mapa mapa;
    int menu = 0;      // Flag para indicar se o jogador está no menu
    int flag = 1;
    int pause = 0;
    int nivel = 1;
    int posicoes = 0;
    char *posicaoAbaixo;
    Ranking ranking[TAM_RANKING];
    Texturas texturas;

    InitWindow(LARGURA, ALTURA, "JOGO");
    mapa_inicializa_texturas(&texturas);
    SetTargetFPS(15);
    menuJogo(&mapa, &nivel, &flag, ranking, &posicoes);
    menu = 0;


    // -------------------------------------------------------------------------------------- //
    // Main game loop

    while (flag && !WindowShouldClose())
    {
        if(menu)
        {
            menuJogo(&mapa, &nivel, &flag, ranking, &posicoes);
            menu = 0;
        }

        if(nivel < 4)
        {
            mapa_localiza_jogador(&mapa);
            posicaoAbaixo = &(mapa.matriz[mapa.player.posicaoJ.linha + 1][mapa.player.posicaoJ.coluna]);

            // SE JOGADOR CAIU MAIS DE 3 POSIÇÕES
            if(mapa.player.queda > 3 && (*posicaoAbaixo == 'X' || *posicaoAbaixo == 'H'))
            {
                mapa_diminui_vida(&mapa);
                mapa.player.queda = 0;
            }

            else if(*posicaoAbaixo == 'X' || *posicaoAbaixo == 'H' || mapa.escada)
            {
                input_jogador(&mapa, &pause);
                if(mapa.player.queda > 0)
                {
                    mapa.player.impacto = 1;
                }
                mapa.player.queda = 0;
            }
            else
            {
                gravidade(&mapa);
            }
        }

        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------

        if(nivel >= 4)
        {
            menu = 1;
            nivel = 1;
            recebe_nome(&mapa);
            carrega_rankings(ranking, &posicoes);
            adiciona_ranking(ranking, &posicoes, cria_ranking(mapa.player.nome, mapa.player.pontuacao));
            salva_rankings(ranking, posicoes);
        }
        else if(mapa.nivel != nivel)
        {
            mapa_muda_nivel(&mapa, &nivel);
        }
        else if(mapa.player.vidas > 0)
        {
            mapa_imprime(mapa, texturas);
        }
        else
        {
            gameOver(&mapa, &menu);
        }
        if(pause)
        {
            pausa_jogo(mapa, &pause);
        }
    }
    return 0;
}
