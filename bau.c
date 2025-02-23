#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "mapa.h"

#define MAXG 10000

/*
    Mapa *mapa - Endereço de uma variável do tipo Mapa (função coloca a chave, bombas e gemas nos baús do mapa)
*/
void inicializa_baus(Mapa *mapa)
{
    srand(time(NULL));
    int i, gema;


    // ESVAZIA TODOS OS BAÚS DO VETOR
    for(i = 0; i < MAXBAUS; i++)
    {
        mapa->baus[i].conteudo = ' ';
    }

    int bau_chave = rand() % mapa->numeroBaus;  // Salva um número aleatório de 0 até mapa->numeroBaus em bau_chave
    int bau_bomba;

    mapa->baus[bau_chave].conteudo = 'K';       // Coloca chave dentro do baú com essa posição no vetor de baús

    // Insere mapa->nivel bombas no vetor de baús
    for(i = 0; i < mapa->nivel; i++)
    {
        // Repete interação até encontrar um baú vazio para colocar a bomba
        do{
            bau_bomba = rand() % mapa->numeroBaus;     // Salva um número aleatório de 0 até mapa->numeroBaus em bau_bomba
            if(bau_bomba != bau_chave)                 // Se esse número é diferente do bau_chave (baú está vazio)
            {
                mapa->baus[bau_bomba].conteudo = 'B';  // Coloca bomba dentro do baú com essa posição no vetor de baús
            }
        }while(bau_bomba == bau_chave);
    }

    // Insere gemas nos baús restantes (vazios) do vetor de baús
    for(i = 0; i < mapa->numeroBaus; i++)
    {
        gema = 1 + rand() % (MAXG);                            // gema recebe um número aleatório de 1 a 10000
        if(mapa->baus[i].conteudo == ' ')                      // Se o baú da posição i do vetor está vazio
        {
            mapa->baus[i].conteudo = gera_gema(mapa, gema);    // Insere uma gema aleatória dentro desse baú
        }
    }
}

/*
    Mapa *mapa - Endereço de uma variável do tipo Mapa (função gera uma gema para ser inserida num baú)
*/
char gera_gema(Mapa *mapa, int gema)
{
    int probabilidade = 1000 * (mapa->nivel - 1); // probabilidade que muda conforme o nível

    if(gema > (6000 + (probabilidade * 1.5)))         // Ametista(!) : Chance inicial de 40% -> Diminui 15% a cada nível
    {
        return '!';
    }
    else if(gema > (4000 + (probabilidade * 1.75)))   // Safira(@)   : Chance inicial de 20% -> Diminui 2,5% a cada nível
    {
        return '@';
    }
    else if(gema > (2000 + probabilidade * 1.75))     // Rubi(#)     : Chance inicial de 20% -> Se mantém constante a cada nível
    {
        return '#';
    }
    else if(gema > (500 + (probabilidade * 1.25)))    // Anel($)     : Chance inicial de 15% -> Aumenta 5% a cada nível
    {
        return '$';
    }
    else                                              // Coroa(%)    : Chance inicial de 5%  -> Aumenta 12,5% a cada nível
    {
        return '%';
    }
}

/*
    Mapa *mapa - Endereço de uma variável do tipo Mapa (função abre um baú e verifica seu conteúdo)
*/
void abre_bau(Mapa *mapa)
{
    int i;
    for(i = 0; i < mapa->numeroBaus; i++)
    {
        // Se o jogador está sobre o bau de posição i do vetor, verifica o conteúdo desse baú...
        if(mapa->player.posicaoJ.linha == mapa->baus[i].posicaoB.linha && mapa->player.posicaoJ.coluna == mapa->baus[i].posicaoB.coluna)
        {
            switch(mapa->baus[i].conteudo)
            {
                // SE BAÚ CONTÉM UMA CHAVE
                case 'K':
                    mapa->player.chave = 1;                                                     // flag de chave se torna verdadeiro
                    jog_aumenta_pontuacao(&mapa->player, 1000);                                 // Jogador ganha 1000 pontos
                    mapa->matriz[mapa->posicaoSaida.linha][mapa->posicaoSaida.coluna] = 'P';    // Saída aparece no mapa
                    break;

                // SE BAÚ CONTÉM UMA BOMBA
                case 'B':
                    jog_aumenta_pontuacao(&mapa->player, -500); // Jogador perde 500 pontos
                    mapa_diminui_vida(mapa);
                    // Jogador perde uma vida e volta à posição inicial do mapa
                    break;

                // SE BAÚ CONTÉM UMA COROA
                case '%':
                    jog_aumenta_pontuacao(&mapa->player, 300);  // Jogador ganha 300 pontos
                    break;

                // SE BAÚ CONTÉM UM ANEL
                case '$':
                    jog_aumenta_pontuacao(&mapa->player, 200);  // Jogador ganha 200 pontos
                    break;

                // SE BAÚ CONTÉM UM RUBI
                case '#':
                    jog_aumenta_pontuacao(&mapa->player, 150);  // Jogador ganha 150 pontos
                    break;

                // SE BAÚ CONTÉM UMA SAFIRA
                case '@':
                    jog_aumenta_pontuacao(&mapa->player, 100);  // Jogador ganha 100 pontos
                    break;

                // SE BAÚ CONTÉM UMA AMETISTA
                case '!':
                    jog_aumenta_pontuacao(&mapa->player, 50);  // Jogador ganha 50 pontos
                    break;

                // SE BAÚ ESTÁ QUEIMADO OU VAZIO
                default:
                    break;
            }

            if(mapa->baus[i].conteudo == 'B') // Se baú continha uma bomba...
            {
                mapa->baus[i].conteudo = '*'; // Baú fica queimado
            }                                 // Senão...
            else
            {
                mapa->baus[i].conteudo = ' '; // Baú fica vazio após ser aberto
            }
        }
    }
}
